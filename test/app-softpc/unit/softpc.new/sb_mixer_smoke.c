#include "lib/types/test.h"
#include "lib/types/types_interface.h"
#include "insignia.h"
#include "sndblst.h"
#include "nt_sb.h"

static lib_u32 applied;
static IU16 voice_gain, midi_gain;

void SbApplyMixer(void)
{
    ++applied;
    voice_gain = MixerGetVoiceGain();
    midi_gain = MixerGetMidiGain();
}

static void write_register(IU8 index, IU8 value)
{
    MixerAddrWrite(index);
    MixerDataWrite(value);
}

static IU8 read_register(IU8 index)
{
    IU8 value = 0xff;
    MixerAddrWrite(index);
    MixerDataRead(&value);
    return value;
}

int main(void)
{
    static const lib_u32 master_levels[8] = {
        0u, 8191u, 16383u, 24575u, 32767u, 40959u, 49151u, 57343u
    };
    static const lib_u32 voice_levels[4] = { 0u, 16383u, 32767u, 49151u };
    lib_u32 master, voice, fm, before;

    ResetMixer();
    lib_test_assert(applied == 3u);
    lib_test_assert(read_register(MIXER_MASTER_VOLUME) == 8u);
    lib_test_assert(read_register(MIXER_VOICE_VOLUME) == 4u);
    lib_test_assert(read_register(MIXER_FM_VOLUME) == 8u);
    lib_test_assert(read_register(MIXER_CD_VOLUME) == 0u);
    for (master = 0u; master < 8u; ++master) {
        for (voice = 0u; voice < 4u; ++voice) {
            lib_u16 expected = (lib_u16)((lib_u64)master_levels[master] *
                voice_levels[voice] / 65535u);
            /* Reserved bits must read back, but must not affect attenuation. */
            IU8 m = (IU8)((master << 1) | 0xf1u);
            IU8 v = (IU8)((voice << 1) | 0xf9u);
            write_register(MIXER_MASTER_VOLUME, m);
            write_register(MIXER_VOICE_VOLUME, v);
            lib_test_assert(voice_gain == expected);
            lib_test_assert(read_register(MIXER_MASTER_VOLUME) == m);
            lib_test_assert(read_register(MIXER_VOICE_VOLUME) == v);
            write_register(MIXER_VOICE_VOLUME, 0u);
            lib_test_assert(voice_gain == 0u);
            write_register(MIXER_VOICE_VOLUME, v);
            write_register(MIXER_MASTER_VOLUME, m);
            lib_test_assert(voice_gain == expected);
        }
        for (fm = 0u; fm < 8u; ++fm) {
            lib_u16 expected = (lib_u16)((lib_u64)master_levels[master] *
                master_levels[fm] / 65535u);
            write_register(MIXER_FM_VOLUME, (IU8)((fm << 1) | 0xf1u));
            lib_test_assert(midi_gain == expected);
        }
    }
    before = applied;
    write_register(MIXER_CD_VOLUME, 0xa5u);
    lib_test_assert(applied == before && read_register(MIXER_CD_VOLUME) == 0xa5u);
    /* XP ignores unknown indices and retains the selected register. */
    MixerAddrWrite(0xffu);
    MixerDataWrite(0x5au);
    lib_test_assert(read_register(MIXER_CD_VOLUME) == 0x5au);
    before = applied;
    lib_test_assert(read_register(MIXER_RESET) == 0xffu);
    lib_test_assert(applied == before + 3u);
    lib_test_assert(read_register(MIXER_MASTER_VOLUME) == 8u);
    lib_test_assert(read_register(MIXER_VOICE_VOLUME) == 4u);
    write_register(MIXER_RESET, 0xffu);
    lib_test_assert(read_register(MIXER_FM_VOLUME) == 8u);
    lib_test_assert(read_register(MIXER_CD_VOLUME) == 0u);
    return 0;
}
