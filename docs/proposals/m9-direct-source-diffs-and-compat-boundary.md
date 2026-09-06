# M9: Direct Source-Diff Portability And Compatibility Boundary

## Objective

Replace every build-time mechanical source transformation with directly
maintained source. `src/mvdm/softpc.new/` retains the original SoftPC path
layout and may contain narrow, documented portability differences that are
small enough for reliable human batch review. Substantial standalone-host
functionality belongs outside that tree in `src/host/compat/` or its
appropriate `src/host/` owner. Retire every build-time C/Python
source-generation transform.

The intended ownership model is:

```text
src/mvdm/softpc.new/  selected original layout plus narrow source-visible
                      compiler/host-ABI corrections
src/host/compat/      new host state, resource ownership, lifecycle, and
                      standalone implementations required by original calls
src/host/             platform, media, input, video, and machine bindings
```

The source tree is consequently an auditable *ported mirror*, not a
byte-identical archival copy. A narrow difference must preserve the selected
machine algorithm and express only its existing intent correctly on supported
x86 and x64 hosts. It must carry a local reason marker and remain visible in a
path-by-path comparison against the selected OpenNT reference. The direct
source diff, rather than a generator's hidden rewrite, is the review artifact
and the compiler input.

## Non-goals

- Do not change guest-visible machine, CPU, C-VID, controller, BIOS, ROM,
  BOP, timing, media, or renderer behavior.
- Do not add NTVDM, DOS/WOW, VDD, CSR, product-service, or multi-machine
  semantics.
- Do not make x86 select a different machine source set from x64. Both widths
  retain one source-selection model; a narrow correction may conditionally
  express the host ABI only where that is necessary.
- Do not move new host ownership into `src/mvdm/softpc.new/` merely because
  the original file has a convenient call site.
- Do not retain generated C/H files or build outputs in tracked source.
- Do not retain a build-time source-transform script, Python interpreter
  requirement, custom command, or `build/generated-*` C/H input merely to
  mechanically rewrite selected SoftPC source.

## Boundary Rule

An in-mirror difference is permitted only when all of the following hold:

1. it is local and reviewable as a narrow C/H diff;
2. it preserves the original machine algorithm and guest contract;
3. it corrects compiler syntax, declaration/calling ABI, host pointer width,
   or a similarly mechanical host representation issue; and
4. it introduces no new host-owned allocation, resource, thread, queue,
   lifecycle, media, presentation, or policy state.

Any change that fails one of those tests is a `src/host/compat/` or other
external-host responsibility. A behavior change whose owner cannot be proven
is a stop condition, not an in-mirror portability diff.

The admission record must group same-shaped narrow differences into review
batches, but need not force unrelated files through one script or one textual
rule. A batch is admissible only when a reviewer can inspect its full direct
C/H diff and identify the common portability rationale without reconstructing
generated output.

## Serial Subtasks

### S1 — Source-transform retirement and direct-diff admission audit

Produce a fresh, reproducible ledger before changing source or build rules.
This is read-only audit work.

The ledger must inventory:

- every CMake source-generation rule, its script, original input, generated
  output, consuming target, and whether both x86 and x64 select it;
- every direct difference in the current `src/mvdm/softpc.new/` tree against
  the selected read-only OpenNT comparison root, including differences absent
  from stale historical manifests;
- every active `src/host/compat/` entry and the original call/ABI boundary it
  satisfies; and
- for each point, one proposed disposition: retained narrow in-mirror direct
  diff, external host compatibility, unselected/removal candidate, or blocked
  behavior question, plus its review batch and focused x86/x64 proof.

The audit must repair or replace the obsolete divergence-audit route so it
targets the current source layout and fails closed for an unclassified
difference. It must record the focused x86/x64 verification needed for each
later migration, but must not delete scripts, alter CMake selection, or edit
machine source.

**Exit:** the owner has one current ledger covering all active transform,
direct-diff, and external-compatibility points; every entry has an owner,
disposition, and review batch; no historical count is presented as current
evidence without a fresh comparison.

### S2 — Apply approved narrow source-visible portability corrections

For entries approved by S1 as mechanical, apply the exact correction to its
same-path file under `src/mvdm/softpc.new/`, preserving the original algorithm
and adding a concise local reason marker. CMake must compile that source or
header directly for both widths. Remove the corresponding generated-source
selection and its mechanical-transform script only when the direct source
replacement has focused and dual-width evidence.

### S3 — Externalize substantial host compatibility

For S1 entries that own native-width storage, lifecycle, Win32 resources,
media, presentation, or standalone policy, retain or move the implementation
to its appropriate `src/host/compat/` or `src/host/` owner. The original source
may receive only the narrow declaration/call ABI required to use that boundary;
it may not acquire the external implementation or its state.

### S4 — Retire all source generation

After all transformed inputs have an approved S2 or S3 disposition, remove
every C/Python source-generation custom command and its mechanical-transform
script. Remove `find_package(Python3)` when it no longer serves any
non-transform build requirement; CMake may not compile a `build/generated-*`
C/H source, nor invoke an external source rewrite. Update architecture,
source-layout, divergence-audit, and build documentation so the direct-diff
ledger is the single explanation of permitted mirror differences.

## Verification

Each admitted implementation subtask must:

1. prove CMake's x86 and x64 source selections are intentional and equivalent
   at the selected-machine boundary, with every compiled C/H input present
   beneath `src/` rather than generated during the build;
2. run focused tests covering the affected ABI or host boundary;
3. complete clean GCC x64 and i686 x86 builds with full CTest; and
4. package-smoke both launchers without modifying user-owned `softpc.ini` or
   guest media; and
5. prove a clean configure/build links the correct `softpc32.exe` and
   `softpc64.exe` from checked-in `src/` inputs without a build-time source
   transform.

## Stop Conditions

Stop for owner direction if an apparent narrow diff changes controller,
firmware, ROM, BOP, timing, renderer, or guest-media semantics; if an external
compatibility move requires the original machine to own new host state; or if
the selected comparison baseline cannot be established reproducibly.
