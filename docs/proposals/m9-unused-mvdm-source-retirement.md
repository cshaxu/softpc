# M9: Retire Unused MVDM Source

## Objective

Shrink `src/mvdm/softpc.new/` to the selected original SoftPC source subset
that the fixed standalone machine actually adopts. Remove only original code
that is demonstrably neither selected by CMake nor required by a source or
transformation input.

## Candidate Work

The initial, confirmed C-source removal set is 15 files (4,921 source lines):

- old EVID route: `base/cpu/src/evid/{ev_glue.c,ev_stubs.c,vglob.c}`;
- unused GFI media-format backends: `base/disks/{gfi_ibm.c,gfi_sflp.c}`;
- historical product startup, termination, environment and XT support:
  `base/support/{get_env.c,main.c,terminat.c,xt.c}`;
- historical CMOS/quick-event variants: `base/system/{cmosnt.c,qevnt.c}`;
- unselected duplicate CCPU/C-VID units:
  `base/ccpu386/{localfm.c,sascdef.c}` and
  `base/cvidc/{ev_stubs.c,vglob.c}`.

The task must separately audit the 279 headers currently not named by any
`#include`. It may delete only headers which remain unreferenced after a clean
dual-width build; C-VIDC headers and any header consumed by generated or
transformed code require explicit proof before removal.

### Header Audit Baseline

The initial filename-include scan found 279 candidate headers. A second scan
over active source, CMake, transforms and tests found 266 whose filename has
no active mention at all. The numerical difference is not a retention list:
several historical headers share a basename with an active x86 header, so a
basename match is insufficient evidence either way.

The header cleanup must use exact paths and compiler dependency output. Its
initial removal candidates are:

- all 93 `host/inc/mips/**` headers: unused MIPS host/profile variants;
- 88 unreferenced `base/cvidc/**` generated-rule headers, while explicitly
  retaining the active C-VIDC bridge headers (`evid_c.h`, `evidfunc.h`,
  `evidgen.h`, `j_c_lang.h`, `cpu4gen.h`, `gdpvar.h`, `sas4gen.h`,
  `pigger_c.h`, `univer_c.h` and their actual dependencies);
- 73 `base/inc/**` candidates, including DPMI, UMB, IPX, CD-ROM, old media
  format, product configuration and obsolete host-interface declarations;
- 25 remaining `host/inc/**` candidates, primarily NT/product monitor,
  thread, VDD, configuration and legacy endpoint declarations.

The task must not delete an active header merely because its basename appears
in this audit. It must prove the exact candidate path is absent from generated
compiler dependencies for both host widths.

### Exact Deletion Manifest

This is the executable initial removal manifest. Every `C0` row was checked
against CMake's source and transform inputs and is not selected. Every `H0`
row was checked for (a) no matching `#include` filename anywhere below
`src/`, and (b) no exact relative-path reference in `CMakeLists.txt`,
`scripts/`, or `test/`. `H0` rows still require the dual-width compiler
dependency proof stated in Verification before deletion.

