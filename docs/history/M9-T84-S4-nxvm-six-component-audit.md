# M9 T84 S4 — NXVM six-component intake audit

S4 is a read-only intake audit completed on 2026-09-25.  SoftPC is frozen at
`2059bdac` (the S4 admission record); the prior exact shared-import base is
NXVM `b7cbb30a9`.  NXVM's six shared roots are clean at
`1e86b8e7ad6470ab8651a21eb89ed72b3db0c6ab`.  No product, shared source,
test, manifest, build artifact, INI, snapshot, guest medium or NXVM file was
changed by this audit.

## Inventory and provenance

All six roots retain the same production/test file universe, except for one
new NXVM test-build helper.  The direct SHA-256 inventory is:

| Root | SoftPC paths | NXVM paths | Changed common paths | SoftPC-only | NXVM-only |
| --- | ---: | ---: | ---: | ---: | ---: |
| `src/lib` | 109 | 109 | 24 | 0 | 0 |
| `src/common` | 23 | 23 | 10 | 0 | 0 |
| `src/x86` | 14 | 14 | 4 | 0 | 0 |
| `test/lib` | 51 | 52 | 37 | 0 | 1 (`register.cmake`) |
| `test/common` | 20 | 20 | 18 | 0 | 0 |
| `test/x86` | 10 | 10 | 7 | 0 | 0 |

Relative to `b7cbb30a9`, NXVM has two committed shared change sets:

- `38ed0f26f` (S7): 17 paths, +376/-120, net +256.  It repairs Machine
  request-completion failure handling, Audio partial delivery and xasm label
  resolution, with focused tests.
- S8 (`38ed0f26f..1e86b8e7`): 97 paths, +831/-896, net -65.  It normalizes
  copied predicate values to `lib_bool` and deduplicates test registration.

Combined by root, the candidate delta is `src/lib` +246/-257 (net -11),
`src/common` +241/-162 (net +79), `src/x86` +167/-157 (net +10), `test/lib`
+320/-283 (net +37), `test/common` +166/-98 (net +68), and `test/x86`
+48/-40 (net +8): 101 paths, +1188/-997, net +191.  Manifests and READMEs
are derivative of their classified source/test changes.

## Per-path disposition ledger

### S7 functional repairs

- **Candidate — `src/common/machine/machine.c` and
  `test/common/machine_wait_smoke.c`.**  One existing request lock now covers
  registration and worker claiming.  Completion-reset or command-wake failure
  clears the pending request; completion-notification failure moves the one
  executor to ERROR, cancels it, joins it before a synchronous caller returns,
  and finishes every pending request.  This removes a real indefinite-wait
  class without adding a second worker, request queue or public interface.
  The test injects reset, signal, cancellation and join failures.  Import it
  as one unit.
- **Candidate — `src/x86/xasm32/aasm32.c`,
  `test/x86/xasm32/xasm32_contract_smoke.c`, and
  `test/x86/xasm32/xasm32_bounds_smoke.c`.**  Paragraph-label instructions
  now have an explicit resolved state.  Missing/duplicate/invalid labels fail
  before output is committed; valid forward/backward labels preserve the
  established marker-NOP encoding.  This stays entirely in x86-xasm32, has no
  new product protocol, and the contract test proves output retention.
- **Defer — `src/lib/audio/stream.c`, `src/lib/audio/win32/stream.c`,
  `test/lib/audio_stream_smoke.c`, and
  `test/lib/audio_win32_platform_smoke.c`.**  The repair correctly consumes
  an accepted prefix on a recoverable platform interruption, retries only its
  suffix through the existing worker/FIFO boundary, clears a pending Win32
  interruption before reset, and requests worker cancellation explicitly on
  destroy.  It is sound in isolation, but the owner has reserved Audio source
  convergence to NXVM; SoftPC S3 intentionally imported only the deterministic
  adapter-test base.  Do not overwrite SoftPC Audio in the next non-Audio
  import.
- **Derivative — `src/{lib,common,x86}/{README.md,MANIFEST.sha256}` and
  `test/{lib,common,x86}/MANIFEST.sha256`.**  Import only with the source/test
  item they record; never separately.

### S8 predicate and test-build normalization

