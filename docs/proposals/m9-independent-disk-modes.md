# Independent floppy and hard-disk media modes

## Product objective

The fixed floppy (`A:`) and hard disk (`C:`) are separate media objects. Each
must therefore have its own access policy:

- `readonly`: guest writes are rejected and never alter the image;
- `direct`: guest writes alter the image file; and
- `overlay`: guest writes go to that medium's volatile overlay.

The canonical startup configuration is:

```ini
floppy=...                  ; optional
floppy_mode=overlay         ; readonly | direct | overlay
hard_disk=...               ; optional
hard_disk_mode=direct       ; readonly | direct | overlay
```

Absent mode keys retain the safe `overlay` default independently. For source
configuration compatibility only, a legacy `media_mode` with neither dedicated
mode key remains accepted and initializes both modes to its value. Mixing
`media_mode` with either dedicated key is a configuration error: key order must
not silently select a policy. New documentation and generated examples use only
the two dedicated keys. This is parser compatibility, not a second runtime
media model.

Drive A replacement is explicit:

```text
floppy insert <readonly|direct|overlay> <image_file_path>
floppy eject
```

`insert` requires both the mode and a nonempty path. The path is copied exactly
as typed after tokenization; only the operation and mode tokens are
case-normalized. `eject` needs no mode because it leaves no mounted medium.
Insertion/ejection remain available only while the existing machine policy
permits removable-media change (stopped or paused); a running request fails
without changing the mounted image. A failed replacement keeps the old floppy
mounted and usable.

## First-principles ownership

`lib/storage` already owns the three-mode medium abstraction and needs no
change. `common/machine` owns serialization of a removable-media request but
does not interpret a policy; it forwards the explicit `lib_storage_medium_mode`
value to its product driver. `vm` owns the mutable fixed-machine configuration
and passes the two startup modes and an accepted replacement mode to the
machine adapter. `compat` owns the original GFI/FDC and fixed-disk host-media
endpoints, including staged replacement and snapshot receivers. `mvdm` remains
the preserved controller source and is unchanged.

There is one mode per live medium, not a global mode plus overrides. The VM
stores `floppy_mode` and `hard_disk_mode`; the current floppy mode changes only
after a successful replacement. With no floppy mounted its retained mode has no
behavioral effect, and the next insert still must name a mode.

The Common driver/API changes from a path-only removable-media operation to:

```c
lib_bool (*set_removable_media)(void *context, const char *path,
    lib_storage_medium_mode mode);
lib_bool common_machine_set_removable_media(common_machine *machine,
    const char *path, lib_storage_medium_mode mode);
```

Common only validates that the enum is in the lib-defined range and forwards
it on the existing serialized worker path. It does not open media, infer modes,
or learn floppy/controller behavior. `path == NULL` is eject; its mode is
ignored by the product driver but is passed as a valid enum so the public call
shape stays singular.

`compat/gfi_image` must prepare a candidate drive (open, size and geometry
validation) before retiring the active drive. Only a fully prepared candidate
replaces the old medium and installs the original GFI callbacks. Eject is the
same commit path with an explicit empty candidate. This prevents a bad path or
bad geometry from turning a failed insert into an unintended eject.

## Snapshot contract

The existing archive already records each present slot's mode. Its prepare
operation currently validates every slot against one global mode; it must
instead receive the current floppy and hard-disk modes and validate each slot
against its own configured/live counterpart. No snapshot format field, version,
magic, archive ordering, Lib API or MVDM controller code changes.

Consequences remain direct and auditable:

- a `readonly` base is fingerprinted and remains write-protected after restore;
- a `direct` base is the same external file and is verified rather than copied;
- an `overlay` slot restores its staged overlay pages and floppy cylinder state;
- an ejected floppy has no slot and does not constrain the retained idle mode.

## Finite implementation ledger

| Candidate | Disposition required for completion |
| --- | --- |
| Global `media_mode` in app/vm/machine startup | Replace with independent values; retain only order-independent parser compatibility for legacy configuration. |
| Path-only removable-media Common/VM call | Extend its one existing request with explicit mode; do not create a second insertion path. |
| Destructive GFI replacement | Commit only after candidate preparation; preserve old medium on failure. |
| Snapshot global-mode validation | Split by floppy and hard-disk mode without changing archive bytes/order. |
| CLI/help/UI/README and configuration tests | Replace grammar and explain the two policies; no stale `floppy insert <image>` documentation remains. |
| Lib/MVDM | Retain unchanged. |

## Proposed staged delivery

### S1 — independent startup and snapshot mode plumbing

Replace the single App/VM/Machine startup mode with `floppy_mode` and
`hard_disk_mode`; attach each original backend with its own mode. Change media
archive preparation to validate the two modes by slot. Update startup, archive,
config and package parsing tests for mixed pairs, including floppy-overlay plus
hard-disk-direct and the legacy parser rule. No Common API or interactive
command grammar changes in this step.

Exit proof: mixed startup media opens correctly; snapshot capture/restore
accepts matching independent modes and rejects mismatches; both widths pass
focused and full regression and package builds.

### S2 — explicit transactional floppy insertion

Add the one explicit mode argument to the existing Common removable-media
request and its VM driver implementation. Make GFI replacement staged, make
the CLI require `floppy insert <mode> <path>`, and update help/UI/README and
command/runtime tests. Cover all three modes, missing/unknown mode, preserved
path case, running-state rejection, eject, and failed replacement retaining
the old image.

Exit proof: no path-only insertion symbol or documentation remains; the Common
request has one route; old-medium retention and all mode combinations pass
focused tests, then x86/x64 full regression and package builds.

## Constraints and acceptance

- No Lib or preserved MVDM source changes.
- No change to snapshot binary layout, CPU/device timing, display, input or
  lifecycle semantics.
- `assets/binary/softpc.ini` remains owner-owned and is never edited by this
  task; its legacy key remains runnable through the explicit parser rule.
- Each admitted S reports actual production/test and protected-mirror numstat,
  runs both widths, refreshes only package EXEs, commits and pushes before the
  next S.
- Final manual acceptance: start with different floppy/hard-disk policies;
  insert and eject each requested floppy mode while paused/stopped; verify
  failed insertion retains the previous disk; save/load an overlay floppy plus
  a differently-modeled hard disk; confirm no source image changes under
  `readonly`/`overlay` and direct changes persist only for `direct`.
