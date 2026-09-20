# M9 T71 S6 Integration Audit

## Coordinator Review

Executor delivery 03fe76276bd170f27db668f7540a2e81c3d28ca8 is pushed to main.
The coordinator reviewed its actual eight changed paths against the original
owner request, active S6 packet, architecture/coding rules and the finite audit
ledger in the [T71 proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
The only production adjustment is local default-height cursor arithmetic;
no extra state, allocation, interface, worker or product branch is introduced.

The added regression demonstrably failed before the repair, then passed on
both widths. Existing shape fallback remains unchanged. Base remains opaque;
leaf frame/resource ownership, VM-only mapping, bounded validation and explicit
producer failure all have source and test dispositions in the proposal ledger.
The separate neutral-attribute candidate is not part of this task. Existing
TODO hardware-height/timer investigations are not claimed repaired by T71.

## Verification

Both Release builds succeeded. Serial full tests: x64 110/110 (113.24 seconds),
x86 110/110 (98.52 seconds). They cover package interaction, snapshot transaction
and cross-process reconstruction, input/lifecycle, four manifests and dependency
gates with negative fixtures. The two x64 Lib manifest checks were repeated
after updating their revision comments; hashes did not change. Documentation
governance and whitespace checks pass. No fresh manual guest or native Linux
presenter validation is claimed.

## Actual Accounting

Counted against 910ecdcc using git diff --numstat, excluding unrelated edits:

| Category | Added | Removed | Net |
| --- | ---: | ---: | ---: |
| Production C/H: geometry.c | 7 | 6 | +1 |
| Test C/H: damage/mouse smoke | 4 | 0 | +4 |
| Two manifests | 4 | 4 | 0 |
| Build/gates | 0 | 0 | 0 |
| P1 documentation | 135 | 3 | +132 |

P1 totals +150/-13. This P2 is documentation-only and counted separately.
T71 final production C/H relative to 7557ca5: +626/-407 (net +219), 34 paths;
tests +661/-336 (net +325), 30 paths. Source structs retain S4 measured sizes:
Console 7112 bytes, Window 984100, Common 985128. Seven main frame values save
2050880 bytes compared with the old layout; UI temporary saves 991272 bytes.
These are layout costs, not whole-process measurements.

Packages are refreshed with no size increase from S5:

- x86 3655213 bytes; SHA-256 E5229A2176F8E83E960B0C9C430E6C5246B2CFFEDEC3370D6D3CC1F7E4AB6182.
- x64 3058756 bytes; SHA-256 90949A63700FAB618EA37E17ADDFBD6181DB4027484BB1BE2D781DECE084E684.

## Closure

S6 exit criteria pass and the coordinator accepts S6. T71 S1--S6 are complete;
T71 itself remains open for owner package acceptance. No new implementation S
is admitted. No temporary diagnostic directory was created. The preexisting
Queue and neutral-attribute proposal edits remain untouched and uncommitted.
