# T100 S4 current real-difference file list

Current source: S4 P55 owner-approved DMA placement restoration from `cf30439b`.
Fixed XP SP1: `a2f6d7c93aa4f11efc51147566d5961c59a8ef89`.
All paths are relative to `src/app-softpc/softpc.new/`.

This is the current actionable inventory: files without real retained-content
differences are removed, not left as pending reviews. Original literal dir
ordinals are preserved; gaps do not imply an unfinished review.
The [frozen file ledger](m9-t100-s4-files.tsv) and
[frozen P2 counts](m9-t100-s4-diff-counts.tsv) remain historical evidence,
not the current pending list. P25 reconciles all 472 identities and current
counts in the [audit record](m9-t100-s4-audit.md#p25-accepted-p24-and-current-aggregate).
P30 removes five further zero-diff rows and applies verified per-file deltas;
see [the cleanup audit](m9-t100-s4-audit.md#p30-nonsemantic-restoration-across-114-files).

P34 removes cpu_vid.h and gmi.h after restoring their original unconditional
declarations, and updates six other rows. Its conditional-wrapper audit is
recorded in the [audit record](m9-t100-s4-audit.md#p34-redundant-non-xp-conditional-wrappers).

## Current screening result

| Disposition | Files |
| --- | ---: |
| Identical after CRLF/LF normalization | 254 |
| Only excluded-body and line-control scaffolding differences | 112 |
| Real retained-content differences listed below | 106 |
| Total | 472 |

## Counting convention

Counts are Git Myers zero-context text blocks, not semantic problems or approval
counts. Omit existing #error bodies and their original counterparts; handle
necessary #line controls separately. Keep added/changed conditional framing,
comments, spacing and other retained-content differences. Normalize CRLF/LF
only. Changed TLS conditions remain counted; unmatched branches are not silently
removed. See the P2/P25 evidence for original and reconciled count methodology.
Retained files can contain already-approved points; this list tracks existence,
while the owner decision ledger tracks approval. No new runtime claim is made.

## Directory totals

| Directory | Files |
| --- | ---: |
| base/bios | 3 |
| base/ccpu386 | 14 |
| base/comms | 2 |
| base/cvidc | 36 |
| base/disks | 3 |
| base/inc | 13 |
| base/keymouse | 3 |
| base/system | 7 |
| base/video | 10 |
| host/inc | 6 |
| host/src | 12 |

## Current list in original literal dir file order

| Order | File | Text blocks | Added | Deleted |
| --- | --- | ---: | ---: | ---: |
| 6 | `base/bios/reset.c` | 1 | +0 | -2 |
| 8 | `base/bios/slave_bs.c` | 1 | +1 | -1 |
| 9 | `base/bios/tape_io.c` | 1 | +1 | -1 |
| 41 | `base/ccpu386/c-files` | 7 | +0 | -9 |
| 42 | `base/ccpu386/call.c` | 1 | +1 | -1 |
| 47 | `base/ccpu386/ccpusas4.c` | 13 | +69 | -11 |
| 64 | `base/ccpu386/cpu4gen.h` | 13 | +13 | -13 |
| 73 | `base/ccpu386/c_bsic.h` | 1 | +1 | -0 |
| 74 | `base/ccpu386/c_debug.c` | 2 | +45 | -0 |
| 78 | `base/ccpu386/c_intr.c` | 1 | +2 | -0 |
| 80 | `base/ccpu386/c_main.c` | 27 | +110 | -16 |
| 88 | `base/ccpu386/c_page.c` | 14 | +20 | -6 |
| 98 | `base/ccpu386/c_tlb.c` | 2 | +58 | -0 |
| 121 | `base/ccpu386/fpu.c` | 5 | +8 | -5 |
| 182 | `base/ccpu386/ntstubs.c` | 4 | +4 | -4 |
| 183 | `base/ccpu386/ntthread.c` | 5 | +36 | -3 |
| 265 | `base/ccpu386/zfrsrvd.c` | 1 | +17 | -0 |
| 267 | `base/comms/com.c` | 5 | +122 | -3 |
| 268 | `base/comms/printer.c` | 2 | +71 | -0 |
| 275 | `base/cvidc/ev_glue.c` | 3 | +3 | -2 |
| 283 | `base/cvidc/sevid000.c` | 408 | +492 | -497 |
| 284 | `base/cvidc/sevid001.c` | 366 | +448 | -447 |
| 285 | `base/cvidc/sevid002.c` | 457 | +594 | -593 |
| 286 | `base/cvidc/sevid003.c` | 521 | +672 | -671 |
| 287 | `base/cvidc/sevid004.c` | 536 | +665 | -664 |
| 288 | `base/cvidc/sevid005.c` | 499 | +664 | -663 |
| 289 | `base/cvidc/sevid006.c` | 542 | +713 | -712 |
| 290 | `base/cvidc/sevid007.c` | 683 | +934 | -933 |
| 291 | `base/cvidc/sevid008.c` | 505 | +686 | -685 |
| 292 | `base/cvidc/sevid009.c` | 343 | +436 | -435 |
| 293 | `base/cvidc/sevid010.c` | 632 | +774 | -773 |
| 294 | `base/cvidc/sevid011.c` | 457 | +570 | -569 |
| 295 | `base/cvidc/sevid012.c` | 509 | +638 | -637 |
| 296 | `base/cvidc/sevid013.c` | 506 | +676 | -675 |
| 297 | `base/cvidc/sevid014.c` | 591 | +794 | -793 |
| 298 | `base/cvidc/sevid015.c` | 519 | +639 | -638 |
| 299 | `base/cvidc/sevid016.c` | 471 | +593 | -630 |
| 300 | `base/cvidc/sevid017.c` | 351 | +469 | -582 |
| 301 | `base/cvidc/sevid018.c` | 366 | +493 | -600 |
| 302 | `base/cvidc/sevid019.c` | 425 | +679 | -903 |
| 303 | `base/cvidc/sevid020.c` | 492 | +636 | -666 |
| 304 | `base/cvidc/sevid021.c` | 533 | +691 | -690 |
| 305 | `base/cvidc/sevid022.c` | 593 | +753 | -752 |
| 306 | `base/cvidc/sevid023.c` | 370 | +487 | -486 |
| 307 | `base/cvidc/sevid024.c` | 406 | +488 | -487 |
| 308 | `base/cvidc/sevid025.c` | 556 | +667 | -666 |
| 309 | `base/cvidc/sevid026.c` | 515 | +598 | -597 |
| 310 | `base/cvidc/sevid027.c` | 408 | +524 | -523 |
| 311 | `base/cvidc/sevid028.c` | 510 | +655 | -654 |
| 312 | `base/cvidc/sevid029.c` | 436 | +530 | -529 |
| 313 | `base/cvidc/sevid030.c` | 299 | +353 | -352 |
| 314 | `base/cvidc/sinit011.c` | 61 | +441 | -440 |
| 315 | `base/cvidc/sinit012.c` | 92 | +700 | -699 |
| 316 | `base/cvidc/sinit013.c` | 171 | +719 | -718 |
| 318 | `base/cvidc/vglfunc.c` | 1 | +5 | -0 |
| 319 | `base/disks/diskbios.c` | 1 | +2 | -2 |
| 320 | `base/disks/fdisk.c` | 2 | +120 | -0 |
| 321 | `base/disks/fla.c` | 3 | +97 | -0 |
| 337 | `base/inc/dma.h` | 1 | +3 | -0 |
| 339 | `base/inc/egacpu.h` | 1 | +4 | -1 |
| 342 | `base/inc/egaports.h` | 1 | +1 | -0 |
| 359 | `base/inc/gvi.h` | 1 | +2 | -0 |
| 367 | `base/inc/ica.h` | 3 | +2 | -1 |
| 369 | `base/inc/ios.h` | 1 | +1 | -1 |
| 377 | `base/inc/quick_ev.h` | 2 | +35 | -0 |
| 381 | `base/inc/sas.h` | 1 | +1 | -1 |
| 383 | `base/inc/sndblst.h` | 2 | +2 | -1 |
| 386 | `base/inc/tape_io.h` | 1 | +0 | -1 |
| 398 | `base/keymouse/keyba.c` | 4 | +121 | -0 |
| 400 | `base/keymouse/mouse.c` | 4 | +57 | -0 |
| 401 | `base/keymouse/ppi.c` | 6 | +25 | -13 |
| 404 | `base/system/at_dma.c` | 15 | +112 | -17 |
| 405 | `base/system/cmos.c` | 4 | +122 | -4 |
| 407 | `base/system/ica.c` | 7 | +119 | -13 |
| 410 | `base/system/quick_ev.c` | 1 | +230 | -0 |
| 411 | `base/system/rom.c` | 3 | +1 | -3 |
| 412 | `base/system/timer.c` | 8 | +371 | -2 |
| 414 | `base/system/unexp_nt.c` | 1 | +0 | -1 |
| 416 | `base/video/egawrtm0.c` | 4 | +4 | -4 |
| 419 | `base/video/ega_prts.c` | 1 | +6 | -0 |
| 421 | `base/video/ega_vide.c` | 1 | +13 | -0 |
| 423 | `base/video/egwrtm12.c` | 11 | +11 | -11 |
| 424 | `base/video/gfx_updt.c` | 131 | +231 | -211 |
| 426 | `base/video/v7_ports.c` | 1 | +1 | -1 |
| 427 | `base/video/v7_video.c` | 1 | +1 | -1 |
| 428 | `base/video/vga_mode.c` | 3 | +3 | -3 |
| 429 | `base/video/vga_prts.c` | 11 | +188 | -23 |
| 431 | `base/video/video.c` | 15 | +15 | -19 |
| 433 | `host/inc/cfpu_def.h` | 13 | +10 | -8 |
| 436 | `host/inc/host_cpu.h` | 2 | +1 | -1 |
| 437 | `host/inc/host_def.h` | 2 | +0 | -2 |
| 444 | `host/inc/insignia.h` | 3 | +10 | -1 |
| 453 | `host/inc/nt_graph.h` | 1 | +2 | -1 |
| 455 | `host/inc/nt_sb.h` | 4 | +52 | -2 |
| 457 | `host/src/nt_cga.c` | 10 | +22 | -52 |
| 458 | `host/src/nt_ega.c` | 9 | +33 | -12 |
| 459 | `host/src/nt_graph.c` | 11 | +31 | -20 |
| 460 | `host/src/nt_keycd.c` | 4 | +2 | -173 |
| 461 | `host/src/nt_munge.c` | 6 | +25 | -1 |
| 462 | `host/src/nt_sb.c` | 24 | +147 | -376 |
| 463 | `host/src/nt_sbdsp.c` | 60 | +523 | -1000 |
| 464 | `host/src/nt_sbfm.c` | 4 | +20 | -102 |
| 465 | `host/src/nt_sbmid.c` | 33 | +285 | -399 |
| 466 | `host/src/nt_sbmxr.c` | 6 | +48 | -0 |
| 467 | `host/src/nt_sound.c` | 12 | +7 | -80 |
| 468 | `host/src/nt_vga.c` | 13 | +37 | -16 |
| **Total** | **106 files** | **15708** | **+24614** | **-24016** |
