# M9 T81 S4: four-package Audio acceptance

## Boundary and result

S4 starts from S3 executor `d8830b53`. Its only purpose is to prove the exact
neutral import set -- `src/lib`, `src/common`, `test/lib`, `test/common` --
continues to stand alone after Audio joins Lib. It does not modify production
or test source, public ABI, product behavior, Core, App or x86.

A fresh temporary root received exactly those four directory trees. There was
no App, Core, x86, product CMake, INI, firmware or media in that root. Hash
comparison before cleanup found 189 copied files and 0 mismatches. `test/lib`
and `test/common` each configured from that copied layout and independently
located only their sibling copied source roots through their existing relative
paths. No configurable-path layer, wrapper runner or import compatibility
branch was added.

## Verification

All copied builds select C11, extensions off and GNU strict warning flags.
Desktop-labelled Lib tests were excluded; no test created a Window/Console UI.

| Copy / width | Lib | Common |
| --- | ---: | ---: |
| x64 | 44/44 | 18/18 |
| x86 | 44/44 | 18/18 |

The Audio native smoke is included in each Lib result. It opens WinMM, submits
silent PCM, clears and destroys it, demonstrating bounded native lifecycle but
not claiming that a physical speaker was audible. Audible PC Speaker behavior
is the explicit S5 product acceptance responsibility.

Fresh Release product launchers were built for both widths. Their code inputs
are identical to S3, so both launchers are intentionally byte-identical to the
S3 artifacts rather than receiving artificial rebuild churn. Hidden background
regression, with only `desktop` labelled cases excluded, passes x64 114/114
(210.77 s) and x86 114/114 (176.91 s). This does not claim Linux runtime or
downstream NES integration.

## Accounting and review

S4 estimate: production +0/-0 and test/build/docs +60..140/-0..20. Actual
tracked code/build/docs change is +0/-0: existing package entries already had
the correct relative topology and strict C11 settings. The only retained
evidence is this task record; temporary copies, builds and logs were removed.
No source change is the smallest correct result: adding plumbing merely to
repeat what the existing self-contained CMake entries already prove would make
the reusable corpus less simple.

Coordinator review confirms the one changed path is evidence only, the copied
files/hash counts match the command results, and no needless import wrapper or
source change was introduced. S4 exit criteria are met and it is closed. The
owner's prior automatic authorization admits S5; T81 remains open.
