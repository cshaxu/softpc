#include "compat/audio.h"

#ifdef NDEBUG
#error Audio lifecycle verification requires live assertions
#endif
#include <assert.h>

int main(void)
{
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(softpc_platform_audio_shutdown() == LIB_STATUS_OK);
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(softpc_platform_audio_shutdown() == LIB_STATUS_OK);
    return 0;
}
