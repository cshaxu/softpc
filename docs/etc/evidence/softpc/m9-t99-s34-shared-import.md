# T99 S34 shared import qualification

## Frozen source and scope

Source: read-only NXVM commit `09ebe521e7fede9c6e5985efadf1acbad4ecfc8e`.
Its eight shared trees equal the original audit at `7794e25f9b1e38aa24d08b2d81ef703480a86878`.
Destination baseline: SoftPC `1ec0d551d137fd151b1f7c14097f589d7a57c251`.
The finite universe is all files in the following eight trees, including
manifests and notices. No file was added or removed. Comparison reads each
fixed Git blob and checks exact destination bytes and the complete path set.
Completion predicate: all 296 members match; no unmatched path is acceptable.

| Tree | Files | Aggregate SHA-256 |
| --- | ---: | --- |
| `src/lib` | 123 | `b8b028e3379275041b0b9bc71579c3221e510eb742230f094c6e94ffb173295b` |
| `test/lib` | 78 | `dfdd202a95b12702631c2e718aa5583c72cc6298c6ec6630abbbd002f991fae6` |
| `src/emulator` | 28 | `06fb13039b7ecf60427e91405e1c61795859b03830d2832580901ab293d048b9` |
| `test/emulator` | 23 | `eb494469e41e1f80b37c0b89c8acaca8c9c7e41b9d229a349f3c4da79ea74e17` |
| `src/x86-debug` | 15 | `95f69cd70a07b417bde0111435aaab0d9bb73267371f1f888f03cacb3c6bf405` |
| `test/x86-debug` | 9 | `3c22a0a78e462c7ec30ab36d316dccff16458431ab928b7c4aec425baadd3c3a` |
| `src/x86-ibmpc` | 13 | `3d904417144db17270ae7756e55fcfde608c8d634c2e12b692480d7ae2b900cc` |
| `test/x86-ibmpc` | 7 | `3e1b153e802a7d12597bdf0917443bd207d772c9a1d67bcad97095bf4434361f` |

Aggregate hashes use sorted Git paths, each followed by a space and its lowercase
file SHA-256, newline terminated, encoded UTF-8. Existing source notices are retained.
There is no build/runtime dependency on NXVM.

## Actual change and review

Lib changes: an additive neutral `BASE_KEY_BREAK`, its one-to-one Types
`VK_CANCEL` wrapper, the existing Win32 decoder case, one test table row, and
two manifest refreshes. Other shared trees are unchanged. No old key value or
structure layout changes, no new state or input path. Linux behavior is unchanged;
this import does not claim new Linux Break decoding.

The sole private production adjustment adds Break to SoftPC's existing
neutral-key/Win32 guest-protocol bridge. Without it, the newly decoded key
would return zero and be dropped. Original `nt_keycd.c` already maps enhanced
scan 0x46 to key number 126; no mirror change is needed. Existing machine smoke
now sends Break make and break through the real adapter after its keyboard
boot check. Native decoder coverage checks scan, E0 identity, repetition and
both transitions through its existing table loop. This is not a claim of a
new end-to-end physical Ctrl-Break guest test.

Similar-issue sweep: inspect all `BASE_KEY_*` consumers, private mapping ranges,
`vm_keyboard_inject_machine_event`, `KeyMsgToKeyCode`, and the original key
number sink. The new key is outside all function/numpad ranges and needs only
the one explicit private mapping. No public aliases, outer-layer dependency,
second translator, or test-only production entry is introduced.

Code counts relative to the destination baseline, excluding manifests/docs/assets:
production +5/-0 (four shared, one App); tests +12/-0 (one shared, eleven App).
Manifest hash lines: +4/-4. No mirror diff, CMake or root test infrastructure change.

## Owner assets

Owner explicitly authorized inclusion of the modified `assets/binary/snapshot`
and deletion of `assets/binary/snapshot-midi`. The snapshot is not rewritten by
this task: 18,648,401 bytes, SHA-256
`cb09a0af514f82dddbb57bec0133f6d7a5a28b92a8730dbf282c3d39a91268fa`.
INI, disks and other snapshots remain untouched.

## Verification

Hidden background cached Ninja/ccache builds completed for x64 and x86;
each rebuilt 264 actions, preserving reusable object trees. Both EXEs published.
Each build reported 89 existing mirror-header warnings, zero non-mirror warnings
and zero errors. No mirror warning suppression or unrelated cleanup was added.

- x64 background: initial 185/186; documentation governance failed because the
  packet rewrite omitted required Recent Closures/Governance headings. Restored
  the headings; dedicated CTest rerun passed 1/1. No production/test fix needed.
- x86 background: 186/186 passed in 81.78 seconds.
- x64 native desktop/audio/package: 8/8 passed in 22.41 seconds.
- x86 native desktop/audio/package: 8/8 passed in 22.14 seconds.
- Final qualified set: all 388 distinct architecture/test cases pass, with the
  documented x64 governance rerun. Do not describe the first run as all green.
- Both machine input smokes, native keyboard decoder tests and original keycode
  table checks passed as part of those suites.
- All eight final trees match the pinned source; upstream HEAD still matches.

Published SHA-256:

| Artifact | SHA-256 |
| --- | --- |
| assets/binary/softpc32.exe | `02fd4994b44a64d733200faa569202e0f7680ad21ecf730d6373f0db903e68cf` |
| assets/binary/softpc64.exe | `f7f9811010cf7628abbf609db825ca745cd9d3822025bd9cd77f9aae27a919bb` |

Executor and coordinator actual-diff review: inspected every changed source,
test and manifest against the owner request. Public code remains unchanged
from upstream; the private mapping is necessary to preserve Break identity at
the existing guest-protocol boundary. No mirror/INI/media edits, additional
state, framework, dependency reversal or duplicate input path. The regression
uses the actual adapter and existing fixture rather than a test-only API.
Snapshot hash remains the owner's supplied bytes. Native tests were run on the
active local desktop, not a claim of separate RDP/manual guest validation.

Raw logs and launch scripts were task-owned build/t99-s34 scratch; after this
summary they are disposable. Final publication awaits owner runtime acceptance;
T99 remains open.
