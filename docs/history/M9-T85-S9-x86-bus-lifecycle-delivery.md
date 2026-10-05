# M9 T85 S9 delivery handoff

Owner authorizes S9 followed directly by S10, with testing after S10.
Executor P1 03c979c7 is pushed. Actual-change review confirms all nine paths:
three private Core code paths +0/-23, one test build path +2/-2; manifests
and governance documents only otherwise. Empty bus lifecycle calls are gone;
real port ownership and allocation rollback remain. The two test link-order
corrections eliminate accidental archive extraction through the removed call.
Dual Release builds and focused 8/8 each pass; background x64 443/443 in
501.37s and x86 443/443 in 302.89s pass, desktop excluded. Shared source/test
manifests, x86 DAG, documentation and diff gates pass. No Lib/Common/SoftPC
runtime or public ABI change. Rebuilt/up-to-date EXE hashes remain unchanged.
Original excluded owner INI/snapshot are the only remaining worktree changes.
This is a verified delivery, not invented owner acceptance or T85 closure.
The coordinator activates S10 under the explicit continuation instruction.
