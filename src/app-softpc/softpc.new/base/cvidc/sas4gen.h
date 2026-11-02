/*[
 * Generated File: sas4gen.h
 *
]*/

typedef	IU32	TYPE_sas_memory_size	IPT0();
typedef	void	TYPE_sas_connect_memory	IPT3(IU32,	lo_addr, IU32,	Int_addr, SAS_MEM_TYPE,	type);
typedef	void	TYPE_sas_enable_20_bit_wrapping	IPT0();
typedef	void	TYPE_sas_disable_20_bit_wrapping	IPT0();
typedef	IBOOL	TYPE_sas_twenty_bit_wrapping_enabled	IPT0();
typedef	SAS_MEM_TYPE	TYPE_sas_memory_type	IPT1(IU32,	addr);
typedef	IU8	TYPE_sas_hw_at	IPT1(IU32,	addr);
typedef	IU16	TYPE_sas_w_at	IPT1(IU32,	addr);
typedef	IU32	TYPE_sas_dw_at	IPT1(IU32,	addr);
typedef	IU8	TYPE_sas_hw_at_no_check	IPT1(IU32,	addr);
typedef	IU16	TYPE_sas_w_at_no_check	IPT1(IU32,	addr);
typedef	IU32	TYPE_sas_dw_at_no_check	IPT1(IU32,	addr);
typedef	void	TYPE_sas_store	IPT2(IU32,	addr, IU8,	val);
typedef	void	TYPE_sas_storew	IPT2(IU32,	addr, IU16,	val);
typedef	void	TYPE_sas_storedw	IPT2(IU32,	addr, IU32,	val);
typedef	void	TYPE_sas_store_no_check	IPT2(IU32,	addr, IU8,	val);
typedef	void	TYPE_sas_storew_no_check	IPT2(IU32,	addr, IU16,	val);
typedef	void	TYPE_sas_storedw_no_check	IPT2(IU32,	addr, IU32,	val);
typedef	void	TYPE_sas_loads	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
typedef	void	TYPE_sas_stores	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
typedef	void	TYPE_sas_loads_no_check	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
typedef	void	TYPE_sas_stores_no_check	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
typedef	void	TYPE_sas_move_bytes_forward	IPT3(IU32,	src, IU32,	dest, IU32,	len);
typedef	void	TYPE_sas_move_words_forward	IPT3(IU32,	src, IU32,	dest, IU32,	len);
typedef	void	TYPE_sas_move_doubles_forward	IPT3(IU32,	src, IU32,	dest, IU32,	len);
typedef	void	TYPE_sas_move_bytes_backward	IPT3(IU32,	src, IU32,	dest, IU32,	len);
typedef	void	TYPE_sas_move_words_backward	IPT3(IU32,	src, IU32,	dest, IU32,	len);
typedef	void	TYPE_sas_move_doubles_backward	IPT3(IU32,	src, IU32,	dest, IU32,	len);
typedef	void	TYPE_sas_fills	IPT3(IU32,	dest, IU8,	val, IU32,	len);
typedef	void	TYPE_sas_fillsw	IPT3(IU32,	dest, IU16,	val, IU32,	len);
typedef	void	TYPE_sas_fillsdw	IPT3(IU32,	dest, IU32,	val, IU32,	len);
typedef	IU8 *	TYPE_sas_scratch_address	IPT1(IU32,	length);
typedef	IU8 *	TYPE_sas_transbuf_address	IPT2(IU32,	dest_addr, IU32,	length);
typedef	void	TYPE_sas_loads_to_transbuf	IPT3(IU32,	src_addr, IU8 *,	dest_addr, IU32,	length);
typedef	void	TYPE_sas_stores_from_transbuf	IPT3(IU32,	dest_addr, IU8 *,	src_addr, IU32,	length);
typedef	IU8	TYPE_sas_PR8	IPT1(IU32,	addr);
typedef	IU16	TYPE_sas_PR16	IPT1(IU32,	addr);
typedef	IU32	TYPE_sas_PR32	IPT1(IU32,	addr);
typedef	void	TYPE_sas_PW8	IPT2(IU32,	addr, IU8,	val);
typedef	void	TYPE_sas_PW16	IPT2(IU32,	addr, IU16,	val);
typedef	void	TYPE_sas_PW32	IPT2(IU32,	addr, IU32,	val);
typedef	void	TYPE_sas_PW8_no_check	IPT2(IU32,	addr, IU8,	val);
typedef	void	TYPE_sas_PW16_no_check	IPT2(IU32,	addr, IU16,	val);
typedef	void	TYPE_sas_PW32_no_check	IPT2(IU32,	addr, IU32,	val);
typedef	IU8 *	TYPE_getPtrToPhysAddrByte	IPT1(IU32,	phys_addr);
typedef	IU8 *	TYPE_get_byte_addr	IPT1(IU32,	phys_addr);
typedef	IU8 *	TYPE_getPtrToLinAddrByte	IPT1(IU32,	lin_addr);
typedef	IBOOL	TYPE_sas_init_pm_selectors	IPT2(IU16,	sel1, IU16,	sel2);
typedef	void	TYPE_sas_overwrite_memory	IPT2(IU32,	addr, IU32,	length);
typedef	void	TYPE_sas_PWS	IPT3(IU32,	dest, IU8 *,	src, IU32,	len);
typedef	void	TYPE_sas_PWS_no_check	IPT3(IU32,	dest, IU8 *,	src, IU32,	len);
typedef	void	TYPE_sas_PRS	IPT3(IU32,	src, IU8 *,	dest, IU32,	len);
typedef	void	TYPE_sas_PRS_no_check	IPT3(IU32,	src, IU8 *,	dest, IU32,	len);
typedef	IBOOL	TYPE_sas_PigCmpPage	IPT3(IU32,	src, IU8 *,	dest, IU32,	len);
typedef	IBOOL	TYPE_IOVirtualised	IPT4(IU16,	port, IU32 *,	value, IU32,	offset, IU8,	width);

