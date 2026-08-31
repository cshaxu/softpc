#include "softpc_machine.h"

#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define SOFTPC_TEXT_COLUMNS 80u
#define SOFTPC_TEXT_ROWS 25u
#define SOFTPC_TEXT_BYTES (SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS)
#define SOFTPC_BOOT_SLICES 200u
#define SOFTPC_SLICE_INSTRUCTIONS 50000u
#define SOFTPC_TRACE_INSTRUCTIONS 100u
#define SOFTPC_TRACE_1K_INSTRUCTIONS 1000u
#define SOFTPC_LONG_TRACE_SLICES 800u
#define SOFTPC_TRANSITION_WARMUP_SLICES 620u
#define SOFTPC_TRANSITION_TRACE_SLICES 600u
#define SOFTPC_PROMPT_SLICES 4000u
#define SOFTPC_SETUP_SLICES 4000u

extern unsigned short c_getSS(void);
extern unsigned short c_getSP(void);

static int read_text_surface(softpc_machine *machine, unsigned char *text)
{
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    unsigned int row;
    if (!softpc_machine_presentation_text(machine, &surface, &columns, &rows,
            &stride, &cell_bytes) || cell_bytes < 1u || stride < columns)
        return 0;
    cells = (const unsigned char *)surface;
    memset(text, ' ', SOFTPC_TEXT_BYTES);
    if (columns > SOFTPC_TEXT_COLUMNS) columns = SOFTPC_TEXT_COLUMNS;
    if (rows > SOFTPC_TEXT_ROWS) rows = SOFTPC_TEXT_ROWS;
    for (row = 0u; row < rows; ++row) {
        unsigned int column;
        for (column = 0u; column < columns; ++column)
            text[row * SOFTPC_TEXT_COLUMNS + column] =
                cells[(row * stride + column) * cell_bytes];
    }
    return 1;
}

static int text_has_printable_character(const unsigned char *text)
{
    unsigned int index;
    for (index = 0u; index < SOFTPC_TEXT_BYTES; ++index) {
        if (text[index] >= 0x20u && text[index] < 0x7fu) return 1;
    }
    return 0;
}

static int text_has_dos_prompt(const unsigned char *text)
{
    unsigned int row;
    for (row = 0u; row < SOFTPC_TEXT_ROWS; ++row) {
        char line[81];
        unsigned int column;
        for (column = 0u; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[row * SOFTPC_TEXT_COLUMNS + column];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (char)character : ' ';
        }
        line[SOFTPC_TEXT_COLUMNS] = '\0';
        for (column = 0u; column + 3u < SOFTPC_TEXT_COLUMNS; ++column) {
            if (((line[column] >= 'A' && line[column] <= 'Z') ||
                 (line[column] >= 'a' && line[column] <= 'z')) &&
                line[column + 1u] == ':' && line[column + 2u] == '\\' &&
                line[column + 3u] == '>')
                return 1;
        }
        for (column = 0u; column + 2u < SOFTPC_TEXT_COLUMNS; ++column) {
            if (((line[column] >= 'A' && line[column] <= 'Z') ||
                 (line[column] >= 'a' && line[column] <= 'z')) &&
                line[column + 1u] == ':' && line[column + 2u] == '>')
                return 1;
        }
        for (column = 0u; column + 1u < SOFTPC_TEXT_COLUMNS; ++column) {
            if (((line[column] >= 'A' && line[column] <= 'Z') ||
                 (line[column] >= 'a' && line[column] <= 'z')) &&
                line[column + 1u] == '>' &&
                (column == 0u || line[column - 1u] == ' '))
                return 1;
        }
    }
    return 0;
}

static int text_has_drive_prompt(const unsigned char *text, char drive)
{
    unsigned int row;
    unsigned int column;
    for (row = 0u; row < SOFTPC_TEXT_ROWS; ++row) {
        for (column = 0u; column + 3u < SOFTPC_TEXT_COLUMNS; ++column) {
            const unsigned char *cell = &text[row * SOFTPC_TEXT_COLUMNS +
                column];
            if (cell[0] == (unsigned char)drive && cell[1] == ':' &&
                cell[2] == '\\' && cell[3] == '>') return 1;
        }
    }
    return 0;
}

static int text_has_line_fragment(const unsigned char *text,
    const char *fragment)
{
    unsigned int row;
    for (row = 0u; row < SOFTPC_TEXT_ROWS; ++row) {
        char line[81];
        unsigned int column;
        for (column = 0u; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[row * SOFTPC_TEXT_COLUMNS + column];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (char)character : ' ';
        }
        line[SOFTPC_TEXT_COLUMNS] = '\0';
        if (strstr(line, fragment) != NULL) return 1;
    }
    return 0;
}

