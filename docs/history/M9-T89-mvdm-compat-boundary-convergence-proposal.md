# M9 T89: MVDM Mirror and Compat Boundary Convergence (archived proposal)

T89 is owner-accepted and closed. The design and admission wording below is
retained historically; see the [completion audit](M9-T89-completion-audit.md).

## T89 S1 design recommendation

Owner admitted this candidate as T89 and requested S1 audit/research/design.
The [T89 record](../history/M9-T89-S1-mirror-compat-convergence.md) freezes all
36 standalone directives across 14 mirror files and gives each an original
behavior, current behavior, chosen owner, receiver and disposition.
[CURRENT](../states/CURRENT.md) alone owns admission/acceptance status.

The executable recommendation is narrower than the original candidate below:

1. **S2: reuse existing endpoints.** Restore original conapi.h spelling in
   four painters through the already-present Compat header; use original
   KeyMsgToKeyCode from Compat and delete the extra Scan-1 lookup ABI/header;
   move the standalone build definition from host_def.h to concrete CMake
   target contracts, including their test consumers. Estimated mirror
   +1/-28, net -27; production overall net reduction about 31--42 lines.
2. **S3: isolate only host-header policy.** Two small Compat headers include
   original cfpu_def.h/nt_graph.h and override only FENV/lock macros. Restore
   those two original macro blocks, retaining endian and painter declarations.
   Estimated mirror +1/-28, net -27; overall production net increase about
   18--38 lines. The owner's next-S admission accepts this bounded ownership
   tradeoff; larger changes still require renewed direction.
3. **S4: verify retained ports and close the finite ledger.** Preserve original
   PIC/PPI state and algorithms, current NT audio endpoint selection, renderer
   transaction/lifecycle/geometry and 64 KiB I/O selection. Correct the reset
   comment: mouse instance initialization does not install a DOS driver.
   Perform full task audit and qualification; no planned hardware rewrite.

4. **S5: complete outstanding qualification.** After owner acceptance of S4,
   run the five existing native desktop tests on each width in a reserved
   desktop period, reuse the accepted build artifacts, and reconcile the
   results with S4's background qualification and final ledger. No planned
   code changes; no additional machine feature or baseline migration.

S2 through S4 are owner-accepted and closed; S5 is now admitted. The admission preserves
the narrowed disposition
standard: every member is either simplified or explicitly retained with an
owner/receiver. Removing every standalone token is not an exit condition.
Do not move PIC predicates to Compat or enable a guest DOS driver as a
structural cleanup. NIDDB initialization succeeds on both widths, but leaves
INT33 and omitted mouse BOP registration unchanged. Synthetic guest INT33
probes and eight existing focused tests per width pass; no DOS-driver package
or Windows guest acceptance is claimed by those probes.

The record contains per-file accounting, concrete include-order requirements,
test gaps and all retained-port reasons. S1 changed no production source;
Current implementation and qualification status belongs to CURRENT.

## Original candidate objective (superseded by the S1 recommendation)

Remove standalone-only implementation selection from the selected
`src/app-softpc/softpc.new/` mirror wherever the original machine algorithm
can execute against a Compat endpoint.  Preserve the original IBM PC device,
CPU and renderer owners; move host-specific implementation choices to
`src/app-softpc/compat/` or to a narrow, documented port ABI.

The goal is not textual zero-diff at the cost of breaking the standalone VM.
It is one executable machine path per responsibility: original MVDM logic
where it is portable, and one Compat endpoint where the original code calls a
retired NT/NTVDM host service.  A necessary callback that has no original
interception point remains a small, explicit mirror port-ABI diff.

## Observed Quality Problem

The current mirror has `SOFTPC_STANDALONE` branches that retain an original
MVDM path while compiling a different standalone path from the same file.
Some are valid ports, but the location obscures ownership and makes the mirror
harder to compare to its source baseline.  This proposal freezes the following
review universe; no unrelated historical `#if`, generated C-VID rule, or
normal hardware state branch is in scope.