struct	SasVector	{
	IU32	(*Sas_memory_size)	IPT0();
	void	(*Sas_connect_memory)	IPT3(IU32,	lo_addr, IU32,	Int_addr, SAS_MEM_TYPE,	type);
	void	(*Sas_enable_20_bit_wrapping)	IPT0();
	void	(*Sas_disable_20_bit_wrapping)	IPT0();
	IBOOL	(*Sas_twenty_bit_wrapping_enabled)	IPT0();
	SAS_MEM_TYPE	(*Sas_memory_type)	IPT1(IU32,	addr);
	IU8	(*Sas_hw_at)	IPT1(IU32,	addr);
	IU16	(*Sas_w_at)	IPT1(IU32,	addr);
	IU32	(*Sas_dw_at)	IPT1(IU32,	addr);
	IU8	(*Sas_hw_at_no_check)	IPT1(IU32,	addr);
	IU16	(*Sas_w_at_no_check)	IPT1(IU32,	addr);
	IU32	(*Sas_dw_at_no_check)	IPT1(IU32,	addr);
	void	(*Sas_store)	IPT2(IU32,	addr, IU8,	val);
	void	(*Sas_storew)	IPT2(IU32,	addr, IU16,	val);
	void	(*Sas_storedw)	IPT2(IU32,	addr, IU32,	val);
	void	(*Sas_store_no_check)	IPT2(IU32,	addr, IU8,	val);
	void	(*Sas_storew_no_check)	IPT2(IU32,	addr, IU16,	val);
	void	(*Sas_storedw_no_check)	IPT2(IU32,	addr, IU32,	val);
	void	(*Sas_loads)	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
	void	(*Sas_stores)	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
	void	(*Sas_loads_no_check)	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
	void	(*Sas_stores_no_check)	IPT3(IU32,	addr, IU8 *,	stringptr, IU32,	len);
	void	(*Sas_move_bytes_forward)	IPT3(IU32,	src, IU32,	dest, IU32,	len);
	void	(*Sas_move_words_forward)	IPT3(IU32,	src, IU32,	dest, IU32,	len);
	void	(*Sas_move_doubles_forward)	IPT3(IU32,	src, IU32,	dest, IU32,	len);
	void	(*Sas_move_bytes_backward)	IPT3(IU32,	src, IU32,	dest, IU32,	len);
	void	(*Sas_move_words_backward)	IPT3(IU32,	src, IU32,	dest, IU32,	len);
	void	(*Sas_move_doubles_backward)	IPT3(IU32,	src, IU32,	dest, IU32,	len);
	void	(*Sas_fills)	IPT3(IU32,	dest, IU8,	val, IU32,	len);
	void	(*Sas_fillsw)	IPT3(IU32,	dest, IU16,	val, IU32,	len);
	void	(*Sas_fillsdw)	IPT3(IU32,	dest, IU32,	val, IU32,	len);
	IU8 *	(*Sas_scratch_address)	IPT1(IU32,	length);
	IU8 *	(*Sas_transbuf_address)	IPT2(IU32,	dest_addr, IU32,	length);
	void	(*Sas_loads_to_transbuf)	IPT3(IU32,	src_addr, IU8 *,	dest_addr, IU32,	length);
	void	(*Sas_stores_from_transbuf)	IPT3(IU32,	dest_addr, IU8 *,	src_addr, IU32,	length);
	IU8	(*Sas_PR8)	IPT1(IU32,	addr);
	IU16	(*Sas_PR16)	IPT1(IU32,	addr);
	IU32	(*Sas_PR32)	IPT1(IU32,	addr);
	void	(*Sas_PW8)	IPT2(IU32,	addr, IU8,	val);
	void	(*Sas_PW16)	IPT2(IU32,	addr, IU16,	val);
	void	(*Sas_PW32)	IPT2(IU32,	addr, IU32,	val);
	void	(*Sas_PW8_no_check)	IPT2(IU32,	addr, IU8,	val);
	void	(*Sas_PW16_no_check)	IPT2(IU32,	addr, IU16,	val);
	void	(*Sas_PW32_no_check)	IPT2(IU32,	addr, IU32,	val);
	IU8 *	(*SasPtrToPhysAddrByte)	IPT1(IU32,	phys_addr);
	IU8 *	(*Sas_get_byte_addr)	IPT1(IU32,	phys_addr);
	IU8 *	(*SasPtrToLinAddrByte)	IPT1(IU32,	lin_addr);
	IBOOL	(*SasRegisterVirtualSelectors)	IPT2(IU16,	sel1, IU16,	sel2);
	void	(*Sas_overwrite_memory)	IPT2(IU32,	addr, IU32,	length);
	void	(*Sas_PWS)	IPT3(IU32,	dest, IU8 *,	src, IU32,	len);
	void	(*Sas_PWS_no_check)	IPT3(IU32,	dest, IU8 *,	src, IU32,	len);
	void	(*Sas_PRS)	IPT3(IU32,	src, IU8 *,	dest, IU32,	len);
	void	(*Sas_PRS_no_check)	IPT3(IU32,	src, IU8 *,	dest, IU32,	len);
	IBOOL	(*Sas_PigCmpPage)	IPT3(IU32,	src, IU8 *,	dest, IU32,	len);
	IBOOL	(*IOVirtualised)	IPT4(IU16,	port, IU32 *,	value, IU32,	offset, IU8,	width);
};

