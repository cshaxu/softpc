# M5 Pristine-Source And Host-Contract Routes

## Audit Result

The canonical-text audit against the selected OpenNT `softpc.new` baseline
reports 60 C/H rows.  The count includes line-ending-normalized source text
only.  It is a routing inventory, not permission to retain a standalone
branch in machine code.

| Rows | Extraction route | Next owner |
| ---: | --- | --- |
| 35 | Reproducible port-ABI overlay | `core/softpc-port-abi` generator |
| 19 | Original host algorithm with independent endpoint | `host/softpc-compat` |
| 6 | Restore original machine source after endpoint extraction | next M5 machine/host-contract cut |

## Exact Route Coverage

### Port-ABI overlay — 35 rows

- CCPU: `base/ccpu386/evid_c.h`, `base/ccpu386/ntthread.c`, and current-only
  `base/ccpu386/softpc_ccpu_facade.c`.  The source peer is restored; generated
  declarations and the verified outer-return adapter own the representation
  change.
- C-VID: `base/cvidc/evidfunc.h`, `evidgen.h`, `j_c_lang.c`, `j_c_lang.h`,
  `sascdef.c`, `sevid019.c`, `sevid020.c`, and `vglfunc.c`.
- Machine declarations: `base/inc/ckmalloc.h`, `cpu_vid.h`, `egacpu.h`,
  `emm.h`, `gmi.h`, `host_com.h`, `host.h`, `ica.h`, `ios.h`, `timeval.h`,
  and `video.h`.
- Native-width/guest-address repairs: `base/comms/com.c`,
  `base/keymouse/keybd_io.c`, `base/keymouse/mouse_io.c`,
  `base/support/time_day.c`, `base/system/cmosnt.c`, `base/system/idetect.c`,
  `base/system/ica.c`, `base/system/rom.c`, `base/support/main.c`, and
  `base/video/gfx_updt.c`.
- Current-only generated declarations: `host/inc/x86/prod/gdpvar.h`,
  `PigReg_c.h`, and `sas4gen.h`.

None of these rows owns a controller policy.  A later implementation moves
each transformation beside the pristine input and proves the x86/x64 generated
result; it does not retain the adjustment in `src/core/softpc`.

### Compatibility host — 19 rows

- Endpoint and declaration carriers: `base/support/ios.c`,
  `host/inc/cfpu_def.h`, `host_cpu.h`, `host_def.h`, `host_emm.h`,
  `insignia.h`, `nt_event.h`, `nt_graph.h`, `nt_inthk.h`, and current-only
  `softpc_standalone_dib.h`.
- Original host algorithms: `host/src/nt_cga.c`, `nt_com.c`, `nt_ega.c`,
  `nt_graph.c`, `nt_keycd.c`, `nt_lpt.c`, `nt_munge.c`, `nt_sound.c`, and
  `nt_vga.c`.

The renderer rows are already governed by completed S1: the original
dispatcher/painters remain and only their finite surface endpoint is supplied
by the independent host.  The remaining rows are host capability contracts,
not device replacements.

### Restore original machine source — 6 rows

- `base/bios/reset.c`: optional LIM, product globals, and attached-media
  configuration must move to named host contracts while original POST is
  restored.
- `base/keymouse/keyba.c`: scan-code ingress and reset notification become
  host/runtime input contracts; original keyboard controller logic remains.
- `base/keymouse/ppi.c`: Timer-2 sound notification is a host audio endpoint.
- `base/system/cmos.c`: fixed-disk presence is read through the media host
  contract rather than a standalone conditional in CMOS logic.
- `base/system/timer.c`: the zero-delay quick-event guard requires an
  evidenced original timer/quick-event contract before it can be removed.
- `base/system/illegalp.c`: unavailable product-driver handling is an
  explicit host-unavailable result, not a standalone conditional in the
  machine.

The next smallest source-changing cut is this last group only after its host
contract declarations are written.  It is bounded to reset, keyboard/PPI,
CMOS, timer and illegal-driver endpoints; it does not alter a BOP selector,
firmware byte, controller protocol, or guest media.

## Reproduction

Run the repository-owned read-only command from the project root with the
selected OpenNT tree.  It reports the same 60 current rows and does not modify
either source tree:

```powershell
.\scripts\audit_pristine_divergence.ps1 -OriginalRoot <selected-opennt-softpc-root>
```