```text
C0 base/cpu/src/evid/ev_glue.c
C0 base/cpu/src/evid/ev_stubs.c
C0 base/cpu/src/evid/vglob.c
C0 base/disks/gfi_ibm.c
C0 base/disks/gfi_sflp.c
C0 base/support/get_env.c
C0 base/support/main.c
C0 base/support/terminat.c
C0 base/support/xt.c
C0 base/system/cmosnt.c
C0 base/system/qevnt.c
C0 base/ccpu386/localfm.c
C0 base/ccpu386/sascdef.c
C0 base/cvidc/ev_stubs.c
C0 base/cvidc/vglob.c

H0 base/cvidc/acc_efi.h
H0 base/cvidc/access_c.h
H0 base/cvidc/add_c.h
H0 base/cvidc/adjust_c.h
H0 base/cvidc/axphst_c.h
H0 base/cvidc/bldr_c.h
H0 base/cvidc/bodge_c.h
H0 base/cvidc/bound_c.h
H0 base/cvidc/bpi_c.h
H0 base/cvidc/bswap_c.h
H0 base/cvidc/btest_c.h
H0 base/cvidc/buffer_c.h
H0 base/cvidc/c2cpug_c.h
H0 base/cvidc/c2cpusad.h
H0 base/cvidc/c2cput_c.h
H0 base/cvidc/chain2_c.h
H0 base/cvidc/chain4_c.h
H0 base/cvidc/cmp_c.h
H0 base/cvidc/contxt_c.h
H0 base/cvidc/conv_c.h
H0 base/cvidc/copies_c.h
H0 base/cvidc/cpnote_c.h
H0 base/cvidc/cpumod_c.h
H0 base/cvidc/cpureg_c.h
H0 base/cvidc/dec_c.h
H0 base/cvidc/ea_c.h
H0 base/cvidc/enter_c.h
H0 base/cvidc/flags_c.h
H0 base/cvidc/flagtb_c.h
H0 base/cvidc/fm_c.h
H0 base/cvidc/fmevid_c.h
H0 base/cvidc/frag_c.h
H0 base/cvidc/gdebug.h
H0 base/cvidc/gdinit_c.h
H0 base/cvidc/gdmain_c.h
H0 base/cvidc/gicvid_c.h
H0 base/cvidc/gldc_c.h
H0 base/cvidc/hpphst_c.h
H0 base/cvidc/idmain_c.h
H0 base/cvidc/inc_c.h
H0 base/cvidc/inst_c.h
H0 base/cvidc/io_c.h
H0 base/cvidc/lc_c.h
H0 base/cvidc/light_c.h
H0 base/cvidc/logic_c.h
H0 base/cvidc/lxs_c.h
H0 base/cvidc/mark_c.h
H0 base/cvidc/misc_c.h
H0 base/cvidc/move_c.h
H0 base/cvidc/muldiv_c.h
H0 base/cvidc/nano_c.h
H0 base/cvidc/nanoa_c.h
H0 base/cvidc/nanob_c.h
H0 base/cvidc/nanoc_c.h
H0 base/cvidc/nanod_c.h
H0 base/cvidc/nanoe_c.h
H0 base/cvidc/page_c.h
H0 base/cvidc/parser.h
H0 base/cvidc/pm_c.h
H0 base/cvidc/ports_c.h
H0 base/cvidc/ppchst_c.h
H0 base/cvidc/prof_c.h
H0 base/cvidc/prot_c.h
H0 base/cvidc/pti_c.h
H0 base/cvidc/pxcptn_c.h
H0 base/cvidc/rdwr_c.h
H0 base/cvidc/read_c.h
H0 base/cvidc/rotate_c.h
H0 base/cvidc/sas_c.h
H0 base/cvidc/segreg_c.h
H0 base/cvidc/sf_c.h
H0 base/cvidc/shift_c.h
H0 base/cvidc/simple_c.h
H0 base/cvidc/stack_c.h
H0 base/cvidc/string_c.h
H0 base/cvidc/strint_c.h
H0 base/cvidc/strsub_c.h
H0 base/cvidc/stub_c.h
H0 base/cvidc/sub_c.h
H0 base/cvidc/sunhst_c.h
H0 base/cvidc/task_c.h
H0 base/cvidc/test_c.h
H0 base/cvidc/trace_c.h
H0 base/cvidc/unchn_c.h
H0 base/cvidc/vglob_c.h
H0 base/cvidc/xchg_c.h
H0 base/cvidc/xfer_c.h
H0 base/cvidc/xlat_c.h

H0 base/inc/acc_efi.h
H0 base/inc/cdrom_fn.h
H0 base/inc/cdrom.h
H0 base/inc/chkmallc.h
H0 base/inc/cntlbop.h
H0 base/inc/cpu.h
H0 base/inc/cpu2.h
H0 base/inc/cpu3.h
H0 base/inc/d_inst.h
H0 base/inc/d_oper.h
H0 base/inc/dfa.h
H0 base/inc/disk_geo.h
H0 base/inc/dpmi.h
H0 base/inc/dterm.h
H0 base/inc/flexlmif.h
H0 base/inc/getptis.h
H0 base/inc/gfiempty.h
H0 base/inc/gfirflop.h
H0 base/inc/harness.h
H0 base/inc/haw.h
H0 base/inc/hfx.h
H0 base/inc/hg_sas.h
H0 base/inc/hg_stat.h
H0 base/inc/host_fio.h
H0 base/inc/host_hun.h
H0 base/inc/host_msw.h
H0 base/inc/host_nls.h
H0 base/inc/host_pth.h
H0 base/inc/host_sig.h
H0 base/inc/host_str.h
H0 base/inc/host_uis.h
H0 base/inc/hosthunt.h
H0 base/inc/ipx.h
H0 base/inc/jcgensyn.h
H0 base/inc/jformate.h
H0 base/inc/keycaps.h
H0 base/inc/kybdmtx.h
H0 base/inc/lm_attr.h
H0 base/inc/lm_clien.h
H0 base/inc/lm_code.h
H0 base/inc/loader.h
H0 base/inc/lock.h
H0 base/inc/low_intp.h
H0 base/inc/lq2500.h
H0 base/inc/m_errorp.h
H0 base/inc/m_fs.h
H0 base/inc/m_keybdp.h
H0 base/inc/m_lpcmp.h
H0 base/inc/make_dis.h
H0 base/inc/mark_ima.h
H0 base/inc/mda.h
H0 base/inc/ms_windo.h
H0 base/inc/msw_dbg.h
H0 base/inc/msw_keyb.h
H0 base/inc/msw_snd.h
H0 base/inc/profile.h
H0 base/inc/rommap.h
H0 base/inc/smeg_hea.h
H0 base/inc/smeg.h
H0 base/inc/spc_icon.h
H0 base/inc/standard.h
H0 base/inc/swinhelp.h
H0 base/inc/swinmgre.h
H0 base/inc/tcpip.h
H0 base/inc/tredltr.h
H0 base/inc/umb.h
H0 base/inc/unix_asy.h
H0 base/inc/unix_cnf.h
H0 base/inc/unix_loc.h
H0 base/inc/vgastate.h
H0 base/inc/winfiles.h
H0 base/inc/winhelp.h
H0 base/inc/worm.h

H0 host/inc/host_emm.h
H0 host/inc/host_trc.h
H0 host/inc/hostgrph.h
H0 host/inc/hostsync.h
H0 host/inc/hosttyps.h
H0 host/inc/hstsystm.h
H0 host/inc/nt_com.h
H0 host/inc/nt_conf.h
H0 host/inc/nt_fdisk.h
H0 host/inc/nt_getxx.h
H0 host/inc/nt_hunt.h
H0 host/inc/nt_inthk.h
H0 host/inc/nt_mem.h
H0 host/inc/nt_mon.h
H0 host/inc/nt_paint.h
H0 host/inc/nt_smenu.h
H0 host/inc/nt_thred.h
H0 host/inc/nt_timer.h
H0 host/inc/nt_vddp.h
H0 host/inc/sdos_ddi.h
H0 host/inc/sim32.h
H0 host/inc/winsigni.h
H0 host/inc/xmsexp.h
H0 host/inc/ppc/pig/c2cpusad.h
H0 host/inc/ppc/prod/c2cpusad.h

H0 host/inc/mips/pig/c2cpusad.h
H0 host/inc/mips/prod/acc_efi.h
H0 host/inc/mips/prod/access_c.h
H0 host/inc/mips/prod/add_c.h
H0 host/inc/mips/prod/adjust_c.h
H0 host/inc/mips/prod/api_c.h
H0 host/inc/mips/prod/axphst_c.h
H0 host/inc/mips/prod/bldr_c.h
H0 host/inc/mips/prod/bodge_c.h
H0 host/inc/mips/prod/bound_c.h
H0 host/inc/mips/prod/bpi_c.h
H0 host/inc/mips/prod/bswap_c.h
H0 host/inc/mips/prod/btest_c.h
H0 host/inc/mips/prod/buffer_c.h
H0 host/inc/mips/prod/c2cpusad.h
H0 host/inc/mips/prod/c2cput_c.h
H0 host/inc/mips/prod/c2prit_c.h
H0 host/inc/mips/prod/c2sast_c.h
H0 host/inc/mips/prod/c2vidt_c.h
H0 host/inc/mips/prod/chain2_c.h
H0 host/inc/mips/prod/chain4_c.h
H0 host/inc/mips/prod/cmp_c.h
H0 host/inc/mips/prod/contxt_c.h
H0 host/inc/mips/prod/conv_c.h
H0 host/inc/mips/prod/copies_c.h
H0 host/inc/mips/prod/cpnote_c.h
H0 host/inc/mips/prod/cpumod_c.h
H0 host/inc/mips/prod/cpureg_c.h
H0 host/inc/mips/prod/dec_c.h
H0 host/inc/mips/prod/ea_c.h
H0 host/inc/mips/prod/enter_c.h
H0 host/inc/mips/prod/evptrs_c.h
H0 host/inc/mips/prod/flags_c.h
H0 host/inc/mips/prod/flagtb_c.h
H0 host/inc/mips/prod/fm_c.h
H0 host/inc/mips/prod/fmevid_c.h
H0 host/inc/mips/prod/frag_c.h
H0 host/inc/mips/prod/gdebug.h
H0 host/inc/mips/prod/gdinit_c.h
H0 host/inc/mips/prod/gdmain_c.h
H0 host/inc/mips/prod/gicvid_c.h
H0 host/inc/mips/prod/gldc_c.h
H0 host/inc/mips/prod/hpphst_c.h
H0 host/inc/mips/prod/idmain_c.h
H0 host/inc/mips/prod/inc_c.h
H0 host/inc/mips/prod/inst_c.h
H0 host/inc/mips/prod/io_c.h
H0 host/inc/mips/prod/lc_c.h
H0 host/inc/mips/prod/light_c.h
H0 host/inc/mips/prod/logic_c.h
H0 host/inc/mips/prod/lxs_c.h
H0 host/inc/mips/prod/mark_c.h
H0 host/inc/mips/prod/miphst_c.h
H0 host/inc/mips/prod/misc_c.h
H0 host/inc/mips/prod/move_c.h
H0 host/inc/mips/prod/muldiv_c.h
H0 host/inc/mips/prod/nano_c.h
H0 host/inc/mips/prod/nanoa_c.h
H0 host/inc/mips/prod/nanob_c.h
H0 host/inc/mips/prod/nanoc_c.h
H0 host/inc/mips/prod/nanod_c.h
H0 host/inc/mips/prod/nanoe_c.h
H0 host/inc/mips/prod/page_c.h
H0 host/inc/mips/prod/parser.h
H0 host/inc/mips/prod/pm_c.h
H0 host/inc/mips/prod/ports_c.h
H0 host/inc/mips/prod/ppchst_c.h
H0 host/inc/mips/prod/prof_c.h
H0 host/inc/mips/prod/prot_c.h
H0 host/inc/mips/prod/pti_c.h
H0 host/inc/mips/prod/pxcptn_c.h
H0 host/inc/mips/prod/rdwr_c.h
H0 host/inc/mips/prod/read_c.h
H0 host/inc/mips/prod/rotate_c.h
H0 host/inc/mips/prod/sas_c.h
H0 host/inc/mips/prod/segreg_c.h
H0 host/inc/mips/prod/sf_c.h
H0 host/inc/mips/prod/shift_c.h
H0 host/inc/mips/prod/simple_c.h
H0 host/inc/mips/prod/stack_c.h
H0 host/inc/mips/prod/string_c.h
H0 host/inc/mips/prod/strint_c.h
H0 host/inc/mips/prod/strsub_c.h
H0 host/inc/mips/prod/stub_c.h
H0 host/inc/mips/prod/sub_c.h
H0 host/inc/mips/prod/sunhst_c.h
H0 host/inc/mips/prod/task_c.h
H0 host/inc/mips/prod/test_c.h
H0 host/inc/mips/prod/trace_c.h
H0 host/inc/mips/prod/unchn_c.h
H0 host/inc/mips/prod/xchg_c.h
H0 host/inc/mips/prod/xfer_c.h
H0 host/inc/mips/prod/xlat_c.h
```