| Member | Current disposition hypothesis | Required final disposition |
| --- | --- | --- |
| `host/src/nt_keycd.c` Scan-1 lookup helper | Original static key table is reusable; NT console-record conversion is not. | Retain one narrow table lookup ABI or an equally non-duplicating overlay; do not copy the table or call `BiosKeyToInputRecord`. |
| `host/src/nt_sound.c` | Original speaker state machine is portable; NT Beep-device I/O is not. | Keep PPI/Timer2 frequency selection; replace only the Beep-device sink with Compat audio. |
| `host/src/nt_graph.c` update transaction | Original painters and dirty calculations are portable; no original standalone publication transaction exists. | Retain only a narrow begin/end presentation ABI if no existing call can be intercepted. |
| `host/src/nt_graph.c` lifecycle and V7 geometry | Original fullscreen/NT console assumptions are not standalone facts. | Use a generic host-surface readiness/geometry ABI or a source-qualified minimal mirror correction; no standalone preprocessor choice. |
| `host/src/{nt_cga,nt_ega,nt_vga,nt_munge}.c` | Painter bodies are portable. | Restore original `conapi.h` include spelling through a Compat overlay; no conditional header selection in the mirror. |
| `base/bios/reset.c` DOS mouse driver | It is a guest INT 33/BOP driver, not a Windows kernel driver; current exclusion may lose guest mouse services. | First prove whether original initialization works with single-instance Compat NIDDB storage; retain it if it does, otherwise record the exact incompatible host contract. |
| `base/keymouse/ppi.c` | PPI/Timer2 hardware state is portable. | Select one complete PPI-state audio endpoint, rather than alternate old edge-only and standalone paths. |
| `base/system/ica.c` | Existing PIC logic remains owner; standalone code adds an INTR deassertion repair. | Move predicate/action to Compat behind one explicit PIC synchronization ABI, retaining only its required call site in the mirror. |
| `host/inc/cfpu_def.h` | Old `fpgetsticky`/`fpsetround` host API is unavailable. | Supply a Compat header overlay using the same `Host*` macro contract backed by ISO C `fenv`. |
| `host/inc/nt_graph.h` | Original mutex requires NTVDM-created handles. | Supply a Compat single-executor lock contract; do not put no-op mutex macros behind a mirror-local standalone branch. |
| `base/inc/ios.h` and `host/inc/host_def.h` | Full 16-bit I/O map and standalone target selection are build/profile facts. | Move target selection to build definitions/overlay headers; preserve the required 64 KiB map. |

## Non-goals

- Do not import NTVDM, VDD, CSR, Windows console-server, kernel-driver, or
  product-shell behavior.
- Do not duplicate the original keyboard table, renderer, device controller,
  or audio state machine in Compat.
- Do not remove a mirror difference solely to lower a line count.
- Do not change Lib, Common, x86, guest media, user INI, snapshots, or package
  policy without separate evidence and approval.
- Preserve the accepted T88 V7/CHAIN2 and Compat publication repairs. T88
  is closed; no unhandled repaint TODO is inferred from this old candidate.

## Destination Model and Examples

An overlay keeps the original source spelling and changes only the selected
host implementation:

```c
/* Original painter source remains unchanged. */
#include "conapi.h"

/* Compat/conapi.h maps the historic call surface to the DIB backend. */
#define SetConsolePalette(output, palette, usage) \
    softpc_compat_set_console_palette((output), (palette), (usage))
```

An unavoidable callback is a narrow port ABI, not a runtime replacement of an
original algorithm:

```c
/* Original PPI and timer decisions remain above this call. */
host_audio_set_ppi_state(value);
```

The rejected pattern is a target-specific choice in the mirror:

```c
#ifdef SOFTPC_STANDALONE
    softpc_standalone_audio_set_tone(freq, duration);
#else
    NtDeviceIoControlFile(...);
#endif
```

The converged design puts `host_audio_set_tone()` behind the selected Compat
ABI.  The original state machine calls one endpoint; only Compat decides how
that endpoint reaches native audio.

## Pre-admission serial hypothesis (superseded)

The following is retained to explain what S1 tested and rejected; it is not
the current implementation sequence. The recommendation above replaces it.

