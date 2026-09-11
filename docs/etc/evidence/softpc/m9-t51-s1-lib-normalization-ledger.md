# M9 T51 S1 — Library Normalization Ledger

## Frozen universe

This ledger freezes the tracked corpus at T51 admission. It was produced from
the following reproducible scans, excluding `docs/`, ignored `build/`, and
historical records deliberately outside the active source/build corpus:

```powershell
git ls-files src/lib
rg -l 'lib/base/|lib/ux-base/|lib/ux-window/|lib/ux-console/' \
  src/lib src/app src/host test CMakeLists.txt
rg -l 'base-console|host-sync|storage-medium|ux-contract|ux-native' \
  src/lib src/app src/host test CMakeLists.txt
rg -l '\bux_[A-Za-z0-9_]+' src/lib src/app src/host test CMakeLists.txt
```

| Existing component directory | Tracked paths | T51 disposition |
| --- | ---: | --- |
| `base/` | 6 | Split: scalar/status headers move to `types/`; logical Console implementation and public contract move to `console/`. |
| `host/` | 13 | Retain directory; rewrite includes and target from `base-console`/`host-sync` to `types`/`console`/`host`. |
| `storage/` | 7 | Retain directory; rewrite base include and target to `types`/`storage`. |
| `ux-base/` | 24 | `git mv` to `ui-base/`; rename its public `ux_*` ABI to `ui_*`. |
| `ux-window/` | 11 | `git mv` to `ui-window/`; rename public and internal UX identifiers to `ui_window`/`ui_*`. |
| `ux-console/` | 6 | `git mv` to `ui-console/`; rename public and internal UX identifiers to `ui_console`/`ui_*`, retaining the neutral `lib_console` API supplied by `console/`. |

The 72-path library corpus has no external source tree, alternate implementation,
or MVDM path in its migration universe. `MANIFEST.sha256` is a derived exact
corpus record and must be regenerated after the atomic move.

## Existing target graph and target disposition

| Existing target | T51 disposition |
| --- | --- |
| `base-console` | Replace with `types` (header-only root ABI) and `console` (logical Console implementation). |
| `host-sync` | Replace with component target `host`. |
| `storage-medium` | Replace with component target `storage`. |
| `ux-base` | Rename to `ui-base`. |
| `ux-window` | Rename to `ui-window`. |
| `ux-console` | Rename to `ui-console`. |
| `ux`, `ux-contract`, `ux-native`, platform `ux-*-native` aliases | Remove. They are obsolete aggregates/aliases, not components or public presenter APIs. |
| `lib-neutral-corpus` | Retain only as the manifest/standalone-build aggregate, rewritten to list the six normalized components. |

The final library-only DAG is:

```text
types
  ├─ console ──> host, ui-console
  ├─ storage
  └─ ui-base ──> ui-window, ui-console
```

No reverse or peer edge is permitted. In particular `host`/`storage` may not
depend on any `ui-*` component; `ui-window` may not depend on `console` or
`host`; and `ui-console` may not depend on `host`.

## External consumer boundary

The application is not a member of the library DAG. It consumes public
interfaces directly, as it must coordinate the independently owned components:

| Consumer class | Current paths | T51 disposition |
| --- | --- | --- |
| SoftPC app public-interface consumers | `control.h`, `input_queue.h`, `keyboard.h`, `main.c`, `monitor.c`, `monitor.h`, `presentation.c`, `prompt_trace.c`, `runtime.c`, `runtime.h` | Rewrite public include paths and `ux_*` ABI spellings. The app intentionally consumes `console` and `ui-base` copied values, plus host/storage/UI-leaf contracts. |
| Standalone-host consumers | `audio.c`, `gfi_image.c`, `hdd_media.c`, `machine.c`, `platform.c` | Rewrite only public include paths/target names; no host behavior change. |
| Unit/diagnostic consumers | `host_console_broker_smoke.c`, `lib_console_event_gate_smoke.c`, `lib_console_smoke.c`, `ux_component_contract_smoke.c`, `ux_console_retirement_barrier_smoke.c`, `ux_leaf_control_capacity_smoke.c`, `win32_keyboard_smoke.c`, `win32_presentation_smoke.c`, `win32_window_bounds_smoke.c`, `runtime_boot_smoke.c` | Rename includes/types and preserve the same assertions. |
| Build/static gates | root `CMakeLists.txt`, `src/lib/CMakeLists.txt`, `src/lib/verify_linux_build_contract.cmake`, `test/support/standalone_source_boundary.cmake` | Rewrite target/source paths; replace the existing old split-UX graph regex with the final graph above and add zero-old-name assertions. |

There is no application requirement to hide public `ui-base` values. The
restriction is only that no *library component* other than `ui-window` and
`ui-console` may depend on it. The application must continue to consume copied
`ui_input_event`, `ui_frame`, and registered-hotkey ABI without receiving any
private mailbox/component implementation header.

## S1 conclusion

The migration has one complete, finite, behavior-preserving route. No cycle,
non-SoftPC source consumer, public binary-compatibility promise, or MVDM
touchpoint was found. S2 may perform the atomic library move; S3 must update
the enumerated external consumers and static gates. Any path newly introduced
before S2/S3 closure joins this ledger and receives the same disposition.
