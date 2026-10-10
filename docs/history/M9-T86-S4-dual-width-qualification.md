# M9 T86 S4: Dual-width Qualification Closure

Original owner request: perform T86's closure audit after a cache-clean
rebuild of both package architectures, using each architecture's first
successful intermediates for every later link and test.

Owner acceptance: “s4先收口”. S4 closes on 2026-10-10. T86 remains open with
no active S.

## Delivered contract

S4 configured one fresh Ninja tree per width, `build/t86-s4-x64` and
`build/t86-s4-x86`, then built each complete 576-target graph exactly once.
The required no-op Ninja query reported no work in both trees. No subsequent
package or test-specific build was used as qualification evidence.

Both widths passed every registered test in its declared lane:

- x64 background: 129/129 in 155.41 seconds; native-desktop: 5/5 in 12.19
  seconds.
- x86 background: 129/129 in 148.04 seconds; native-desktop: 5/5 in 11.96
  seconds.

The package's normal and compact Console integration routes pass in both
desktop lanes. Public Lib, Emulator and Product suites, non-public App/Core
checks and App integration tests are represented in the enumerated inventory.

The only precise test-budget exceptions are retained: `lib.types-layout-selftest`
is 180 seconds and `emulator.verifier-negative` is 60 seconds; all other
shared fixtures remain at 30 seconds. The layout self-test took 54.02 seconds
on x64 and 54.39 seconds on x86, confirming that the ordinary limit would be
incorrect.

## Actual-change review

The delivery P is `e11367c0`. It contains the two refreshed package EXEs,
the exact timeout declarations and their test-manifest records, one Lib
Console reader-handoff repair/proof, and a package fixture correction. No
public interface, product policy, additional executor, state machine or
configuration/media/snapshot behavior was introduced.

After S4 delivery, `c7b5c3a8` adds a focused fault-injection regression for
the same existing Console broker path: a deactivate-time input flush failure
returns `LIB_STATUS_IO_ERROR`, retains retryable backend state, and succeeds
once the injected failure is removed. It changes tests and the Lib test
manifest only; production behavior remains unchanged.

The user-owned package INI remained byte-identical:
`38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D`.
S4's refreshed EXEs are x86
`9A57BA777E951F12F77EB4A6D73B898D81EA4D4C3126E1BDDB2F4D97BA0BD3A8` and
x64 `39E89B329152E9A211F56E31F6331FD1FDD3714D8AECF0F1B4E9800C59B7928E`.

The full one-build ledger and CTest evidence remain in the
[T86 qualification audit](M9-T86-completion-audit.md). This S
closure is not a T86 closure and makes no further manual-acceptance claim.
