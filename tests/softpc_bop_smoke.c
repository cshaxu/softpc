#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "bios.h"
#include "build_id.h"

extern IBOOL softpc_device_bop_dispatch(IU8 number, IU32 argument);
extern void c_setAL(IU8 value);
extern void c_setCX(IU16 value);
extern ISM32 c_setDS(IU16 value);
extern IU16 c_getAX(void);
extern IU16 c_getBX(void);

static void make_boot_disk(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fseek(file, 1474560L - 1L, SEEK_SET) == 0);
    assert(fputc(0, file) == 0);
    assert(fclose(file) == 0);
}

int main(void)
{
    const char *path = "softpc-bop-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    /* BOP 18 is original ROM BASIC fallback firmware, unlike product
       selectors intentionally absent from the standalone machine table. */
    assert(softpc_device_bop_dispatch(BIOS_BASIC, 0u) == TRUE);

    c_setAL(1u);
    c_setDS(0u);
    c_setCX(0x600u);
    assert(softpc_device_bop_dispatch(0x21u, 0u) == TRUE);
    {
        unsigned char name[5];
        assert(softpc_machine_read_physical(machine, 0x600u, name,
            sizeof(name)) == SOFTPC_MACHINE_OK);
        assert(name[0] == 'B' && name[1] == 'a' && name[2] == 's' &&
            name[3] == 'e' && name[4] == '$');
    }
    assert(c_getAX() == 0u);
    assert(c_getBX() == BUILD_ID_CODE);

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
