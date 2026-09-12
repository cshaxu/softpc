#ifndef LIB_TYPES_LINUX_CLOCK_H
#define LIB_TYPES_LINUX_CLOCK_H

#include <time.h>

typedef struct timespec lib_linux_timespec;
#define lib_linux_clock_gettime clock_gettime

#endif
