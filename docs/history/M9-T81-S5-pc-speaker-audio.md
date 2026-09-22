# M9 T81 S5: PC Speaker through Lib Audio

## Boundary

S5 replaces only the standalone Compat presentation endpoint. The preserved
`src/core/softpc.new/host/src/nt_sound.c` remains unchanged: it still owns
Timer2/PPI state and emits the same frequency/stop requests. Compat retains
one worker because a sustained tone needs a product-time source; Lib remains
only the bounded PCM/native-output owner.

The worker opens one 48 kHz mono Audio stream at platform start. While the
original device has requested a valid frequency, it synthesizes a 512-frame
signed-16-bit square-wave block, submits it, and waits up to 5 ms for either a
new device request or native capacity. A full native stream is ordinary
backpressure; native failure clears the request and takes the existing single
clear exit. Gate-off, invalid requests and worker wake-to-idle clear unsounded
PCM. Shutdown joins the worker first, then destroys Audio; a native cleanup
failure retains the stream for the existing retry path.

No direct `Beep`, WinMM identifier or SDK include remains in Compat. Audio is
linked only by `softpc-machine`, which is the owner of the Compat object
files. There is no new guest route, Common interface, Lib API, second worker,
software queue, resampler, mixer, configuration field or snapshot effect.

## Actual change and proof

`git diff --numstat 85457912` gives production C/H +60/-6 and root CMake
+1/-1: production/build +61/-7 (net +54). The existing injected Core audio
failure test is rewritten, not duplicated: +59/-52 (net +7). Total tracked
text is +120/-59 (net +61), excluding two rebuilt EXEs. This is below the
planned production +100..180/-40..90 because the old worker, events and
ownership shape were reused rather than wrapping them in another endpoint.

The deterministic test now proves one complete PCM block is submitted for an
active tone and that a submission I/O failure clears exactly once before the
worker returns to its existing wait exit. Existing audio lifecycle and original
sound-state tests pass on both widths, retaining the original Timer2/PPI
request semantics. `git diff --check` passes; changed-path review confirms no
file under `core/softpc.new` changed.

- x64 targeted Audio/state/failure: 3/3 pass.
- x86 targeted Audio/state/failure: 3/3 pass.
- x64 background: 113/114 on first run; the only failure was the pre-existing
  `softpc-runtime-restart-boot-smoke` prompt roundtrip timeout. Its immediate
  isolated rerun passes in 60.17 s. All other selected tests passed.
- x86 background: 114/114 pass (151.74 s). Desktop-labelled tests are excluded.

| Package | SHA256 |
| --- | --- |
| [softpc32.exe](/O:/repos.hobby/softpc/assets/binary/softpc32.exe) | 8C4310DC282BFD59FD0A05BF74DD7B855A742F526BD84F9B598BF0BBFD542F4F |
| [softpc64.exe](/O:/repos.hobby/softpc/assets/binary/softpc64.exe) | 3DC205F11FD4F8FC6A9B42D1413A0F1AB96C0A1FF8FA39ECA3D92DD562AAD6A3 |

S5 implementation is ready for the owner's bounded audible DOS/Win3.x PC
Speaker check. T81 remains open until that acceptance and its separate
whole-task audit.
