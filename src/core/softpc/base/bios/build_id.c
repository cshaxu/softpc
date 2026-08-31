#include "insignia.h"
#include "host_def.h"

#include "xt.h"
#include CpuH
#include "sas.h"
#include "build_id.h"

enum
{
	BASE_MODULE = 1
};

static char base_name[] = {"Base$"};
#define base_name_len 5

GLOBAL void Get_build_id IFN0()
{
	switch (getAL()) {
	case BASE_MODULE:
		write_intel_byte_string(getDS(), getCX(), (host_addr)base_name,
			base_name_len);
		setBX(BUILD_ID_CODE);
		setAX(0);
		break;
	}
}
