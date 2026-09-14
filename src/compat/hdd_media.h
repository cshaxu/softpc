#ifndef SOFTPC_HDD_MEDIA_H
#define SOFTPC_HDD_MEDIA_H

#include "insignia.h"

/* Private host-media configuration bridge.  Original CMOS/config callbacks
   ask the platform for the attached fixed-media path; the image backend owns
   that attachment state. */
const CHAR *softpc_hdd_media_config_path(unsigned int index);

#endif