static void print_text_screen(const unsigned char *text)
{
    unsigned int row;
    for (row = 0u; row < SOFTPC_TEXT_ROWS; ++row) {
        char line[81];
        unsigned int column;
        for (column = 0u; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[row * SOFTPC_TEXT_COLUMNS + column];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (char)character : ' ';
        }
        line[SOFTPC_TEXT_COLUMNS] = '\0';
        fprintf(stderr, "%s\n", line);
    }
}

static void print_instruction(softpc_machine *machine, uint16_t cs,
    uint32_t eip)
{
    unsigned char instruction[8];
    uint32_t address = 0u;
    if (softpc_machine_instruction_address(machine, &address) ==
            SOFTPC_MACHINE_OK &&
        softpc_machine_read_physical(machine, address, instruction,
            sizeof(instruction)) == SOFTPC_MACHINE_OK)
        fprintf(stderr, "    %05x: %02x %02x %02x %02x %02x %02x %02x %02x\n",
            (unsigned int)address, instruction[0], instruction[1],
            instruction[2], instruction[3], instruction[4], instruction[5],
            instruction[6], instruction[7]);
}

static int is_trace_option(const char *argument)
{
    return strcmp(argument, "--trace") == 0 ||
        strcmp(argument, "--trace-1k") == 0 ||
        strcmp(argument, "--trace-long") == 0 ||
        strcmp(argument, "--trace-slices") == 0 ||
        strcmp(argument, "--trace-transition") == 0;
}

static int send_scancode(softpc_machine *machine, unsigned char scan_code)
{
    return softpc_machine_key_scancode(machine, scan_code) ==
        SOFTPC_MACHINE_OK;
}

static int send_key(softpc_machine *machine, unsigned char scan_code)
{
    /* The restored 8042 exposes one output byte at a time.  Feed make and
       break events on separate execution intervals, just as the interactive
       host bridge does; submitting both at once can discard the make event. */
    if (!send_scancode(machine, scan_code))
        return 0;
#ifdef _WIN32
    Sleep(25u);
#endif
    if (softpc_machine_run(machine, 10000u) != SOFTPC_MACHINE_OK ||
        !send_scancode(machine, (unsigned char)(scan_code | 0x80u)))
        return 0;
#ifdef _WIN32
    Sleep(25u);
#endif
    return softpc_machine_run(machine, 10000u) == SOFTPC_MACHINE_OK;
}

static int send_character(softpc_machine *machine, char character)
{
    static const unsigned char letters[26] = {
        0x1eu, 0x30u, 0x2eu, 0x20u, 0x12u, 0x21u, 0x22u, 0x23u,
        0x17u, 0x24u, 0x25u, 0x26u, 0x32u, 0x31u, 0x18u, 0x19u,
        0x10u, 0x13u, 0x1fu, 0x14u, 0x16u, 0x2fu, 0x11u, 0x2du,
        0x15u, 0x2cu
    };
    static const unsigned char digits[10] = {
        0x0bu, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u,
        0x09u, 0x0au
    };
    if (character >= 'a' && character <= 'z')
        return send_key(machine, letters[(unsigned int)(character - 'a')]);
    if (character >= 'A' && character <= 'Z')
        return send_key(machine, letters[(unsigned int)(character - 'A')]);
    if (character >= '0' && character <= '9')
        return send_key(machine, digits[(unsigned int)(character - '0')]);
    if (character == ' ') return send_key(machine, 0x39u);
    if (character == ':') {
        return send_scancode(machine, 0x2au) && send_key(machine, 0x27u) &&
            send_scancode(machine, 0xaau);
    }
    return 0;
}

static int send_command(softpc_machine *machine, const char *command)
{
    const char *cursor;
    for (cursor = command; *cursor != '\0'; ++cursor)
        if (!send_character(machine, *cursor)) return 0;
    return send_key(machine, 0x1cu);
}

/* The no-window smoke executable has no interactive message loop.  Queue the
   same BIOS-visible keystrokes that DOS consumes through INT 16h, instead of
   relying on host timer delivery while the test runs tight CPU slices. */
static int bios_scan_code(char character, unsigned char *scan_out)
{
    static const unsigned char letters[26] = {
        0x1eu, 0x30u, 0x2eu, 0x20u, 0x12u, 0x21u, 0x22u, 0x23u,
        0x17u, 0x24u, 0x25u, 0x26u, 0x32u, 0x31u, 0x18u, 0x19u,
        0x10u, 0x13u, 0x1fu, 0x14u, 0x16u, 0x2fu, 0x11u, 0x2du,
        0x15u, 0x2cu
    };
    static const unsigned char digits[10] = {
        0x0bu, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u,
        0x09u, 0x0au
    };
    if (character >= 'a' && character <= 'z')
        *scan_out = letters[(unsigned int)(character - 'a')];
    else if (character >= '0' && character <= '9')
        *scan_out = digits[(unsigned int)(character - '0')];
    else if (character == ':') *scan_out = 0x27u;
    else return 0;
    return 1;
}

