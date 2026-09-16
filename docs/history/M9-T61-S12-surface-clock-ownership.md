# M9 T61 S12: original surface and host clock ownership

Owner serial admission and before/after accounting requirements remain in the
[proposal](M9-T61-S7-support-ownership-proposal.md).

Implementation `e2ef91f` is committed and pushed. Actual-commit review confirms
the VM no longer infers dimensions from dirty rectangles or owns three geometry
caches. It copies the original DIB's dimensions and pixels. The original V7
table and painters remain unchanged. A complete-frame baseline passed all V7
60h–69h and VGA 12h/13h cases plus text/graphics transitions before removal;
half-frame refresh then reproduced 752x410 becoming 376x410. The same matrix now
passes both widths, including right-half repaint and no-dirty behavior.

The four-line GetPerfCounter body moved from mirror nt_sound to existing Compat
audio with its units/wrap unchanged. The mirror retains its original IMPORT;
sound state stays in MVDM. No forwarding API, new file, or shared change.

Production three paths +12/-28, net -16 (estimated -15 to -25); tests two paths
+70/-0 (estimated +80 to +120). Mirror nt_sound diff reduced +64 to +59, with no
original deletion. No build changes. Detailed first-run test-fixture correction,
counts and EXE hashes are in the
[evidence](../etc/evidence/softpc/pristine-divergence-current.md#s12-实施前审计).

Full builds/suites: x64 98/98 (56.98s), x86 98/98 (72.78s). Coordinator reviewed
the actual production diff and ran sound, audio lifecycle/failure, VGA frame and
sound-state tests: x64 5/5 (2.08s), x86 5/5 (2.73s). Documentation/diff checks
pass; owned logs removed, INI/media unchanged, fixed EXEs refreshed.

S12 closes and S13 is admitted for whole-ledger convergence. T61 remains open
for owner manual acceptance; automation does not claim that acceptance.