extern	struct	SasVector	Sas;

#ifdef	CCPU
IMPORT	IU32	c_sas_memory_size	IPT0();
#define	sas_memory_size()	c_sas_memory_size()
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 134

#ifdef	CCPU
IMPORT	void	c_sas_connect_memory	IPT3(IU32, lo_addr, IU32, Int_addr, SAS_MEM_TYPE, type);
#define	sas_connect_memory(lo_addr, Int_addr, type)	c_sas_connect_memory(lo_addr, Int_addr, type)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 147

#ifdef	CCPU
IMPORT	void	c_sas_enable_20_bit_wrapping	IPT0();
#define	sas_enable_20_bit_wrapping()	c_sas_enable_20_bit_wrapping()
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 160

#ifdef	CCPU
IMPORT	void	c_sas_disable_20_bit_wrapping	IPT0();
#define	sas_disable_20_bit_wrapping()	c_sas_disable_20_bit_wrapping()
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 173

#ifdef	CCPU
IMPORT	IBOOL	c_sas_twenty_bit_wrapping_enabled	IPT0();
#define	sas_twenty_bit_wrapping_enabled()	c_sas_twenty_bit_wrapping_enabled()
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 186

#ifdef	CCPU
IMPORT	SAS_MEM_TYPE	c_sas_memory_type	IPT1(IU32, addr);
#define	sas_memory_type(addr)	c_sas_memory_type(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 199

