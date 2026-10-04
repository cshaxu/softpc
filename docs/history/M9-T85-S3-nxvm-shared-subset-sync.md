# M9 T85 S3: NXVM Shared Subset Synchronization Closure

Owner request: append an S to open T85, import useful differing NXVM shared
files, omit the newly expanded x86 chips/Core/Product capabilities and keep
the selected subset exactly identical. Owner accepts S3 closure on 2026-10-04
after confirming that this delivery changes no production code.

## Coverage and actual-change review

The [proposal ledger](../proposals/m9-windows-101-startup-repair.md) freezes
SoftPC's 227 six-root paths against NXVM
9240a3041f8298bc8b166848e3aed6db2ea542ac. SHA-256 proves 219 selected files
plus test/register.cmake byte-identical. The eight x86 package metadata files
explicitly retain the local debug/xasm32-only build/documentation/verification
scope. All 495 NXVM-only files remain excluded. No retained C/H implementation
or test body differs and no external repository becomes a build dependency.

Reviewed a8e91e44..a6a777d1: five changed paths, comprising three governance
records and two test-package files. P1 cec8a3dc imports RUN_SERIAL TRUE and
its upstream comment for library.kvm_window_modal plus the manifest hash;
P2 a6a777d1 records actual-change review. Both are pushed.
Counts from git diff --numstat a8e91e44 a6a777d1 -- src test:
production +0/-0, test/build +2/-0 (net +2), manifest +1/-1 (net zero).
The review accounts for all 227 paths, with exact import or explicit local
packaging disposition. No new state, API, runtime path or executable change.

## Verification and limits

Both Release builds pass. Generated CTest metadata on both widths confirms
RUN_SERIAL and the retained desktop label; x64 JSON listing agrees.
x86 background passes 121/121 (144.52 seconds). x64 initially passes 120/121
(205.19 seconds): only documentation governance fails because a premature S3
history filename made the still-active packet appear to require S4. The
record ordering was corrected without weakening the checker; rerun passes
1/1 (0.22 seconds), and x86 governance recheck passes too. Every background
case has a passing result; the first x64 full invocation was not all green.
All six manifests and DAG/corpus checks pass. Five desktop cases per width
remain excluded; no RDP or Linux runtime qualification is claimed.

Both package EXEs remain byte-identical to S2:

- x86: B13CD6242C69B479609F61EA1E4DCCDAD6A956BD65812DF511E72E6093956DDF
- x64: A3D6DAD0702FFDF9E11D71ADEC7D149BA70865D18B775D107B455636476D123F

This closure changes documents only. No new scratch/media/trace tree was
created; existing build trees remain reusable. User INI and snapshot edits
remain untouched and unstaged. RDP mouse escape is separately recorded in
TODO as suspected Lib/RDP interaction, not diagnosed or repaired by S3.
T85 remains open with no active S; Queue and deferred work are unchanged.
