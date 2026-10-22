# M9 T98 S7 - Window sizing

Owner accepted S7 and admitted S8 on 2026-10-21. Executor ed9bb7b8 and
coordinator review 7bab2e62 are pushed. T98 remains open.

Initial/automatic sizing and maximize fit the work area. Manual sizing and
title-bar natural-size restoration retain aspect ratio without a desktop cap.
Production C/H +16/-7 (net +9); tests +55/-3 (net +52). No frame, App or
mirror production changes accompanied this repair.

Both EXEs were refreshed. x86 final 192/192 passed; x64 final 190/192 plus
two unchanged isolated passes cover all 192 registrations. This is not an
all-green x64 aggregate run. Intermittent integration failures remain in TODO.
The proposal retains the exact verification, package hashes and actual-diff
review. Task scratch was removed; both reusable caches remain. The owner's
untracked snapshot.debug is preserved and excluded from delivery.
