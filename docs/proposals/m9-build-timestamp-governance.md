# Build timestamp governance

Owner reports a Nov 6 banner in the Nov 8 rebuilt package. composition.c owns
__DATE__/__TIME__; Ninja reused its old object. Relinking alone cannot refresh
that data. Disable compiler launcher for that single object and force its
recompilation through a build-local stamp. Keep other caches and the banner ABI.

Root publication checks fresh object ordering and embedded date/time; repeated
unchanged-source builds plus old-artifact rejection prove the missing contract.
No runtime clock substitute, source touching or generated C/H is admitted.
Owner accepts the delivery on 2026-11-08; see [Td S23 closure](../history/M9-Td-S23-build-timestamp.md). CURRENT owns remaining T99 status.
