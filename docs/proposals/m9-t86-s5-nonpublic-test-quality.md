# T86 S5: Non-public Test Quality Audit And Repair

## Objective

Bring the non-public SoftPC tests up to the same engineering standard as the
public `test/lib`, `test/emulator`, and `test/product` suites without pretending
that App/Core/Compat/mirror tests are reusable public components.

The audited universe is finite:

1. every tracked source, header and script below `test/app-softpc/unit/`;
2. every tracked source, header and script below `test/app-softpc/integration/`;
3. every root-CMake executable, test registration, property and target-source
   clause whose test name begins `app-softpc/`, `core/`, `compat/`,
   `softpc.new/`, `integration/`, or `checks/` and is not owned by a public
   suite; and
4. each test-only fixture/helper or `tools/checks` script reached by one of
   those registrations.

Generated build files, public test corpora, user assets and comparison-repo
files are excluded. A ledger member is complete only when it has one recorded
owner, kind (unit/integration/static check), executable or script route,
dependency set, working-directory/resource disposition, and one disposition:
retain, merge/remove, correct, or explicitly defer.

## Audit criteria

The public suites establish the baseline:

- one source-of-truth CTest registration with owner-path name and meaningful
  labels;
- test-local fixtures rather than cross-suite implementation imports;
- inward-only test dependencies, with a documented narrow exception only for
  the concrete product it owns;
- no duplicated scenario whose assertions add no distinct contract;
- deterministic setup/teardown, isolated working directories and exact native
  desktop/resource locks where needed;
- normal project Types and naming rules in new test code; no raw host ABI
  leakage where a Lib type/adapter already exists;
- assertions that prove both the claimed result and cleanup/failure behavior,
  without accepting an invalid shortcut merely because it passes today.

The audit compares NXVM only where it demonstrates a general test-construction
practice. It does not copy tests or make SoftPC's build dependent on NXVM.

## Repair boundary

S5 may delete or merge redundant tests, move a fixture to its actual non-public
owner, correct test setup/assertions, normalize CMake registration/properties,
and add a narrowly targeted regression for a proven missing failure or cleanup
contract. It must not change runtime/product behavior, public component source
or shared tests. A finding that needs such a change is recorded with its owner
and handed to a separately admitted task.

## Verification plan

After the audit ledger is complete, run static registration/ownership scans and
the documentation/manifest gates. Run every changed test plus neighbouring
owner tests on x64 and x86. If root test registration or resource scheduling
changes, run the declared background and serial desktop CTest presets for both
widths. Record the exact suite/label counts and every excluded case.

## Audit ledger and repair disposition

The finite CTest inventory contains 47 non-public routes: 28 App unit tests,
8 App integration tests, and 11 repository checks. The unit ownership split
is now `product` 1, `machine` 16, `compat` 9, and preserved `softpc.new` 2.
Each route has one owner path; root CMake derives its App tier label and
private build working directory from that same registered path.

- **Corrected owner drift:** nine Compat tests formerly placed under
  `unit/machine` now live under `unit/compat`; the C-VID/keyboard/media/audio
  test names and CTest names move with them. The two direct original-host
  layout/state tests now live under `unit/softpc.new`. This is a relocation,
  not a test semantic change.
- **Corrected duplicated registration knowledge:** three manually maintained
  CTest classification lists have been removed. The already authoritative
  registered owner path is now the sole label source. Desktop integration
  cases retain their explicit desktop label and root working directory; every
  other route retains a private directory.
- **Corrected test-local dependency spelling:** direct owner-local probes use
  normal source-root includes instead of four-level relative walks. Their
  direct implementation inclusion remains intentional: those tests inject
  owner-local native failures or verify preserved C90 behavior that a linked
  public contract cannot expose.
- **Corrected fixture vocabulary:** App-only cleanup/time helpers are now
  explicitly named fixtures, use the existing Lib Types/CRT/Win32 test
  vocabulary, and remain App-local. They do not import a public-suite helper
  merely because the operation is similar.
- **Corrected assertion build contract:** `config_smoke` receives the same
  build-time assertion enablement as the other App tests; it no longer
  undefines `NDEBUG` in its source.
- **Strengthened static sweep:** the standalone boundary check now scans all
  four App unit owners and verifies path-derived tier registration.
- **Corrected x86 configuration:** the declared MinGW and Ninja x86 presets
  now select the existing `i686-w64-mingw32-gcc` compiler rather than relying
  on an ambient `gcc` that can silently select x64 and then fail the explicit
  architecture gate.

Retained after review: the integration machine fixture remains the one shared
fixture for its three composition tests; the original-C tests retain their
original ABI vocabulary; no duplicate executable or outward test-suite
dependency was found. No production, public corpus, user asset, or published
EXE is changed by this S.

## Implementation evidence

- Fresh x64 and x86 MinGW preset configuration succeeds; x86 reports a
  32-bit host pointer width through the fixed declared preset.
- x64 focused owner/Compat/mirror regressions pass, including all relocated
  tests and the three static gates. The App unit subset also exercises the
  relocated owners in parallel without a shared working directory.
- x86 runs the same relocated owner/Compat/mirror regressions in the existing
  i686 build tree; the clean declared x86 preset separately rebuilds and
  passes the direct-implementation `parallel_failure_smoke`, plus the
  standalone-source and build-ownership gates.
- `git diff --check`, documentation governance, build ownership and standalone
  source-boundary gates pass. The latter now actively sees Compat and
  `softpc.new` units.

No Product executable or user-facing asset is rebuilt because every changed
input is CMake, documentation, or test-only source.
