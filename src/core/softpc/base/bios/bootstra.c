#include "insignia.h"
#include "host_def.h"
/*
 * Original SoftPC BIOS bootstrap services.  The accompanying BIOS4 ROM
 * sequence performs BOP 19, INT 13, BOP 90, INT 13, BOP 91, INT 13, BOP 92
 * before transferring to 0000:7c00.
 */

#include <stdio.h>
#include TypesH

#include "xt.h"
#include "bios.h"
#include "sas.h"
#include CpuH

void bootstrap()
{
    setAX(0);
    setDX(0x80);
}

void bootstrap1()
{
    setAH(2);
    setAL(1);
    setDX(0);
    setCX(1);
    setES(DOS_SEGMENT);
    setBX(DOS_OFFSET);
}

void bootstrap2()
{
    if (getCF()) {
        setAH(2);
        setAL(1);
        setCH(0);
        setCL(1);
        setDH(0);
        setDL(0x80);
        setES(DOS_SEGMENT);
        setBX(DOS_OFFSET);
    }
}

void bootstrap3()
{
    char *p;
    char error_str[80];

    if (getCF()) {
        sprintf(error_str, "DOS boot error - cannot open hard disk file");
        p = error_str;
        while (*p != '\0') {
            setAH(14);
            setAL(*p++);
            bop(BIOS_VIDEO_IO);
        }
    }
    setIF(1);
}
