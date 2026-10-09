# M9 T85 S18 — Ninja build-route closure

The owner-directed first S establishes a separate Ninja developer route before
the following Emulator rename S.

## Delivered

- Added isolated Ninja x64/x86 configure, package and background-test presets
  with bounded eight-way build/test scheduling.
- Kept the existing MinGW Makefiles route intact.
- Isolated background App tests that create relative media/snapshot artifacts;
  source-media and process-serial tests retain their deliberate dispositions.
- Repaired two stale Common/test-Common manifest hashes and updated the shared
  boundary selftest after removal of the retired x86/IBM-PC roots.

## Verification

- Ninja configures on x64 and x86.  x64 Common/Product background tests pass
  36/36; changed Makefiles-path smoke/gate coverage passes 8/8.
- The same warm App 1--8 batch measured 10.66 seconds serially and 8.07
  seconds with eight jobs.  This is local evidence, not a permanent benchmark.
- The x86 Ninja route builds representative Common/Product targets and passes
  its focused five-test set.  No package executable is retained from S18.

`9eb2d220` is the complete pushed delivery.  The owner's two-S instruction
admits S19; this record does not claim a runtime behavior change.
