# M9 T85 S11 delivered corpus and owner-directed continuation

Original request: 准入一个s来导入.
P1 b45b2180 and independent review P2 4c36853c are pushed. All 843 shared
paths match pinned NXVM 43e9700e. Production is +0/-0; thirty-two test/build
units are +1430/-122, net +1308, including five inward test relocations.
Dual full Release builds pass; focused 28/28 plus PC timeline 1/1 each;
background x64 469/469 (460.53s), x86 469/469 (327.16s), desktop excluded.
Full finite ledger, failure disclosure and review remain in the
[proposal](../proposals/m9-windows-101-startup-repair.md#s11-eight-package-test-corpus-synchronization).

Owner now requests: 没关系，没关系，你可以准入一个新的 S 任务，把 NXVM 的所有有差异的部分导入进来，然后再跑这个测试，四个测试。
This replaces S11's wait with the next synchronization, S12. Its completed
technical delivery is archived so there is only one active packet. No manual
product acceptance is inferred; S12 must qualify the newer corpus independently.
T85 remains open. Package EXEs retain their accepted hashes; the original owner
snapshot remains untouched and excluded. Existing INI bytes were preserved and
its owner edit shipped in S11 per Execution Rules.