- **Candidate, atomic six-root bundle — Common:**
  `src/common/session/control.c`, `control_state.c`, `control_state.h`,
  `session.c`, `session_interface.h`, `ui/ui.c`, `ui/ui_interface.h`, and all
  18 changed `test/common` paths.  Predicate returns, callbacks and stored
  flags use `lib_bool`/`LIB_TRUE`/`LIB_FALSE` instead of anonymous `lib_i32`
  or raw 0/1.  `lib_bool` is the existing `lib_i32` alias, so function-call
  ABI is unchanged; this is semantic typing, not a new boolean subsystem.
- **Candidate, atomic six-root bundle — Lib:**
  `src/lib/base/{process.c,sync.c}`, `console/console_interface.h`,
  `console-broker/{console.c,win32/console.c}`, `storage/file.c`,
  `kvm-base/{event_interface.h,frame_interface.h}`, `kvm-console/console.c`,
  `kvm-window/{frame_interface.h,geometry.[ch],motion.[ch],render.[ch],win32/component.c,win32/geometry.c,win32/mouse.[ch]}`,
  and the 37 changed `test/lib` paths.  The change makes copied KVM/Console
  flags unambiguous and removes a pair of Console-broker forwarding wrappers
  plus redundant Storage scopes.  `kvm_window_frame` and text cursor fields
  have layout changes, so all receivers must be rebuilt together.  SoftPC's
  App/Machine assignments and callbacks compile from the same source tree and
  use values compatible with the new fields, but a partial binary replacement
  is forbidden.
- **Candidate, atomic six-root bundle — x86:** `src/x86/debug/command.c`,
  `xasm32/{aasm32.c,dasm32.c}`, and the seven changed `test/x86` paths.  These
  are the same predicate normalization at x86 call sites plus the S7 label
  repair; they keep the existing DOS/x86 command behavior.
- **Conditionally reject as currently located — `test/lib/register.cmake` and
  the three test CMake entrypoints.**  The 26-line registration helper removes
  duplicate CMake functions, but `test/common` and `test/x86` now include a
  file under `test/lib`.  That is clean only if all three test roots are one
  inseparable source package.  It breaks the previously useful property that
  `test/common` or `test/x86` can be copied and configured with only its own
  matching root plus production dependencies.  Before exact import, choose
  one policy: retain the small duplicated registration functions to keep the
  three test components independently portable, or formally define one
  combined test-support component outside `test/lib`.  Do not import a hidden
  reverse test dependency by accident.

The explicit path lists above cover every changed production path.  The
remaining changed test paths in each listed test root are mechanical call-site
updates from raw boolean literals to `LIB_TRUE`/`LIB_FALSE`, CMake registration
calls, and their manifests; they add no production route or test-only hook.

## Overall quality and boundary result

NXVM's current package entrypoints select strict C11, no compiler extensions,
and `-Wall -Wextra -Wpedantic -Werror` for all six standalone roots.  Its
existing corpus verifiers still reject raw platform APIs from Common and
noncanonical platform vocabulary from Lib.  Inspection of the changed
production source found no direct CRT allocation/copy calls, raw fixed-width
standard types, `windows.h`, platform `#ifdef`, TODO/FIXME or `#if 0` branch
added outside the appropriate Lib platform implementation.  The changed
Machine path retains one executor and one request slot; the changed Audio path
retains one worker/FIFO; the xasm repair retains one parser/assembler path.
No second implementation or product-specific dependency was found.

The only material quality reservation is the test-registration ownership
above.  There is also a deliberate import boundary: Lib copied frames are
documented as in-process values rather than a stable serialized ABI.  That
is correct, but means the S8 predicate layout change must rebuild SoftPC App,
VM and Compat receivers together with all six imported roots.

## Recommended next admission

Do not admit a single undifferentiated copy.  First settle the three-test-root
portability policy.  If independent roots remain required, ask NXVM for a
shared correction and then import the resulting **non-Audio atomic bundle**:
S7 Machine request repair, S7 xasm label repair, S8 predicate normalization,
their exact tests/CMake/manifests/READMEs, and all receiving SoftPC builds in
one S task.  Keep the four S7 Audio source/test paths deferred until the
separate Audio convergence owner accepts them.  This preserves a single shared
corpus instead of creating a SoftPC-only fork.

Verification performed here is read-only: SHA-256 root inventories, committed
Git/path/line-delta comparison, direct public-header and SoftPC receiver
inspection, C11/dependency/verifier-entrypoint review, plus documentation and
whitespace gates after this record.  It does not claim a build, runtime,
desktop, Linux or product regression run.
