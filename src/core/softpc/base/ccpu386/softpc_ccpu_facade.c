#include "insignia.h"
#include "host_def.h"

#include "../cvidc/evidgen.h"
#include "cpu4.h"

#ifndef PIG

extern struct VideoVector C_Video;
extern struct VideoVector Video;

/* The original CPU_40 product bootstrap installs the generated C-VID vector
 * before EGA/VGA port initialisation.  The detached one-machine host owns
 * that bootstrap explicitly. */
void softpc_ccpu_install_video_vector(void)
{
    Video = C_Video;
}

/* ROM discovery invokes this real-mode address provider.  The stripped
 * CPU40 executor does not export the historical generic helper spelling. */
IU32 c_effective_addr IFN2(IU16, selector, IU32, offset)
{
    return ((IU32)selector << 4) + offset;
}

#endif /* !PIG */
