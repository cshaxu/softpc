# PIC rejection boundary

M9 T60 S1 is closed after executor delivery fa27786 and actual-diff review.
The owner admitted the reference-guided repair plan, then explicitly required
separate approval for any Lib/Common change. The original request, finite
T60 ledger, source-mirror disposition, red/green proof and full-suite evidence
are retained in the [proposal](M9-T60-reference-repairs-proposal.md).

## Coordinator review

Reviewed git diff 549ed44 fa27786 against the S1 packet. The only production
change is a four-line guard in the original do_intrupt entry, before stack,
IVT/IDT or flags effects. The -1 PIC rejection narrows to IU16 65535; valid
byte vectors and exception vectors are unaffected. Both CPU acknowledgement
branches and the FPU immediate dispatch share this entry. No new state,
dispatcher, ABI, platform branch or upstream code payload is introduced.

The existing product IRQ smoke adds 43 lines and retains valid PIT and 8042
reset checks. Its new direct-dispatch assertion failed before the fix; after
the fix it checks register/stack preservation and a stale CPU request through
the original executor. No duplicate sentinel predicate substitutes for CPU
execution. Protected-mode-specific probing and manual guest testing are not
claimed. Production net +4, test net +43, combined +47 against 549ed44.

Both complete builds refresh the fixed x86/x64 packages. Full CTest passes
97/97 at each width; after pushing P1, IRQ/BOP/x87 focused checks pass 3/3 at
each width. Documentation, source boundaries, shared manifests/DAGs and diff
whitespace pass. All four shared roots have zero diff. INI, media, ROMs and
App/VM/Compat are unchanged. Generated types-layout fixtures were removed;
existing build trees and test logs remain available.

P1 was pushed with a clean worktree. P2 records review/closure only and does
not alter the verified binaries. T60 remains open; S2-S7 are not completed.
