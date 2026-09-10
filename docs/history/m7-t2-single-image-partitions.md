# M7 T2: Single-Image Partition Compatibility Proof

## Goal

Prove that one configured fixed-disk image is exposed as a whole physical
disk, so its MBR, primary partitions, extended partition and logical volumes
remain a guest OS concern rather than a VM drive-count feature.

## Dependency

M7 T1 must close first.

## Scope

- Verify raw sector offsets, geometry reporting, read/write modes and BIOS
  disk services against representative partition tables.
- Add bounded synthetic and real-media overlay evidence where useful.
- Keep exactly one configured fixed-disk image and one configured floppy.

## Exclusions

The host must not parse partitions, assign DOS letters, or expose a second
disk merely to provide multiple guest volumes.

## Exit Evidence

x64/x86 regression proves a partitioned image remains bootable and that host
media modes preserve the entire image contract.
