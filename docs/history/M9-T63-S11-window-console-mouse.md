# M9 T63 S11: raw Console mouse exclusion beside Window

Common UI now assigns each KVM leaf a private input context. While a Window is
live, only a raw VM-Console mouse event is consumed; Console keyboard, text
and registered hotkeys retain their original route, Window mouse continues to
flow, and Console mouse resumes after Window destruction. The Window-liveness
read is atomic because Console input and Window destruction use independent
workers.

The composition proof covers all four routes. Full x86 and x64 suites each
passed 106/106. `678cb2b` is pushed. No Lib, VM, Compat, MVDM, Session or
public ABI change occurred.
