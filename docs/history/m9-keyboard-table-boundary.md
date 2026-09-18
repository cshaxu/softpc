# Original keyboard-table boundary repair

## Objective

Repair the two one-past-end reads in the preserved `KeyMsgToKeyCode()` Scan-1
table lookup without changing the mapping of any valid ordinary or enhanced
keyboard record.

## Design

The table's byte size is also its exclusive upper bound.  Each lookup changes
only `scan > sizeof(table)` to `scan >= sizeof(table)`.  A local reason marker
identifies this as an original-source memory-safety correction.  It introduces
no host policy, mapper, compatibility wrapper, new input path, or product
state.

The existing table fingerprint test already proves all valid regular indices
`0..132` and enhanced indices `0..83`.  Extend it to prove the two first
invalid indices return zero, including through the public
`KeyMsgToKeyCode()` path:

| Table | Last valid | First invalid | Required outcome |
| --- | ---: | ---: | --- |
| Regular | 132 | 133 | `0` |
| Enhanced | 83 | 84 | `0` |

## Boundaries

- The original `nt_keycd.c` table remains the sole Scan-1 mapping owner.
- The narrow direct mirror diff is required because the defect is at that
  lookup boundary; moving it into Compat/VM would retain the unsafe original
  route or duplicate the table.
- No Lib, Common, App, VM, Compat interface, keyboard event schema, or guest
  behavior changes.

## Completion proof

Run the keycode focused test and full x64/x86 suites; refresh both package
EXEs.  The final audit must show exactly two functional mirror comparison
changes, tests proving the two first invalid inputs, no new mapping table, and
no transferred debt.
