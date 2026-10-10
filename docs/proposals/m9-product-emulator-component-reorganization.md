# Product and Emulator component reorganization

## Intent

The reusable stack becomes:

```text
lib < emulator < product
```

`lib` remains the innermost platform capability layer. `emulator` is the
renamed neutral machine/UI/session layer. `product` is the outer shared
application-facing surface: command/debug policy, assembly utility and
session/UI composition.

This is a structural reorganization, not a new runtime path.  Every move uses
`git mv`; exactly one implementation remains after each move.

## S17 — establish Product

Move `src/x86/xasm32` to `src/product/xasm32`, `src/x86/debug` to
`src/product/debug`, and `src/ibmpc/product` to `src/product/surface`.
Move their test owners into `test/product`.  Rename the moved component's
internal/public symbols, target names, test registrations, manifest entries
and direct references to `product_*`, so its vocabulary matches its ownership.

After relocation, delete the now-unused `src/x86`, `src/ibmpc`, `test/x86` and
`test/ibmpc` trees, their CMake entry points and their obsolete Product build
helpers. Product's corpus gate verifies the positive dependency rule: Lib has
no outer dependency, Emulator may consume Lib only, and Product may consume
Lib or Emulator only. This S does not rename Common.

## S19 — rename Common to Emulator

Rename `src/common` and `test/common` to `src/emulator` and `test/emulator`.
Rename the component-local `common_*` interfaces, targets, directories,
manifests and all direct consumers to `emulator_*`.  This is mechanical naming
and ownership normalization: lifecycle/session/UI semantics must not change.

## S20 — optional Ninja compiler-cache route

Keep the regular Ninja and Makefiles presets dependency-free. Add a separate,
opt-in `ninja-ccache-*` preset family that invokes the existing C compiler
through `ccache`; it neither places cache entries in the repository nor
changes product compilation, runtime or package behavior. Measure x64/x86
cold and warm full rebuilds with a disposable cache directory, and separately
measure CTest to distinguish compiler-cache benefit from test execution time.
Do not retain the route if its configured cache cannot prove correct dual-width
builds and the expected warm-cache compilation improvement.

## S20 P2 — synchronize the current shared six-component corpus

Import NXVM's current `src/{lib,emulator,product}` and
`test/{lib,emulator,product}` byte-for-byte, together with the shared
`test/register.cmake` helper.  This is a corpus synchronization, not a local
redesign: remove local files absent upstream and accept upstream moves between
Emulator and Product as authored.  Preserve App, Core, Compat, user INI,
media, package assets and every NXVM-external SoftPC path.

Before committing, prove the exact path/hash ledger for all six roots and the
registration helper, run the component corpus/manifest/dependency gates, then
perform proportional x64/x86 build and background-test verification.  No local
shim, compatibility wrapper or SoftPC-only change is permitted inside the six
shared roots.

The import audit found one shared Product contract gap: Emulator Product
correctly parses the fixed `save` and `load` commands, while Product Surface
has no route for an App to implement them. S20 P2 therefore adds one
Product-neutral callback which receives the already-parsed operation and its
argument tail. It does not add parsing, a Session request kind, a completion
queue, an executor or a product-specific fallback. SoftPC registers its
existing snapshot implementation; another App may explicitly report unsupported.
The existing Emulator Machine state-I/O calls already enqueue executor work at
a safe point and synchronously report its completion to the sole control
caller, so an additional asynchronous completion protocol would duplicate that
mechanism and is out of scope.

## S20 P3 — restore the neutral Surface preamble and narrow ERROR admission

S20 P2 accidentally replaced SoftPC's existing generic Surface opening with
NXVM's newer App-owned opening arrangement, removing the user-visible
`<product name>`, blank-line, `Built on <date> <time>`, blank-line preamble.
Restore the prior Product-neutral Surface behavior: the App supplies the name;
Surface formats the stable preamble and names no particular product. Add a
focused entry proof that intercepts this one output sink and verifies the
required framing without starting a native Console.

The same review found P2's ERROR admission too broad. ERROR rejects only the
five fixed lifecycle operations (`start`, `stop`, `pause`, `resume`, `reset`)
and the fixed `save`/`load` operations. It
continues to forward `debug` and every App extension: entering the debugger is
independent of machine state, and each receiver owns any later machine-state
check. Prove both fixed and extension routing at the Emulator monitor boundary.
Do not change Lib, Core, Compat, user configuration or machine-state ownership.
