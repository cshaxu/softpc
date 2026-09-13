# M9 T55 S21: KVM corpus rename

## Admission

The owner accepted S20 package testing and directed a pure shared-library
rename. The existing `ui-*` vocabulary is too broad: these components implement
keyboard, video, and mouse presentation, while the cooked monitor remains a
separate application-owned console interaction.

## Frozen ledger

| Item | Required change | Explicit exclusion | Proof |
| --- | --- | --- | --- |
| Component paths and targets | Rename `ui-base`, `ui-window`, and `ui-console` directories, root headers, CMake targets, manifest entries, and every include to `kvm-base`, `kvm-window`, and `kvm-console`. | Do not merge components or change their allowed DAG. | Configure/build graph and strict library CTest. |
| C identifier namespace | Rename every active lib/app/test `ui_` function/type/variable and `UI_` constant to the equivalent `kvm_`/`KVM_` spelling. | Do not change value layouts, signatures, event meanings, hotkey rules, or runtime protocol. | Focused tests plus both full CTest runs. |
| Neutral external identity/prose | Rename active lib prose and the native Window class from UX/UI wording to KVM wording where they identify this corpus. | Cooked monitor remains monitor/console, not KVM; retained historical records are not rewritten. | Zero-old-name static gate and documentation gate. |
| Static regression gate | Add a repository check that rejects active non-historical old component paths and `ui_`/`UI_` identifiers in source, tests, CMake, manifest, and current authorities. | Do not scan or rewrite historical records. | Positive gate plus an old-spelling fixture/probe. |

## Design

This is one mechanical rename, not a new abstraction. `kvm-base` remains the
common copied event/frame/mailbox/registered-chord implementation; `kvm-window`
and `kvm-console` remain independent leaves with their existing private native
workers. The direct dependency graph is unchanged:

```text
types      -> console + host + storage + kvm-base + kvm-window + kvm-console
console    -> host + kvm-console
kvm-base   -> kvm-window + kvm-console
```

SoftPC continues to own the monitor, lifecycle, and product policy. Renamed
KVM events remain copied input/presentation values only; no new guest or VM
semantic is added.

## Non-goals

Do not alter `src/mvdm/softpc.new`, package INI/media, frame delivery, broker
transactions, monitor commands, UI behavior, native platform implementation,
or Linux support. Do not retain alias headers, target aliases, compatibility
macros, or any second old-name production path.

## Verification

Run the old-name static fixture and whole active-tree gate; focused KVM
component tests; fresh x64/x86 builds and full CTest; strict-library CTest;
manifest, component-DAG, standalone-source-boundary, and documentation
governance gates. Record changed-path accounting and an independent post-P
review before package testing.
