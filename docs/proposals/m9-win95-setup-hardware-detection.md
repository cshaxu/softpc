# Win95 Setup hardware-detection display-loss investigation

## Product observation

An owner-supplied paused snapshot reaches the Windows 95 Setup hardware
detection sequence after `load` and `resume`. The copied graphical display
later becomes text, then guest execution appears permanently stalled. This is
not treated as a KVM rendering defect until the first differing machine,
renderer, copied-frame and presentation facts are compared.

## Scope and invariant

The external snapshot, its configured disk image and owner INI are read-only
diagnostic fixtures. The fixture's current hashes are recorded in untracked
run evidence only. Any diagnostic run uses the configured overlay route and
re-verifies the supplied snapshot/image after exit. No fixture is copied into
the repository, package assets or test corpus.

The product retains one executor, one original renderer and one copied-frame
publication path. A correct repair must preserve guest-visible display mode
semantics and actual hardware detection; it may not pin graphics mode, filter
text frames, suppress mode changes, add a frontend retry, or special-case
Windows 95/Setup/snapshot content.

## S1: bounded causal reproduction

Establish an x64 package from current tracked sources and place a diagnostic
copy beside the owner fixture. Reproduce with the documented monitor sequence;
observe the first transition among original video state, Compat DIB/text
surface, VM copied frame and KVM presentation. The diagnostic must be
time-bounded, clean up its owned build logs/processes, and prove fixture hashes
unchanged. Static source tracing may identify existing callback boundaries, but
S1 adds no source instrumentation or behavior change.

S1 exits with one of: a reproducible first bad boundary and a minimal S2 repair
candidate; a reproducible machine stall with enough CPU/device facts to assign
ownership; or a documented inability to reproduce. It does not claim the
hardware-detection problem fixed.

### S1 evidence and conclusion

The current x64 package was placed beside the owner fixture.  Two fresh
`load snapshot.1` / `resume` runs were bounded and then terminated without
committing their overlay state.  The supplied snapshot and fixed-disk image
retained their recorded SHA-256 values after both runs.

The guest's `C:\SETUPLOG.TXT` stops at `Detection=Started`.  The original
bounded run recorded `*:DETECTKBD` in its newly written `C:\DETCRASH.LOG`,
and another bounded fresh process remained responsive but consumed essentially
one CPU for the whole observation window while Setup was still in detection.
The graphical-to-text change is therefore Setup's failure recovery path, not a
copied-frame or KVM route defect.

The owner subsequently replaced the diagnostic snapshot and its current
`DETCRASH.LOG` identifies `*:DETECTSYSTEM`, not `DETECTKBD`.  `MSDET.INF`
maps that phase to the system detector group (PIC, DMA, CMOS, timer, speaker
and numeric-processor probes).  The earlier keyboard observation remains
historical evidence for S2 only; later work must use the current
`DETECTSYSTEM` marker and must not infer a keyboard cause from it.

The original non-NTVDM 8042 implementation has one independently real defect:
its command `0xC0` (Read Input Port) marks a byte available but does not assign
the local byte returned through port `0x60`.  The modeled input-port state
already exists as `input_port_val`, is initialized by `AT_kbd_init`, and is
archived/restored.  Returning an uninitialized stack value makes any probe
nondeterministic.

That defect was not sufficient to explain this fixture.  An instrumented
detector window observed the restored ordinary value (`0xBF`) and no `0xC0`
command before Setup still entered Safe Recovery with `DETECTKBD`.  S2 retains
the controller repair as a separately proven device contract, but does not
claim that it fixes Setup.

## S2: 8042 input-port semantics

Change the existing `0xC0` receiver to return `input_port_val`, preserving the
current output-buffer/status behavior.  Do not add a new controller, a
compatibility profile, or a Windows-specific branch.  Add a port-level test
which initializes the ordinary AT controller, issues `0xC0` through port
`0x64`, and verifies the initialized input-port value is returned by `0x60`.
The test must also prove the normal VGA configuration remains the expected
value, so it checks the semantic state rather than an arbitrary local value.

S2 must rebuild both widths and run the focused plus full suites.  The
owner-provided fixture remains external and unmodified; an x64 package may be
copied beside it for the owner to repeat `load snapshot.1` / `resume`.

