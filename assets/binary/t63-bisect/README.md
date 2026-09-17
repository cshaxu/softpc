# T63 regression bisect packages

Each pair is the exact committed package from the end of the named T63 stage;
it was extracted from Git rather than rebuilt. `softpc.ini` is local to this
directory and resolves the installed image in `assets/media/`.

Test one architecture at a time, starting with `s0-pre-t63`, then advance
through `s1` to `s8`. Record the first version that begins flashing when the
same `win` sequence is used.

| File suffix | Source commit |
| --- | --- |
| `s0-pre-t63` | `54b2009` |
| `s1` | `7216429` |
| `s2` | `d4dda9d` |
| `s3` | `29018c3` |
| `s4` | `9f7bcbf` |
| `s5` | `cdd0a12` |
| `s6` | `f6e2e82` |
| `s7` | `8647824` |
| `s8` | `1727ee1` |
