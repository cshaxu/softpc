# M9 T38: Direct Source Transform Retirement

## Outcome

Closed. The four mechanical source-transform scripts are removed. Their 52
previously generated C/H inputs are maintained directly at the corresponding
paths under `src/mvdm/softpc.new/`; CMake consumes those checked-in paths and
has no Python requirement, custom source-rewrite command, or generated C/H
target input.

The original CCPU archive order is retained as explicit source selection, not
as a generated-source side effect. C-VID event glue now explicitly selects the
same shared ABI headers that its former generated-directory placement selected
implicitly. This keeps the x64 GDP layout correct without a build-tree source
copy.

The divergence audit lists all 52 materialized points explicitly as
`port-abi-direct`; it remains fail-closed for every other current difference.
The source-boundary gate rejects any future transform script, Python source
generation, custom command, or generated device/CCPU/C-VID source input.

## Verification

- Fresh MinGW Make x64 configure/build: passed; full CTest 23/23 passed.
- Fresh MSYS2 i686 MinGW Make configure/build: passed; full CTest 23/23
  passed.
- Both package-smoke tests passed and produced `softpc32.exe` and
  `softpc64.exe` from checked-in source.
- The direct-source boundary gate and the current pristine divergence audit
  passed; `git diff --check` passed.

## Non-goals Kept

No blocked behavioral point was recast as a source transform, and no new
machine state, lifecycle, media, presentation, or policy implementation was
placed in the preserved source mirror. User-owned `softpc.ini` and guest media
were not edited.