## S3: reject PIC acknowledgements before CCPU delivery

The original PIC explicitly returns signed `-1` when a queued host interrupt
has become spurious, masked, or has completed its callback action before CCPU
accepts it.  Current standalone CCPU narrows that result into the unsigned
hardware-vector slot and still performs the external-interrupt side effects.
`do_intrupt()` has a defensive later rejection, but that is the wrong boundary:
the CCPU dispatch site has already treated a non-vector as a delivered
interrupt.

Keep the existing acknowledgement, clear the one pending CCPU request exactly
once, and execute `EXT`, timer synchronization, interrupt delivery, and saved
EIP update only for a nonnegative acknowledgement.  This is a generic
original-device/CCPU contract repair; it must not mention Windows, Setup,
keyboard detection, a snapshot, or a product mode in production code.

Strengthen the existing IRQ smoke so its stale CPU wake exercises the actual
CCPU loop and observes that a rejected acknowledgement produces no external
delivery state.  Retain `do_intrupt()`'s defensive invalid-vector guard as a
second boundary for direct callers; the smoke must not use that guard as the
sole proof.  Compare the narrow CCPU change with the read-only NTVDM64
equivalent only as research evidence, not as a runtime dependency.

S3 is a bounded candidate, not a causal declaration.  Its fixture rerun must
either pass beyond the prior Safe Recovery / `DETECTKBD` failure, or record the
next concrete original-device fact for a later independently admitted S.

## S4: restore the original SAS no-wrap allocation contract

The owner fixture contains `WINA20.386`, its detector names `DETECTKBD`, and
the image retains the literal `A20 Hardware Error`. This makes the A20/SAS
boundary the next concrete candidate, without turning a product symptom into
a branch.

The original CCPU `sas_init()` contract requires `host_sas_init(size)` to
provide `size + NOWRAP_PROTECTION` bytes. It needs that tail while A20 is
enabled and code probes the edge of installed memory; `Length_of_M_area`
remains the guest-visible RAM limit. Current Compat allocates only an
unexplained 8 KiB tail, while the authoritative original host commits the
full `NOWRAP_PROTECTION` range. Restore that contract by using the original
named constant in Compat's sole backing allocation. Do not change guest RAM,
add an A20 profile, or edit the preserved mirror.

S4 exits only after x86/x64 build and focused memory/keyboard plus snapshot
regressions. The fixture rerun may establish sufficiency; if it does not,
retain the corrected generic allocation and continue from the next measured
device fact rather than masking the failure.

### S4 evidence and conclusion

The full original no-wrap allocation is now present, but the supplied fixture
still enters Setup recovery.  The serialized SAS wrap mask is `0xffffffff` and
is restored unchanged; the apparent zero reported by an early diagnostic was
an argument-order error in the diagnostic itself, not an A20 result.  S4 is a
valid restoration of the original allocation contract, but is not the fixture
root.

## S5: restore the CCPU TLS `setjmp` object ABI

The standalone x64 port selects the original NTVDM TLS simulation-stack path,
whose helpers return `jmp_buf *`.  It currently passes that pointer directly
to `setjmp`; modern headers require the referred-to `jmp_buf` array object.
This produces a compiler diagnostic at each affected CCPU exception/simulation
boundary.  The established NTVDM implementation uses `setjmp(*helper())`.

Change all three selected CCPU call sites consistently to the latter form.
This is a compiler/port ABI correction only: it adds no controller, no timing
policy, no snapshot behavior and no product/content condition.  Preserve the
existing TLS owner and matching `longjmp` route.  Verify x86/x64 compilation
has no such pointer-to-array warning, run the CCPU/snapshot suites, and repeat
the bounded external fixture run.  If it still fails, record the next CPU or
device fact rather than infer causation from the display result.

## S6: preserve the restored CPU continuation boundary

The snapshot restore path rebuilds the original CPU/device image and asks the
existing CCPU lifecycle to resume at its copied continuation entry.  Common
has already requested PAUSED while the write transaction is in progress, but
the ordinary snapshot observer only handled a capture request.  The restored
CPU could therefore execute until an unrelated later lifecycle checkpoint
before it noticed that PAUSED request.  That is not a valid restore: the
captured `CS:EIP`, stack and device state must remain exact when
`common_machine_write_state()` returns.