#ifdef	CCPU
IMPORT	IU8	c_sas_hw_at	IPT1(IU32, addr);
#define	sas_hw_at(addr)	c_sas_hw_at(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 212

#ifdef	CCPU
IMPORT	IU16	c_sas_w_at	IPT1(IU32, addr);
#define	sas_w_at(addr)	c_sas_w_at(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 225

#ifdef	CCPU
IMPORT	IU32	c_sas_dw_at	IPT1(IU32, addr);
#define	sas_dw_at(addr)	c_sas_dw_at(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 238

#ifdef	CCPU
IMPORT	IU8	c_sas_hw_at	IPT1(IU32, addr);
#define	sas_hw_at_no_check(addr)	c_sas_hw_at(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 251

#ifdef	CCPU
IMPORT	IU16	c_sas_w_at	IPT1(IU32, addr);
#define	sas_w_at_no_check(addr)	c_sas_w_at(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 264

#ifdef	CCPU
IMPORT	IU32	c_sas_dw_at	IPT1(IU32, addr);
#define	sas_dw_at_no_check(addr)	c_sas_dw_at(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 277

#ifdef	CCPU
IMPORT	void	c_sas_store	IPT2(IU32, addr, IU8, val);
#define	sas_store(addr, val)	c_sas_store(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 290

#ifdef	CCPU
IMPORT	void	c_sas_storew	IPT2(IU32, addr, IU16, val);
#define	sas_storew(addr, val)	c_sas_storew(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 303

#ifdef	CCPU
IMPORT	void	c_sas_storedw	IPT2(IU32, addr, IU32, val);
#define	sas_storedw(addr, val)	c_sas_storedw(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 316

#ifdef	CCPU
IMPORT	void	c_sas_store	IPT2(IU32, addr, IU8, val);
#define	sas_store_no_check(addr, val)	c_sas_store(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 329

#ifdef	CCPU
IMPORT	void	c_sas_storew	IPT2(IU32, addr, IU16, val);
#define	sas_storew_no_check(addr, val)	c_sas_storew(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 342

#ifdef	CCPU
IMPORT	void	c_sas_storedw	IPT2(IU32, addr, IU32, val);
#define	sas_storedw_no_check(addr, val)	c_sas_storedw(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 355

#ifdef	CCPU
IMPORT	void	c_sas_loads	IPT3(IU32, addr, IU8 *, stringptr, IU32, len);
#define	sas_loads(addr, stringptr, len)	c_sas_loads(addr, stringptr, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 368

#ifdef	CCPU
IMPORT	void	c_sas_stores	IPT3(IU32, addr, IU8 *, stringptr, IU32, len);
#define	sas_stores(addr, stringptr, len)	c_sas_stores(addr, stringptr, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 381

#ifdef	CCPU
IMPORT	void	c_sas_loads_no_check	IPT3(IU32, addr, IU8 *, stringptr, IU32, len);
#define	sas_loads_no_check(addr, stringptr, len)	c_sas_loads_no_check(addr, stringptr, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 394

