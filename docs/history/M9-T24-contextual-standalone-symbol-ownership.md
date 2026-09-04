# M9 T24: Contextual Standalone Symbol Ownership

## Result

Closed.  Application-owned global families now use the `app_*` namespace:
runtime, monitor, console, window, keyboard, and launcher startup helpers.
Their declarations, direct test consumers, and source-boundary enforcement
were updated together.

## Boundary Finding

The proposal began with a broad inventory of 320 `softpc_*` candidates outside
the recovered source mirror.  The audit established that the host and
compatibility families cannot be renamed independently: selected original
sources and generated CCPU/C-VID forms import their existing spellings.  The
observed ABI includes `softpc_platform_*`, `softpc_ccpu_lifecycle_*`,
`softpc_gdp_*`, `softpc_host_cmos_fixed_disk_type`, and
`softpc_ccpu_interrupt_map_address`.

Those spellings are retained as compatibility-host machine ABI, not treated as
application naming debt.  The static boundary test now enforces the useful
rule: application-owned names cannot retain an unclassified `softpc_*`
spelling, while documented machine-façade types/functions remain legal app
dependencies.

## Invariants Preserved

- No source below `src/mvdm/softpc.new/` changed.
- No guest-visible behavior, controller, ROM, media, executable name, or
  user-owned `artifacts/binary/softpc.ini` changed.
- The application still reaches the machine only through the established
  `softpc_machine_*` façade.

## Verification

- GCC x64 rebuild and full CTest: 20/20 passed.
- GCC x86 rebuild and full CTest: 20/20 passed.
- The package-smoke integration tests passed at both widths.