Keep Common and Lib unchanged.  The VM adapter records one pending restore
rendezvous immediately before it enters the existing CCPU lifecycle.  Its
first lifecycle observer call consumes that marker and invokes Common's
already-installed executor callback, which enters the existing PAUSED
rendezvous before another guest instruction is decoded.  Clearing the marker
on every completion/failure path makes it a one-shot boundary, not a second
executor or state machine.

Extend the existing snapshot transaction smoke: decode the saved archive,
write it through the public Common API, then immediately recapture it and
compare CPU execution state, RAM and page types.  This must fail if any guest
instruction runs before the restored machine is parked.  Re-run the external
`snapshot.02` fixture without modifying its snapshot, INI or disk image.

## Repair requirements for a later S

Any proposed repair identifies the single state owner and preserves the
original MVDM source wherever a host/port-ABI adaptation suffices. Direct
mirror changes require a portability/ABI reason, original-diff ledger entry,
and x86/x64 proof. VM/Compat may copy/bridge original facts but may not create
a second controller, timing source, display state, or device protocol.

The post-repair matrix must cover the supplied snapshot route plus existing
graphics/text, reset, snapshot and dual-width regressions. It must compare
fixture hashes and retain no owner media/configuration change.

## S7: use the new stack descriptor when loading a call-gate stack pointer

The fixture reached a generic protected-mode CCPU error, not a display error.
A 16-bit call gate entered a 16-bit target code segment at higher privilege,
while its 386 TSS supplied `SS=0x0030` and `ESP=0x80010df0`.  The new SS has
the 32-bit stack-address attribute.  The CCPU call-gate path instead selected
`SET_SP` solely because the gate has 16-bit operands, discarding the high ESP
bits.  Its normal word pushes then overwrote low memory that an ensuing
real-mode trampoline uses as a far pointer, producing an invalid-opcode loop.

The architecture already has the correct primitive: `set_current_SP()` selects
SP or ESP from the newly loaded SS descriptor.  Replace the local
operand-size branch in every privilege-changing stack-load path (CALL gate,
lower-privilege IRET and lower-privilege RETF) with that helper.  Instruction
operand size still controls the width of copied parameters and return items;
it must not determine the address width of the newly loaded stack.

No product, snapshot, content, presentation or timing condition is admitted.
The proof is a focused CCPU stack-width test plus dual-width regression and a
bounded external `snapshot.02` run that confirms no overwritten trampoline
pointer or resulting real-mode invalid-opcode loop.  The fixture hashes remain
unchanged.

### S7 evidence

The bounded `snapshot.02` probe established that the uncorrected 16-bit call
gate read the correct `ESP0=0x80010df0` from the 386 TSS, then truncated it to
`SP=0x0df0` after loading the 32-bit stack segment.  Its word pushes therefore
wrote through the low-memory real-mode trampoline far-pointer at `0070:06de`.
The later trampoline consumed the corrupted pointer and entered a real-mode
invalid-opcode loop.

After the one-line replacement with `set_current_SP(new_sp)`, the same bounded
probe records neither the low-memory write nor the invalid-opcode loop and
retains a valid 1280x768 graphics frame before the probe's own pause boundary.
The focused call-stack, 8042/PIC and snapshot transaction tests pass on both
host widths.  The external `snapshot.02` and fixed-disk SHA-256 values remain
unchanged.  The normal non-integration regression suite passes 105/105 on x64
and x86.  The two pre-existing package Window smoke cases remain environment
blocked at their window-observation stage after printing `Machine started.`;
they are not used as evidence for this CCPU repair and receive no product
workaround here. Their environment attribution is unproven; the observed
failure is retained explicitly rather than reported as a full-suite pass.

The owner subsequently confirmed that the early-detection snapshot passes
hardware detection, accepted the later memory-warning prompt, and saved a new
checkpoint. This verifies the original reported stall repair, not every later
Setup stage. The owner approved S7 closure while explicitly keeping T70 open.
That approval accepts the bounded verification above; the two Window smoke
failures are not silently counted as passed.