#ifdef	CCPU
IMPORT	void	c_sas_stores_no_check	IPT3(IU32, addr, IU8 *, stringptr, IU32, len);
#define	sas_stores_no_check(addr, stringptr, len)	c_sas_stores_no_check(addr, stringptr, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 407

#ifdef	CCPU
IMPORT	void	c_sas_move_bytes_forward	IPT3(IU32, src, IU32, dest, IU32, len);
#define	sas_move_bytes_forward(src, dest, len)	c_sas_move_bytes_forward(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 420

#ifdef	CCPU
IMPORT	void	c_sas_move_words_forward	IPT3(IU32, src, IU32, dest, IU32, len);
#define	sas_move_words_forward(src, dest, len)	c_sas_move_words_forward(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 433

#ifdef	CCPU
IMPORT	void	c_sas_move_doubles_forward	IPT3(IU32, src, IU32, dest, IU32, len);
#define	sas_move_doubles_forward(src, dest, len)	c_sas_move_doubles_forward(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 446

#ifdef	CCPU
IMPORT	void	c_sas_move_bytes_backward	IPT3(IU32, src, IU32, dest, IU32, len);
#define	sas_move_bytes_backward(src, dest, len)	c_sas_move_bytes_backward(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 459

#ifdef	CCPU
IMPORT	void	c_sas_move_words_backward	IPT3(IU32, src, IU32, dest, IU32, len);
#define	sas_move_words_backward(src, dest, len)	c_sas_move_words_backward(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 472

#ifdef	CCPU
IMPORT	void	c_sas_move_doubles_backward	IPT3(IU32, src, IU32, dest, IU32, len);
#define	sas_move_doubles_backward(src, dest, len)	c_sas_move_doubles_backward(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 485

#ifdef	CCPU
IMPORT	void	c_sas_fills	IPT3(IU32, dest, IU8, val, IU32, len);
#define	sas_fills(dest, val, len)	c_sas_fills(dest, val, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 498

#ifdef	CCPU
IMPORT	void	c_sas_fillsw	IPT3(IU32, dest, IU16, val, IU32, len);
#define	sas_fillsw(dest, val, len)	c_sas_fillsw(dest, val, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 511

#ifdef	CCPU
IMPORT	void	c_sas_fillsdw	IPT3(IU32, dest, IU32, val, IU32, len);
#define	sas_fillsdw(dest, val, len)	c_sas_fillsdw(dest, val, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 524

#ifdef	CCPU
IMPORT	IU8 *	c_sas_scratch_address	IPT1(IU32, length);
#define	sas_scratch_address(length)	c_sas_scratch_address(length)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 537

#ifdef	CCPU
IMPORT	IU8 *	c_sas_transbuf_address	IPT2(IU32, dest_addr, IU32, length);
#define	sas_transbuf_address(dest_addr, length)	c_sas_transbuf_address(dest_addr, length)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 550

#ifdef	CCPU
IMPORT	void	c_sas_loads	IPT3(IU32, src_addr, IU8 *, dest_addr, IU32, length);
#define	sas_loads_to_transbuf(src_addr, dest_addr, length)	c_sas_loads(src_addr, dest_addr, length)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 563

#ifdef	CCPU
IMPORT	void	c_sas_stores	IPT3(IU32, dest_addr, IU8 *, src_addr, IU32, length);
#define	sas_stores_from_transbuf(dest_addr, src_addr, length)	c_sas_stores(dest_addr, src_addr, length)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 576

#ifdef	CCPU
IMPORT	IU8	phy_r8	IPT1(IU32, addr);
#define	sas_PR8(addr)	phy_r8(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 589

#ifdef	CCPU
IMPORT	IU16	phy_r16	IPT1(IU32, addr);
#define	sas_PR16(addr)	phy_r16(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 602