### S1 — Freeze the branch ledger and prove mouse-driver reachability

Re-run the finite scan of every `SOFTPC_STANDALONE` occurrence listed above,
the corresponding CMake-selected translation unit, and its original-source
peer.  For every occurrence, record original behavior, current standalone
behavior, chosen owner, source-level destination, and focused receiver.

Specifically boot a disposable guest route that invokes DOS INT 33 and prove
whether `mouse_driver_initialisation()` works with the existing single-instance
NIDDB Compat endpoint.  This S may add diagnostics and focused proof only; it
does not change production source.

### S2 — Header-overlay and input/FPU/profile convergence

Introduce reviewed Compat include overlays for `conapi.h`, `cfpu_def.h`, and
the graph-lock/profile declarations.  Restore original header spellings in
the four painter files.  Retain the narrow Scan-1 lookup only if S1 proves
that duplicating or exporting the original table would be worse; otherwise
move that ABI without table duplication.  Move `SOFTPC_STANDALONE` target
selection from MVDM headers to CMake/Compat include selection while retaining
the full I/O-map contract.

Focused proof: Scan-1 make/break translation, FPU rounding/exception
translation, and CGA/EGA/VGA painter compilation and frame checks on x64/x86.

### S3 — Audio and PPI endpoint convergence

Keep `PlaySound`, `HostPpiState`, Timer2 and PPI device transitions in their
original owners.  Replace only retired NT Beep-device operations with one
Compat audio endpoint.  Remove the mirror-local standalone branch that chooses
between edge-only PPI helpers and complete-state notification.  Prove speaker
off, frequency change, Timer2 gate, PPI pulse/click, reset, and teardown.

Example expected ownership:

```c
/* MVDM state machine */
host_audio_set_tone(frequency, duration);

/* Compat endpoint */
void host_audio_set_tone(ULONG frequency, ULONG duration) {
    softpc_standalone_audio_set_tone(frequency, duration);
}
```

### S4 — Renderer host-boundary convergence

Restore original painter include paths through the S2 overlays.  Convert
display begin/end publication, surface readiness, cursor and V7 geometry
adaptations into the smallest explicit host ABI possible.  Preserve original
paint routines, dirty-region calculations, palette handling and renderer
ownership.  Any necessary `nt_graph.c` call remains a documented port ABI;
do not introduce a second renderer or a frontend filter.

Focused proof: text/CGA/EGA/VGA/V7 frame output, palette, cursor, dirty
updates, mode transition, and existing T88 receiver preservation on both
widths.

### S5 — Reset, mouse, PPI/PIC and task-level closure audit

Apply S1's mouse result: restore guest DOS mouse-driver initialization if it
works, or retain exclusion only with a proven incompatible contract and an
explicit feature disposition.  Move PIC INTR-deassert predicate/action to a
Compat endpoint, leaving one source-level call if required.  Recheck reset,
IRQ masking/unmasking, PPI audio stop, mouse hardware and INT 33 behavior.

The final audit proves every frozen ledger member has no mirror-local
standalone implementation selection, except any owner-approved narrow port
ABI listed with its rationale and focused receiver.

## Verification and Delivery

Every source-changing S performs a pre/post mirror-diff ledger, focused owner
tests, x64 and x86 Release builds, serial background CTest on both widths,
source-boundary and manifest gates, `git diff --check`, package EXE refresh,
commit and push.  All logs, probes and generated files remain under ignored
`build/`.  User INI, guest media and snapshots are preserved.

Each S reports C/H production and test +/−/net counts, changed paths, retained
mirror ABI diffs, EXE hashes and links, and a similar-issue disposition.  S5
also records the finite closure ledger and independently reviews the actual
committed diff before requesting owner acceptance.

## Stop Conditions

Stop for owner direction if a proposed cleanup requires a guest-visible
semantic choice, proves that a required guest device has no standalone host
contract, needs a Lib/Common/x86 API change, or conflicts with accepted T88
behavior. A failed attempt to remove a branch is evidence to
retain a narrowly documented port ABI, not permission to reintroduce a second
machine/device/renderer implementation.
