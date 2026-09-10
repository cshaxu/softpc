# Source And Research Policy

## Scope And Authority

This supporting policy operationalizes the source/research boundary summarized
by [Architecture Rules](../../../rules/ARCHITECTURE.md). It records procedure
and evidence requirements; it does not grant a copyright license, make a
distribution promise, or create a runtime dependency.

## Repository Source Categories

- `src/mvdm/softpc.new/` is the preserved selected SoftPC source mirror. Its
  historic provenance and retained-diff evidence live in the M8 history and
  indexed evidence records. It is not a place for standalone behavior or an
  unreviewed external copy; changes require an admitted mirror, port-ABI, or
  equivalent owner-approved boundary task.
- `src/{app,host,lib}/` is repository-maintained standalone code. `src/lib/`
  is the canonical shared corpus and is imported/adopted only through a task
  that records the reviewed upstream revision, source/destination surface,
  semantic changes, and verification.
- `O:\repos.external\opennt-src-2` and `O:\repos.hobby\nxvm` are read-only
  comparison material. They are never build inputs, runtime dependencies, or
  a source of unrecorded copy/paste imports.
- Existing in-tree copyright and historical notice text is evidence, not a
  statement that this repository owns or may relicense that material.

## External Source Admission

No external source, patch payload, generated source, or translated code enters
this repository without a separately admitted task. Before the first import,
that task records the upstream identity and immutable revision, origin and
destination paths, applicable notices/license terms, owner authorization,
intended boundary, semantic changes, and focused verification. If any item is
unknown, conflicting, or independently licensed, stop: retain only a research
reference and request owner/legal direction.

Independent project code may guide behavior only through project-owned tests,
experiments, or neutral requirements. It is not copied, transliterated, or
made a dependency merely because it was useful as a reference.

## Research And Documentation

Research material is supporting evidence, never an architecture authority.
Hardware manuals, reports, scans, OCR, and local research binaries remain in
an owner-managed external archive. A repository record may cite its source,
edition or identity, acquisition context, hash, and the neutral conclusion
used by a project-owned test; it must not commit proprietary material or turn
a research lead into a product requirement without an admitted design task.

## Legacy Material Boundary

The repository already tracks historical SoftPC mirror ROM inputs and guest
media images. Their presence is a provenance fact, not a legal conclusion or
an authorization to copy, replace, catalogue, download, or redistribute them.
They remain fixed legacy inputs unless an owner/legal review records their
source, rights, intended package status, and verification in a dedicated task.

## Binaries, Firmware, And Media

No new Microsoft binary, third-party firmware, external guest medium, or
generated trace enters the repository, default package, release assets, or
runtime dependency set without that dedicated review. A future
bring-your-own-binary or firmware profile requires an owner-approved design
and legal/provenance review before code, catalogue, hash list, acquisition
mechanism, replacement asset, or package behavior is added.

The checked-in package may use only its existing fixed executable artifacts,
user-owned `assets/binary/softpc.ini`, the existing `assets/media/` inputs, and
the established embedded ROM mirror inputs. This statement does not authorize
redistribution beyond the rights actually established by the owner.

## License Decision Boundary

This repository currently has no root license file. No contributor or task may
infer an MIT, OpenNT, Microsoft, Insignia, NXVM, or other distribution grant
from repository presence, historical provenance, or a comparison checkout.
Adding a root license, changing notices, publishing a redistribution claim, or
accepting a new third-party license requires explicit owner/legal direction in
a dedicated task.
