#include "insignia.h"
#include "host_def.h"

#include "../cvidc/evidgen.h"
#include "cpu4.h"
#include "c_main.h"
#include "c_bsic.h"

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

/* tape_io.c retains the original pre-CPU40 descriptor-helper ABI.  CPU40
 * already owns descriptor decoding; this compatibility entry only exposes
 * that decoded state under the older mechanical interface. */
void retrieve_descr_fields IFN4(half_word *, ar, sys_addr *, base,
    word *, limit, sys_addr, descriptor_address)
{
    CPU_DESCR descriptor;

    read_descriptor_linear(descriptor_address, &descriptor);
    *ar = descriptor.AR;
    *base = descriptor.base;
    *limit = (word)descriptor.limit;
}

#endif /* !PIG */
