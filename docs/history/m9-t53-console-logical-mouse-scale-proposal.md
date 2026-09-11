# M9 T53 — Console Logical Mouse Scale

## Purpose

Repair the raw Console mouse conversion so one native Console character-cell
movement produces the same eight-unit logical relative movement on either axis.
This corrects the visible vertical two-row jump while preserving the existing
application input queue, machine adapter, Window input path, and native
Console ownership model.

## Fixed Boundary

`ui-console` alone derives a relative UI mouse event from successive native
Console cell coordinates.  Its first position establishes the baseline.  Each
later cell delta is expressed in logical eight-unit cells on both axes.  The
library does not interpret guest mouse protocols or modify the recovered
machine.  In particular, this task does not add raw-Console pointer capture,
does not alter `ui-window`, and does not change application mouse coalescing.

## Evidence And Completion

The focused smoke delivers successive raw-Console positions directly to one
`ui-console` logical Console and proves the emitted X and Y one-cell deltas are
both eight units.  A similar-issue sweep records every production conversion
from raw Console mouse coordinates and each retained `8`/`16` presentation
constant.  Fresh fixed x64 and x86 CTest, packaging, manifest, governance, and
diff checks are required before closure.
