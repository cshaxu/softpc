# M9 T88 S3: Colour/resolution regression coverage

Owner request: add tests for other colour modes and resolutions in a new S3,
close accepted S2, and include restoration of the original condition order.
After both architectures exposed 63h/64h repaint failures, the owner explicitly
accepts the coverage and requests S3 closure with a clean worktree. This closes
test-coverage work only; it does not declare the production defect repaired.

Implementation P1 is `c7720bc9`, pushed to origin/main. Coordinator review checks
the actual changed files against baseline `911c4749`, the requests, finite
ledger and verification evidence. The only production change is predicate
ordering in v7_ports.c, +1/-1 net zero; the existing App VGA test adds 115 lines,
deletes none. Combined code delta is +116/-1, net +115. No new target, state,
public API, renderer, Lib/Emulator/Compat code or shared corpus is added.

The [proposal's S3 ledger](M9-T88-win31-file-manager-vga-width-transition.md#s3-accepted-coverage-disposition)
records 19 BIOS graphics modes in both orders per architecture, exact 07/0F
fallback behavior, CPU write/incremental/full comparisons, and ten V7 modes'
pointer movement/background restoration. Geometry and pointer checks pass;
63/64 repaint comparisons fail in both orders and widths. The final assertion
executes after pre-existing receivers and fixture cleanup. Its failure remains
visible; no exclusions, softened assertions or mode-specific production fixes
are introduced. The defect transfers to [TODO](../states/TODO.md) for separate
owner admission, not to the distinct Compat generation-readiness S4.

Both Release packages build and are refreshed. Shared suites pass 84/84 and
the other App/checks receivers pass 46/46 per width. The final focused VGA
receiver fails exactly on the retained repaint assertion on each width. Five
desktop tests per width remain excluded. Initial parallel timeouts and an
invalid temporary fixture ordering are explicitly discarded/qualified in the
proposal; neither is disguised as a passing test or a production diagnosis.
Documentation governance and whitespace checks pass. The normal build trees
and bounded failure logs are retained for the open T88 investigation; disposed
test media is removed by the final receiver before its failed assertion.

Package hashes are retained in the proposal. INI, media and snapshots are
unchanged. S3 is closed after owner acceptance and pushed implementation;
T88 stays open, with no next S admitted by this closure.
