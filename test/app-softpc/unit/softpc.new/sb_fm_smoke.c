#include "lib/types/test.h"
#include "insignia.h"
#include "host_def.h"
#include "nt_sb.h"
#include "compat/devices/snapshot.h"

static IU8 status(void)
{
    IU8 value = 0xff;
    FMStatusRead(&value);
    return value;
}

int main(void)
{
    /* Independent transition table for no flag, timer 1, timer 2, both.
     * Both reset low-bit states (06 and 00) and every index/data are tested. */
    static const IU8 flags[4] = {0x00, 0xc0, 0xa0, 0xe0};
    lib_u32 cleared, before, index, data;
    softpc_device_sb_fm_state saved, invalid;
    for (cleared = 0; cleared != 2; ++cleared) {
        for (before = 0; before != 4; ++before) {
            for (index = 0; index != 256; ++index) {
                for (data = 0; data != 256; ++data) {
                    IU8 expected = (IU8)(flags[before] | (cleared ? 0 : 6));
                    ResetFM();
                    FMRegisterSelect(4);
                    FMDataWrite((IU8)(before | (cleared ? 0x80 : 0)));
                    lib_test_assert(status() == expected);
                    FMRegisterSelect((IU8)index);
                    lib_test_assert(status() == expected);
                    FMDataWrite((IU8)data);
                    if (index == 4 && data >= 128) expected = 0;
                    if (index == 4 || (index >= 176 && index <= 189))
                        expected = (IU8)(expected | flags[data % 4]);
                    lib_test_assert(status() == expected);
                    lib_test_assert(status() == expected); /* Reads do not clear. */
                    softpc_device_snapshot_capture_sb_fm(&saved);
                    ResetFM();
                    lib_test_assert(softpc_device_snapshot_restore_sb_fm(&saved));
                    lib_test_assert(status() == expected);
                    /* The restored index, not only status, controls the next write. */
                    FMDataWrite(0x80);
                    lib_test_assert(status() == (index == 4 ? 0 : expected));
                }
            }
        }
    }
    FMRegisterSelect(4);
    FMDataWrite(3);
    ResetFM();
    lib_test_assert(status() == 6);
    FMDataWrite(3); /* Reset also clears the selected register. */
    lib_test_assert(status() == 6);
    softpc_device_snapshot_capture_sb_fm(&saved);
    for (data = 0; data < 256; ++data) {
        lib_bool valid = (data == 0 || data == 6 || data == 0xa0 ||
            data == 0xa6 || data == 0xc0 || data == 0xc6 ||
            data == 0xe0 || data == 0xe6);
        invalid = saved;
        invalid.status = (IU8)data;
        lib_test_assert(softpc_device_snapshot_restore_sb_fm(&invalid) == valid);
        lib_test_assert(status() == (valid ? data : saved.status));
        lib_test_assert(softpc_device_snapshot_restore_sb_fm(&saved));
    }
    return 0;
}