static int queue_bios_command(softpc_machine *machine, const char *command)
{
    unsigned short head, tail, word;
    unsigned char scan;
    const char *cursor;
    for (cursor = command; ; ++cursor) {
        unsigned char ascii = *cursor == '\0' ? '\r' : (unsigned char)*cursor;
        if (*cursor == '\0') scan = 0x1cu;
        else if (!bios_scan_code(*cursor, &scan)) return 0;
        if (softpc_machine_read_physical(machine, 0x41au, &head, sizeof(head)) !=
                SOFTPC_MACHINE_OK ||
            softpc_machine_read_physical(machine, 0x41cu, &tail, sizeof(tail)) !=
                SOFTPC_MACHINE_OK) return 0;
        word = (unsigned short)ascii | ((unsigned short)scan << 8);
        if (softpc_machine_write_physical(machine, tail, &word, sizeof(word)) !=
                SOFTPC_MACHINE_OK) return 0;
        tail = (unsigned short)(tail + 2u);
        if (tail >= 0x43eu) tail = 0x41eu;
        if (tail == head || softpc_machine_write_physical(machine, 0x41cu,
                &tail, sizeof(tail)) != SOFTPC_MACHINE_OK) return 0;
        if (*cursor == '\0') return 1;
    }
}

int main(int argc, char **argv)
{
    softpc_machine_options options = { NULL, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    unsigned char text[SOFTPC_TEXT_BYTES];
    uint16_t cs = 0u;
    uint32_t eip = 0u;
    unsigned int index;
    unsigned int slices = SOFTPC_BOOT_SLICES;
    unsigned int warmup_slices = 0u;
    int trace = 0;
    int require_prompt = 0;
    int windows_setup = 0;
    int date_accepted = 0;
    int time_accepted = 0;
    int setup_stage = 0;
    uint64_t slice_instructions = SOFTPC_SLICE_INSTRUCTIONS;
    softpc_machine_result result;

    for (index = 1u; index < (unsigned int)argc; ++index) {
        if ((strcmp(argv[index], "--floppy") == 0 ||
            strcmp(argv[index], "--hdd") == 0) && index + 1u < (unsigned int)argc) {
            const char *path = argv[++index];
            if (strcmp(argv[index - 1u], "--floppy") == 0) {
                if (options.floppy_path != NULL) goto usage;
                options.floppy_path = path;
            } else {
                if (options.hard_disk_path != NULL) goto usage;
                options.hard_disk_path = path;
            }
        } else if (is_trace_option(argv[index]) && !trace) {
            trace = 1;
            if (strcmp(argv[index], "--trace") == 0)
                slice_instructions = SOFTPC_TRACE_INSTRUCTIONS;
            else if (strcmp(argv[index], "--trace-1k") == 0)
                slice_instructions = SOFTPC_TRACE_1K_INSTRUCTIONS;
            else if (strcmp(argv[index], "--trace-long") == 0) {
                slice_instructions = SOFTPC_TRACE_INSTRUCTIONS;
                slices = SOFTPC_LONG_TRACE_SLICES;
            } else if (strcmp(argv[index], "--trace-transition") == 0) {
                slice_instructions = 1u;
                slices = SOFTPC_TRANSITION_TRACE_SLICES;
                warmup_slices = SOFTPC_TRANSITION_WARMUP_SLICES;
            }
        } else if (strcmp(argv[index], "--until-prompt") == 0 &&
            !require_prompt) {
            require_prompt = 1;
            slices = SOFTPC_PROMPT_SLICES;
        } else if (strcmp(argv[index], "--windows-setup") == 0 &&
            !windows_setup) {
            windows_setup = 1;
            require_prompt = 1;
            slices = SOFTPC_PROMPT_SLICES;
        } else if (strcmp(argv[index], "--overlay") == 0) {
            options.media_mode = SOFTPC_MEDIA_OVERLAY;
        } else goto usage;
    }
    if (options.floppy_path == NULL && options.hard_disk_path == NULL) {
usage:
        fprintf(stderr, "Usage: %s [--floppy floppy.img] [--hdd hard-disk.img] [--overlay] [--until-prompt|--windows-setup] [--trace|--trace-1k|--trace-long|--trace-slices|--trace-transition]\n",
            argv[0]);
        return 2;
    }
    result = softpc_machine_create(&options, &machine);
    if (result != SOFTPC_MACHINE_OK) goto failed;
    result = softpc_machine_reset(machine);
    if (result != SOFTPC_MACHINE_OK) goto failed;
    for (index = 0u; index < warmup_slices; ++index) {
        result = softpc_machine_run(machine, SOFTPC_TRACE_INSTRUCTIONS);
        if (result != SOFTPC_MACHINE_OK) goto failed;
    }
    for (index = 0u; index < slices; ++index) {
        result = softpc_machine_run(machine, slice_instructions);
        if (result != SOFTPC_MACHINE_OK) goto failed;
        if (trace) {
            (void)softpc_machine_instruction_pointer(machine, &cs, &eip);
            fprintf(stderr, "%03u %04x:%08x ss:sp=%04x:%04x\n",
                index + warmup_slices,
                (unsigned int)cs, (unsigned int)eip,
                (unsigned int)c_getSS(), (unsigned int)c_getSP());
            print_instruction(machine, cs, eip);
        }
        if (read_text_surface(machine, text)) {
            if (require_prompt && !date_accepted && text_has_line_fragment(
                    text, "Enter new date")) {
                if (!send_key(machine, 0x1cu)) goto failed;
                date_accepted = 1;
                continue;
            }
            if (require_prompt && !time_accepted && text_has_line_fragment(
                    text, "Enter new time")) {
                if (!send_key(machine, 0x1cu)) goto failed;
                time_accepted = 1;
                continue;
            }
            if (require_prompt && text_has_dos_prompt(text)) {
                if (windows_setup) {
                    unsigned int setup_slice;
                    unsigned int changed_instruction_addresses = 0u;
                    uint32_t previous_address = 0u;
                    int graphics_seen = 0;
                    int frame_seen = 0;
                    const void *bits;
                    const void *info;
                    uint32_t width;
                    uint32_t height;
                    int32_t left, top, right, bottom;
                    /* Reproduce the user's actual boot path: the floppy
                       reaches A:\\>, then DOS changes to C:\\> before the
                       Windows 3.1 installer command is typed. */
                    if (setup_stage == 0) {
                        if (!text_has_drive_prompt(text, 'A')) continue;
                        print_text_screen(text);
                        if (!queue_bios_command(machine, "c:")) goto failed;
                        setup_stage = 1;
                        continue;
                    }
                    if (setup_stage == 1) {
                        if (!text_has_drive_prompt(text, 'C')) continue;
                        if (!queue_bios_command(machine, "ewin31")) goto failed;
                        setup_stage = 2;
                    }
                    for (setup_slice = 0u; setup_slice < SOFTPC_SETUP_SLICES;
                         ++setup_slice) {
                        uint32_t address = 0u;
                        result = softpc_machine_run(machine,
                            SOFTPC_SLICE_INSTRUCTIONS);
                        if (result != SOFTPC_MACHINE_OK) goto failed;
                        if (softpc_machine_instruction_address(machine,
                                &address) == SOFTPC_MACHINE_OK &&
                            (setup_slice == 0u || address != previous_address))
                            ++changed_instruction_addresses;
                        previous_address = address;
                        if (softpc_machine_presentation_is_graphics(machine)) {
                            graphics_seen = 1;
                            if (softpc_machine_presentation_dib(machine, &bits,
                                    &info, &width, &height) && bits != NULL &&
                                info != NULL && width != 0u && height != 0u)
                                frame_seen = 1;
                            while (softpc_machine_presentation_take_dirty(machine,
                                    &left, &top, &right, &bottom)) frame_seen = 1;
                        }
                    }
                    if (!graphics_seen || !frame_seen ||
                        changed_instruction_addresses < 64u) {
                        fprintf(stderr, "softpc-real-boot-smoke: Windows Setup did not make sustained graphics progress (graphics=%d frame=%d instruction-addresses=%u)\n",
                            graphics_seen, frame_seen,
                            changed_instruction_addresses);
                        goto failed;
                    }
                }
                softpc_machine_destroy(machine);
                return 0;
            }
            if (!require_prompt && text_has_printable_character(text)) {
                softpc_machine_destroy(machine);
                return 0;
            }
        }
    }
    (void)softpc_machine_instruction_pointer(machine, &cs, &eip);
    fprintf(stderr, "softpc-real-boot-smoke: guest produced no text output at %04x:%08x\n",
        (unsigned int)cs, (unsigned int)eip);
    if (require_prompt) print_text_screen(text);
    print_instruction(machine, cs, eip);
failed:
    if (result != SOFTPC_MACHINE_OK)
        fprintf(stderr, "softpc-real-boot-smoke: %s\n",
            softpc_machine_result_name(result));
    softpc_machine_destroy(machine);
    return 1;
}
