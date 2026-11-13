#ifndef SOFTPC_CCPU_ABI_H
#define SOFTPC_CCPU_ABI_H

/* Match the recovered CCPU ABI (IU32 is unsigned long on Windows). No CPU
 * state is duplicated here; these are the original register accessors. */
#define DECLARE_REGISTER(name) \
    extern unsigned long c_get##name(void); \
    extern void c_set##name(unsigned long value)
DECLARE_REGISTER(EAX);
DECLARE_REGISTER(ECX);
DECLARE_REGISTER(EDX);
DECLARE_REGISTER(EBX);
DECLARE_REGISTER(ESP);
DECLARE_REGISTER(EBP);
DECLARE_REGISTER(ESI);
DECLARE_REGISTER(EDI);
DECLARE_REGISTER(EIP);
DECLARE_REGISTER(EFLAGS);
DECLARE_REGISTER(CR0);
DECLARE_REGISTER(CR2);
DECLARE_REGISTER(CR3);
#undef DECLARE_REGISTER
/* The original segment setters catch CPU exceptions and return their number. */
#define DECLARE_SEGMENT(name) \
    extern unsigned short c_get##name(void); \
    extern long c_set##name(unsigned short value); \
    extern unsigned long c_get##name##_BASE(void); \
    extern unsigned long c_get##name##_LIMIT(void); \
    extern unsigned short c_get##name##_AR(void); \
    extern unsigned short c_get##name##_SELECTOR(void); \
    extern void c_set##name##_SELECTOR(unsigned short value); \
    extern void c_set##name##_BASE_LIMIT_AR(unsigned long base, \
        unsigned long limit, unsigned short ar)
DECLARE_SEGMENT(ES);
DECLARE_SEGMENT(CS);
DECLARE_SEGMENT(SS);
DECLARE_SEGMENT(DS);
DECLARE_SEGMENT(FS);
DECLARE_SEGMENT(GS);
#undef DECLARE_SEGMENT
extern unsigned long c_getGDT_BASE(void), c_getIDT_BASE(void);
extern unsigned short c_getGDT_LIMIT(void), c_getIDT_LIMIT(void);
extern unsigned long c_getLDT_BASE(void), c_getLDT_LIMIT(void);
extern unsigned short c_getLDT_SELECTOR(void);
extern unsigned long c_getTR_BASE(void), c_getTR_LIMIT(void);
extern unsigned short c_getTR_SELECTOR(void), c_getTR_AR(void);

extern long c_getCPL(void);
extern void c_setCPL(long value);
extern void c_setGDT_BASE_LIMIT(unsigned long base, unsigned short limit);
extern void c_setIDT_BASE_LIMIT(unsigned long base, unsigned short limit);
extern void c_setLDT_SELECTOR(unsigned short selector);
extern void c_setLDT_BASE_LIMIT(unsigned long base, unsigned long limit);
extern void c_setTR_SELECTOR(unsigned short selector);
extern void c_setTR_BASE_LIMIT_AR(unsigned long base, unsigned long limit,
    unsigned short ar);

void c_cpu_init(void);
void c_cpu_reset(void);
void c_cpu_simulate(void);
void c_cpu_terminate(void);
void ccpu386newthread(void);
void ccpu386exitthread(void);
void ccpu386UnsimulateOuter(void);

#endif
