# M9 T59: Whole-Task Closure

Owner request: “把todo清理干净 不要保留 然后收口本t任务”; commit and
push explicitly approved. Reviewed baseline bea5f8b through final delivery
6f1e017. The initial request and later product-boundary admission are retained
in the [original proposal](M9-T59-product-boundary-cleanup-proposal.md).

## Finite completion ledger

Each admitted stage has its own implementation and actual-change review
record. The following links retain the specific scope, exceptions and tests;
later scope additions do not retroactively broaden earlier permissions.

| Stage | Completed scope and evidence |
| --- | --- |
| S1 | [Bounded TODO investigation and approved x87 layout repair](M9-T59-S1-delivery-review.md); unreproduced observations are not represented as product fixes. |
| S2 | [Single VM build ownership](M9-T59-S2-build-ownership.md). |
| S3 | [Consolidated Compat ABI declarations](M9-T59-S3-abi-declarations.md). |
| S4 | [VM singleton admission and recreation](M9-T59-S4-vm-admission.md). |
| S5 | [Product-boundary gates and convergence](M9-T59-S5-boundary-convergence.md). |
| S6 | [Monitor line ownership, notification delivery and viewport](M9-T59-S6-monitor-delivery.md); [retained design](M9-T59-S6-monitor-line-viewport-proposal.md). |
| S7 | [Original debugger output and CLI restoration](M9-T59-S7-debug-original-restoration.md). |
| S8 | [Single-control freeze, removal of batch mailbox API](M9-T59-S8-kvm-single-control.md). |
| S9 | [One Console output binding](M9-T59-S9-console-output-binding.md). |
| S10 | [Ctrl+Alt+T to content Alt+Tab](M9-T59-S10-alt-tab-hotkey.md). |
| S11 | [Common state and publication simplification](M9-T59-S11-common-state-simplification.md). |
| S12 | [Event-driven Session wait](M9-T59-S12-session-event-wait.md). |
| S13 | [Shared Base synchronization](M9-T59-S13-base-synchronization.md). |
| S14 | [Consumer mutex unification](M9-T59-S14-base-mutex-unification.md). |
| S15 | [Base Event reuse](M9-T59-S15-base-event-reuse.md). |
| S16 | [Single-allocation task ownership](M9-T59-S16-base-task-flattening.md). |
| S17 | [Storage ownership and Console disposal cleanup](M9-T59-S17-storage-ownership-cleanup.md). |
| S18 | [Console broker naming and ownership](M9-T59-S18-console-broker-naming.md). |
| S19 | [Paused machine wait failure termination](M9-T59-S19-machine-paused-wait-failure.md). |
| S20 | [HLT and audio failure handling](M9-T59-S20-hlt-audio-failure.md). |
| S21 | [Linear debugger command boundaries](M9-T59-S21-debug-linear-boundaries.md). |
| S22 | [DOS DEBUG memory-command semantics](M9-T59-S22-debug-dos-semantics.md); interactive E and DOS environment remain explicitly excluded. |

## Whole-task review

The original TODO investigation is complete under its bounded-evidence rule.
The x87 correction changed the approved MVDM layout boundary, not its arithmetic
engine. S2–S5 establish VM build ownership, Compat declarations and admission;
later explicitly admitted stages update shared Lib/Common and tests. App
retains product injection. No external comparison checkout is an acceptance
or runtime dependency, and none was modified by this closure.

Changed-path review covers src/app, src/vm, src/compat, the approved MVDM header,
src/common, src/lib, test corpora, build/gate definitions, documentation and
the two fixed binaries. Per-stage records carry behavioral proof. Aggregate
production C/H diff bea5f8b..6f1e017 is +2364/-2642, net -278 lines across
96 numstat entries (including relocation accounting). This is not a claim
that every individual stage reduced code. The closure itself changes only
governance documents, not source, tests, binaries, configuration or media.

S22 full suites passed x64 97/97 (52.63 s) and x86 97/97 (54.09 s); subsequent
post-commit focused suites passed 6/6 each. Earlier failed attempts and their
dispositions remain in stage history rather than being erased. Package hashes
rechecked during closure match those tested artifacts:

- softpc32.exe: 7D2D65CB7E8AB1E0E7CAC48AB05C289C1350C0321ED9C70D0D291A74FA55E8A2.
- softpc64.exe: 45CAB2236EB33F8FA39EF2482E6066C4FC0E4F7E931122B08332F97DFD35AEE4.

No new runtime build is required for this documentation-only closure.
Documentation governance and whitespace validation are rerun before commit.

## Remaining scope and disposition

Owner retires the remaining compact-Console fixture observation without a
diagnosed-fix claim. Its historical S21/S22 evidence is unchanged; TODO has
no deferred items, and no replacement tracking item is created.

The [queue](../states/QUEUE.md) remains ordered: XP mirror rebase, Win3.1
display roundtrip, then overlay page lookup. These are separate unadmitted
candidates, not unfinished T59 stages. DOS environment and interactive E
remain deliberate exclusions, not new debt. Owner closure ends the testing
wait; it does not manufacture an additional manual acceptance result.

T59 and S22 are closed. No next T or S is admitted.
