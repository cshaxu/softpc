# M9 T81 S2: Neutral Lib Audio Contract

## Request And Boundary

Owner approved automatic execution through T81 S5 after S1. S2 establishes
only the reusable Lib Audio contract, deterministic validation proof and its
shared build topology. It does not produce audio, alter SoftPC/Core/Common/x86
or add a producer thread, software ring or guest-device route.

The executor delivery is `fbfa9437`, pushed to `origin/main` before this
coordinator review. The retained plan is
[the retained proposal](M9-T81-neutral-audio-stream-proposal.md).

## Actual-Change Review

`git show --check fbfa9437` is clean. Review finds one public opaque stream
and one private platform contract. The root validates only the admitted PCM
formats and bounded calls, owns the generic active/failure state, and delegates
platform ownership without copying product semantics. The two selected platform
leaves deliberately return `UNSUPPORTED`; this is an incomplete endpoint, not
a silent playback success. The deterministic test substitutes that private
endpoint and proves validation, zero/short submission, active/clear behavior,
failure latching and retry-safe destruction.

| Requirement | S2 disposition |
| --- | --- |
| Neutral `src/lib/audio`, `test/lib/audio` placement | Added Audio root and shared test; no product path changes |
| Typed PCM contract | Opaque stream, copied `lib_i16` interleaved PCM and bounded 512-frame calls |
| No hidden queue/thread | No worker, callback, software ring or retry route exists |
| Platform boundary | Root owns policy; same private platform shape has Win32/Linux placeholders |
| Accurate unsupported result | Create fails `UNSUPPORTED`; no samples are accepted or claimed played |
| Exact shared corpus integration | CMake, component graph, README, C11 manifests and documentation updated |

The S2 changed production C/H count is +259/-0 (net +259): five new Audio
files plus the scalar alias. Test C is +121/-0. Build/manifest integration is
+129/-114; documentation including the restored active proposal is +55/-15.
The binary rebuilds are byte-identical in size and carry no Audio linkage yet.
These counts are from `git diff --numstat fbfa9437^ fbfa9437`, with generated
manifest lines reported separately from source.

## Verification

- Strict isolated `test/lib` C11 background suites: x64 42/42 (52.72 s), x86
  42/42 (54.45 s), including `library.audio_stream`.
- Focused embedded Audio, Types and manifest tests pass on both widths.
- Product background regression: x64 112/112 (147.82 s); x86 followed the
  same 112-item route, with its final 12/12 independently rerun after a tool
  session ended while silent. No desktop-labelled test was run.
- Both Release package targets built and refreshed
  [softpc64.exe](/O:/repos.hobby/softpc/assets/binary/softpc64.exe) and
  [softpc32.exe](/O:/repos.hobby/softpc/assets/binary/softpc32.exe).
- Lib/test manifests, Types layout, component DAG, `git diff --check` and
  documentation governance pass.

## Similar-Issue Sweep

The searched S2 boundary was `src/lib/{audio,types}` plus the component-DAG
map and all Lib source include/link forms. Audio is the only new platform
consumer; it has no native SDK include, raw external identifier, unbounded
queue or allocation outside Types wrappers. Existing components' platform
mechanics are outside this admitted construction scope. Native buffer cleanup,
WinMM declarations and any repeated native slot state are explicitly received
by S3; guest speaker adaptation is S5.

S2 exit criteria are satisfied and it is closed. T81 remains open; S3 is
automatically active under the owner's prior authorization.
