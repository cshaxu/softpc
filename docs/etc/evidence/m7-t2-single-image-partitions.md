# M7 T2 Single-Image Partition Proof

## Fixture

`softpc-partition-image-smoke` creates one temporary raw hard-disk image with:

- an MBR boot sector;
- a primary-partition record beginning at LBA 1;
- an extended-partition record beginning at LBA 17; and
- an EBR at LBA 17 that describes a logical volume beginning at LBA 18.

The MBR boot program uses original INT 13h to read LBA 1 and LBA 18 through
the fixed-disk CHS path. It writes their distinct markers into guest RAM. The
test then verifies those markers and reopens the host image to confirm its MBR
and EBR bytes remain intact in overlay mode.

## Meaning

The standalone host attaches the complete image as one physical disk. It does
not parse MBR/EBR records, assign DOS drive letters, or expose additional
devices for guest logical volumes. Partition interpretation remains entirely
with the guest OS and software, as it should for a VM disk backend.

## Verification

- x64 clean serial CTest: 19/19 passed.
- x86 clean serial CTest: 19/19 passed.
- The focused partition-image smoke passed on both widths.
