#ifndef SOFTPC_AUDIO_H
#define SOFTPC_AUDIO_H

#include "lib/types/types_interface.h"

/* The application creates and joins this product presentation worker. The
 * original device callback only updates its tone request. */
lib_status softpc_platform_audio_start(void);
/* A failed destroy retains the live worker and its dependencies so the VM
 * owner keeps process-wide admission closed. */
lib_status softpc_platform_audio_shutdown(void);

#endif
