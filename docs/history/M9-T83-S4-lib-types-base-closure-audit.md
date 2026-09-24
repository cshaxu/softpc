# M9 T83 S4 Lib Types/Base closure audit

## Scope

S4 audited every tracked C source/header below `src/app-softpc/` and
`test/app-softpc/`. The preserved `src/app-softpc/softpc.new/` mirror and the
six shared components were excluded.

The resulting ledger contains 109 in-scope files. Fifty-four direct consumers
now include and use the public Types contract; App test timing now uses the
Base clock/synchronization interfaces. Four Compat headers remain retained
port-ABI bridges because mirror translation units include them:

- `compat/ccpu/snapshot.h`;
- `compat/cvidc/gdp_rule_access.h`;
- `compat/cvidc/gdp_state.h`;
- `compat/devices/snapshot.h`.

Those headers retain their original scalar vocabulary so the excluded mirror
does not acquire a Lib include-path dependency. C stream I/O, formatting and
native Console verification remain direct because the existing Lib contracts
do not provide equivalents.

## Permanent gate

`tools/checks/lib_types_base_convergence.cmake` scans the admitted App scope
and rejects direct standard scalar headers/types, allocation, memory/text, and
clock/sleep services. Its CTest negative fixture proves a new bypass is
rejected and that the mirror exemption remains effective.

## Verification

- x64 Release build completed and refreshed `softpc64.exe`.
- x86 Release build completed and refreshed `softpc32.exe`.
- x64 full CTest: 123/123 passed in 261.61s; restart-boot passed in 57.62s.
- x86 full CTest: 123/123 passed in 287.96s, including restart-boot in 57.76s.
- The convergence gate and its negative fixture passed on both widths.
- `git diff --check` and documentation governance passed.

## Artifacts

| Artifact | SHA-256 |
| --- | --- |
| `assets/binary/softpc32.exe` | `7B28739226BD927271F494EB23B90D6986C8776F75AD72D982C74EFCABEEDD8A` |
| `assets/binary/softpc64.exe` | `454C015FA9292DF1BFCCFD62CD61E787ECD0A4F303E70DA7C565AF55B4B968A0` |

No corrective P was needed: the initial one-off restart timing failure did not
reproduce in focused repetition or in the final full x64 and x86 routes.
