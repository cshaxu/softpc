/* Fixed 16-bit PC I/O bus for the standalone SoftPC machine. */
#ifndef ANSI
#define ANSI 1
#endif
#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include CpuH
#include "ios.h"

typedef void (*IOS_FUNC_INB) IPT2(io_addr, io_address, half_word *, value);
typedef void (*IOS_FUNC_INW) IPT2(io_addr, io_address, word *, value);
typedef void (*IOS_FUNC_INSB) IPT3(io_addr, io_address, half_word *, valarray, word, count);
typedef void (*IOS_FUNC_INSW) IPT3(io_addr, io_address, word *, valarray, word, count);
typedef void (*IOS_FUNC_OUTB) IPT2(io_addr, io_address, half_word, value);
typedef void (*IOS_FUNC_OUTW) IPT2(io_addr, io_address, word, value);
typedef void (*IOS_FUNC_OUTSB) IPT3(io_addr, io_address, half_word *, valarray, word, count);
typedef void (*IOS_FUNC_OUTSW) IPT3(io_addr, io_address, word *, valarray, word, count);
typedef void (*IOS_FUNC_IND) IPT2(io_addr, io_address, IU32 *, value);
typedef void (*IOS_FUNC_INSD) IPT3(io_addr, io_address, IU32 *, valarray, word, count);
typedef void (*IOS_FUNC_OUTD) IPT2(io_addr, io_address, IU32, value);
typedef void (*IOS_FUNC_OUTSD) IPT3(io_addr, io_address, IU32 *, valarray, word, count);

char Ios_in_adapter_table[PC_IO_MEM_SIZE];
char Ios_out_adapter_table[PC_IO_MEM_SIZE];
IOS_FUNC_INB Ios_inb_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_INW Ios_inw_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_INSB Ios_insb_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_INSW Ios_insw_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_OUTB Ios_outb_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_OUTW Ios_outw_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_OUTSB Ios_outsb_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_OUTSW Ios_outsw_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_IND Ios_ind_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_INSD Ios_insd_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_OUTD Ios_outd_function[IO_MAX_NUMBER_ADAPTORS];
IOS_FUNC_OUTSD Ios_outsd_function[IO_MAX_NUMBER_ADAPTORS];

void ind(io_addr p, IU32 *v);
void outd(io_addr p, IU32 v);

static unsigned port_index(io_addr port) { return ((unsigned)port) & 0xffffU; }
static void empty_inb(io_addr p, half_word *v) { UNUSED(p); *v = IO_EMPTY_PORT_BYTE_VALUE; }
static void empty_outb(io_addr p, half_word v) { UNUSED(p); UNUSED(v); }
static void generic_inw(io_addr p, word *v) { half_word lo, hi; inb(p, &lo); inb(p + 1, &hi); *v = (word)(lo | ((word)hi << 8)); }
static void generic_outw(io_addr p, word v) { outb(p, (half_word)v); outb(p + 1, (half_word)(v >> 8)); }
static void generic_ind(io_addr p, IU32 *v) { word lo, hi; inw(p, &lo); inw(p + 2, &hi); *v = (IU32)lo | ((IU32)hi << 16); }
static void generic_outd(io_addr p, IU32 v) { outw(p, (word)v); outw(p + 2, (word)(v >> 16)); }
static void generic_insb(io_addr p, half_word *v, word n) { while (n--) inb(p, v++); }
static void generic_outsb(io_addr p, half_word *v, word n) { while (n--) outb(p, *v++); }
static void generic_insw(io_addr p, word *v, word n) { while (n--) inw(p, v++); }
static void generic_outsw(io_addr p, word *v, word n) { while (n--) outw(p, *v++); }
static void generic_insd(io_addr p, IU32 *v, word n) { while (n--) ind(p, v++); }
static void generic_outsd(io_addr p, IU32 *v, word n) { while (n--) outd(p, *v++); }

