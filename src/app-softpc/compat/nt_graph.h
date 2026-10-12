#ifndef SOFTPC_COMPAT_NT_GRAPH_H
#define SOFTPC_COMPAT_NT_GRAPH_H

#include "../softpc.new/host/inc/nt_graph.h"

/* The single executor owns rendering; there is no native Console mutex. */
#undef GrabMutex
#undef RelMutex
#define GrabMutex(mutex) ((void)(mutex))
#define RelMutex(mutex) ((void)(mutex))

#endif
