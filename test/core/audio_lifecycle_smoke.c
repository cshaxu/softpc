#include "audio.h"

#include <assert.h>

int main(void)
{
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    softpc_platform_audio_shutdown();
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    softpc_platform_audio_shutdown();
    return 0;
}
