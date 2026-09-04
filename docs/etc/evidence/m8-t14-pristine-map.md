# M8 T14 Pristine-map Evidence

## Comparison Contract

The manifest compares this repository's complete `src/` file inventory with
the selected read-only OpenNT baseline:

- baseline tree: `nt/private/mvdm/softpc.new`;
- baseline repository revision: `5e4619ab61c2aa76151e03973cce340be2933e61`;
- current recovered-tree root: `src/core/softpc/`;
- current standalone roots: the remaining paths below `src/`;
- extension peers for the current XMS/allocator remnants: OpenNT's sibling
  `xms.486`, `suballoc`, and `inc` paths.

The generator records raw SHA-256 for both sides and a CRLF/terminal-newline
canonical hash for text comparison. A raw difference that has matching
canonical hashes is `normalized-identical-original`, not a source change.

## Reproduction

From the repository root, run:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass `
  -File scripts/generate_pristine_map.ps1 `
  -OriginalRoot O:\repos.external\opennt-src-2\nt\private\mvdm\softpc.new
```

This replaces only the tracked evidence file
[`m8-t14-pristine-map.tsv`](m8-t14-pristine-map.tsv). It does not modify the
OpenNT tree or any SoftPC machine source. Two consecutive runs produced the
same manifest SHA-256:

```text
d1b5230546e02ca3b9ecf67a39f4306ee22f42b7ff7d5962d6e88dafcf86a3b5
```

## Inventory Result

| Classification | Rows | Meaning |
| --- | ---: | --- |
| `identical-original` | 753 | Raw bytes match the selected original peer. |
| `normalized-identical-original` | 44 | Only CRLF/terminal-newline representation differs. |
| `direct-original-diff` | 60 | Same peer exists but canonical source differs. |
| `local-standalone` | 38 | No selected original peer; the row names its proposed owner. |
| `original-omitted` | 182 | Selected original path is not present in current `src/`; 146 are source/header inputs and 36 are resource/build inputs. |
| `excluded-binary-output` | 145 | Historical object/library/resource output; explicitly excluded from source migration. |

The TSV has 1,222 rows. Its `cmake_selection` column identifies 119 CCPU
manifest inputs, 34 C-VID glob inputs, 105 literal CMake references, 637
current paths not directly selected, and 327 original-only paths not selected.

## Direct-difference Review And Disposition

All 60 direct rows were enumerated with a per-file `git diff --no-index
--numstat` review. The manifest is the authoritative per-path review table;
this summary records the family-level result.

| Family | Direct rows | Proposed disposition | Reason for later task |
| --- | ---: | --- | --- |
| BIOS / firmware | 3 | `overlay` | T16 must make the minimal host/build adaptation patch-visible. |
| CCPU | 3 | `overlay` | Pointer-width and executor compatibility must remain reproducible. |
| C-VID | 8 | `overlay` | Generated-rule/GDP compatibility belongs in a visible overlay. |
| Controllers and machine support | 25 | `overlay` | Preserve original algorithms while exposing existing direct modifications. |
| Original `host/inc` and `host/src/nt_*` | 17 | `host` | T17 owns the standalone host implementation boundary. |
| XMS/allocator remnants | 4 | `remove-unselected` | They are not directly selected by CMake; T19 may remove them only after the source-route proof. |

The 60 direct rows divide into 39 `overlay`, 17 `host`, and 4
`remove-unselected` proposals. Thirty-one are directly selected by CMake;
the remaining 29 are headers or unselected historical inputs. No direct
difference is labelled pristine or left without a proposed disposition.

## Handoff To T15 And T16

T15 consumes the paths classified `retain-pristine`, `overlay`, and `host` to
create the mirror/patch composition harness. T16 evacuates every applied
`overlay` row from the recovered tree. Original-omitted rows are not a request
to import code: they are a classification checkpoint requiring a later
selection decision. Binary rows remain excluded from `src/`.
