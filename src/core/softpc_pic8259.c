#include "insignia.h"
#include "host_def.h"
#include "ica.h"
#include "ios.h"

/* Fixed-machine 8259 pair.  This deliberately implements the mechanical
 * interrupt-controller contract needed by CCPU and device ports, without
 * monitor callbacks, virtual-device exports, or guest-service hooks. */

typedef struct softpc_pic {
    IU8 irr;
    IU8 isr;
    IU8 imr;
    IU8 base;
    IU8 init_step;
    IU8 expect_icw4;
} softpc_pic;

static softpc_pic softpc_pics[2];

void (*ica_inb_func) IPT2(io_addr, port, IU8 *, value);
void (*ica_outb_func) IPT2(io_addr, port, IU8, value);
void (*ica_hw_interrupt_func) IPT3(IU32, adapter, IU32, line, IS32, count);
void (*ica_clear_int_func) IPT2(IU32, adapter, IU32, line);

static IU8 softpc_pic_first_pending(pic)
softpc_pic *pic;
{
    IU8 line;
    for (line = 0; line < 8u; ++line)
        if ((pic->irr & (IU8)(1u << line)) != 0u &&
            (pic->imr & (IU8)(1u << line)) == 0u)
            return line;
    return 0xffu;
}

static void softpc_pic_signal_cpu(void)
{
    if (softpc_pic_first_pending(&softpc_pics[ICA_MASTER]) != 0xffu)
        host_set_hw_int();
}

static void softpc_pic_reset(pic, base, mask)
softpc_pic *pic;
IU8 base;
IU8 mask;
{
    pic->irr = 0u;
    pic->isr = 0u;
    pic->imr = mask;
    pic->base = base;
    pic->init_step = 0u;
    pic->expect_icw4 = 0u;
}

static IU32 softpc_pic_for_port(port)
io_addr port;
{
    return port >= 0xa0u ? ICA_SLAVE : ICA_MASTER;
}

static void softpc_pic_inb(port, value)
io_addr port;
IU8 *value;
{
    softpc_pic *pic = &softpc_pics[softpc_pic_for_port(port)];
    *value = (port & 1u) == 0u ? pic->irr : pic->imr;
}

static void softpc_pic_outb(port, value)
io_addr port;
IU8 value;
{
    softpc_pic *pic = &softpc_pics[softpc_pic_for_port(port)];
    if ((port & 1u) == 0u) {
        if ((value & 0x10u) != 0u) {
            pic->irr = 0u;
            pic->isr = 0u;
            pic->init_step = 1u;
            pic->expect_icw4 = (value & 1u) != 0u;
        } else if ((value & 0x20u) != 0u) {
            IU8 line;
            for (line = 0u; line < 8u; ++line)
                if ((pic->isr & (IU8)(1u << line)) != 0u) {
                    pic->isr &= (IU8)~(1u << line);
                    break;
                }
        }
    } else if (pic->init_step == 1u) {
        pic->base = value & 0xf8u;
        pic->init_step = 2u;
    } else if (pic->init_step == 2u) {
        pic->init_step = pic->expect_icw4 ? 3u : 0u;
    } else if (pic->init_step == 3u) {
        pic->init_step = 0u;
    } else {
        pic->imr = value;
    }
    softpc_pic_signal_cpu();
}

void SWPIC_init_funcptrs(void)
{
    ica_inb_func = softpc_pic_inb;
    ica_outb_func = softpc_pic_outb;
    ica_hw_interrupt_func = SWPIC_hw_interrupt;
    ica_clear_int_func = SWPIC_clear_int;
}

void ica0_init(void)
{
    io_define_inb(ICA0_ADAPTOR, softpc_pic_inb);
    io_define_outb(ICA0_ADAPTOR, softpc_pic_outb);
    io_connect_port(0x20u, ICA0_ADAPTOR, IO_READ | IO_WRITE);
    io_connect_port(0x21u, ICA0_ADAPTOR, IO_READ | IO_WRITE);
}

