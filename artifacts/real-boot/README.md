# Standalone SoftPC real-ROM boot evidence

These images are local verification artifacts and are deliberately not tracked.
They are copies of existing local media, made before running the detached
SoftPC real-ROM boot probe so the shared media is never mounted writable.

## 2026-08-30

| Path | Source | SHA-256 | Probe |
| --- | --- | --- | --- |
| `msdos622_en_boot.img` | `O:\\assets\\msdos\\msdos622_en_boot.img` | `1AB300A0A54B8F384CC457424EA0D2F3F46BEF11C0172429C6B207B2EC539E6E` | `build-incremental\\softpc-real-boot-smoke.exe --floppy artifacts\\real-boot\\msdos622_en_boot.img` |
| `hdd-msdos500.img` | `O:\\assets\\hdd-msdos500.img` | `F4D1E81BC410BB9A7558667B7C3741A9664E84077A3774E73104CD24B631D688` | `build-incremental\\softpc-real-boot-smoke.exe --hdd artifacts\\real-boot\\hdd-msdos500.img` |

Both probes exited successfully. Their acceptance condition is printable text
in guest text video memory after the original firmware boot path has run.
