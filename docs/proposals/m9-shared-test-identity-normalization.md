# Shared public-test registration conformance

## Observed design defect

The public helper preserves the complete test-relative identity, but 21
explicit Lib/Emulator script and verifier registrations omit their owner label.
The latest NXVM worktree attempts to repair that omission by renaming `lib.*`
to `library.*`, which conflicts with the canonical public directory and the
approved SoftPC component name.

CTest names are part of the public verification interface: they are used for
selection, diagnostics and future collision avoidance.  They should identify
the owning shared component and test purpose without relying on a coincidental
filename or a private directory layout.

## Intended normalization

Retain every existing public CTest identity. The grammar is:

```text
lib.<relative-test-source>
emulator.<relative-test-source>
product.<relative-test-source>
```

Examples include `lib.manifest`, `emulator.product/monitor`,
`product.surface/command`, and `product.xasm32/xasm32_contract`. A root-owned
test has no artificial directory segment. Every entry must retain one owner
prefix and have labels `unit;<owner>`; `desktop` and resource locks stay
additive. There are no aliases or duplicate registrations.

The task changes only test registration/build metadata and manifests. One
Lib fixture reset is rewritten in place to avoid compiler-created giant stack
temporaries; it preserves the same reset values and assertions. The task does
not alter production code, test behavior, component dependency direction,
public C interfaces, package behavior, user configuration or media.

## Scope and acceptance

The finite universe is every CTest registration defined by
`test/lib/CMakeLists.txt`, `test/emulator/CMakeLists.txt`,
`test/product/CMakeLists.txt`, and the shared `test/register.cmake` helper.
Before editing, record all 87 current public registrations and the 21
unlabelled entries. After editing, prove every selected test has exactly one
registration, every name conforms to the existing grammar, every entry has
`unit;<owner>`, and all public component suites pass standalone and embedded on
x64 and x86. Preserve existing desktop labels and resource locks.

Do not fold in production refactoring, new test behavior, compatibility
aliases, `library.*` renames, unrelated App/Core tests or a new framework. If
a current name is consumed by an external CI contract that cannot retain its
existing spelling, stop for owner direction rather than retaining a second
registration.
