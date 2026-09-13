# M9 T55 S21: KVM corpus rename

The owner accepted S20 package testing and directed the mechanical namespace
rename. The admitted design remains in
[`M9-T55-S21-kvm-corpus-rename.md`](../../../history/M9-T55-S21-kvm-corpus-rename.md).

## Result

- The three shared keyboard/video/mouse components are now `kvm-base`,
  `kvm-window`, and `kvm-console`. Their direct dependency graph is unchanged.
- Every active component path, include, CMake target, symbol, test target,
  Window class identity, app consumer, manifest entry, and component document
  uses KVM spelling. The cooked monitor remains a distinct monitor/Console
  concept.
- `verify_kvm_naming.cmake` scans active source, tests, build files, and
  current authorities for the retired namespace. Its deliberate old-identifier
  fixture proves rejection; history and retained prior evidence are outside its
  scope.
- An independent reverse mechanical comparison restores the active source and
  test spelling to the S20 vocabulary before comparing it with the S20 tree.
  It found no semantic difference; the only non-mechanical source additions
  are the naming gate, its negative fixture, and their CMake registration.
- No wrapper, alias target/header, worker, queue, state, data layout, or
  control-path behavior was introduced.

## Verification

- Fresh x64 package build and full CTest: 58/58 passed.
- Fresh x86 package build and full CTest: 58/58 passed.
- Strict shared-library build and CTest: 8/8 passed, including KVM naming
  positive and forbidden-fixture checks.
- Manifest, component-DAG, KVM naming, standalone-source-boundary, and
  documentation-governance gates passed. `git diff --check` passed.

## Worktree boundary

The only retained user-owned worktree change is `assets/binary/softpc.ini`.
It was not read, rewritten, or staged by this task.
