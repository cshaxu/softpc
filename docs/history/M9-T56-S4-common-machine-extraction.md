# M9 T56 S4: common machine extraction

## Owner requirement

Move generic executor lifecycle, lifecycle/input queues, run generation and
complete-frame publication out of `app` into `common/machine`; keep the
SoftPC/MVDM operations in one injected product driver. Every S baseline and
delivery must contain both x86 and x64 package EXEs.

## Executor delivery

`common/machine` is now the sole production owner of the executor worker,
lifecycle queue, KVM input queue, run generation, double-buffered completed
frames and machine completion sinks. `app/runtime.[ch]` and
`app/input_queue.[ch]` were removed in the same change.

`app/machine_driver.[ch]` is the only SoftPC-specific adapter. It supplies
reset/run/stop/wake, heartbeat and executor callbacks, guest key/mouse
conversion, MVDM frame capture, removable-media handling and product prompt
trace observation. It contains no generic queue or worker. `common/session`
now receives a `common_machine` directly instead of a runtime adapter.

The new deterministic fake-driver smoke uses event barriers rather than
Sleep. It proves cold start, a complete published frame, input wake, run
generation, reset's one completed fact, resume, stop and worker join. The old
runtime-named test surface is test-only compatibility glue around the new
production component; the runtime/restart/input-continuation tests therefore
exercise common/machine rather than a parallel route.

The x86 preset explicitly binds the MSYS2 i686 compiler and its bin directory
to the same 32-bit DLL set. This prevents a foreign MinGW DLL earlier on PATH
from making `cc1.exe` fail and silently leaving only an x64 package.

## Verification

- x64: full CTest 59/59 passed.
- x86: full CTest 59/59 passed.
- strict standalone lib build and CTest: 8/8 passed.
- Documentation/DAG gates and `git diff --check` passed.
- Package SHA-256: `softpc32.exe`
  `7C735F8AE9474BE5DFCCEC59C4BCFCCA87050BD245437CD5B2C99BEF5A6210E9`;
  `softpc64.exe`
  `26DC5A9FA1175A3D3B80B3969591285E8506D72B73D57EA5CCFC9483890378C0`.

Only the two package executables were refreshed. The user-owned INI and all
media bytes were preserved.

## Reviewer closure

Reviewed commit `7946ce0` against the S4 packet, the S1 ledger and product UI
authority. There is one production executor path, one input queue and one
frame publication path. The generic component has no SoftPC/MVDM dependency;
all original-machine operations remain in the injected driver. S4 is closed.

