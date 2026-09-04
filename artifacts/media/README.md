# Standalone SoftPC real-ROM boot evidence

These are local, deliberately untracked boot-media copies. They are not build
output: keep reusable firmware and guest media here, while generated traces and
temporary test copies belong in `build/artifacts/`.

## 2026-08-30

| Path | Source | SHA-256 | Probe |
| --- | --- | --- | --- |
| `msdos622_en_boot.img` | `O:\\assets\\msdos\\msdos622_en_boot.img` | `1AB300A0A54B8F384CC457424EA0D2F3F46BEF11C0172429C6B207B2EC539E6E` | `build\\softpc-real-boot-smoke.exe --floppy artifacts\\real-boot\\msdos622_en_boot.img` |
| `hdd-msdos500.img` | `O:\\assets\\hdd-msdos500.img` | `F4D1E81BC410BB9A7558667B7C3741A9664E84077A3774E73104CD24B631D688` | `build\\softpc-real-boot-smoke.exe --hdd artifacts\\real-boot\\hdd-msdos500.img` |
| `softpc-win31-clean.img` | `O:\\assets\\softpc-win31-clean.img` | `756AC620C6A5B5A5F34EFB633CDB086F3B3420E92E7E099EBE707919504945B2` | Clean Win31 HDD baseline; use directly only when intentionally changing it, otherwise use an overlay. |

Both probes exited successfully. Their acceptance condition is printable text
in guest text video memory after the original firmware boot path has run.
