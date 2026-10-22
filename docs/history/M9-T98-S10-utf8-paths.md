# M9 T98 S10 Closure

Owner accepted and closed S10 on 2026-10-21 and admitted S11 Window sizing,
capture and mode-transition investigation. T98 remains open.

Executor/review commits: 0151689d / 97e492b5 (UTF-8), bf01eead / 296d4165
(owner-admitted warning follow-up). The proposal retains the finite caller
audit, actual changes and verification chronology.

All owned file entries use the existing Storage UTF-8 boundary. COM/LPT retain
lazy append, accepted-prefix accounting and flush visibility. The follow-up
corrects the VGA test's native C-VID ABI, without changing production or mirror
code. Non-mirror compilation is warning-free on both Windows architectures;
original mirror header warnings remain exempt by owner instruction.

Final x86/x64 each pass 192/192 registrations (184 background, five desktop,
three native audio), no test retries/skips. All 242 non-mirror C objects per
architecture were rebuilt with cache. Task scratch was retired, cache retained.
INI, media and the pre-existing untracked snapshot.debug were preserved.

Final package SHA-256:

- softpc32: 78FE670609EDAB63B5B6532920C54544D217941C02004CF7F4EABFEDD828F888
- softpc64: 0A52927A7B91AEE1295928C62E505100BBC97A55A13D55FE40F1C97A1F0F314A
