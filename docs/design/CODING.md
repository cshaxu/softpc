# Source Layout

The current tree is transitional. The approved target is:

```text
src/
  core/
    softpc-pristine/
    softpc-port-abi/
  host/
    softpc-compat/
  vm/
    runtime/
    frontends/{console,win32}/
    main.c
```

Directories appear only in their admitted migration task. `softpc-pristine`
contains no standalone behavior branches. `softpc-port-abi` contains generated
or overlay code necessary to compile that baseline on x86/x64. `host` owns
platform capability implementations; `vm/runtime` owns orchestration and
mailboxes; frontends own only user interaction and presentation.