#ifdef	CCPU
IMPORT	IU32	phy_r32	IPT1(IU32, addr);
#define	sas_PR32(addr)	phy_r32(addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 615

#ifdef	CCPU
IMPORT	void	phy_w8	IPT2(IU32, addr, IU8, val);
#define	sas_PW8(addr, val)	phy_w8(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 628

#ifdef	CCPU
IMPORT	void	phy_w16	IPT2(IU32, addr, IU16, val);
#define	sas_PW16(addr, val)	phy_w16(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 641

#ifdef	CCPU
IMPORT	void	phy_w32	IPT2(IU32, addr, IU32, val);
#define	sas_PW32(addr, val)	phy_w32(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 654

#ifdef	CCPU
IMPORT	void	phy_w8_no_check	IPT2(IU32, addr, IU8, val);
#define	sas_PW8_no_check(addr, val)	phy_w8_no_check(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 667

#ifdef	CCPU
IMPORT	void	phy_w16_no_check	IPT2(IU32, addr, IU16, val);
#define	sas_PW16_no_check(addr, val)	phy_w16_no_check(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 680

#ifdef	CCPU
IMPORT	void	phy_w32_no_check	IPT2(IU32, addr, IU32, val);
#define	sas_PW32_no_check(addr, val)	phy_w32_no_check(addr, val)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 693

#ifdef	CCPU
IMPORT	IU8 *	c_GetPhyAdd	IPT1(IU32, phys_addr);
#define	getPtrToPhysAddrByte(phys_addr)	c_GetPhyAdd(phys_addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 706

#ifdef	CCPU
IMPORT	IU8 *	c_get_byte_addr	IPT1(IU32, phys_addr);
#define	get_byte_addr(phys_addr)	c_get_byte_addr(phys_addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 719

#ifdef	CCPU
IMPORT	IU8 *	c_GetLinAdd	IPT1(IU32, lin_addr);
#define	getPtrToLinAddrByte(lin_addr)	c_GetLinAdd(lin_addr)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 732

#ifdef	CCPU
IMPORT	IBOOL	c_SasRegisterVirtualSelectors	IPT2(IU16, sel1, IU16, sel2);
#define	sas_init_pm_selectors(sel1, sel2)	c_SasRegisterVirtualSelectors(sel1, sel2)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 745

#ifdef	CCPU

#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 757

#ifdef	CCPU
IMPORT	void	c_sas_PWS	IPT3(IU32, dest, IU8 *, src, IU32, len);
#define	sas_PWS(dest, src, len)	c_sas_PWS(dest, src, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 770

#ifdef	CCPU
IMPORT	void	c_sas_PWS_no_check	IPT3(IU32, dest, IU8 *, src, IU32, len);
#define	sas_PWS_no_check(dest, src, len)	c_sas_PWS_no_check(dest, src, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 783

#ifdef	CCPU
IMPORT	void	c_sas_PRS	IPT3(IU32, src, IU8 *, dest, IU32, len);
#define	sas_PRS(src, dest, len)	c_sas_PRS(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 796

#ifdef	CCPU
IMPORT	void	c_sas_PRS_no_check	IPT3(IU32, src, IU8 *, dest, IU32, len);
#define	sas_PRS_no_check(src, dest, len)	c_sas_PRS_no_check(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 809

#ifdef	CCPU
IMPORT	IBOOL	c_sas_PigCmpPage	IPT3(IU32, src, IU8 *, dest, IU32, len);
#define	sas_PigCmpPage(src, dest, len)	c_sas_PigCmpPage(src, dest, len)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 822

#ifdef	CCPU
IMPORT	IBOOL	c_IOVirtualised	IPT4(IU16, port, IU32 *, value, IU32, offset, IU8, width);
#define	IOVirtualised(port, value, offset, width)	c_IOVirtualised(port, value, offset, width)
#else	/* CCPU */
#error "Unsupported SoftPC mirror configuration: non-CCPU"
#endif	/* CCPU */
#line 835

/*======================================== END ========================================*/

