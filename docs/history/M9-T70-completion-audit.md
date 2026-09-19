# M9 T70 Completion Audit

## Authority And Frozen Coverage

Original task: investigate and repair Windows 95 Setup hardware-detection
failure using owner-supplied checkpoints, without masking guest mode changes.
Subsequent owner admissions cover snapshot remounting, first boot/display,
Window capture and four bounded Lib repairs. Final owner instruction:
"t70收口吧". This audit closes that admitted scope, not all future emulator work.

Coverage units are S1--S12, their surviving changed component paths, final
packages, disclosed verification limits and Queue/TODO dispositions. Earlier
withdrawn local snapshot proposals and withdrawn premature S9 closure are not
successful deliveries. Historical records remain chronological evidence.

| Scope | Result and evidence |
| --- | --- |
| S1 | Distinguished Setup recovery from presenter failure; bounded investigation completed. |
| S2--S6 | 8042 input-port value, rejected PIC acknowledgement, SAS allocation, TLS setjmp ABI and restored-executor rendezvous repaired; retained independently of the eventual Setup cause. [Ledger](M9-T70-S6-investigation-record.md). |
| S7 | SS-based stack width restored for privilege-changing CALL/IRET/RETF. CALL has causal fixture evidence; owner confirms hardware detection succeeds. [Review](M9-T70-S7-stack-width-repair.md), ce5f535. |
| S8 | Snapshot-owned paths/modes; detach all slots then reopen even same-path media. Codec/SHA/overlay checks retained; owner accepts. [Review](M9-T70-S8-snapshot-media-remount.md), f54e19f. |
| S9 | PIT read estimates no longer manufacture interrupts. Package visibility fixture repaired separately; both full suites 109/109. Owner subsequently reports successful installation and desktop. [Review](M9-T70-S9-win95-first-boot.md), 5d6fe18 / 7c0c786. |
| S10 | Planar stride/wrap and V7 preserve-mode validation repaired; full logo/normal desktop verified and owner accepted. [Review](M9-T70-S10-win95-display-startup.md), 62eec11. |
| S11 | Captured Window motion recenters within one input path; ownership loss releases. Owner confirms mouse repair. [Review](M9-T70-S11-window-mouse-capture.md), 58ac394. |
| S12 | Failed task join retains resources, geometry fitting is idempotent, Linux locks follow open descriptions, startup cleanup consolidated. Owner approves closure after disclosed results/limits. [Review](M9-T70-S12-lib-lifetime-geometry-cleanup.md), 8cb23e9 / f6ab0dc. |

## Coordinator Review

Reviewed original request, per-S evidence/dispositions, Git delivery history and
aggregate changed-path inventory against baseline 95c467a. Device/CPU repairs
remain in their original owners; snapshot/media adaptation stays in VM/Compat;
Window capture/geometry and platform resource ownership stay in Lib. S12's
Common/App/audio changes propagate failed shutdown rather than adding another
lifecycle owner. No Setup-name predicate, diagnostic-only renderer, new executor
or parallel input route is admitted by these repairs. Detailed per-S diff
counts and similar-issue proofs remain in the linked records.

This closure changes documentation only. It neither rebuilds packages nor
reruns guest installation. Final S12 verification remains x86 109/109 and x64
109/109, including snapshot/package regression, all four corpus manifests and
dependency checks. Current package hashes were rechecked at closure:

- x86: B70BCB79FD6E378032E7569600AB9F6F19FDA2D185E77F340D35FA89E9CDF7A4
- x64: 6503A85A89755335866F344F399220B923514AA14865CE345D769897E13575D9

Earlier S7/S8 package failures were diagnosed and resolved by S9's test-launch
repair; they are not silently rewritten as historical passes. S11's separate
parallel-run unexpected input remains a disclosed observation, not a diagnosed
product repair; final serial suites pass. Linux-native lock execution was
unavailable and was disclosed before S12 acceptance; Windows-controlled Linux
tests are not kernel proof. No Linux certification is claimed.

## Remaining Work And Closure

The Queue retains, in order: KVM text/frame contract clarification, XP SP1
mirror rebase, overlay-page indexing. They are separately scoped candidates,
not incomplete T70 fixes. The first proposal is newly requested design work;
it is not admitted by closing T70. Existing TODO entries remain the earlier
nonreproducible Win3.1 Window-height oscillation and intermittent BIOS-tick
test. Neither is claimed repaired by this task.

No admitted implementation S remains open. Owner acceptance, recorded limited
claims and final dual-width delivery satisfy this T's closure boundary.
Documentation governance and whitespace review validate this closure change;
no source, executable, INI or guest medium is changed. T70 is closed, and no
next T/S is automatically admitted.
