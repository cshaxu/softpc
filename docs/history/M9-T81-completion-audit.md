# M9 T81: neutral audio stream closure

## Scope and result

T81 introduced a reusable, neutral PCM stream in `src/lib/audio` and
`test/lib/audio`, then connected SoftPC's existing PC-speaker presentation
boundary without adding a guest sound card. The retained
[proposal](M9-T81-neutral-audio-stream-proposal.md) records the approved S1--S8
scope, revisions, and finite acceptance ledger.

The final owner acceptance was a cold DOS boot followed by the first invocation
of `AUDIO.COM`: sound is now audible on that first invocation. This closes the
last T81 criterion. Linux remains an explicit unsupported Audio leaf; Win3.x
and Win95 sound-card emulation, DMA, IRQ, OPL and MIDI were not added.

## Final causal repair

The actual remaining first-tone defect was below Lib Audio. `timer_post()`
started Timer2 high while PPI reset bookkeeping recorded the gate low. The first
guest PPI rise was consequently treated as a gate loss and replaced the 439 Hz
waveform with silence. `ppi_init()` also reset the port register without
resetting that bookkeeping bit, producing the inverse mismatch after reset.

The accepted repair starts Timer2 low and resets the PPI gate-memory bit low.
It also removes the obsolete Compat Timer2 hook: the original PPI
`HostPpiState()` transition is the only PC-speaker presentation path. No Audio
public API, Common state, product polling, second producer, second queue, or
guest timing workaround is retained.

## Changed-path accounting

Compared with the admitted baseline `2b17749a` and endpoint commit `9aa53de9`,
using `git diff --numstat 2b17749a..9aa53de9 -- ':!docs/**' ':!assets/**'`:

| Category | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production C and headers | 16 | 1,119 | 89 | +1,030 |
| Test C | 8 | 1,193 | 68 | +1,125 |
| Build/check CMake | 5 | 40 | 5 | +35 |
| Total code/build/tests | 29 | 2,352 | 162 | +2,190 |

Most additions establish the previously absent reusable Audio component,
deterministic fake/native endpoint proofs and Core speaker handoff coverage.
The final first-tone repair itself is narrow: `timer.c` +1/-1, `ppi.c` +9/-6,
and obsolete Compat/mirror hooks -32 lines overall. The tracked Win3.1 image
updated by this closure is owner-approved media, excluded from code counts.

## Verification

- Full background CTest: x64 **115/115**, x86 **115/115**; desktop-interactive
  tests remain excluded from those background routes.
- Native Audio endpoint first/reuse smoke passed on both widths.
- Focused Core sound tests use the exact `AUDIO.COM` PIT/PPI program order and
  prove non-silent PCM on first boot, after reset, and through a continuous
  executor slice on both widths.
- Documentation governance and `git diff --check` pass for this closure.

Accepted package hashes from `9aa53de9`:

| Package | SHA256 |
| --- | --- |
| `softpc32.exe` | `1585D6EE8C7C0D27355270377FF18307C8538136DA7A1419B5CCFFC19AC0AEDD` |
| `softpc64.exe` | `7230BDF73DFB09F55F8A2F2EB39C56A81593BC38EE444B9A22C4C7B75E5AA17A` |

## Closure disposition

All eight admitted S scopes have a recorded result in the archived proposal or
their S evidence. The owner accepted the only manual audible criterion. No
T81 debt transfers to TODO or Queue; the unrelated XP mirror rebase remains
the sole unadmitted Queue candidate. T81 is closed.