void ica1_init(void)
{
    io_define_inb(ICA1_ADAPTOR, softpc_pic_inb);
    io_define_outb(ICA1_ADAPTOR, softpc_pic_outb);
    io_connect_port(0xa0u, ICA1_ADAPTOR, IO_READ | IO_WRITE);
    io_connect_port(0xa1u, ICA1_ADAPTOR, IO_READ | IO_WRITE);
}

void ica0_post(void)
{
    softpc_pic_reset(&softpc_pics[ICA_MASTER], 0x08u, 0xb8u);
}

void ica1_post(void)
{
    softpc_pic_reset(&softpc_pics[ICA_SLAVE], 0x70u, 0x9du);
}

void SWPIC_hw_interrupt(adapter, line, count)
IU32 adapter;
IU32 line;
IS32 count;
{
    softpc_pic *pic;
    if (adapter > ICA_SLAVE || line > 7u || count <= 0) return;
    pic = &softpc_pics[adapter];
    pic->irr |= (IU8)(1u << line);
    if (adapter == ICA_SLAVE)
        softpc_pics[ICA_MASTER].irr |= (IU8)(1u << 2u);
    softpc_pic_signal_cpu();
}

void SWPIC_clear_int(adapter, line)
IU32 adapter;
IU32 line;
{
    if (adapter > ICA_SLAVE || line > 7u) return;
    softpc_pics[adapter].irr &= (IU8)~(1u << line);
}

void ica_hw_interrupt_cancel(adapter, line)
IU32 adapter;
IU32 line;
{
    SWPIC_clear_int(adapter, line);
}

IU8 ica_scan_irr(adapter)
IU32 adapter;
{
    IU8 line;
    if (adapter > ICA_SLAVE) return 7u;
    line = softpc_pic_first_pending(&softpc_pics[adapter]);
    return line == 0xffu ? 7u : (IU8)(0x80u | line);
}

IS32 ica_intack(hook_address)
IU32 *hook_address;
{
    softpc_pic *pic = &softpc_pics[ICA_MASTER];
    IU8 line = softpc_pic_first_pending(pic);
    if (hook_address != NULL) *hook_address = 0u;
    if (line == 0xffu) return (IS32)(pic->base + 7u);
    pic->irr &= (IU8)~(1u << line);
    pic->isr |= (IU8)(1u << line);
    if (line == 2u && softpc_pic_first_pending(&softpc_pics[ICA_SLAVE]) != 0xffu) {
        softpc_pic *slave = &softpc_pics[ICA_SLAVE];
        IU8 slave_line = softpc_pic_first_pending(slave);
        slave->irr &= (IU8)~(1u << slave_line);
        slave->isr |= (IU8)(1u << slave_line);
        return (IS32)(slave->base + slave_line);
    }
    return (IS32)(pic->base + line);
}

void ica_interrupt_cpu(adapter, line)
IU32 adapter;
IU32 line;
{
    UNUSED(adapter);
    UNUSED(line);
    softpc_pic_signal_cpu();
}

void ica_eoi(adapter, line, rotate)
IU32 adapter;
IS32 *line;
IBOOL rotate;
{
    IU8 bit;
    UNUSED(rotate);
    if (adapter > ICA_SLAVE || line == NULL) return;
    for (bit = 0u; bit < 8u; ++bit)
        if ((softpc_pics[adapter].isr & (IU8)(1u << bit)) != 0u) {
            softpc_pics[adapter].isr &= (IU8)~(1u << bit);
            *line = bit;
            return;
        }
}

IBOOL action_interrupt(adapter, line, func, parm)
IU32 adapter;
IU32 line;
ICA_CALLBACK func;
IU32 parm;
{
    UNUSED(adapter); UNUSED(line); UNUSED(func); UNUSED(parm);
    return FALSE;
}

void cancel_action_interrupt(adapter, line)
IU32 adapter;
IU32 line;
{
    UNUSED(adapter); UNUSED(line);
}

void ica_async_hw_interrupt(adapter, line, count)
IU32 adapter;
IU32 line;
IS32 count;
{
    SWPIC_hw_interrupt(adapter, line, count);
}