void inb(io_addr p, half_word *v) { Ios_inb_function[(unsigned char)Ios_in_adapter_table[port_index(p)]](p, v); }
void outb(io_addr p, half_word v) { Ios_outb_function[(unsigned char)Ios_out_adapter_table[port_index(p)]](p, v); }
void inw(io_addr p, word *v) { Ios_inw_function[(unsigned char)Ios_in_adapter_table[port_index(p)]](p, v); }
void outw(io_addr p, word v) { Ios_outw_function[(unsigned char)Ios_out_adapter_table[port_index(p)]](p, v); }
void ind(io_addr p, IU32 *v) { Ios_ind_function[(unsigned char)Ios_in_adapter_table[port_index(p)]](p, v); }
void outd(io_addr p, IU32 v) { Ios_outd_function[(unsigned char)Ios_out_adapter_table[port_index(p)]](p, v); }
void insb(io_addr p, half_word *v, word n) { Ios_insb_function[(unsigned char)Ios_in_adapter_table[port_index(p)]](p, v, n); }
void outsb(io_addr p, half_word *v, word n) { Ios_outsb_function[(unsigned char)Ios_out_adapter_table[port_index(p)]](p, v, n); }
void insw(io_addr p, word *v, word n) { Ios_insw_function[(unsigned char)Ios_in_adapter_table[port_index(p)]](p, v, n); }
void outsw(io_addr p, word *v, word n) { Ios_outsw_function[(unsigned char)Ios_out_adapter_table[port_index(p)]](p, v, n); }
void insd(io_addr p, IU32 *v, word n) { Ios_insd_function[(unsigned char)Ios_in_adapter_table[port_index(p)]](p, v, n); }
void outsd(io_addr p, IU32 *v, word n) { Ios_outsd_function[(unsigned char)Ios_out_adapter_table[port_index(p)]](p, v, n); }

void io_define_inb(half_word a, IOS_FUNC_INB f) { Ios_inb_function[a] = f; Ios_inw_function[a] = generic_inw; Ios_insb_function[a] = generic_insb; Ios_insw_function[a] = generic_insw; Ios_ind_function[a] = generic_ind; Ios_insd_function[a] = generic_insd; }
void io_define_outb(half_word a, IOS_FUNC_OUTB f) { Ios_outb_function[a] = f; Ios_outw_function[a] = generic_outw; Ios_outsb_function[a] = generic_outsb; Ios_outsw_function[a] = generic_outsw; Ios_outd_function[a] = generic_outd; Ios_outsd_function[a] = generic_outsd; }
void io_define_in_routines(half_word a, IOS_FUNC_INB b, IOS_FUNC_INW w, IOS_FUNC_INSB sb, IOS_FUNC_INSW sw) { io_define_inb(a, b); if (w) Ios_inw_function[a] = w; if (sb) Ios_insb_function[a] = sb; if (sw) Ios_insw_function[a] = sw; }
void io_define_out_routines(half_word a, IOS_FUNC_OUTB b, IOS_FUNC_OUTW w, IOS_FUNC_OUTSB sb, IOS_FUNC_OUTSW sw) { io_define_outb(a, b); if (w) Ios_outw_function[a] = w; if (sb) Ios_outsb_function[a] = sb; if (sw) Ios_outsw_function[a] = sw; }
void io_connect_port(io_addr p, half_word a, half_word mode) { if (mode & IO_READ) Ios_in_adapter_table[port_index(p)] = (char)a; if (mode & IO_WRITE) Ios_out_adapter_table[port_index(p)] = (char)a; }
void io_disconnect_port(io_addr p, half_word a) { UNUSED(a); Ios_in_adapter_table[port_index(p)] = EMPTY_ADAPTOR; Ios_out_adapter_table[port_index(p)] = EMPTY_ADAPTOR; }
IOS_FUNC_INB *get_inb_ptr(io_addr p) { return &Ios_inb_function[(unsigned char)Ios_in_adapter_table[port_index(p)]]; }
IOS_FUNC_OUTB *get_outb_ptr(io_addr p) { return &Ios_outb_function[(unsigned char)Ios_out_adapter_table[port_index(p)]]; }
void io_init(void) { unsigned i; io_define_inb(EMPTY_ADAPTOR, empty_inb); io_define_outb(EMPTY_ADAPTOR, empty_outb); for (i = 0; i < PC_IO_MEM_SIZE; ++i) { Ios_in_adapter_table[i] = EMPTY_ADAPTOR; Ios_out_adapter_table[i] = EMPTY_ADAPTOR; } }