## Non-goals

- Do not remove or alter the active `CPU_40_STYLE + CCPU + C_VID` route:
  `base/cvidc/` generated rules, CCPU, the C-VID GDP bridge, or EGA/VGA/V7
  controller code.
- Do not change guest behavior, timing, BIOS/BOP behavior, media semantics,
  machine profile, ROMs, or the standalone host ABI.
- Do not rewrite historical evidence merely because it records a file that
  was present at the time of that evidence.

## Rationale

The mirror is a selected original source subset, not a full archival copy.
Keeping obsolete EVID, non-selected GFI format backends, and NT/product-era
alternate startup/event paths makes the active machine boundary harder to
audit. Their absence from CMake means they cannot provide current VM behavior.

## Verification

1. Re-run a source-selection audit: no retained removal candidate may appear
   in CMake, a transformation input, or a reachable include dependency.
2. Emit compiler dependency files for x64 and x86 and compare exact header
   paths, including transformed CCPU/C-VIDC sources, before deleting a header.
3. Confirm CMake has no reference to each removed path.
4. Configure, build, and run full CTest with GCC for both x64 and x86.
5. Run package smoke for both produced launchers and confirm the fixed
   C-VIDC/V7 machine still boots.

## Exit

Every confirmed unused C source above is absent; any removed headers have
dependency evidence; no stale build/test/configuration path remains; and
dual-width verification preserves current behavior.
