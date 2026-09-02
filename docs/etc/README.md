# Supporting Material Index

`etc/` contains task evidence and supporting design detail. It does not define
active work, architecture, or queue order.

- `evidence/` contains reproducible command results and audit records.
- `evidence/m3-t3-s1-boundary-and-cvid-width.md` records the CCPU return-boundary
  result and the associated generated C-VID field-width audit.
- `evidence/m3-t4-s1-lifecycle-boundary.md` records why the original CCPU
  unsimulate path cannot be repurposed for standalone lifecycle control.
- `evidence/m5-renderer-executor-acceptance.md` records original-renderer,
  generated-CCPU return, dual-width regression and Windows Setup acceptance.
- `evidence/m5-original-font-presentation.md` records the original EGA/VGA
  font-plane export used by the Win32 text presentation endpoint.
- `evidence/m5-illegal-driver-bop-restoration.md` records restoration of the
  original incompatible-driver BOP path through a declaration-only port ABI.
- `evidence/m5-keyboard-reset-restoration.md` records restoration of the
  original CPU_40 keyboard reset path through a generated CCPU ABI overlay.
- `evidence/m5-pristine-host-contract-routes.md` maps every current
  pristine-source audit row to an extraction route and next owner.
- `evidence/m7-t1-original-media-host-audit.md` records the symbol-level
  disposition of the original OpenNT raw-media host candidates.
- `evidence/m7-t2-single-image-partitions.md` records the guest INT 13h proof
  that one raw fixed-disk image carries guest-owned partition metadata.
- `evidence/m7-t3-v7-display-compatibility.md` records the original V7 BIOS,
  controller, and `nt_*` painter proof for standalone DIB presentation.
- `evidence/m7-t4-com-lpt-endpoints.md` records optional standalone COM1 and
  LPT1 output endpoints while preserving the original controller paths.
- `evidence/m7-t5-speaker-endpoint.md` records the bounded standalone playback
  endpoint for the original PPI/Timer-2 speaker state machine.
