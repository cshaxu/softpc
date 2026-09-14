# M9 T56 S16 — shared tests and first-start correction

Owner requested Common/Lib tests in test/common and test/lib, with the four
source/test trees transferable unchanged. Implementation and detailed evidence
remain in the [Common proposal](../proposals/m9-common-corpus-convergence.md).
The delivered private lib_size cleanup and physical-key identity regression
accompany the test migration; original assertions were preserved.

Initial delivery 09d8c90 / 20ca47f failed owner first-start testing. The follow-up
8179def / 662ed4b corrected Host Win32 viewport/frame-capacity ordering, with
native narrow/short/scrolled Console regressions and compact package startup.
It did not weaken clipped-write failure handling or change VM lifecycle.
Final product suites passed x86/x64 80/80, strict standalone Lib 37/37;
shared Common isolation passed 14/14. Compact package startup passed three
consecutive times per width. Actual-diff coordinator review is recorded in
662ed4b and the proposal. Horizontal-scrollback metadata and intermittent
package timing observations remain explicit TODOs, not claims of repair.

Owner confirmed: “测试通过；nxvm项目正在adopt，等我反馈再看是继续S任务修复还是收口。”
Owner subsequently admitted Common-only S17. S16 is accepted and closed;
T56 and its suspended S13 whole-task audit remain open.

Migration count against 4cf8629: production two C files +6/-3; 43 file moves,
one new 68-line physical-key test, CMake net -106. Corrective count against
20ca47f: one production C file +9/-7; two test files +79/-4; root CMake +3/-1.
INI/media were not agent-modified. Final EXE SHA256:

- x86: 4BC701D606BA8B21C0087B7CE3E9F1A411F1F8C5E014C2EE71636F4BC4329AC4
- x64: 3162EFAF23D0E5D55924B4A3CBB587790184070473A1E3C41EDD5842DBE81986
