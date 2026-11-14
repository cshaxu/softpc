# T100 S4 real-difference file list

Frozen SoftPC: `6ff24aee` (production `cda2d427`). XP SP1:
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`.
All paths below are relative to `src/app-softpc/softpc.new/`.

## Screening result

| Disposition | Files |
| --- | ---: |
| Identical after CRLF/LF normalization; do not re-audit | 254 |
| Only existing excluded bodies and line-control scaffolding differ; do not re-audit | 97 |
| Remaining actual differences; retained for ordered review | 121 |
| Total | 472 |

The [frozen file ledger](m9-t100-s4-files.tsv) records each disposition and both
raw hashes. These are file counts, not semantic points or diff-line counts.
The original literal dir ordering is retained, with gaps for excluded files.

## Comparison boundary

For text, only CRLF/LF is normalized. Binary identity uses exact bytes.
Within the other 218 files, match conditional parent/arm positions and omit
bodies replaced by an existing single #error; retain the surrounding conditions,
else/endif spelling, comments and all other content for exact comparison.
Line-control scaffolding is separated from retained content. No blanket
whitespace/comment normalization or semantic equivalence is used.

All 97 excluded files have identical retained content and conditional framing
under that comparison. No unmatched conditional mapping is classified as equal.
Every one of the 121 listed files has a retained-content difference, even when
ignoring line controls; none is present solely because of a conservative
line-control or unmatched-branch warning. Added/changed conditional frameworks
remain real differences. gvi.h, ppi.c and nt_cga.c also need line-control review;
that does not remove their independently confirmed retained differences.

A real difference can be a historical notice, spelling/compiler accommodation,
ABI boundary, product choice or behavioral fix. This list does not approve
retention or declare 121 bugs. The 254+97 files are removed from the subsequent
semantic audit workload unless their frozen bytes change.

## Directory totals

| Directory | Files |
| --- | ---: |
| base/bios | 3 |
| base/ccpu386 | 14 |
| base/comms | 4 |
| base/cvidc | 36 |
| base/disks | 5 |
| base/inc | 15 |
| base/keymouse | 4 |
| base/support | 1 |
| base/system | 8 |
| base/video | 12 |
| host/inc | 7 |
| host/src | 12 |

## Complete list in literal dir file order

| Original order | File |
| ---: | --- |
| 6 | `base/bios/reset.c` |
| 8 | `base/bios/slave_bs.c` |
| 9 | `base/bios/tape_io.c` |
| 41 | `base/ccpu386/c-files` |
| 42 | `base/ccpu386/call.c` |
| 47 | `base/ccpu386/ccpusas4.c` |
| 64 | `base/ccpu386/cpu4gen.h` |
| 73 | `base/ccpu386/c_bsic.h` |
| 74 | `base/ccpu386/c_debug.c` |
| 78 | `base/ccpu386/c_intr.c` |
| 80 | `base/ccpu386/c_main.c` |
| 88 | `base/ccpu386/c_page.c` |
| 98 | `base/ccpu386/c_tlb.c` |
| 121 | `base/ccpu386/fpu.c` |
| 182 | `base/ccpu386/ntstubs.c` |
| 183 | `base/ccpu386/ntthread.c` |
| 265 | `base/ccpu386/zfrsrvd.c` |
| 267 | `base/comms/com.c` |
| 268 | `base/comms/printer.c` |
| 269 | `base/comms/printer_.c` |
| 270 | `base/comms/rs232_io.c` |
| 275 | `base/cvidc/ev_glue.c` |
| 283 | `base/cvidc/sevid000.c` |
| 284 | `base/cvidc/sevid001.c` |
| 285 | `base/cvidc/sevid002.c` |
| 286 | `base/cvidc/sevid003.c` |
| 287 | `base/cvidc/sevid004.c` |
| 288 | `base/cvidc/sevid005.c` |
| 289 | `base/cvidc/sevid006.c` |
| 290 | `base/cvidc/sevid007.c` |
| 291 | `base/cvidc/sevid008.c` |
| 292 | `base/cvidc/sevid009.c` |
| 293 | `base/cvidc/sevid010.c` |
| 294 | `base/cvidc/sevid011.c` |
| 295 | `base/cvidc/sevid012.c` |
| 296 | `base/cvidc/sevid013.c` |
| 297 | `base/cvidc/sevid014.c` |
| 298 | `base/cvidc/sevid015.c` |
| 299 | `base/cvidc/sevid016.c` |
| 300 | `base/cvidc/sevid017.c` |
| 301 | `base/cvidc/sevid018.c` |
| 302 | `base/cvidc/sevid019.c` |
| 303 | `base/cvidc/sevid020.c` |
| 304 | `base/cvidc/sevid021.c` |
| 305 | `base/cvidc/sevid022.c` |
| 306 | `base/cvidc/sevid023.c` |
| 307 | `base/cvidc/sevid024.c` |
| 308 | `base/cvidc/sevid025.c` |
| 309 | `base/cvidc/sevid026.c` |
| 310 | `base/cvidc/sevid027.c` |
| 311 | `base/cvidc/sevid028.c` |
| 312 | `base/cvidc/sevid029.c` |
| 313 | `base/cvidc/sevid030.c` |
| 314 | `base/cvidc/sinit011.c` |
| 315 | `base/cvidc/sinit012.c` |
| 316 | `base/cvidc/sinit013.c` |
| 318 | `base/cvidc/vglfunc.c` |
| 319 | `base/disks/diskbios.c` |
| 320 | `base/disks/fdisk.c` |
| 321 | `base/disks/fla.c` |
| 324 | `base/disks/gfi.c` |
| 325 | `base/disks/gfi_mpty.c` |
| 333 | `base/inc/config.h` |
| 335 | `base/inc/cpu_vid.h` |
| 337 | `base/inc/dma.h` |
| 339 | `base/inc/egacpu.h` |
| 342 | `base/inc/egaports.h` |
| 358 | `base/inc/gmi.h` |
| 359 | `base/inc/gvi.h` |
| 367 | `base/inc/ica.h` |
| 369 | `base/inc/ios.h` |
| 377 | `base/inc/quick_ev.h` |
| 379 | `base/inc/rs232.h` |
| 381 | `base/inc/sas.h` |
| 383 | `base/inc/sndblst.h` |
| 386 | `base/inc/tape_io.h` |
| 394 | `base/inc/video.h` |
| 398 | `base/keymouse/keyba.c` |
| 399 | `base/keymouse/keybd_io.c` |
| 400 | `base/keymouse/mouse.c` |
| 401 | `base/keymouse/ppi.c` |
| 403 | `base/support/time_day.c` |
| 404 | `base/system/at_dma.c` |
| 405 | `base/system/cmos.c` |
| 406 | `base/system/dummy_nt.c` |
| 407 | `base/system/ica.c` |
| 410 | `base/system/quick_ev.c` |
| 411 | `base/system/rom.c` |
| 412 | `base/system/timer.c` |
| 414 | `base/system/unexp_nt.c` |
| 416 | `base/video/egawrtm0.c` |
| 419 | `base/video/ega_prts.c` |
| 420 | `base/video/ega_read.c` |
| 421 | `base/video/ega_vide.c` |
| 422 | `base/video/ega_writ.c` |
| 423 | `base/video/egwrtm12.c` |
| 424 | `base/video/gfx_updt.c` |
| 426 | `base/video/v7_ports.c` |
| 427 | `base/video/v7_video.c` |
| 428 | `base/video/vga_mode.c` |
| 429 | `base/video/vga_prts.c` |
| 431 | `base/video/video.c` |
| 433 | `host/inc/cfpu_def.h` |
| 436 | `host/inc/host_cpu.h` |
| 437 | `host/inc/host_def.h` |
| 440 | `host/inc/host_hfx.h` |
| 444 | `host/inc/insignia.h` |
| 453 | `host/inc/nt_graph.h` |
| 455 | `host/inc/nt_sb.h` |
| 457 | `host/src/nt_cga.c` |
| 458 | `host/src/nt_ega.c` |
| 459 | `host/src/nt_graph.c` |
| 460 | `host/src/nt_keycd.c` |
| 461 | `host/src/nt_munge.c` |
| 462 | `host/src/nt_sb.c` |
| 463 | `host/src/nt_sbdsp.c` |
| 464 | `host/src/nt_sbfm.c` |
| 465 | `host/src/nt_sbmid.c` |
| 466 | `host/src/nt_sbmxr.c` |
| 467 | `host/src/nt_sound.c` |
| 468 | `host/src/nt_vga.c` |
