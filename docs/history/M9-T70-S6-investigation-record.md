# M9 T70 S1--S6 investigation record

This is the completed-step record required to continue the still-open M9 T70
investigation. It records bounded findings only; it is not a T-level closure.
The subsequent repair is recorded in
[S7 closure](M9-T70-S7-stack-width-repair.md); T70 remains open.

| Step | Completed bounded result | Disposition |
| --- | --- | --- |
| M9-T70-S1 | The supplied Windows 95 Setup fixture enters its own recovery path during hardware detection; the graphics-to-text change is not a KVM presentation failure. | Continue with measured machine facts. |
| M9-T70-S2 | The original 8042 `0xC0` command returned an uninitialized byte; it now returns its initialized input-port value and the port-level proof passes. | Valid controller repair, insufficient to explain this fixture. |
| M9-T70-S3 | A rejected PIC acknowledgement was narrowed into a CCPU delivery slot. The CCPU dispatch boundary now rejects it before external-interrupt side effects. | Valid generic CCPU/PIC repair, insufficient to explain this fixture. |
| M9-T70-S4 | Compat now allocates the original SAS no-wrap protection range rather than a smaller unexplained tail. | Valid original-allocation restoration, insufficient to explain this fixture. |
| M9-T70-S5 | The selected TLS simulation-stack path now passes the referred-to `jmp_buf` object to `setjmp`. | Valid calling-ABI correction; no product policy changed. |
| M9-T70-S6 | Snapshot restore rendezvous now parks the restored executor before another guest instruction is decoded, and immediate recapture proves copied CPU/RAM/page state is preserved. | Valid snapshot-continuation repair; the later Setup failure required a separate CCPU investigation. |

The original owner fixture, its INI and fixed disk image remain external and
untracked. No step above claims that the hardware-detection failure is closed.
