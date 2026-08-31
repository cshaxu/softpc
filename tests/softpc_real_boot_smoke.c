#include "softpc_machine.h"

#include <stdio.h>
#include <string.h>

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
        if (strstr(line, ":\\>") != NULL) return 1;
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
        } else if (strcmp(argv[index], "--overlay") == 0) {
            options.media_mode = SOFTPC_MEDIA_OVERLAY;
        } else goto usage;
    }
    if (options.floppy_path == NULL && options.hard_disk_path == NULL) {
usage:
        fprintf(stderr, "Usage: %s [--floppy floppy.img] [--hdd hard-disk.img] [--overlay] [--until-prompt] [--trace|--trace-1k|--trace-long|--trace-slices|--trace-transition]\n",
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
            if (require_prompt && text_has_dos_prompt(text)) {
                print_text_screen(text);
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
