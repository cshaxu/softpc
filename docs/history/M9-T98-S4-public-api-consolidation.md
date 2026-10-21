# M9 T98 S4 - Public API consolidation

Owner admission requested removal of no-consumer public capability and
test-oriented API surface across the shared corpus. Owner accepted S4 on
2026-10-21.

Delivery `34cc3af842309d8bb834adbf5e5ca81b69fe3130` removes the unsupported,
unconsumed FM leaf, including its CMake, test, documentation and dependency
entries. Product composition now exports only `emulator_product_run()`; the
owner-local composition fixture includes the implementation to retain its
existing failure and reverse-destruction coverage without publishing staged
assembly functions.

The frame exchange no longer exports manual `lock_latest`/`unlock_latest`.
Machine caches its published metadata for Session, while the remaining scoped
observer invokes a callback under exchange protection and never lets a caller
retain a frame pointer. This preserves the pre-existing non-consuming
diagnostic-copy behavior without exposing lock ownership.

Accounting excluding rebuilt packages: production +55/-81 (net -26), tests
+94/-91 (net +3), documentation/build +45/-17 (net +28). Both package EXEs
were rebuilt. Per architecture, 184 non-desktop/non-native-audio cases passed
normally; the sole Junction-permission cleanup selftest passed separately with
the required host permission, completing all 185 qualified cases. Manifests,
dependency/Types/LF gates, documentation governance, diff whitespace and build
residue checks passed. User-owned `assets/binary/snapshot.debug` was not
modified.

S4 is closed. T98 remains open.
