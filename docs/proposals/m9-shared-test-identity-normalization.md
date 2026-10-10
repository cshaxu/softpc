# Shared public-test identity normalization

## Observed design defect

The shared test registration helper derives a target name from only the final
source-path segment. Its public CTest identity must instead retain the complete
test-relative path. Product also mixes that path-preserving style with manually
registered dot, underscore and hyphen names. Lib uses a separate `library.*`
prefix even though its public directory is `lib`.

CTest names are part of the public verification interface: they are used for
selection, diagnostics and future collision avoidance.  They should identify
the owning shared component and test purpose without relying on a coincidental
filename or a private directory layout.

## Intended normalization

Use the public test-relative source identity at each registration site. The
shared helper constructs the CTest name from the corpus root and that complete
relative identity. The resulting grammar is:

```text
lib.<relative-test-source>
emulator.<relative-test-source>
product.<relative-test-source>
```

Examples include `emulator.product/monitor`, `emulator.session_monitor`,
`product.surface/command`, and `product.xasm32/xasm32_contract`. A root-owned
test has no artificial directory segment. All existing selector names must be
inventoried and renamed consistently; there must be no hidden aliases or
duplicate test registrations.

The task changes only test registration/build metadata and, where useful,
test directory/file spelling. It does not alter production code, test behavior,
component dependency direction, public C interfaces, package behavior, user
configuration or media.

## Scope and acceptance

The finite universe is every CTest registration defined by
`test/lib/CMakeLists.txt`, `test/emulator/CMakeLists.txt`,
`test/product/CMakeLists.txt`, and the shared `test/register.cmake` helper.
Before editing, record each current name, source, owner and proposed final
identity; after editing, prove that every selected test has exactly one
registration, names conform to the grammar, and all public component suites
pass on x64 and x86.  Preserve the existing desktop labels and resource locks.

Do not fold in production refactoring, new test behavior, test-framework
features, compatibility aliases or unrelated App/Core tests.  If a current
name is consumed by an external CI contract that cannot move atomically, stop
for owner direction rather than retaining a second registration.
