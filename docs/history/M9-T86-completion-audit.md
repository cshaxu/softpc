# M9 T86 — test-quality completion audit

## Purpose

The owner approved T86 closure on 2026-10-10 after six bounded deliveries:

1. raw Console/cooked-monitor handoff repair;
2. SoftPC-only test ownership and isolation normalization;
3. public shared-test registration conformance; and
4. an independent dual-width qualification run;
5. non-public App/Core/Compat test ownership and quality repair; and
6. inward component-boundary enforcement, shared X86 naming repair, and final
   current-NXVM six-root convergence.

The final step does not change product behavior. It proves that the committed
tree can be built and tested from fresh x64 and x86 Ninja trees without hiding
duplicate compilation behind separate package and test builds.

## Finite S ledger

- **S1:** repaired the raw Console/cooked-monitor handoff, including native
  geometry and retry coverage. [Delivery record](M9-T86-S1-console-handoff-delivery.md).
- **S2:** normalized non-public App test ownership and isolation.
  [Closure](M9-T86-S2-test-ownership-closure.md).
- **S3:** made public-test registration ownership and labels explicit.
  [Closure](M9-T86-S3-public-test-registration-closure.md).
- **S4:** qualified a clean one-build-per-width graph, background suite and
  serial desktop lane. [Closure](M9-T86-S4-dual-width-qualification.md).
- **S5:** completed the finite non-public test quality ledger, assertion,
  fixture, registration and scheduling repair.
  [Delivery record](M9-T86-S5-nonpublic-test-quality-delivery.md).
- **S6:** renamed the shared Product corpus to X86, enforced the inward
  dependency order, repaired C spelling/target/output defects, and converged
  all six shared roots with NXVM's current worktree.
  [Closure record](M9-T86-S6-inward-component-boundaries.md).

## One-build rule

For each architecture, the audit creates one empty task-owned build directory,
configures it once, then invokes Ninja's complete default graph once. That
single build produces the package executable and every test target. The later
CTest runs execute those outputs only. A follow-up Ninja query must report no
work, demonstrating that no package or suite was rebuilt independently.

This is not a compiler-cache benchmark. No prior build tree or ccache result is
accepted as compilation evidence.

## Acceptance ledger

For each width, record:

- compiler path and pointer width;
- one clean configuration and complete-build log;
- the no-op rebuild result;
- CTest JSON inventory divided into background and the serial `desktop` lane;
- all public `lib`, `emulator`, and `product` tests, all App/Core checks, and
  all `app-softpc/integration` tests;
- documentation, manifest, corpus and boundary checks;
- the matching refreshed package EXE hash.

The x86 compiler is selected explicitly before configuration; the x64 and x86
trees may not share object files. `assets/binary/softpc.ini`, media and
snapshots are inputs, never rewritten.

## Closure rule

Passing this audit supports a recommendation to close T86. It does not itself
claim manual product acceptance or close the numeric task; the owner decides
that separately.

## S4 delivery evidence

The final qualification used two newly-created Ninja directories,
`build/t86-s4-x64` and `build/t86-s4-x86`. Each was configured once with its
explicit MinGW compiler and `CMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY`:
the latter avoids only CMake's host executable ABI probe in the controlled
runner; the complete project graph is still compiled and linked by that
compiler. Each directory then ran exactly one complete `cmake --build` graph
of 576 targets. `ninja -n` subsequently reported `no work to do` for both.

Both configurations ran all 129 non-desktop tests once with `-j8`, then all
five `desktop` tests once serially. Results were:

- x64: 129/129 in 155.41 seconds; desktop 5/5 in 12.19 seconds.
- x86: 129/129 in 148.04 seconds; desktop 5/5 in 11.96 seconds.

The package normal and compact Console routes passed in both desktop lanes.
The exact restored timing contracts were present in generated CTest metadata:
`lib.types-layout-selftest` is 180 seconds and
`emulator.verifier-negative` is 60 seconds. The former took 54.02 seconds on
x64 and 54.39 seconds on x86 during this cold qualification, so the ordinary
30-second budget would be false-negative-prone.

The Lib reader-handoff repair flushes input only after the old reader has
joined, before a replacement reader can own it. Its existing cancellation
smoke now proves that deactivation contributes that one final flush. The
package fixture no longer asserts that a raw Console must remain hidden after
the raw broker correctly focuses it; it hides its own attached Console again
and continues observing through handles. Neither change adds an ABI, state
machine or product policy.

The user INI remains byte-identical:
`38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D`.
The refreshed executables are x86
`9A57BA777E951F12F77EB4A6D73B898D81EA4D4C3126E1BDDB2F4D97BA0BD3A8`
and x64
`39E89B329152E9A211F56E31F6331FD1FDD3714D8AECF0F1B4E9800C59B7928E`.

## Post-qualification deliveries and closure disposition

S5 added no production behavior; it completed its 47-route non-public test
ledger and passed the affected dual-width owner, integration and static lanes.
S6 then made the reusable order explicit as `lib < emulator < x86 <
app-softpc`, with static inward-boundary proof. Its P2 background qualification
built each width once and passed the complete non-desktop inventory, 131/131
on x64 and 131/131 on x86. P3 corrected a duplicated test-only success word;
P4 corrected stale shared-root LF attributes and adopted NXVM's equivalent
test-lib formatting. P3/P4 passed their focused receivers on both widths.

At closure, SoftPC and NXVM's current worktree match exactly across all six
shared roots: 109 `src/lib`, 49 `test/lib`, 30 `src/emulator`, 22
`test/emulator`, 24 `src/x86`, and 12 `test/x86` files. The preserved OpenNT
mirror's explicit CRLF checkout exceptions remain intentionally outside that
shared-LF corpus. P4 has no production/link input, so the P2 package EXEs
remain current: x86
`A70F6F3D52DC54D0CE51C9FC59C9C101A3E95DFAE0F80DB4D55EF0D6E7A81450` and
x64 `895DB10F594D5EEAACB370D0F6B1B7FF1D5A7BE5CBFCAFE4647B9F06019BA613`.

The owner explicitly accepts S6 and directs this closure. T86 leaves no
unaccepted task-owned receiver, dirty worktree change, or deferred test-quality
repair. The intermittent Win3.1 File Manager geometry issue is admitted as
separate T87 S1 investigation, not an unclosed T86 defect.
