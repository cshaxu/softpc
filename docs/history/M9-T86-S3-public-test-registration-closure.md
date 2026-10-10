# M9 T86 S3 closure: public-test registration conformance

The owner accepted S3 and admitted the T86 closing qualification step on
2026-10-10.

S3 retained the public `lib.*`, `emulator.*` and `product.*` identities,
labelled the 21 explicit verifier/script registrations that had lacked an
owner, and added a configuration-time check that every shared test uses its
component prefix and `unit;<owner>` labels. It intentionally did not import
NXVM's uncommitted `library.*` spelling.

During qualification, the x64 Lib capture-contract fixture exposed a test-only
stack exhaustion: three whole `kvm_window_frame` compound assignments caused
the compiler to reserve roughly three MiB of automatic temporary storage.
Equivalent in-place clears preserve every existing assertion and reset value.
Production code and public interfaces are unchanged.

Test/build code changed by `+55/-6` (net `+49`); production code changed
`+0/-0`. Independent Lib, Emulator and Product suites passed respectively
48/48, 21/21 and 15/15 on each width. Root non-desktop CTest passed 129/129
on x64 and x86. The public ledger reports 87 registrations, zero bad labels
and zero `library.*` names. Documentation governance and all three manifests
passed on both widths. Executor P1 is `2f706c37`.
