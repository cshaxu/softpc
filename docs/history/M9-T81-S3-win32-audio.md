# M9 T81 S3: Win32 fixed-slot Audio endpoint

## Admission and boundary

The owner authorized automatic completion of T81 S2--S5. S3 starts from the
closed S2 contract (`543d6286`) and adds Windows playback only: one Audio-owned
`waveOut` handle with four prepared PCM slots. It does not change Common,
Core, App or x86; it adds no Lib worker, software ring, callback, polling
thread, guest device or Linux playback implementation.

The executor delivery is `d8830b53`, pushed to `origin/main`. The retained
design is [the neutral Audio proposal](M9-T81-neutral-audio-stream-proposal.md).

## Actual design and review

`src/lib/types/win32/audio.h` is the sole new SDK boundary. It exposes only
renamed WinMM declarations/types; `audio` alone links `winmm`. The Audio Win32
leaf owns the output handle, headers and four fixed 512-frame sample arrays
directly. A public call reclaims completed native headers, copies an accepted
prefix into a free slot, and submits it. There is no staging buffer or second
owner of caller samples.

The root stream policy retains only active and terminal-native-failure state.
The review corrected one contract bug: `LIMIT_EXCEEDED` means all slots are
temporarily occupied, not an I/O failure, so it must not latch the stream.
Open, prepare, write, reset, unprepare and close failures remain observable;
failed destruction retains resources for retry-safe cleanup. Linux preserves
the S2 unsupported platform leaf unchanged.

`git show --check d8830b53` is clean. The changed-path sweep found no SDK
include outside Types, no product linkage, no callback, no worker and no
duplicate Audio queue. Four native slots are the sole bounded buffering
mechanism.

## Actual change ledger

Counts are `git diff --numstat 543d6286..d8830b53`, excluding rebuilt EXE
binary byte counts.

| Area | Added | Removed | Net | Result |
| --- | ---: | ---: | ---: | --- |
| Production C/H | 177 | 15 | +162 | Fixed-slot Win32 endpoint, Types aliases and recoverable-full correction |
| Test C | 165 | 1 | +164 | Native silent smoke, injected native lifecycle failures and backpressure proof |
| Build/manifests/docs | 28 | 19 | +9 | Audio `winmm` ownership, targets, corpus manifests and contract wording |
| Total tracked text | 370 | 35 | +335 | S3 adds the required real native endpoint without a second queue/thread |

The original estimate was production +180..300/-0..30 and tests/build
+150..250/-0..15. Production is one line below because the existing root
contract was reused; the test/build total is higher only when generated
manifests and documentation are counted together, while test C itself is
within the estimate.

## Verification

- Injected platform test proves open/prepare/write/reset/unprepare/close
  failure ownership, reset retry, completed-slot reuse and no unsafe free.
- Native Windows silent stream smoke proves open, query, enqueue, clear and
  destroy without requiring audible/manual output.
- Strict isolated `test/lib` C11: x64 44/44 (64.02 s), x86 44/44 (47.87 s).
- Product background: x64 114/114 (212.80 s). x86 ran 1--107, then 108--119
  separately after removing an earlier stale test child; every selected item
  passed. Desktop-labelled tests were excluded.
- Component DAG, Lib/test manifests, Types layout, documentation governance
  and `git diff --check` pass. No Linux runtime, audible product output or NES
  integration claim is made.

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| [softpc32.exe](/O:/repos.hobby/softpc/assets/binary/softpc32.exe) | 3693615 | 7EB6969914802C16D073B30AE35E01F331DE84A6E7152271723A117C9F632E8A |
| [softpc64.exe](/O:/repos.hobby/softpc/assets/binary/softpc64.exe) | 3080018 | 4E4FCE8922991E362CB193CDE9DB0E38C8F4267E4A74A71D10635A17579B518B |

The files are rebuilt deliverables, but their current product link graph has
no Audio consumer until S5, so their byte sizes/hashes do not yet demonstrate
Audio playback.

S3 exit criteria are met and S3 is closed. T81 remains open; S4 is
automatically active under the owner's prior authorization.
