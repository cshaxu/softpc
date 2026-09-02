# M5 Delivered Launcher Configuration

## Defect

The fixed output configuration referred to `O:\assets\hdd.img`, which is not
present.  Both delivered launchers consequently failed before the monitor with
`invalid argument or media`.

## Resolution

The fixed configuration now names the available and verified pair:

```ini
floppy=O:\assets\fdd.img
hard_disk=O:\assets\hdd1.img
media_mode=overlay
```

The output configuration is synchronized for this delivery.  CMake continues
to create it only when absent, so a later local rebuild does not overwrite an
operator's existing fixed configuration.

## Verification

- `softpc64.exe` and `softpc32.exe`, run without arguments from
  `build/output`, each reached the monitor and accepted `exit` (exit code 0).
- The read-only overlay Windows Setup probe using this media pair reached
  `Welcome to Setup.` on x64 and x86.
- The image files were only opened for overlay-mode tests; neither was
  modified.
