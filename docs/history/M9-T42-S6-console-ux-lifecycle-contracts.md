# M9 T42 S6 — Console/UX lifecycle contracts

## Outcome

The generic library now makes Current Console ownership and UX retirement
mechanical facts rather than SoftPC policy.  A process has one host broker;
replacement validates its next binding before disturbing the current reader,
serializes native output/sink turnover, rejects stale generation callbacks,
and restores the former active binding on activation failure.  A non-current
logical Console reports `NOT_CURRENT`.

`ux-window` and `ux-console` remain independent leaves.  Their common base
now gives STOP a reserved terminal FIFO slot, waits for component retirement,
assigns monotonic source identities, and reports a copied-input delivery
failure explicitly.  Console retirement detaches its logical Console event
sink before publishing `SOURCE_RETIRED`, so that notification is the final
raw-input fact for the source.  The SoftPC queue latches an allocation-free
failure record if its dynamic storage cannot grow.

## Proof

- GCC x64 full build and CTest: 27/27 passed, including package smoke.
- Deterministic broker tests cover singleton creation, prepare failure,
  activation rollback, stale/non-current writes, and replacement ordering.
- Base and UX tests cover binding-generation rejection, complete registered
  chord suppression, mismatch replay, source identity uniqueness, explicit
  sink failure, and STOP admission behind a full normal control FIFO.
- The x86 shared-library targets `base-console`, `host-console`, `ux-base`,
  `ux-console`, and `ux-window` compile; their Console probes pass.  The full
  retained-MVDM x86 launcher remains blocked by the host's GCC 16 legacy-C
  incompatibility and old `.lib` linker spelling, outside S6's lib corpus.

## Next

S7 owns actual SoftPC real-thread integration/barrier coverage.  It must not
add product policy to `src/lib` or reopen this generic ownership boundary.
