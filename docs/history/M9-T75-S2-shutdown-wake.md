# M9 T75 S2 Closure

Executor 9b314372 is pushed. Coordinator reviewed its actual Machine wait,
scripted schedule, removal of the native x86 STOP workaround, three manifests
and package/evidence changes. No API/state owner or second shutdown path was
introduced. The existing Base cancellation event remains signaled even after
the inner callback consumes command_event; outer faults still use ERROR and
pending requests still complete through finish_requests.

The original request, finite ledger and sweep are in the active T75 proposal.
Old deterministic schedule fails; repaired focused tests pass 50 repetitions
per width. Release/background x64 110/110 (185.64s), x86 110/110 (168.37s).
Five desktop tests per width were excluded. Gates/diff check pass.
Production +5/-3, tests +36/-5: total +41/-8, net +33 versus estimated +44;
three manifest entries each +1/-1, excluded from code count. INI/media/Core
and Lib remain unchanged. Two EXE hashes are recorded in the proposal.
S2 closes under the owner's serial authorization; S3 follows, T75 remains open.
