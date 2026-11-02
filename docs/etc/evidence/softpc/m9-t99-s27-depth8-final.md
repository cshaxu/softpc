# M9 T99 S27: eighth-depth reconciliation

Baseline is S26 closure `975596e6`. The [exclusion delivery](m9-t99-s27-depth8-excluded.md)
was pushed as `1adedb19` after dual builds, publication checks and full
qualification. It records the initial x64 verifier timeout, isolated retry
and passing full `-j2` reruns: 194 tests per architecture, without skips.

The one eighth-depth candidate `#ifndef GISP_CPU` is selected in both actual
architectures and matches the XP original; no frame restoration or production
change was needed. The [current ninth-depth queue](m9-t99-s27-depth9-provisional-queue.tsv)
and [XP direct-child queue](m9-t99-s27-depth9-xp-direct-queue.tsv) each
contain the same two candidates. They are overlapping entries, not four
issues; no ninth-depth body was assessed.

No production or test source changed in S27. Only documents and queue
evidence changed after the exclusion qualification, so its published binaries
and full test results remain applicable. The initial timeout is not attributed
to a confirmed root cause.
