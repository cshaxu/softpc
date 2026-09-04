/* Generated from the selected CCPU GDP layout. */
#ifndef SOFTPC_GDP_SLOTS_H
#define SOFTPC_GDP_SLOTS_H

#include "gdp_state.h"

/* Replace only direct fixed-base forms after the historical header has
 * established every original spelling and helper macro. */
#undef GLOBAL_HookNumber
#define GLOBAL_HookNumber (*((IUH *)softpc_gdp_slot((const void *)Gdp, 0u, sizeof(IUH))))
#undef GLOBAL_EfiNumber
#define GLOBAL_EfiNumber (*((IUH *)softpc_gdp_slot((const void *)Gdp, 4u, sizeof(IUH))))
#undef GLOBAL_SubrRingPtr
#define GLOBAL_SubrRingPtr (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 8u, sizeof(IUH*))))
#undef GLOBAL_SubrRingLowIncl
#define GLOBAL_SubrRingLowIncl (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 12u, sizeof(IUH*))))
#undef GLOBAL_SubrRingHighIncl
#define GLOBAL_SubrRingHighIncl (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 16u, sizeof(IUH*))))
#undef GLOBAL_WhichAssertion
#define GLOBAL_WhichAssertion (*((IUH *)softpc_gdp_slot((const void *)Gdp, 20u, sizeof(IUH))))
#undef GLOBAL_WhereAmI
#define GLOBAL_WhereAmI (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 24u, sizeof(IU32*))))
#undef GLOBAL_CoRoRet
#define GLOBAL_CoRoRet (*((IUH *)softpc_gdp_slot((const void *)Gdp, 28u, sizeof(IUH))))
#undef GLOBAL_ErrorNumber
#define GLOBAL_ErrorNumber (*((IUH *)softpc_gdp_slot((const void *)Gdp, 32u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_0
#define GLOBAL_EDL_WORKSPACE_0 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 36u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_1
#define GLOBAL_EDL_WORKSPACE_1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 40u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_2
#define GLOBAL_EDL_WORKSPACE_2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 44u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_3
#define GLOBAL_EDL_WORKSPACE_3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 48u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_4
#define GLOBAL_EDL_WORKSPACE_4 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 52u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_5
#define GLOBAL_EDL_WORKSPACE_5 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 56u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_6
#define GLOBAL_EDL_WORKSPACE_6 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 60u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_7
#define GLOBAL_EDL_WORKSPACE_7 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 64u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_8
#define GLOBAL_EDL_WORKSPACE_8 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 68u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_9
#define GLOBAL_EDL_WORKSPACE_9 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 72u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_10
#define GLOBAL_EDL_WORKSPACE_10 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 76u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_11
#define GLOBAL_EDL_WORKSPACE_11 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 80u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_12
#define GLOBAL_EDL_WORKSPACE_12 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 84u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_13
#define GLOBAL_EDL_WORKSPACE_13 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 88u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_14
#define GLOBAL_EDL_WORKSPACE_14 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 92u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_15
#define GLOBAL_EDL_WORKSPACE_15 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 96u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_16
#define GLOBAL_EDL_WORKSPACE_16 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 100u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_17
#define GLOBAL_EDL_WORKSPACE_17 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 104u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_18
#define GLOBAL_EDL_WORKSPACE_18 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 108u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_19
#define GLOBAL_EDL_WORKSPACE_19 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 112u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_20
#define GLOBAL_EDL_WORKSPACE_20 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 116u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_21
#define GLOBAL_EDL_WORKSPACE_21 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 120u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_22
#define GLOBAL_EDL_WORKSPACE_22 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 124u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_23
#define GLOBAL_EDL_WORKSPACE_23 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 128u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_24
#define GLOBAL_EDL_WORKSPACE_24 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 132u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_25
#define GLOBAL_EDL_WORKSPACE_25 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 136u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_26
#define GLOBAL_EDL_WORKSPACE_26 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 140u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_27
#define GLOBAL_EDL_WORKSPACE_27 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 144u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_28
#define GLOBAL_EDL_WORKSPACE_28 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 148u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_29
#define GLOBAL_EDL_WORKSPACE_29 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 152u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_30
#define GLOBAL_EDL_WORKSPACE_30 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 156u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_31
#define GLOBAL_EDL_WORKSPACE_31 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 160u, sizeof(IUH))))
#undef GLOBAL_R_EAX
#define GLOBAL_R_EAX (*((IUH *)softpc_gdp_slot((const void *)Gdp, 164u, sizeof(IUH))))
#undef GLOBAL_R_EBX
#define GLOBAL_R_EBX (*((IUH *)softpc_gdp_slot((const void *)Gdp, 168u, sizeof(IUH))))
#undef GLOBAL_R_ECX
#define GLOBAL_R_ECX (*((IUH *)softpc_gdp_slot((const void *)Gdp, 172u, sizeof(IUH))))
#undef GLOBAL_R_EDX
#define GLOBAL_R_EDX (*((IUH *)softpc_gdp_slot((const void *)Gdp, 176u, sizeof(IUH))))
#undef GLOBAL_R_EBP
#define GLOBAL_R_EBP (*((IUH *)softpc_gdp_slot((const void *)Gdp, 180u, sizeof(IUH))))
#undef GLOBAL_R_ESI
#define GLOBAL_R_ESI (*((IUH *)softpc_gdp_slot((const void *)Gdp, 184u, sizeof(IUH))))
#undef GLOBAL_R_EDI
#define GLOBAL_R_EDI (*((IUH *)softpc_gdp_slot((const void *)Gdp, 188u, sizeof(IUH))))
#undef GLOBAL_EsSel
#define GLOBAL_EsSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 194u, sizeof(IU16))))
#undef GLOBAL_EsBase
#define GLOBAL_EsBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 196u, sizeof(IU32))))
#undef GLOBAL_EsDesc
#define GLOBAL_EsDesc (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 200u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_EsStruct_rlimit
#define GLOBAL_EsStruct_rlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 204u, sizeof(IU32))))
#undef GLOBAL_EsStruct_wlimit
#define GLOBAL_EsStruct_wlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 208u, sizeof(IU32))))
#undef GLOBAL_EsDescSanctuary
#define GLOBAL_EsDescSanctuary (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 212u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_CsSel
#define GLOBAL_CsSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 218u, sizeof(IU16))))
#undef GLOBAL_CsBase
#define GLOBAL_CsBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 220u, sizeof(IU32))))
#undef GLOBAL_CsDesc
#define GLOBAL_CsDesc (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 224u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_CsStruct_rlimit
#define GLOBAL_CsStruct_rlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 228u, sizeof(IU32))))
#undef GLOBAL_CsStruct_wlimit
#define GLOBAL_CsStruct_wlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 232u, sizeof(IU32))))
#undef GLOBAL_CsDescSanctuary
#define GLOBAL_CsDescSanctuary (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 236u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_SsSel
#define GLOBAL_SsSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 242u, sizeof(IU16))))
#undef GLOBAL_SsBase
#define GLOBAL_SsBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 244u, sizeof(IU32))))
#undef GLOBAL_SsDesc
#define GLOBAL_SsDesc (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 248u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_SsStruct_rlimit
#define GLOBAL_SsStruct_rlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 252u, sizeof(IU32))))
#undef GLOBAL_SsStruct_wlimit
#define GLOBAL_SsStruct_wlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 256u, sizeof(IU32))))
#undef GLOBAL_SsDescSanctuary
#define GLOBAL_SsDescSanctuary (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 260u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_DsSel
#define GLOBAL_DsSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 266u, sizeof(IU16))))
#undef GLOBAL_DsBase
#define GLOBAL_DsBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 268u, sizeof(IU32))))
#undef GLOBAL_DsDesc
#define GLOBAL_DsDesc (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 272u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_DsStruct_rlimit
#define GLOBAL_DsStruct_rlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 276u, sizeof(IU32))))
#undef GLOBAL_DsStruct_wlimit
#define GLOBAL_DsStruct_wlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 280u, sizeof(IU32))))
#undef GLOBAL_DsDescSanctuary
#define GLOBAL_DsDescSanctuary (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 284u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_FsSel
#define GLOBAL_FsSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 290u, sizeof(IU16))))
#undef GLOBAL_FsBase
#define GLOBAL_FsBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 292u, sizeof(IU32))))
#undef GLOBAL_FsDesc
#define GLOBAL_FsDesc (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 296u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_FsStruct_rlimit
#define GLOBAL_FsStruct_rlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 300u, sizeof(IU32))))
#undef GLOBAL_FsStruct_wlimit
#define GLOBAL_FsStruct_wlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 304u, sizeof(IU32))))
#undef GLOBAL_FsDescSanctuary
#define GLOBAL_FsDescSanctuary (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 308u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_GsSel
#define GLOBAL_GsSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 314u, sizeof(IU16))))
#undef GLOBAL_GsBase
#define GLOBAL_GsBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 316u, sizeof(IU32))))
#undef GLOBAL_GsDesc
#define GLOBAL_GsDesc (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 320u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_GsStruct_rlimit
#define GLOBAL_GsStruct_rlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 324u, sizeof(IU32))))
#undef GLOBAL_GsStruct_wlimit
#define GLOBAL_GsStruct_wlimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 328u, sizeof(IU32))))
#undef GLOBAL_GsDescSanctuary
#define GLOBAL_GsDescSanctuary (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 332u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_CPL
#define GLOBAL_CPL (*((IUH *)softpc_gdp_slot((const void *)Gdp, 336u, sizeof(IUH))))
#undef GLOBAL_GdtrBase
#define GLOBAL_GdtrBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 340u, sizeof(IU32))))
#undef GLOBAL_GdtrLimit
#define GLOBAL_GdtrLimit (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 346u, sizeof(IU16))))
#undef GLOBAL_LdtSel
#define GLOBAL_LdtSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 350u, sizeof(IU16))))
#undef GLOBAL_LdtrBase
#define GLOBAL_LdtrBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 352u, sizeof(IU32))))
#undef GLOBAL_LdtrLimit
#define GLOBAL_LdtrLimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 356u, sizeof(IU32))))
#undef GLOBAL_TrSel
#define GLOBAL_TrSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 362u, sizeof(IU16))))
#undef GLOBAL_TrBase
#define GLOBAL_TrBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 364u, sizeof(IU32))))
#undef GLOBAL_TrLimit
#define GLOBAL_TrLimit (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 368u, sizeof(IU32))))
#undef GLOBAL_TrDescSt
#define GLOBAL_TrDescSt (*((IUH *)softpc_gdp_slot((const void *)Gdp, 372u, sizeof(IUH))))
#undef GLOBAL_TrIoBase
#define GLOBAL_TrIoBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 376u, sizeof(IU32))))
#undef GLOBAL_TrIoLimit
#define GLOBAL_TrIoLimit (*((IS32 *)softpc_gdp_slot((const void *)Gdp, 380u, sizeof(IS32))))
#undef GLOBAL_IdtrBase
#define GLOBAL_IdtrBase (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 384u, sizeof(IU32))))
#undef GLOBAL_IdtrLimit
#define GLOBAL_IdtrLimit (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 390u, sizeof(IU16))))
#undef GLOBAL_AbortPigRun
#define GLOBAL_AbortPigRun (*((IUH *)softpc_gdp_slot((const void *)Gdp, 392u, sizeof(IUH))))
#undef GLOBAL_RegsAndFlagsUndefined
#define GLOBAL_RegsAndFlagsUndefined (*((IUH *)softpc_gdp_slot((const void *)Gdp, 396u, sizeof(IUH))))
#undef GLOBAL_SigalrmOccurred
#define GLOBAL_SigalrmOccurred (*((IUH *)softpc_gdp_slot((const void *)Gdp, 400u, sizeof(IUH))))
#undef GLOBAL_PigEnabled
#define GLOBAL_PigEnabled (*((IUH *)softpc_gdp_slot((const void *)Gdp, 404u, sizeof(IUH))))
#undef GLOBAL_EFLAGS
#define GLOBAL_EFLAGS (*((IUH *)softpc_gdp_slot((const void *)Gdp, 408u, sizeof(IUH))))
#undef GLOBAL_Ft
#define GLOBAL_Ft (*((IUH *)softpc_gdp_slot((const void *)Gdp, 412u, sizeof(IUH))))
#undef GLOBAL_F1
#define GLOBAL_F1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 416u, sizeof(IUH))))
#undef GLOBAL_F2
#define GLOBAL_F2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 420u, sizeof(IUH))))
#undef GLOBAL_F3
#define GLOBAL_F3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 424u, sizeof(IUH))))
#undef GLOBAL_R_CR0
#define GLOBAL_R_CR0 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 428u, sizeof(IUH))))
#undef GLOBAL_R_CR1
#define GLOBAL_R_CR1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 432u, sizeof(IUH))))
#undef GLOBAL_R_CR2
#define GLOBAL_R_CR2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 436u, sizeof(IUH))))
#undef GLOBAL_R_CR3
#define GLOBAL_R_CR3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 440u, sizeof(IUH))))
#undef GLOBAL_R_CR4
#define GLOBAL_R_CR4 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 444u, sizeof(IUH))))
#undef GLOBAL_R_CR5
#define GLOBAL_R_CR5 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 448u, sizeof(IUH))))
#undef GLOBAL_R_CR6
#define GLOBAL_R_CR6 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 452u, sizeof(IUH))))
#undef GLOBAL_R_CR7
#define GLOBAL_R_CR7 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 456u, sizeof(IUH))))
#undef GLOBAL_R_TR0
#define GLOBAL_R_TR0 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 460u, sizeof(IUH))))
#undef GLOBAL_R_TR1
#define GLOBAL_R_TR1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 464u, sizeof(IUH))))
#undef GLOBAL_R_TR2
#define GLOBAL_R_TR2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 468u, sizeof(IUH))))
#undef GLOBAL_R_TR3
#define GLOBAL_R_TR3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 472u, sizeof(IUH))))
#undef GLOBAL_R_TR4
#define GLOBAL_R_TR4 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 476u, sizeof(IUH))))
#undef GLOBAL_R_TR5
#define GLOBAL_R_TR5 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 480u, sizeof(IUH))))
#undef GLOBAL_R_TR6
#define GLOBAL_R_TR6 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 484u, sizeof(IUH))))
#undef GLOBAL_R_TR7
#define GLOBAL_R_TR7 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 488u, sizeof(IUH))))
#undef GLOBAL_R_DR0
#define GLOBAL_R_DR0 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 492u, sizeof(IUH))))
#undef GLOBAL_R_DR1
#define GLOBAL_R_DR1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 496u, sizeof(IUH))))
#undef GLOBAL_R_DR2
#define GLOBAL_R_DR2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 500u, sizeof(IUH))))
#undef GLOBAL_R_DR3
#define GLOBAL_R_DR3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 504u, sizeof(IUH))))
#undef GLOBAL_R_DR4
#define GLOBAL_R_DR4 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 508u, sizeof(IUH))))
#undef GLOBAL_R_DR5
#define GLOBAL_R_DR5 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 512u, sizeof(IUH))))
#undef GLOBAL_R_DR6
#define GLOBAL_R_DR6 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 516u, sizeof(IUH))))
#undef GLOBAL_R_DR7
#define GLOBAL_R_DR7 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 520u, sizeof(IUH))))
#undef GLOBAL_PARAM1
#define GLOBAL_PARAM1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 524u, sizeof(IUH))))
#undef GLOBAL_PARAM2
#define GLOBAL_PARAM2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 528u, sizeof(IUH))))
#undef GLOBAL_PARAM3
#define GLOBAL_PARAM3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 532u, sizeof(IUH))))
#undef GLOBAL_BytePtr
#define GLOBAL_BytePtr (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 536u, sizeof(IU8*))))
#undef GLOBAL_WordPtr
#define GLOBAL_WordPtr (*((IU16* *)softpc_gdp_slot((const void *)Gdp, 540u, sizeof(IU16*))))
#undef GLOBAL_DwordPtr
#define GLOBAL_DwordPtr (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 544u, sizeof(IU32*))))
#undef GLOBAL_EaOffs
#define GLOBAL_EaOffs (*((IUH *)softpc_gdp_slot((const void *)Gdp, 548u, sizeof(IUH))))
#undef GLOBAL_DATA1
#define GLOBAL_DATA1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 552u, sizeof(IUH))))
#undef GLOBAL_DATA2
#define GLOBAL_DATA2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 556u, sizeof(IUH))))
#undef GLOBAL_InNanoCpu
#define GLOBAL_InNanoCpu (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 563u, sizeof(IBOOL))))
#undef GLOBAL_UseNanoCpu
#define GLOBAL_UseNanoCpu (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 567u, sizeof(IBOOL))))
#undef GLOBAL_UseLightCompiler
#define GLOBAL_UseLightCompiler (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 571u, sizeof(IBOOL))))
#undef GLOBAL_UseCCodeCopier
#define GLOBAL_UseCCodeCopier (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 575u, sizeof(IBOOL))))
#undef GLOBAL_seenWithFlags
#define GLOBAL_seenWithFlags (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 579u, sizeof(IBOOL))))
#undef GLOBAL_needNextIntelEip
#define GLOBAL_needNextIntelEip (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 583u, sizeof(IBOOL))))
#undef GLOBAL_LastSetCopierUniverse
#define GLOBAL_LastSetCopierUniverse (*((struct EntryPointCacheREC* *)softpc_gdp_slot((const void *)Gdp, 584u, sizeof(struct EntryPointCacheREC*))))
#undef GLOBAL_CopierUniverse
#define GLOBAL_CopierUniverse (*((struct EntryPointCacheREC* *)softpc_gdp_slot((const void *)Gdp, 588u, sizeof(struct EntryPointCacheREC*))))
#undef GLOBAL_CopierBitMap
#define GLOBAL_CopierBitMap (*((struct ConstraintBitMapREC* *)softpc_gdp_slot((const void *)Gdp, 592u, sizeof(struct ConstraintBitMapREC*))))
#undef GLOBAL_lastCopierBitMap
#define GLOBAL_lastCopierBitMap (*((struct ConstraintBitMapREC* *)softpc_gdp_slot((const void *)Gdp, 596u, sizeof(struct ConstraintBitMapREC*))))
#undef GLOBAL_currPFragInfoRec
#define GLOBAL_currPFragInfoRec (*((struct FragmentInfoREC* *)softpc_gdp_slot((const void *)Gdp, 600u, sizeof(struct FragmentInfoREC*))))
#undef GLOBAL_copierCleanups
#define GLOBAL_copierCleanups (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 604u, sizeof(IU8*))))
#undef GLOBAL_lastHostCleanup
#define GLOBAL_lastHostCleanup (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 608u, sizeof(IU8*))))
#undef GLOBAL_lastHostAddress
#define GLOBAL_lastHostAddress (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 612u, sizeof(IU32*))))
#undef GLOBAL_lastIntelAddress
#define GLOBAL_lastIntelAddress (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 616u, sizeof(IU32))))
#undef GLOBAL_destHashTable
#define GLOBAL_destHashTable (*((struct JUMP_REC** *)softpc_gdp_slot((const void *)Gdp, 620u, sizeof(struct JUMP_REC**))))
#undef GLOBAL_jumpHashTable
#define GLOBAL_jumpHashTable (*((struct JUMP_REC** *)softpc_gdp_slot((const void *)Gdp, 624u, sizeof(struct JUMP_REC**))))
#undef GLOBAL_freeJumpRecPtr
#define GLOBAL_freeJumpRecPtr (*((struct JUMP_REC* *)softpc_gdp_slot((const void *)Gdp, 628u, sizeof(struct JUMP_REC*))))
#undef GLOBAL_nextFreeJumpRec
#define GLOBAL_nextFreeJumpRec (*((struct JUMP_REC* *)softpc_gdp_slot((const void *)Gdp, 632u, sizeof(struct JUMP_REC*))))
#undef GLOBAL_freeJumpRecCount
#define GLOBAL_freeJumpRecCount (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 636u, sizeof(IU32))))
#undef GLOBAL_poolJumpRecCount
#define GLOBAL_poolJumpRecCount (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 640u, sizeof(IU32))))
#undef GLOBAL_vctPtrs
#define GLOBAL_vctPtrs (*((struct VCT_NODE_REC** *)softpc_gdp_slot((const void *)Gdp, 644u, sizeof(struct VCT_NODE_REC**))))
#undef GLOBAL_anonPtrs
#define GLOBAL_anonPtrs (*((struct VCT_NODE_REC** *)softpc_gdp_slot((const void *)Gdp, 648u, sizeof(struct VCT_NODE_REC**))))
#undef GLOBAL_tuples
#define GLOBAL_tuples (*((struct TUPLE_REC* *)softpc_gdp_slot((const void *)Gdp, 652u, sizeof(struct TUPLE_REC*))))
#undef GLOBAL_cursor
#define GLOBAL_cursor (*((struct TUPLE_REC* *)softpc_gdp_slot((const void *)Gdp, 656u, sizeof(struct TUPLE_REC*))))
#undef GLOBAL_tuplePtr
#define GLOBAL_tuplePtr (*((struct TUPLE_REC* *)softpc_gdp_slot((const void *)Gdp, 660u, sizeof(struct TUPLE_REC*))))
#undef GLOBAL_patchRecPtr
#define GLOBAL_patchRecPtr (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 664u, sizeof(IU8*))))
#undef GLOBAL_srcPtr
#define GLOBAL_srcPtr (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 668u, sizeof(IU32*))))
#undef GLOBAL_dstPtr
#define GLOBAL_dstPtr (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 672u, sizeof(IU32*))))
#undef GLOBAL_PatchMeBodyAddr
#define GLOBAL_PatchMeBodyAddr (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 676u, sizeof(IU32*))))
#undef GLOBAL_patchTable
#define GLOBAL_patchTable (*((IU32** *)softpc_gdp_slot((const void *)Gdp, 680u, sizeof(IU32**))))
#undef GLOBAL_patchNames
#define GLOBAL_patchNames (*((IU8** *)softpc_gdp_slot((const void *)Gdp, 684u, sizeof(IU8**))))
#undef GLOBAL_CopierFt
#define GLOBAL_CopierFt (*((IUH *)softpc_gdp_slot((const void *)Gdp, 688u, sizeof(IUH))))
#undef GLOBAL_FtIsLazy
#define GLOBAL_FtIsLazy (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 695u, sizeof(IBOOL))))
#undef GLOBAL_lastLazyFt
#define GLOBAL_lastLazyFt (*((IUH *)softpc_gdp_slot((const void *)Gdp, 696u, sizeof(IUH))))
#undef GLOBAL_univVarMask
#define GLOBAL_univVarMask (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 700u, sizeof(IU32))))
#undef GLOBAL_zCoRoRetEFI
#define GLOBAL_zCoRoRetEFI (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 706u, sizeof(IU16))))
#undef GLOBAL_zPatchMeEFI
#define GLOBAL_zPatchMeEFI (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 710u, sizeof(IU16))))
#undef GLOBAL_zPostPopEFI
#define GLOBAL_zPostPopEFI (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 714u, sizeof(IU16))))
#undef GLOBAL_zAdjustHspEFI
#define GLOBAL_zAdjustHspEFI (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 718u, sizeof(IU16))))
#undef GLOBAL_cumulativeStackMovement
#define GLOBAL_cumulativeStackMovement (*((ISH *)softpc_gdp_slot((const void *)Gdp, 720u, sizeof(ISH))))
#undef GLOBAL_lastInstructionInFragment
#define GLOBAL_lastInstructionInFragment (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 727u, sizeof(IBOOL))))
#undef GLOBAL_lateInInstruction
#define GLOBAL_lateInInstruction (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 731u, sizeof(IBOOL))))
#undef GLOBAL_ReSelectVariant
#define GLOBAL_ReSelectVariant (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 735u, sizeof(IBOOL))))
#undef GLOBAL_ReSelectTupleSkipCnt
#define GLOBAL_ReSelectTupleSkipCnt (*((ISH *)softpc_gdp_slot((const void *)Gdp, 736u, sizeof(ISH))))
#undef GLOBAL_postPopPending
#define GLOBAL_postPopPending (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 743u, sizeof(IBOOL))))
#undef GLOBAL_postPopSize
#define GLOBAL_postPopSize (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 744u, sizeof(IU32))))
#undef GLOBAL_compTimeFtBitNum
#define GLOBAL_compTimeFtBitNum (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 748u, sizeof(IU32))))
#undef GLOBAL_accIsReadBitMask
#define GLOBAL_accIsReadBitMask (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 752u, sizeof(IU32))))
#undef GLOBAL_noFlagsBitMask
#define GLOBAL_noFlagsBitMask (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 756u, sizeof(IU32))))
#undef GLOBAL_OutlyingDispatchEIPAddr
#define GLOBAL_OutlyingDispatchEIPAddr (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 760u, sizeof(IU32*))))
#undef GLOBAL_PARAM1RegId
#define GLOBAL_PARAM1RegId (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 764u, sizeof(IU32))))
#undef GLOBAL_stashedImmed1
#define GLOBAL_stashedImmed1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 768u, sizeof(IUH))))
#undef GLOBAL_stashedImmed2
#define GLOBAL_stashedImmed2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 772u, sizeof(IUH))))
#undef GLOBAL_stashedImmed3
#define GLOBAL_stashedImmed3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 776u, sizeof(IUH))))
#undef GLOBAL_stashedImmed4
#define GLOBAL_stashedImmed4 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 780u, sizeof(IUH))))
#undef GLOBAL_stashedImmed5
#define GLOBAL_stashedImmed5 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 784u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_32
#define GLOBAL_EDL_WORKSPACE_32 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 788u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_33
#define GLOBAL_EDL_WORKSPACE_33 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 792u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_34
#define GLOBAL_EDL_WORKSPACE_34 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 796u, sizeof(IUH))))
#undef GLOBAL_CleanedRec
#define GLOBAL_CleanedRec (*((struct CleanedREC *)softpc_gdp_slot((const void *)Gdp, 800u, sizeof(struct CleanedREC))))
#undef GLOBAL_Universe
#define GLOBAL_Universe (*((struct EntryPointCacheREC* *)softpc_gdp_slot((const void *)Gdp, 820u, sizeof(struct EntryPointCacheREC*))))
#undef GLOBAL_EntryPointCache
#define GLOBAL_EntryPointCache (*((struct EntryPointCacheREC* *)softpc_gdp_slot((const void *)Gdp, 824u, sizeof(struct EntryPointCacheREC*))))
#undef GLOBAL_CsLinear
#define GLOBAL_CsLinear (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 828u, sizeof(IU32))))
#undef GLOBAL_JumpCounter
#define GLOBAL_JumpCounter (*((ISH *)softpc_gdp_slot((const void *)Gdp, 832u, sizeof(ISH))))
#undef GLOBAL_JumpRestart
#define GLOBAL_JumpRestart (*((ISH *)softpc_gdp_slot((const void *)Gdp, 836u, sizeof(ISH))))
#undef GLOBAL_JumpCalibrate
#define GLOBAL_JumpCalibrate (*((ISH *)softpc_gdp_slot((const void *)Gdp, 840u, sizeof(ISH))))
#undef GLOBAL_InitialJumpCounter
#define GLOBAL_InitialJumpCounter (*((ISH *)softpc_gdp_slot((const void *)Gdp, 844u, sizeof(ISH))))
#undef GLOBAL_minimumInitialVal
#define GLOBAL_minimumInitialVal (*((IUH *)softpc_gdp_slot((const void *)Gdp, 848u, sizeof(IUH))))
#undef GLOBAL_IretHookStack
#define GLOBAL_IretHookStack (*((struct IretHookStackREC* *)softpc_gdp_slot((const void *)Gdp, 852u, sizeof(struct IretHookStackREC*))))
#undef GLOBAL_IretHookStackIndex
#define GLOBAL_IretHookStackIndex (*((ISH *)softpc_gdp_slot((const void *)Gdp, 856u, sizeof(ISH))))
#undef GLOBAL_InstructionCount
#define GLOBAL_InstructionCount (*((IUH *)softpc_gdp_slot((const void *)Gdp, 860u, sizeof(IUH))))
#undef GLOBAL_CsSelectorHashTable
#define GLOBAL_CsSelectorHashTable (*((struct CsSelectorHashREC* *)softpc_gdp_slot((const void *)Gdp, 864u, sizeof(struct CsSelectorHashREC*))))
#undef GLOBAL_NextFreeUniverseHandle
#define GLOBAL_NextFreeUniverseHandle (*((IUH *)softpc_gdp_slot((const void *)Gdp, 868u, sizeof(IUH))))
#undef GLOBAL_UniHashTable
#define GLOBAL_UniHashTable (*((struct UniHashREC* *)softpc_gdp_slot((const void *)Gdp, 872u, sizeof(struct UniHashREC*))))
#undef GLOBAL_NextFreeUniHashEntry
#define GLOBAL_NextFreeUniHashEntry (*((struct UniHashREC* *)softpc_gdp_slot((const void *)Gdp, 876u, sizeof(struct UniHashREC*))))
#undef GLOBAL_NewUniverseBitMapRec
#define GLOBAL_NewUniverseBitMapRec (*((struct ConstraintBitMapREC *)softpc_gdp_slot((const void *)Gdp, 880u, sizeof(struct ConstraintBitMapREC))))
#undef GLOBAL_Constraint2CvMap
#define GLOBAL_Constraint2CvMap (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 888u, sizeof(IU32*))))
#undef GLOBAL_InsertBPIs
#define GLOBAL_InsertBPIs (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 895u, sizeof(IBOOL))))
#undef GLOBAL_UseUniverseHash
#define GLOBAL_UseUniverseHash (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 899u, sizeof(IBOOL))))
#undef GLOBAL_VirtualiseDataSel
#define GLOBAL_VirtualiseDataSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 902u, sizeof(IU16))))
#undef GLOBAL_VirtualiseCodeSel
#define GLOBAL_VirtualiseCodeSel (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 906u, sizeof(IU16))))
#undef GLOBAL_VirtualiseSelsSet
#define GLOBAL_VirtualiseSelsSet (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 911u, sizeof(IBOOL))))
#undef GLOBAL_EDL_WORKSPACE_35
#define GLOBAL_EDL_WORKSPACE_35 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 912u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_36
#define GLOBAL_EDL_WORKSPACE_36 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 916u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_37
#define GLOBAL_EDL_WORKSPACE_37 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 920u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_38
#define GLOBAL_EDL_WORKSPACE_38 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 924u, sizeof(IUH))))
#undef GLOBAL_TraceRingRec
#define GLOBAL_TraceRingRec (*((struct TraceRingREC *)softpc_gdp_slot((const void *)Gdp, 928u, sizeof(struct TraceRingREC))))
#undef GLOBAL_EAXsaved
#define GLOBAL_EAXsaved (*((IUH *)softpc_gdp_slot((const void *)Gdp, 948u, sizeof(IUH))))
#undef GLOBAL_EBXsaved
#define GLOBAL_EBXsaved (*((IUH *)softpc_gdp_slot((const void *)Gdp, 952u, sizeof(IUH))))
#undef GLOBAL_ECXsaved
#define GLOBAL_ECXsaved (*((IUH *)softpc_gdp_slot((const void *)Gdp, 956u, sizeof(IUH))))
#undef GLOBAL_EDXsaved
#define GLOBAL_EDXsaved (*((IUH *)softpc_gdp_slot((const void *)Gdp, 960u, sizeof(IUH))))
#undef GLOBAL_ESIsaved
#define GLOBAL_ESIsaved (*((IUH *)softpc_gdp_slot((const void *)Gdp, 964u, sizeof(IUH))))
#undef GLOBAL_EDIsaved
#define GLOBAL_EDIsaved (*((IUH *)softpc_gdp_slot((const void *)Gdp, 968u, sizeof(IUH))))
#undef GLOBAL_EBPsaved
#define GLOBAL_EBPsaved (*((IUH *)softpc_gdp_slot((const void *)Gdp, 972u, sizeof(IUH))))
#undef GLOBAL_SafeToReturnToFragment
#define GLOBAL_SafeToReturnToFragment (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 979u, sizeof(IBOOL))))
#undef GLOBAL_InsideTheCpu
#define GLOBAL_InsideTheCpu (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 983u, sizeof(IBOOL))))
#undef GLOBAL_SimulateContext
#define GLOBAL_SimulateContext (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 984u, sizeof(IUH*))))
#undef GLOBAL_SimulateNestingLevel
#define GLOBAL_SimulateNestingLevel (*((IUH *)softpc_gdp_slot((const void *)Gdp, 988u, sizeof(IUH))))
#undef GLOBAL_Pigging
#define GLOBAL_Pigging (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 995u, sizeof(IBOOL))))
#undef GLOBAL_tempEIP
#define GLOBAL_tempEIP (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 996u, sizeof(IU32))))
#undef GLOBAL_UseEntryPointCache
#define GLOBAL_UseEntryPointCache (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1003u, sizeof(IBOOL))))
#undef GLOBAL_CpuIsInitialised
#define GLOBAL_CpuIsInitialised (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1007u, sizeof(IBOOL))))
#undef GLOBAL_AR_FixupWanted
#define GLOBAL_AR_FixupWanted (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1011u, sizeof(IBOOL))))
#undef GLOBAL_D6isBop
#define GLOBAL_D6isBop (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1015u, sizeof(IBOOL))))
#undef GLOBAL_BopTable
#define GLOBAL_BopTable (*((IU8** *)softpc_gdp_slot((const void *)Gdp, 1016u, sizeof(IU8**))))
#undef GLOBAL_LxS_hackyfix
#define GLOBAL_LxS_hackyfix (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1020u, sizeof(IUH))))
#undef GLOBAL_SavedFt
#define GLOBAL_SavedFt (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1024u, sizeof(IUH))))
#undef GLOBAL_SavedCF
#define GLOBAL_SavedCF (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1028u, sizeof(IUH))))
#undef GLOBAL_SavedZF
#define GLOBAL_SavedZF (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1032u, sizeof(IUH))))
#undef GLOBAL_SavedOF
#define GLOBAL_SavedOF (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1036u, sizeof(IUH))))
#undef GLOBAL_UsedD6
#define GLOBAL_UsedD6 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1040u, sizeof(IUH))))
#undef GLOBAL_buildOp1
#define GLOBAL_buildOp1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1044u, sizeof(IUH))))
#undef GLOBAL_buildOp2
#define GLOBAL_buildOp2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1048u, sizeof(IUH))))
#undef GLOBAL_buildOp3
#define GLOBAL_buildOp3 (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 1052u, sizeof(IU32))))
#undef GLOBAL_buildOp4
#define GLOBAL_buildOp4 (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 1056u, sizeof(IU32))))
#undef GLOBAL_buildOp5
#define GLOBAL_buildOp5 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1060u, sizeof(IUH))))
#undef GLOBAL_buildOp6
#define GLOBAL_buildOp6 (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1067u, sizeof(IBOOL))))
#undef GLOBAL_EDL_WORKSPACE_39
#define GLOBAL_EDL_WORKSPACE_39 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1068u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_40
#define GLOBAL_EDL_WORKSPACE_40 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1072u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_41
#define GLOBAL_EDL_WORKSPACE_41 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1076u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_42
#define GLOBAL_EDL_WORKSPACE_42 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1080u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_43
#define GLOBAL_EDL_WORKSPACE_43 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1084u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_44
#define GLOBAL_EDL_WORKSPACE_44 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1088u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_45
#define GLOBAL_EDL_WORKSPACE_45 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1092u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_46
#define GLOBAL_EDL_WORKSPACE_46 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1096u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_47
#define GLOBAL_EDL_WORKSPACE_47 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1100u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_48
#define GLOBAL_EDL_WORKSPACE_48 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1104u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_49
#define GLOBAL_EDL_WORKSPACE_49 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1108u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_50
#define GLOBAL_EDL_WORKSPACE_50 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1112u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_51
#define GLOBAL_EDL_WORKSPACE_51 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1116u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_52
#define GLOBAL_EDL_WORKSPACE_52 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1120u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_53
#define GLOBAL_EDL_WORKSPACE_53 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1124u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_54
#define GLOBAL_EDL_WORKSPACE_54 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1128u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_55
#define GLOBAL_EDL_WORKSPACE_55 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1132u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_56
#define GLOBAL_EDL_WORKSPACE_56 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1136u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_57
#define GLOBAL_EDL_WORKSPACE_57 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1140u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_58
#define GLOBAL_EDL_WORKSPACE_58 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1144u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_59
#define GLOBAL_EDL_WORKSPACE_59 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1148u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_60
#define GLOBAL_EDL_WORKSPACE_60 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1152u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_61
#define GLOBAL_EDL_WORKSPACE_61 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1156u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_62
#define GLOBAL_EDL_WORKSPACE_62 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1160u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_63
#define GLOBAL_EDL_WORKSPACE_63 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1164u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_64
#define GLOBAL_EDL_WORKSPACE_64 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1168u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_65
#define GLOBAL_EDL_WORKSPACE_65 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1172u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_66
#define GLOBAL_EDL_WORKSPACE_66 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1176u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_67
#define GLOBAL_EDL_WORKSPACE_67 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1180u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_68
#define GLOBAL_EDL_WORKSPACE_68 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1184u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_69
#define GLOBAL_EDL_WORKSPACE_69 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1188u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_70
#define GLOBAL_EDL_WORKSPACE_70 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1192u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_71
#define GLOBAL_EDL_WORKSPACE_71 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1196u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_72
#define GLOBAL_EDL_WORKSPACE_72 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1200u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_73
#define GLOBAL_EDL_WORKSPACE_73 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1204u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_74
#define GLOBAL_EDL_WORKSPACE_74 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1208u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_75
#define GLOBAL_EDL_WORKSPACE_75 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1212u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_76
#define GLOBAL_EDL_WORKSPACE_76 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1216u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_77
#define GLOBAL_EDL_WORKSPACE_77 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1220u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_78
#define GLOBAL_EDL_WORKSPACE_78 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1224u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_79
#define GLOBAL_EDL_WORKSPACE_79 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1228u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_80
#define GLOBAL_EDL_WORKSPACE_80 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1232u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_81
#define GLOBAL_EDL_WORKSPACE_81 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1236u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_82
#define GLOBAL_EDL_WORKSPACE_82 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1240u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_83
#define GLOBAL_EDL_WORKSPACE_83 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1244u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_84
#define GLOBAL_EDL_WORKSPACE_84 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1248u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_85
#define GLOBAL_EDL_WORKSPACE_85 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1252u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_86
#define GLOBAL_EDL_WORKSPACE_86 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1256u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_87
#define GLOBAL_EDL_WORKSPACE_87 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1260u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_88
#define GLOBAL_EDL_WORKSPACE_88 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1264u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_89
#define GLOBAL_EDL_WORKSPACE_89 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1268u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_90
#define GLOBAL_EDL_WORKSPACE_90 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1272u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_91
#define GLOBAL_EDL_WORKSPACE_91 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1276u, sizeof(IUH))))
#undef GLOBAL_VGAGlobals
#define GLOBAL_VGAGlobals (*((struct VGAGLOBALSETTINGS *)softpc_gdp_slot((const void *)Gdp, 1280u, sizeof(struct VGAGLOBALSETTINGS))))
#undef GLOBAL_VidMarkFuncTable
#define GLOBAL_VidMarkFuncTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 1436u, sizeof(IUH**))))
#undef GLOBAL_VidReadFuncTable
#define GLOBAL_VidReadFuncTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 1440u, sizeof(IUH**))))
#undef GLOBAL_VidWriteFuncTable
#define GLOBAL_VidWriteFuncTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 1444u, sizeof(IUH**))))
#undef GLOBAL_EDL_WORKSPACE_92
#define GLOBAL_EDL_WORKSPACE_92 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1448u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_93
#define GLOBAL_EDL_WORKSPACE_93 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1452u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_94
#define GLOBAL_EDL_WORKSPACE_94 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1456u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_95
#define GLOBAL_EDL_WORKSPACE_95 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1460u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_96
#define GLOBAL_EDL_WORKSPACE_96 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1464u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_97
#define GLOBAL_EDL_WORKSPACE_97 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1468u, sizeof(IUH))))
#undef GLOBAL_ActiveVideoWrites
#define GLOBAL_ActiveVideoWrites (*((struct EVIDWRITES *)softpc_gdp_slot((const void *)Gdp, 1472u, sizeof(struct EVIDWRITES))))
#undef GLOBAL_EDL_WORKSPACE_98
#define GLOBAL_EDL_WORKSPACE_98 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1520u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_99
#define GLOBAL_EDL_WORKSPACE_99 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1524u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_100
#define GLOBAL_EDL_WORKSPACE_100 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1528u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_101
#define GLOBAL_EDL_WORKSPACE_101 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1532u, sizeof(IUH))))
#undef GLOBAL_ActiveVideoReads
#define GLOBAL_ActiveVideoReads (*((struct EVIDREADS *)softpc_gdp_slot((const void *)Gdp, 1536u, sizeof(struct EVIDREADS))))
#undef GLOBAL_EDL_WORKSPACE_102
#define GLOBAL_EDL_WORKSPACE_102 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1556u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_103
#define GLOBAL_EDL_WORKSPACE_103 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1560u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_104
#define GLOBAL_EDL_WORKSPACE_104 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1564u, sizeof(IUH))))
#undef GLOBAL_ActiveVideoMarks
#define GLOBAL_ActiveVideoMarks (*((struct EVIDMARKS *)softpc_gdp_slot((const void *)Gdp, 1568u, sizeof(struct EVIDMARKS))))
#undef GLOBAL_MaxIntelPageNumber
#define GLOBAL_MaxIntelPageNumber (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 1584u, sizeof(IU32))))
#undef GLOBAL_PageDirectoryPtr
#define GLOBAL_PageDirectoryPtr (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 1588u, sizeof(IU32*))))
#undef GLOBAL_DebuggerPFLA
#define GLOBAL_DebuggerPFLA (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 1592u, sizeof(IU32))))
#undef GLOBAL_DebuggerFaultAction
#define GLOBAL_DebuggerFaultAction (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1596u, sizeof(IUH))))
#undef GLOBAL_InsideDebugger
#define GLOBAL_InsideDebugger (*((ISH *)softpc_gdp_slot((const void *)Gdp, 1600u, sizeof(ISH))))
#undef GLOBAL_EDL_WORKSPACE_105
#define GLOBAL_EDL_WORKSPACE_105 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1604u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_106
#define GLOBAL_EDL_WORKSPACE_106 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1608u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_107
#define GLOBAL_EDL_WORKSPACE_107 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1612u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_108
#define GLOBAL_EDL_WORKSPACE_108 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1616u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_109
#define GLOBAL_EDL_WORKSPACE_109 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1620u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_110
#define GLOBAL_EDL_WORKSPACE_110 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1624u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_111
#define GLOBAL_EDL_WORKSPACE_111 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1628u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_112
#define GLOBAL_EDL_WORKSPACE_112 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1632u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_113
#define GLOBAL_EDL_WORKSPACE_113 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1636u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_114
#define GLOBAL_EDL_WORKSPACE_114 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1640u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_115
#define GLOBAL_EDL_WORKSPACE_115 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1644u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_116
#define GLOBAL_EDL_WORKSPACE_116 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1648u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_117
#define GLOBAL_EDL_WORKSPACE_117 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1652u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_118
#define GLOBAL_EDL_WORKSPACE_118 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1656u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_119
#define GLOBAL_EDL_WORKSPACE_119 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1660u, sizeof(IUH))))
#undef GLOBAL_VirtualisationBIOSOffsets
#define GLOBAL_VirtualisationBIOSOffsets (*((struct VirtualisationBIOSOffsetsREC *)softpc_gdp_slot((const void *)Gdp, 1664u, sizeof(struct VirtualisationBIOSOffsetsREC))))
#undef GLOBAL_DelayedPDTEoverwiteList
#define GLOBAL_DelayedPDTEoverwiteList (*((struct DelayedPDTEoverwiteREC* *)softpc_gdp_slot((const void *)Gdp, 1712u, sizeof(struct DelayedPDTEoverwiteREC*))))
#undef GLOBAL_SasMemoryType
#define GLOBAL_SasMemoryType (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 1716u, sizeof(IU8*))))
#undef GLOBAL_PhysicalPageRecords
#define GLOBAL_PhysicalPageRecords (*((struct PhysicalPageREC* *)softpc_gdp_slot((const void *)Gdp, 1720u, sizeof(struct PhysicalPageREC*))))
#undef GLOBAL_PhysicalPageMemory
#define GLOBAL_PhysicalPageMemory (*((IU8** *)softpc_gdp_slot((const void *)Gdp, 1724u, sizeof(IU8**))))
#undef GLOBAL_TwentyBitWrapStatus
#define GLOBAL_TwentyBitWrapStatus (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1731u, sizeof(IBOOL))))
#undef GLOBAL_MultipleRecompilationCount
#define GLOBAL_MultipleRecompilationCount (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1732u, sizeof(IUH))))
#undef GLOBAL_MaxMultipleRecompilation
#define GLOBAL_MaxMultipleRecompilation (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1736u, sizeof(IUH))))
#undef GLOBAL_HideCompiledFragment
#define GLOBAL_HideCompiledFragment (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1743u, sizeof(IBOOL))))
#undef GLOBAL_RestartAfterCodeOverwrite
#define GLOBAL_RestartAfterCodeOverwrite (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1747u, sizeof(IBOOL))))
#undef GLOBAL_DoingCompilation
#define GLOBAL_DoingCompilation (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1751u, sizeof(IBOOL))))
#undef GLOBAL_SanityCheckStructures
#define GLOBAL_SanityCheckStructures (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 1755u, sizeof(IBOOL))))
#undef GLOBAL_FragCounts
#define GLOBAL_FragCounts (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 1756u, sizeof(IU8*))))
#undef GLOBAL_ControlBlock
#define GLOBAL_ControlBlock (*((struct CompilationControlREC *)softpc_gdp_slot((const void *)Gdp, 1760u, sizeof(struct CompilationControlREC))))
#undef GLOBAL_EDL_WORKSPACE_120
#define GLOBAL_EDL_WORKSPACE_120 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1776u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_121
#define GLOBAL_EDL_WORKSPACE_121 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1780u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_122
#define GLOBAL_EDL_WORKSPACE_122 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1784u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_123
#define GLOBAL_EDL_WORKSPACE_123 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1788u, sizeof(IUH))))
#undef GLOBAL_CompilationBlock
#define GLOBAL_CompilationBlock (*((struct BLOCK_TO_COMPILE *)softpc_gdp_slot((const void *)Gdp, 1792u, sizeof(struct BLOCK_TO_COMPILE))))
#undef GLOBAL_EDL_WORKSPACE_124
#define GLOBAL_EDL_WORKSPACE_124 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1824u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_125
#define GLOBAL_EDL_WORKSPACE_125 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1828u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_126
#define GLOBAL_EDL_WORKSPACE_126 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1832u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_127
#define GLOBAL_EDL_WORKSPACE_127 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1836u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_128
#define GLOBAL_EDL_WORKSPACE_128 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1840u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_129
#define GLOBAL_EDL_WORKSPACE_129 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1844u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_130
#define GLOBAL_EDL_WORKSPACE_130 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1848u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_131
#define GLOBAL_EDL_WORKSPACE_131 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1852u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_132
#define GLOBAL_EDL_WORKSPACE_132 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1856u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_133
#define GLOBAL_EDL_WORKSPACE_133 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1860u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_134
#define GLOBAL_EDL_WORKSPACE_134 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1864u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_135
#define GLOBAL_EDL_WORKSPACE_135 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1868u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_136
#define GLOBAL_EDL_WORKSPACE_136 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1872u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_137
#define GLOBAL_EDL_WORKSPACE_137 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1876u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_138
#define GLOBAL_EDL_WORKSPACE_138 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1880u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_139
#define GLOBAL_EDL_WORKSPACE_139 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1884u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_140
#define GLOBAL_EDL_WORKSPACE_140 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1888u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_141
#define GLOBAL_EDL_WORKSPACE_141 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1892u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_142
#define GLOBAL_EDL_WORKSPACE_142 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1896u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_143
#define GLOBAL_EDL_WORKSPACE_143 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1900u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_144
#define GLOBAL_EDL_WORKSPACE_144 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1904u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_145
#define GLOBAL_EDL_WORKSPACE_145 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1908u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_146
#define GLOBAL_EDL_WORKSPACE_146 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1912u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_147
#define GLOBAL_EDL_WORKSPACE_147 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 1916u, sizeof(IUH))))
#undef GLOBAL_ProtAllocationRec
#define GLOBAL_ProtAllocationRec (*((struct ProtAllocationREC *)softpc_gdp_slot((const void *)Gdp, 1920u, sizeof(struct ProtAllocationREC))))
#undef GLOBAL_LightCompiledLRUrec
#define GLOBAL_LightCompiledLRUrec (*((struct BufferIndexREC* *)softpc_gdp_slot((const void *)Gdp, 2016u, sizeof(struct BufferIndexREC*))))
#undef GLOBAL_NextPhysicalPage
#define GLOBAL_NextPhysicalPage (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2020u, sizeof(IUH))))
#undef GLOBAL_FreeMaps
#define GLOBAL_FreeMaps (*((struct TranslationMapREC* *)softpc_gdp_slot((const void *)Gdp, 2024u, sizeof(struct TranslationMapREC*))))
#undef GLOBAL_TranslationCache
#define GLOBAL_TranslationCache (*((struct TranslationCacheREC* *)softpc_gdp_slot((const void *)Gdp, 2028u, sizeof(struct TranslationCacheREC*))))
#undef GLOBAL_CrossPageInstructions
#define GLOBAL_CrossPageInstructions (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 2032u, sizeof(IU8*))))
#undef GLOBAL_IHook
#define GLOBAL_IHook (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 2036u, sizeof(IU32))))
#undef GLOBAL_InterruptRec
#define GLOBAL_InterruptRec (*((struct InterruptREC *)softpc_gdp_slot((const void *)Gdp, 2040u, sizeof(struct InterruptREC))))
#undef GLOBAL_SasReInitNow
#define GLOBAL_SasReInitNow (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2051u, sizeof(IBOOL))))
#undef GLOBAL_SasReInitSize
#define GLOBAL_SasReInitSize (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 2052u, sizeof(IU32))))
#undef GLOBAL_EDL_WORKSPACE_148
#define GLOBAL_EDL_WORKSPACE_148 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2056u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_149
#define GLOBAL_EDL_WORKSPACE_149 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2060u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_150
#define GLOBAL_EDL_WORKSPACE_150 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2064u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_151
#define GLOBAL_EDL_WORKSPACE_151 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2068u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_152
#define GLOBAL_EDL_WORKSPACE_152 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2072u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_153
#define GLOBAL_EDL_WORKSPACE_153 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2076u, sizeof(IUH))))
#undef GLOBAL_QuickTickerRec
#define GLOBAL_QuickTickerRec (*((struct QuickTickerREC *)softpc_gdp_slot((const void *)Gdp, 2080u, sizeof(struct QuickTickerREC))))
#undef GLOBAL_PigSynchCount
#define GLOBAL_PigSynchCount (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2104u, sizeof(IUH))))
#undef GLOBAL_CodeBufferNoRepair
#define GLOBAL_CodeBufferNoRepair (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2111u, sizeof(IBOOL))))
#undef GLOBAL_OutLinePatchBlock
#define GLOBAL_OutLinePatchBlock (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2112u, sizeof(IU32*))))
#undef GLOBAL_OutLinePatchBlockSize
#define GLOBAL_OutLinePatchBlockSize (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 2116u, sizeof(IU32))))
#undef GLOBAL_AllBuffers
#define GLOBAL_AllBuffers (*((struct BufferIndexREC* *)softpc_gdp_slot((const void *)Gdp, 2120u, sizeof(struct BufferIndexREC*))))
#undef GLOBAL_LightBufferLRU
#define GLOBAL_LightBufferLRU (*((struct BufferIndexREC* *)softpc_gdp_slot((const void *)Gdp, 2124u, sizeof(struct BufferIndexREC*))))
#undef GLOBAL_CompilationBuffer
#define GLOBAL_CompilationBuffer (*((struct BufferIndexREC* *)softpc_gdp_slot((const void *)Gdp, 2128u, sizeof(struct BufferIndexREC*))))
#undef GLOBAL_PendingDeletions
#define GLOBAL_PendingDeletions (*((struct BufferIndexREC* *)softpc_gdp_slot((const void *)Gdp, 2132u, sizeof(struct BufferIndexREC*))))
#undef GLOBAL_FragmentInfoArray
#define GLOBAL_FragmentInfoArray (*((struct FragmentInfoREC* *)softpc_gdp_slot((const void *)Gdp, 2136u, sizeof(struct FragmentInfoREC*))))
#undef GLOBAL_HostCodeBufferLimit
#define GLOBAL_HostCodeBufferLimit (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2140u, sizeof(IU32*))))
#undef GLOBAL_CopiedCleanups
#define GLOBAL_CopiedCleanups (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 2144u, sizeof(IU8*))))
#undef GLOBAL_FreeDebugInfoList
#define GLOBAL_FreeDebugInfoList (*((struct DebugInfoREC* *)softpc_gdp_slot((const void *)Gdp, 2148u, sizeof(struct DebugInfoREC*))))
#undef GLOBAL_CodeBufferOverrun
#define GLOBAL_CodeBufferOverrun (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2152u, sizeof(IU32*))))
#undef GLOBAL_OverrunHighWaterMark
#define GLOBAL_OverrunHighWaterMark (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2156u, sizeof(IU32*))))
#undef GLOBAL_NumberOfBuffers
#define GLOBAL_NumberOfBuffers (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 2162u, sizeof(IU16))))
#undef GLOBAL_BpiCompilationBuffer
#define GLOBAL_BpiCompilationBuffer (*((struct BufferIndexREC* *)softpc_gdp_slot((const void *)Gdp, 2164u, sizeof(struct BufferIndexREC*))))
#undef GLOBAL_NextFragmentIndex
#define GLOBAL_NextFragmentIndex (*((struct FragmentIndexREC* *)softpc_gdp_slot((const void *)Gdp, 2168u, sizeof(struct FragmentIndexREC*))))
#undef GLOBAL_NextFragmentData
#define GLOBAL_NextFragmentData (*((struct FragmentDataREC* *)softpc_gdp_slot((const void *)Gdp, 2172u, sizeof(struct FragmentDataREC*))))
#undef GLOBAL_FpuDisabled
#define GLOBAL_FpuDisabled (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2179u, sizeof(IBOOL))))
#undef GLOBAL_NpxControl
#define GLOBAL_NpxControl (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2180u, sizeof(IUH))))
#undef GLOBAL_NpxStatus
#define GLOBAL_NpxStatus (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2184u, sizeof(IUH))))
#undef GLOBAL_NpxFEA
#define GLOBAL_NpxFEA (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2188u, sizeof(IUH))))
#undef GLOBAL_NpxFDS
#define GLOBAL_NpxFDS (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2192u, sizeof(IUH))))
#undef GLOBAL_NpxFIP
#define GLOBAL_NpxFIP (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2196u, sizeof(IUH))))
#undef GLOBAL_NpxFOP
#define GLOBAL_NpxFOP (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2200u, sizeof(IUH))))
#undef GLOBAL_NpxFCS
#define GLOBAL_NpxFCS (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2204u, sizeof(IUH))))
#undef GLOBAL_NpxLastSel
#define GLOBAL_NpxLastSel (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2208u, sizeof(IUH))))
#undef GLOBAL_NpxLastOff
#define GLOBAL_NpxLastOff (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2212u, sizeof(IUH))))
#undef GLOBAL_DoAPop
#define GLOBAL_DoAPop (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2219u, sizeof(IBOOL))))
#undef GLOBAL_NpxException
#define GLOBAL_NpxException (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2223u, sizeof(IBOOL))))
#undef GLOBAL_npxRounding
#define GLOBAL_npxRounding (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2224u, sizeof(IUH))))
#undef GLOBAL_tag_or
#define GLOBAL_tag_or (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2228u, sizeof(IUH))))
#undef GLOBAL_tag_xor
#define GLOBAL_tag_xor (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2232u, sizeof(IUH))))
#undef GLOBAL_hostFpuExceptions
#define GLOBAL_hostFpuExceptions (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2236u, sizeof(IUH))))
#undef GLOBAL_MaxBCDValue
#define GLOBAL_MaxBCDValue (*((struct FPSTACKENTRY *)softpc_gdp_slot((const void *)Gdp, 2240u, sizeof(struct FPSTACKENTRY))))
#undef GLOBAL_FPUpload
#define GLOBAL_FPUpload (*((struct FPSTACKENTRY *)softpc_gdp_slot((const void *)Gdp, 2256u, sizeof(struct FPSTACKENTRY))))
#undef GLOBAL_ConstTable
#define GLOBAL_ConstTable (*((struct FPSTACKENTRY* *)softpc_gdp_slot((const void *)Gdp, 2272u, sizeof(struct FPSTACKENTRY*))))
#undef GLOBAL_FPTemp
#define GLOBAL_FPTemp (*((struct FPSTACKENTRY* *)softpc_gdp_slot((const void *)Gdp, 2276u, sizeof(struct FPSTACKENTRY*))))
#undef GLOBAL_FPUStackBase
#define GLOBAL_FPUStackBase (*((struct FPSTACKENTRY* *)softpc_gdp_slot((const void *)Gdp, 2280u, sizeof(struct FPSTACKENTRY*))))
#undef GLOBAL_TOSPtr
#define GLOBAL_TOSPtr (*((struct FPSTACKENTRY* *)softpc_gdp_slot((const void *)Gdp, 2284u, sizeof(struct FPSTACKENTRY*))))
#undef GLOBAL_Npx64BitZero
#define GLOBAL_Npx64BitZero (*((struct FP_I64 *)softpc_gdp_slot((const void *)Gdp, 2288u, sizeof(struct FP_I64))))
#undef GLOBAL_Npx64BitMaxNeg
#define GLOBAL_Npx64BitMaxNeg (*((struct FP_I64 *)softpc_gdp_slot((const void *)Gdp, 2296u, sizeof(struct FP_I64))))
#undef GLOBAL_Npx64BitHalfMaxNeg
#define GLOBAL_Npx64BitHalfMaxNeg (*((struct FP_I64 *)softpc_gdp_slot((const void *)Gdp, 2304u, sizeof(struct FP_I64))))
#undef GLOBAL_Npx64BitVal1
#define GLOBAL_Npx64BitVal1 (*((struct FP_I64 *)softpc_gdp_slot((const void *)Gdp, 2312u, sizeof(struct FP_I64))))
#undef GLOBAL_FscaleTable
#define GLOBAL_FscaleTable (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 2320u, sizeof(IUH*))))
#undef GLOBAL_CompZeroTable
#define GLOBAL_CompZeroTable (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2324u, sizeof(IU32*))))
#undef GLOBAL_BCDLowNibble
#define GLOBAL_BCDLowNibble (*((struct FP_I64* *)softpc_gdp_slot((const void *)Gdp, 2328u, sizeof(struct FP_I64*))))
#undef GLOBAL_BCDHighNibble
#define GLOBAL_BCDHighNibble (*((struct FP_I64* *)softpc_gdp_slot((const void *)Gdp, 2332u, sizeof(struct FP_I64*))))
#undef GLOBAL_FpatanTable
#define GLOBAL_FpatanTable (*((struct FPSTACKENTRY* *)softpc_gdp_slot((const void *)Gdp, 2336u, sizeof(struct FPSTACKENTRY*))))
#undef GLOBAL_PigSynchTable
#define GLOBAL_PigSynchTable (*((struct PigSynchREC* *)softpc_gdp_slot((const void *)Gdp, 2340u, sizeof(struct PigSynchREC*))))
#undef GLOBAL_PigMissTable
#define GLOBAL_PigMissTable (*((struct PigSynchREC* *)softpc_gdp_slot((const void *)Gdp, 2344u, sizeof(struct PigSynchREC*))))
#undef GLOBAL_PigSynchPool
#define GLOBAL_PigSynchPool (*((struct PigSynchREC* *)softpc_gdp_slot((const void *)Gdp, 2348u, sizeof(struct PigSynchREC*))))
#undef GLOBAL_EDL_WORKSPACE_154
#define GLOBAL_EDL_WORKSPACE_154 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2352u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_155
#define GLOBAL_EDL_WORKSPACE_155 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2356u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_156
#define GLOBAL_EDL_WORKSPACE_156 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2360u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_157
#define GLOBAL_EDL_WORKSPACE_157 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2364u, sizeof(IUH))))
#undef GLOBAL_PigCleanedRec
#define GLOBAL_PigCleanedRec (*((struct CleanedREC *)softpc_gdp_slot((const void *)Gdp, 2368u, sizeof(struct CleanedREC))))
#undef GLOBAL_PigSynchWanted
#define GLOBAL_PigSynchWanted (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2391u, sizeof(IBOOL))))
#undef GLOBAL_SadAX
#define GLOBAL_SadAX (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2392u, sizeof(ISH))))
#undef GLOBAL_SadBX
#define GLOBAL_SadBX (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2396u, sizeof(ISH))))
#undef GLOBAL_SadCX
#define GLOBAL_SadCX (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2400u, sizeof(ISH))))
#undef GLOBAL_SadDX
#define GLOBAL_SadDX (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2404u, sizeof(ISH))))
#undef GLOBAL_SadBP
#define GLOBAL_SadBP (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2408u, sizeof(ISH))))
#undef GLOBAL_SadSP
#define GLOBAL_SadSP (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2412u, sizeof(ISH))))
#undef GLOBAL_SadSI
#define GLOBAL_SadSI (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2416u, sizeof(ISH))))
#undef GLOBAL_SadDI
#define GLOBAL_SadDI (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2420u, sizeof(ISH))))
#undef GLOBAL_SadEIP
#define GLOBAL_SadEIP (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2424u, sizeof(ISH))))
#undef GLOBAL_SadEFLAGS
#define GLOBAL_SadEFLAGS (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2428u, sizeof(ISH))))
#undef GLOBAL_Parameter1
#define GLOBAL_Parameter1 (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2432u, sizeof(ISH))))
#undef GLOBAL_Parameter2
#define GLOBAL_Parameter2 (*((ISH *)softpc_gdp_slot((const void *)Gdp, 2436u, sizeof(ISH))))
#undef GLOBAL_BpiKnownTable
#define GLOBAL_BpiKnownTable (*((ISH* *)softpc_gdp_slot((const void *)Gdp, 2440u, sizeof(ISH*))))
#undef GLOBAL_BpiWorkTable
#define GLOBAL_BpiWorkTable (*((ISH* *)softpc_gdp_slot((const void *)Gdp, 2444u, sizeof(ISH*))))
#undef GLOBAL_BpiLabelTable
#define GLOBAL_BpiLabelTable (*((ISH* *)softpc_gdp_slot((const void *)Gdp, 2448u, sizeof(ISH*))))
#undef GLOBAL_BpiFragment
#define GLOBAL_BpiFragment (*((struct FragmentDataREC* *)softpc_gdp_slot((const void *)Gdp, 2452u, sizeof(struct FragmentDataREC*))))
#undef GLOBAL_BpiCompiledCode
#define GLOBAL_BpiCompiledCode (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2456u, sizeof(IU32*))))
#undef GLOBAL_BpiCompiledStep
#define GLOBAL_BpiCompiledStep (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2460u, sizeof(IU32*))))
#undef GLOBAL_BpiCompiledUser
#define GLOBAL_BpiCompiledUser (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2464u, sizeof(IU32*))))
#undef GLOBAL_OpBpirealFt
#define GLOBAL_OpBpirealFt (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2468u, sizeof(IUH))))
#undef GLOBAL_OpBpirealF1
#define GLOBAL_OpBpirealF1 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2472u, sizeof(IUH))))
#undef GLOBAL_OpBpirealF2
#define GLOBAL_OpBpirealF2 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2476u, sizeof(IUH))))
#undef GLOBAL_OpBpirealF3
#define GLOBAL_OpBpirealF3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2480u, sizeof(IUH))))
#undef GLOBAL_OpBpirealUniv
#define GLOBAL_OpBpirealUniv (*((struct EntryPointCacheREC* *)softpc_gdp_slot((const void *)Gdp, 2484u, sizeof(struct EntryPointCacheREC*))))
#undef GLOBAL_OpBpirealWhereAmI
#define GLOBAL_OpBpirealWhereAmI (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 2488u, sizeof(IU32*))))
#undef GLOBAL_EDL_WORKSPACE_158
#define GLOBAL_EDL_WORKSPACE_158 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2492u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_159
#define GLOBAL_EDL_WORKSPACE_159 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2496u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_160
#define GLOBAL_EDL_WORKSPACE_160 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2500u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_161
#define GLOBAL_EDL_WORKSPACE_161 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2504u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_162
#define GLOBAL_EDL_WORKSPACE_162 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2508u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_163
#define GLOBAL_EDL_WORKSPACE_163 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2512u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_164
#define GLOBAL_EDL_WORKSPACE_164 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2516u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_165
#define GLOBAL_EDL_WORKSPACE_165 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2520u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_166
#define GLOBAL_EDL_WORKSPACE_166 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2524u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_167
#define GLOBAL_EDL_WORKSPACE_167 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2528u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_168
#define GLOBAL_EDL_WORKSPACE_168 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2532u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_169
#define GLOBAL_EDL_WORKSPACE_169 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2536u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_170
#define GLOBAL_EDL_WORKSPACE_170 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2540u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_171
#define GLOBAL_EDL_WORKSPACE_171 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2544u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_172
#define GLOBAL_EDL_WORKSPACE_172 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2548u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_173
#define GLOBAL_EDL_WORKSPACE_173 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2552u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_174
#define GLOBAL_EDL_WORKSPACE_174 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2556u, sizeof(IUH))))
#undef GLOBAL_NpxOpndBuff
#define GLOBAL_NpxOpndBuff (*((struct OpndBuffREC *)softpc_gdp_slot((const void *)Gdp, 2560u, sizeof(struct OpndBuffREC))))
#undef GLOBAL_GLDC_FreeRecs
#define GLOBAL_GLDC_FreeRecs (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 2688u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_GLDC_UsedBuffs
#define GLOBAL_GLDC_UsedBuffs (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 2692u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_GLDC_FreeBuffs
#define GLOBAL_GLDC_FreeBuffs (*((struct GLDC_REC* *)softpc_gdp_slot((const void *)Gdp, 2696u, sizeof(struct GLDC_REC*))))
#undef GLOBAL_NewRingOffsetPtr
#define GLOBAL_NewRingOffsetPtr (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 2700u, sizeof(IUH*))))
#undef GLOBAL_GLDC_Index_High_Water
#define GLOBAL_GLDC_Index_High_Water (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2704u, sizeof(IUH))))
#undef GLOBAL_GLDC_Context_High_Water
#define GLOBAL_GLDC_Context_High_Water (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2708u, sizeof(IUH))))
#undef GLOBAL_GLDC_IndexPtr
#define GLOBAL_GLDC_IndexPtr (*((struct GLDC_REC** *)softpc_gdp_slot((const void *)Gdp, 2712u, sizeof(struct GLDC_REC**))))
#undef GLOBAL_GLDC_CrBase
#define GLOBAL_GLDC_CrBase (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 2716u, sizeof(IUH*))))
#undef GLOBAL_EDL_WORKSPACE_175
#define GLOBAL_EDL_WORKSPACE_175 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2720u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_176
#define GLOBAL_EDL_WORKSPACE_176 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2724u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_177
#define GLOBAL_EDL_WORKSPACE_177 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2728u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_178
#define GLOBAL_EDL_WORKSPACE_178 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2732u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_179
#define GLOBAL_EDL_WORKSPACE_179 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2736u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_180
#define GLOBAL_EDL_WORKSPACE_180 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2740u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_181
#define GLOBAL_EDL_WORKSPACE_181 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2744u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_182
#define GLOBAL_EDL_WORKSPACE_182 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2748u, sizeof(IUH))))
#undef GLOBAL_GLDC_DUMMY_STORE
#define GLOBAL_GLDC_DUMMY_STORE (*((struct GLDC_REC *)softpc_gdp_slot((const void *)Gdp, 2752u, sizeof(struct GLDC_REC))))
#undef GLOBAL_EDL_WORKSPACE_183
#define GLOBAL_EDL_WORKSPACE_183 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2792u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_184
#define GLOBAL_EDL_WORKSPACE_184 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2796u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_185
#define GLOBAL_EDL_WORKSPACE_185 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2800u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_186
#define GLOBAL_EDL_WORKSPACE_186 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2804u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_187
#define GLOBAL_EDL_WORKSPACE_187 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2808u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_188
#define GLOBAL_EDL_WORKSPACE_188 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2812u, sizeof(IUH))))
#undef GLOBAL_GLDC_NULL_STORE
#define GLOBAL_GLDC_NULL_STORE (*((struct GLDC_REC *)softpc_gdp_slot((const void *)Gdp, 2816u, sizeof(struct GLDC_REC))))
#undef GLOBAL_hackyfix
#define GLOBAL_hackyfix (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2856u, sizeof(IUH))))
#undef GLOBAL_IDC_ArrayPtr
#define GLOBAL_IDC_ArrayPtr (*((struct IDC_REC* *)softpc_gdp_slot((const void *)Gdp, 2860u, sizeof(struct IDC_REC*))))
#undef GLOBAL_IDC_EntryTableBase
#define GLOBAL_IDC_EntryTableBase (*((struct IDC_ENTRY* *)softpc_gdp_slot((const void *)Gdp, 2864u, sizeof(struct IDC_ENTRY*))))
#undef GLOBAL_IDC_IdtSeqVal
#define GLOBAL_IDC_IdtSeqVal (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2868u, sizeof(IUH))))
#undef GLOBAL_IDC_IdtHighWater
#define GLOBAL_IDC_IdtHighWater (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2872u, sizeof(IUH))))
#undef GLOBAL_IDC_IdtCntrlVal
#define GLOBAL_IDC_IdtCntrlVal (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2876u, sizeof(IUH))))
#undef GLOBAL_IDC_IdtCntrlValNoCheck
#define GLOBAL_IDC_IdtCntrlValNoCheck (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2880u, sizeof(IUH))))
#undef GLOBAL_PX_trace
#define GLOBAL_PX_trace (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2887u, sizeof(IBOOL))))
#undef GLOBAL_PX_doing_contributory
#define GLOBAL_PX_doing_contributory (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2891u, sizeof(IBOOL))))
#undef GLOBAL_PX_doing_page_fault
#define GLOBAL_PX_doing_page_fault (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2895u, sizeof(IBOOL))))
#undef GLOBAL_PX_doing_double_fault
#define GLOBAL_PX_doing_double_fault (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2899u, sizeof(IBOOL))))
#undef GLOBAL_PX_doing_fault
#define GLOBAL_PX_doing_fault (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2903u, sizeof(IBOOL))))
#undef GLOBAL_PX_source
#define GLOBAL_PX_source (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2904u, sizeof(IUH))))
#undef GLOBAL_RF_OnXcptnWanted
#define GLOBAL_RF_OnXcptnWanted (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 2911u, sizeof(IBOOL))))
#undef GLOBAL_PX_Cleaned_Eip
#define GLOBAL_PX_Cleaned_Eip (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 2912u, sizeof(IU32))))
#undef GLOBAL_CInbTable
#define GLOBAL_CInbTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2916u, sizeof(IUH**))))
#undef GLOBAL_CInwTable
#define GLOBAL_CInwTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2920u, sizeof(IUH**))))
#undef GLOBAL_CIndTable
#define GLOBAL_CIndTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2924u, sizeof(IUH**))))
#undef GLOBAL_COutbTable
#define GLOBAL_COutbTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2928u, sizeof(IUH**))))
#undef GLOBAL_COutwTable
#define GLOBAL_COutwTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2932u, sizeof(IUH**))))
#undef GLOBAL_COutdTable
#define GLOBAL_COutdTable (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2936u, sizeof(IUH**))))
#undef GLOBAL_InAdapFromPort
#define GLOBAL_InAdapFromPort (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 2940u, sizeof(IU8*))))
#undef GLOBAL_OutAdapFromPort
#define GLOBAL_OutAdapFromPort (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 2944u, sizeof(IU8*))))
#undef GLOBAL_InbFuncWrapper
#define GLOBAL_InbFuncWrapper (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2948u, sizeof(IUH**))))
#undef GLOBAL_InwFuncWrapper
#define GLOBAL_InwFuncWrapper (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2952u, sizeof(IUH**))))
#undef GLOBAL_IndFuncWrapper
#define GLOBAL_IndFuncWrapper (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2956u, sizeof(IUH**))))
#undef GLOBAL_OutbFuncWrapper
#define GLOBAL_OutbFuncWrapper (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2960u, sizeof(IUH**))))
#undef GLOBAL_OutwFuncWrapper
#define GLOBAL_OutwFuncWrapper (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2964u, sizeof(IUH**))))
#undef GLOBAL_OutdFuncWrapper
#define GLOBAL_OutdFuncWrapper (*((IUH** *)softpc_gdp_slot((const void *)Gdp, 2968u, sizeof(IUH**))))
#undef GLOBAL_TempByteDest
#define GLOBAL_TempByteDest (*((IU8 *)softpc_gdp_slot((const void *)Gdp, 2975u, sizeof(IU8))))
#undef GLOBAL_TempWordDest
#define GLOBAL_TempWordDest (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 2978u, sizeof(IU16))))
#undef GLOBAL_TempDoubleDest
#define GLOBAL_TempDoubleDest (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 2980u, sizeof(IU32))))
#undef GLOBAL_MaxValidAdaptor
#define GLOBAL_MaxValidAdaptor (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2984u, sizeof(IUH))))
#undef GLOBAL_IOSPortMask
#define GLOBAL_IOSPortMask (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 2990u, sizeof(IU16))))
#undef GLOBAL_EDL_WORKSPACE_189
#define GLOBAL_EDL_WORKSPACE_189 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2992u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_190
#define GLOBAL_EDL_WORKSPACE_190 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 2996u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_191
#define GLOBAL_EDL_WORKSPACE_191 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3000u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_192
#define GLOBAL_EDL_WORKSPACE_192 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3004u, sizeof(IUH))))
#undef GLOBAL_SegDescPtrLookupREC
#define GLOBAL_SegDescPtrLookupREC (*((struct DYNAMIC_DESC_PTR_LOOKUP *)softpc_gdp_slot((const void *)Gdp, 3008u, sizeof(struct DYNAMIC_DESC_PTR_LOOKUP))))
#undef GLOBAL_EDL_WORKSPACE_193
#define GLOBAL_EDL_WORKSPACE_193 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3032u, sizeof(IUH))))
#undef GLOBAL_EDL_WORKSPACE_194
#define GLOBAL_EDL_WORKSPACE_194 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3036u, sizeof(IUH))))
#undef GLOBAL_SegBaseLookupREC
#define GLOBAL_SegBaseLookupREC (*((struct DYNAMIC_SEG_BASE_LOOKUP *)softpc_gdp_slot((const void *)Gdp, 3040u, sizeof(struct DYNAMIC_SEG_BASE_LOOKUP))))
#undef GLOBAL_HSP
#define GLOBAL_HSP (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3064u, sizeof(IU8*))))
#undef GLOBAL_ESPsanctuary
#define GLOBAL_ESPsanctuary (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3068u, sizeof(IU32))))
#undef GLOBAL_truePopLimit
#define GLOBAL_truePopLimit (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3072u, sizeof(IU8*))))
#undef GLOBAL_innerPopLimit
#define GLOBAL_innerPopLimit (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3076u, sizeof(IU8*))))
#undef GLOBAL_truePushLimit
#define GLOBAL_truePushLimit (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3080u, sizeof(IU8*))))
#undef GLOBAL_innerPushLimit
#define GLOBAL_innerPushLimit (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3084u, sizeof(IU8*))))
#undef GLOBAL_notionalSsBase
#define GLOBAL_notionalSsBase (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3088u, sizeof(IU8*))))
#undef GLOBAL_stackIsWrappable
#define GLOBAL_stackIsWrappable (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3095u, sizeof(IBOOL))))
#undef GLOBAL_stackIsBig
#define GLOBAL_stackIsBig (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3099u, sizeof(IBOOL))))
#undef GLOBAL_stackMask
#define GLOBAL_stackMask (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3100u, sizeof(IU32))))
#undef GLOBAL_stackNeedsNormalising
#define GLOBAL_stackNeedsNormalising (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3107u, sizeof(IBOOL))))
#undef GLOBAL_laInTopPage
#define GLOBAL_laInTopPage (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3108u, sizeof(IU32))))
#undef GLOBAL_laInBottomPage
#define GLOBAL_laInBottomPage (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3112u, sizeof(IU32))))
#undef GLOBAL_hspOK
#define GLOBAL_hspOK (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3119u, sizeof(IBOOL))))
#undef GLOBAL_pushScratch
#define GLOBAL_pushScratch (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3120u, sizeof(IU8*))))
#undef GLOBAL_pushScratchInUse
#define GLOBAL_pushScratchInUse (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3127u, sizeof(IBOOL))))
#undef GLOBAL_popScratch
#define GLOBAL_popScratch (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3128u, sizeof(IU8*))))
#undef GLOBAL_popScratchInUse
#define GLOBAL_popScratchInUse (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3135u, sizeof(IBOOL))))
#undef GLOBAL_stackJumpCounterCeiling
#define GLOBAL_stackJumpCounterCeiling (*((ISH *)softpc_gdp_slot((const void *)Gdp, 3136u, sizeof(ISH))))
#undef GLOBAL_stackJumpCounterDebt
#define GLOBAL_stackJumpCounterDebt (*((ISH *)softpc_gdp_slot((const void *)Gdp, 3140u, sizeof(ISH))))
#undef GLOBAL_prevJumpCounter
#define GLOBAL_prevJumpCounter (*((ISH *)softpc_gdp_slot((const void *)Gdp, 3144u, sizeof(ISH))))
#undef GLOBAL_SafeStackEnabled
#define GLOBAL_SafeStackEnabled (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3151u, sizeof(IBOOL))))
#undef GLOBAL_SafeStackIronFrig
#define GLOBAL_SafeStackIronFrig (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3155u, sizeof(IBOOL))))
#undef GLOBAL_newNanoSafeLow
#define GLOBAL_newNanoSafeLow (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3156u, sizeof(IU32))))
#undef GLOBAL_newNanoSafeSpan
#define GLOBAL_newNanoSafeSpan (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3160u, sizeof(IU32))))
#undef GLOBAL_newHSP
#define GLOBAL_newHSP (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3164u, sizeof(IU8*))))
#undef GLOBAL_newTruePopLimit
#define GLOBAL_newTruePopLimit (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3168u, sizeof(IU8*))))
#undef GLOBAL_newTruePushLimit
#define GLOBAL_newTruePushLimit (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3172u, sizeof(IU8*))))
#undef GLOBAL_newNotionalSsBase
#define GLOBAL_newNotionalSsBase (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3176u, sizeof(IU8*))))
#undef GLOBAL_newStackIsWrappable
#define GLOBAL_newStackIsWrappable (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3183u, sizeof(IBOOL))))
#undef GLOBAL_newStackIsBig
#define GLOBAL_newStackIsBig (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3187u, sizeof(IBOOL))))
#undef GLOBAL_newStackMask
#define GLOBAL_newStackMask (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3188u, sizeof(IU32))))
#undef GLOBAL_newLaInBottomPage
#define GLOBAL_newLaInBottomPage (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3192u, sizeof(IU32))))
#undef GLOBAL_newHspOK
#define GLOBAL_newHspOK (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3199u, sizeof(IBOOL))))
#undef GLOBAL_newPushScratch
#define GLOBAL_newPushScratch (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3200u, sizeof(IU8*))))
#undef GLOBAL_newPushScratchInUse
#define GLOBAL_newPushScratchInUse (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3207u, sizeof(IBOOL))))
#undef GLOBAL_EOIEnable
#define GLOBAL_EOIEnable (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3208u, sizeof(IU8*))))
#undef GLOBAL_AddProfilePtr
#define GLOBAL_AddProfilePtr (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 3212u, sizeof(IUH*))))
#undef GLOBAL_MaxProfileData
#define GLOBAL_MaxProfileData (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 3216u, sizeof(IUH*))))
#undef GLOBAL_FreeContextHead
#define GLOBAL_FreeContextHead (*((struct ContextREC* *)softpc_gdp_slot((const void *)Gdp, 3220u, sizeof(struct ContextREC*))))
#undef GLOBAL_ValidContextHead
#define GLOBAL_ValidContextHead (*((struct ContextREC* *)softpc_gdp_slot((const void *)Gdp, 3224u, sizeof(struct ContextREC*))))
#undef GLOBAL_CurrentContext
#define GLOBAL_CurrentContext (*((IU8 *)softpc_gdp_slot((const void *)Gdp, 3231u, sizeof(IU8))))
#undef GLOBAL_SeenGDTUse
#define GLOBAL_SeenGDTUse (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3235u, sizeof(IBOOL))))
#undef GLOBAL_SeenLDTUse
#define GLOBAL_SeenLDTUse (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3239u, sizeof(IBOOL))))
#undef GLOBAL_Context
#define GLOBAL_Context (*((struct ContextREC* *)softpc_gdp_slot((const void *)Gdp, 3240u, sizeof(struct ContextREC*))))
#undef GLOBAL_tmpESP
#define GLOBAL_tmpESP (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3244u, sizeof(IUH))))
#undef GLOBAL_intelPtr
#define GLOBAL_intelPtr (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3248u, sizeof(IU8*))))
#undef GLOBAL_functions
#define GLOBAL_functions (*((IU32** *)softpc_gdp_slot((const void *)Gdp, 3252u, sizeof(IU32**))))
#undef GLOBAL_instrCountdown
#define GLOBAL_instrCountdown (*((ISH *)softpc_gdp_slot((const void *)Gdp, 3256u, sizeof(ISH))))
#undef GLOBAL_nextPlace
#define GLOBAL_nextPlace (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 3260u, sizeof(IU32*))))
#undef GLOBAL_cutBackStackCounter
#define GLOBAL_cutBackStackCounter (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3264u, sizeof(IUH))))
#undef GLOBAL_nanoCompParameter
#define GLOBAL_nanoCompParameter (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3268u, sizeof(IUH))))
#undef GLOBAL_intelCopyPtrComp
#define GLOBAL_intelCopyPtrComp (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3272u, sizeof(IU8*))))
#undef GLOBAL_nrOfBlocksToCompile
#define GLOBAL_nrOfBlocksToCompile (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3276u, sizeof(IUH))))
#undef GLOBAL_first32constraints
#define GLOBAL_first32constraints (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3280u, sizeof(IU32))))
#undef GLOBAL_last16constraints
#define GLOBAL_last16constraints (*((IU16 *)softpc_gdp_slot((const void *)Gdp, 3286u, sizeof(IU16))))
#undef GLOBAL_successorBlockPtr
#define GLOBAL_successorBlockPtr (*((struct BLOCK_RECORD* *)softpc_gdp_slot((const void *)Gdp, 3288u, sizeof(struct BLOCK_RECORD*))))
#undef GLOBAL_successorBlockNr
#define GLOBAL_successorBlockNr (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3292u, sizeof(IUH))))
#undef GLOBAL_nrOfInstrsParsed
#define GLOBAL_nrOfInstrsParsed (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3296u, sizeof(IUH))))
#undef GLOBAL_intelPtrLimit
#define GLOBAL_intelPtrLimit (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3300u, sizeof(IU8*))))
#undef GLOBAL_blockStartIntelPtr
#define GLOBAL_blockStartIntelPtr (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3304u, sizeof(IU8*))))
#undef GLOBAL_blockStartCodeOffset
#define GLOBAL_blockStartCodeOffset (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3308u, sizeof(IUH))))
#undef GLOBAL_finalCodeOffset
#define GLOBAL_finalCodeOffset (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3312u, sizeof(IUH))))
#undef GLOBAL_blockExitCondition
#define GLOBAL_blockExitCondition (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3316u, sizeof(IUH))))
#undef GLOBAL_blockNr
#define GLOBAL_blockNr (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3320u, sizeof(IUH))))
#undef GLOBAL_blockPtr
#define GLOBAL_blockPtr (*((struct BLOCK_RECORD* *)softpc_gdp_slot((const void *)Gdp, 3324u, sizeof(struct BLOCK_RECORD*))))
#undef GLOBAL_nextPlaceAgain
#define GLOBAL_nextPlaceAgain (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 3328u, sizeof(IU32*))))
#undef GLOBAL_nanoOp3
#define GLOBAL_nanoOp3 (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3332u, sizeof(IUH))))
#undef GLOBAL_sibByte
#define GLOBAL_sibByte (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3336u, sizeof(IUH))))
#undef GLOBAL_dynamicOffset
#define GLOBAL_dynamicOffset (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3340u, sizeof(IU32))))
#undef GLOBAL_eaSegCode
#define GLOBAL_eaSegCode (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3344u, sizeof(IUH))))
#undef GLOBAL_instrStartIntelPtr
#define GLOBAL_instrStartIntelPtr (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3348u, sizeof(IU8*))))
#undef GLOBAL_topLevel
#define GLOBAL_topLevel (*((IUH* *)softpc_gdp_slot((const void *)Gdp, 3352u, sizeof(IUH*))))
#undef GLOBAL_defaultPrimaryActions
#define GLOBAL_defaultPrimaryActions (*((IU32** *)softpc_gdp_slot((const void *)Gdp, 3356u, sizeof(IU32**))))
#undef GLOBAL_actualPrimaryActions
#define GLOBAL_actualPrimaryActions (*((IU32** *)softpc_gdp_slot((const void *)Gdp, 3360u, sizeof(IU32**))))
#undef GLOBAL_codeSegment
#define GLOBAL_codeSegment (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3364u, sizeof(IUH))))
#undef GLOBAL_codeOffset
#define GLOBAL_codeOffset (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3368u, sizeof(IUH))))
#undef GLOBAL_codeSegmentBase
#define GLOBAL_codeSegmentBase (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3372u, sizeof(IUH))))
#undef GLOBAL_codeSegmentLimit
#define GLOBAL_codeSegmentLimit (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3376u, sizeof(IUH))))
#undef GLOBAL_destCodeSegment
#define GLOBAL_destCodeSegment (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3380u, sizeof(IUH))))
#undef GLOBAL_destCodeOffset
#define GLOBAL_destCodeOffset (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3384u, sizeof(IUH))))
#undef GLOBAL_linearAddress
#define GLOBAL_linearAddress (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3388u, sizeof(IU32))))
#undef GLOBAL_nanoEax
#define GLOBAL_nanoEax (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3392u, sizeof(IU32))))
#undef GLOBAL_nanoEcx
#define GLOBAL_nanoEcx (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3396u, sizeof(IU32))))
#undef GLOBAL_nanoEdx
#define GLOBAL_nanoEdx (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3400u, sizeof(IU32))))
#undef GLOBAL_nanoEbx
#define GLOBAL_nanoEbx (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3404u, sizeof(IU32))))
#undef GLOBAL_nanoEsp
#define GLOBAL_nanoEsp (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3408u, sizeof(IU32))))
#undef GLOBAL_nanoEbp
#define GLOBAL_nanoEbp (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3412u, sizeof(IU32))))
#undef GLOBAL_nanoEsi
#define GLOBAL_nanoEsi (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3416u, sizeof(IU32))))
#undef GLOBAL_nanoEdi
#define GLOBAL_nanoEdi (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3420u, sizeof(IU32))))
#undef GLOBAL_espToRestore
#define GLOBAL_espToRestore (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3424u, sizeof(IU32))))
#undef GLOBAL_entryExitCount
#define GLOBAL_entryExitCount (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3428u, sizeof(IU32))))
#undef GLOBAL_instructionCount
#define GLOBAL_instructionCount (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3432u, sizeof(IU32))))
#undef GLOBAL_nanoDebugControl
#define GLOBAL_nanoDebugControl (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3436u, sizeof(IU32))))
#undef GLOBAL_compilationThreshold
#define GLOBAL_compilationThreshold (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3440u, sizeof(IU32))))
#undef GLOBAL_hashTableMask
#define GLOBAL_hashTableMask (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3444u, sizeof(IUH))))
#undef GLOBAL_blockFreeList
#define GLOBAL_blockFreeList (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3448u, sizeof(IUH))))
#undef GLOBAL_maxBlocksToCompile
#define GLOBAL_maxBlocksToCompile (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3452u, sizeof(IUH))))
#undef GLOBAL_bases
#define GLOBAL_bases (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 3456u, sizeof(IU32*))))
#undef GLOBAL_descriptors
#define GLOBAL_descriptors (*((struct GLDC_REC** *)softpc_gdp_slot((const void *)Gdp, 3460u, sizeof(struct GLDC_REC**))))
#undef GLOBAL_blocksToCompile
#define GLOBAL_blocksToCompile (*((struct BLOCK_TO_COMPILE* *)softpc_gdp_slot((const void *)Gdp, 3464u, sizeof(struct BLOCK_TO_COMPILE*))))
#undef GLOBAL_byteMemory
#define GLOBAL_byteMemory (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3468u, sizeof(IU8*))))
#undef GLOBAL_wordMemory
#define GLOBAL_wordMemory (*((IU16* *)softpc_gdp_slot((const void *)Gdp, 3472u, sizeof(IU16*))))
#undef GLOBAL_longMemory
#define GLOBAL_longMemory (*((IU32* *)softpc_gdp_slot((const void *)Gdp, 3476u, sizeof(IU32*))))
#undef GLOBAL_blockHashChains
#define GLOBAL_blockHashChains (*((IU16* *)softpc_gdp_slot((const void *)Gdp, 3480u, sizeof(IU16*))))
#undef GLOBAL_blockRecords
#define GLOBAL_blockRecords (*((struct BLOCK_RECORD* *)softpc_gdp_slot((const void *)Gdp, 3484u, sizeof(struct BLOCK_RECORD*))))
#undef GLOBAL_intelCopyMemoryExec
#define GLOBAL_intelCopyMemoryExec (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3488u, sizeof(IU8*))))
#undef GLOBAL_intelCopyMemoryComp
#define GLOBAL_intelCopyMemoryComp (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3492u, sizeof(IU8*))))
#undef GLOBAL_intelCopyMemoryCompEnd
#define GLOBAL_intelCopyMemoryCompEnd (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3496u, sizeof(IU8*))))
#undef GLOBAL_SfDecrementerVal
#define GLOBAL_SfDecrementerVal (*((IUH *)softpc_gdp_slot((const void *)Gdp, 3500u, sizeof(IUH))))
#undef GLOBAL_SfQEventPending
#define GLOBAL_SfQEventPending (*((IBOOL *)softpc_gdp_slot((const void *)Gdp, 3507u, sizeof(IBOOL))))
#undef GLOBAL_LogicalBaseAddrForIO
#define GLOBAL_LogicalBaseAddrForIO (*((IU8* *)softpc_gdp_slot((const void *)Gdp, 3508u, sizeof(IU8*))))
#undef GLOBAL_CurrentUniverse
#define GLOBAL_CurrentUniverse (*((IU32 *)softpc_gdp_slot((const void *)Gdp, 3512u, sizeof(IU32))))
#undef GLOBAL_TraceVector
#define GLOBAL_TraceVector ((IU8 *)softpc_gdp_slot((const void *)Gdp, 3516u, GLOBAL_TraceVectorSize))

#endif /* SOFTPC_GDP_SLOTS_H */
