# M9 T47 S7 — Shared-lib product identity sweep

## Outcome

The shared Window no longer exposes an importing product through its Win32
class or its first user-visible title:

- native class registration uses `LibUxWindow`;
- `ux_window_options.initial_title` is copied before native startup and is the
  only initial Window title; SoftPC supplies its own stateful product title;
- the shared manifest revision is neutral, and all shared prose uses
  application-neutral wording.

The standalone CMake project name `nxvm_shared_library` remains by explicit
owner exemption. It is the sole permitted NXVM identity in `src/lib`.

## Evidence

- The source-boundary test rejects `SoftPC`, `Insignia`, and `NXVM` across
  shared production source and README, excluding the admitted CMake project
  identity. It also proves the copied initial-title path remains present.
- The shared-library manifest verifies, including the refreshed neutral
  revision and corpus hashes.
- Fresh x64 and x86 package builds each passed full CTest, 33/33.
- Documentation governance verification passed.

## Closure

This changes neither MVDM nor machine behavior. It adds no product callback or
policy to lib: title identity belongs to the application at creation and via
the existing later-title API. T47 remains active; any subsequent subtask needs
separate owner admission.
