# T94 S3 - Base Input Ownership

Baseline: 7f64ec53. Owner explicitly closes S2 and admits this S on 2026-10-17.
T94 stays open. [CURRENT](../states/CURRENT.md) owns active status; the
[proposal ledger](../proposals/m9-win95-sound-blaster-recognition.md#s3---owner-admitted-public-input-ownership-repair)
maps the bounded requirements to verification.

## Original owner request

```text
请审计并修复 SoftPC 公共组件的输入定义归属问题，完成提交推送后，供 NXVM 原样导入。

先读取 AGENTS.md、产品 guide、CURRENT、当前 proposal 和适用的架构、编码、执行规范；按最新状态准入合适的 S。保留无关修改，不读取或修改 NXVM 仓库。

一、确认并落实所有权

1. lib/types
   - 只拥有基础类型、通用状态/常量及一对一的 CRT、SDK 声明包装。
   - 不拥有项目输入领域定义。
   - “多个组件使用”不是把领域定义放入 Types 的理由。
   - 保留现有合法 CRT/SDK/atomic 包装，不进行无关迁移。

2. lib/base
   - 拥有统一的中性按键编号、修饰键、扩展键标志、鼠标按钮及输入记录。
   - 拥有原生输入解码、键盘布局查询、UTF-16 代理对合并。
   - Console 与 KVM 均依赖 Base，不得因此让 Console 依赖 KVM。
   - 不拥有热键匹配、held-key 状态、组合键合成或 KVM 事件投递。

3. lib/kvm-base
   - 保留热键、held-key 生命周期、字符转按键组合、KVM 事件构造与投递。
   - 直接采用 Base 的唯一输入定义，不建立同义别名。
   - 不恢复第二套原生解码。

二、先复核，再做最小修复

A. 输入定义迁移
   - 将 src/lib/types/input_values.h 的输入领域定义迁入 Base。
   - 优先建立 base/input_interface.h，作为 Console/KVM 使用的中性输入合同。
   - 保留 input_adapter_interface.h 作为原生生产者内部适配合同。
   - 根据当前命名规范明确符号前缀，统一修改全部生产和测试调用者。
   - 不保留旧头文件、兼容别名或双定义。
   - 不改变数值、字段布局、扫描码含义或输入行为。

B. 删除热键同义词汇
   - hotkey_interface.h 中 KVM_HOTKEY_KEY_*、
     KVM_HOTKEY_MODIFIER_* 若只是 Base 输入定义的别名，应删除。
   - 热键匹配、注册、Window 修饰键查询及测试直接使用唯一输入定义。
   - Window 普通修饰键查询不得仅为这些常量依赖热键头。

C. 收紧结果码归属
   - 核查 kvm_keyboard_submit_input()。
   - 它接收已解码输入，只需表达投递成功/失败时，采用 lib_bool。
   - BASE_KEYBOARD_ACCEPTED/REJECTED/UNMAPPED 保留在原生解码及必要的原生适配入口。
   - 原生适配入口显式转换投递结果，不新增枚举或状态。
   - 保留 Window 仅对 UNMAPPED transition 请求字符翻译的行为。
   - 若实际调用者证明存在其他必要结果，先报告证据，不机械改接口。

D. 清理实际构建依赖
   - 核查原生解码迁入 Base 后，kvm-base 是否已无 user32 调用。
   - 无真实用途则删除 kvm-base 的 user32 链接。
   - Base、Window 等保留各自真实需要的原生依赖。
   - 不以聚合目标或传递链接掩盖缺失依赖。

E. 文档与防回归
   - 同步 Lib、Types、Base、KVM 的 README、接口注释及实际受影响的当前架构说明。
   - 明确 Base 输入合同与 KVM 策略边界。
   - 补 Types 所有权负例，阻止输入领域定义重新塞入 Types；
     不误伤合法基础定义和 CRT/SDK 包装。
   - 更新依赖声明和对应 manifest。
   - 测试分别归属 Base 解码、KVM 策略、Console/Window 接线，
     不复制测试专用生产入口。

三、验证和交付

实施前报告实际发现、拟改文件、接口变化和预计增删行数。

实施后：
- 扫描全部真实调用者，包括 App。
- 验证普通字符、Unicode 代理对、重复键、扩展键、左右修饰键、
  Pause/PrintScreen、热键消费和输入重置行为不变。
- x86/x64 构建并运行受影响测试、公共组件完整单元测试及必要接线回归。
- 按治理规范更新受影响产物。
- 核对 manifest、Types、依赖、命名和测试边界检查。
- 原生 MIDI/音频的测试分类与设备可用性分别报告；
  不得把移入 integration 解释为修复原生设备打开失败。
- 报告实际增删行数、唯一保留路径、删除的别名/旧路径及未执行验证。
- 完成实际 diff 审查、提交推送，给出可供 NXVM 导入的固定 commit，
  并列明公共八树之外需要同步的根测试基础设施变更。

不新增组件框架、恢复线程、全局输入状态或兼容层；
不改变产品热键、Console/Window 体验、固件、媒体或用户配置。
```

## Executor source audit

The neutral declarations move by Git relocation, with base_key/base_key_input,
BASE_KEY_* and BASE_MOUSE_BUTTON_* as the unique vocabulary. Normalizing only
the approved symbol/header-guard rename makes the old and new headers identical;
all numeric values, field types/order and semantics remain unchanged.
The native adapter remains internal and is not an application event API.

Apart from an ownership-comment correction, changed production C/H outside
four paths are mechanical substitutions.
The four exceptions are hotkey_interface.h (remove six aliases), input.c and
input_interface.h (boolean delivery and explicit native-result conversion),
and Window win32/input.c (consume Base rather than the hotkey header).
There is no native User32 operation in KVM Base. Its redundant link is removed;
Base, Window and Console broker retain their actual native libraries.

The complete src/test old-token scan leaves only intentional negative fixtures
and cleanup of the retired header in the reused test fixture. SDK declaration
wrappers, atomic support, original mirror, audio and root test infrastructure
are untouched. Console's existing Base dependency is sufficient: no KVM edge
is added. Production submission callers are Console and the existing KVM
native-record adapter; the latter keeps all three decode outcomes and the
Window UNMAPPED-only translation condition unchanged.

## Verification mapping

- Base Win32/Linux decode tests: characters, surrogate pairs/malformed prefixes,
  repeats, extended identity, modifier sides, Pause and Print Screen.
- KVM input_delivery: existing physical/text/chord behavior, plus explicit
  null/sink rejection and native accepted/rejected/unmapped conversion.
- KVM hotkey/lifetime tests: shared held ledger, repeat/release and consumption.
- Window/Console lifetime and admission tests: real message-handler paths with
  native fixtures, translation, input reset and rejected delivery.
- Emulator physical identity and App input/snapshot/restart regressions:
  complete consumers use the renamed contract.
- Types layout negatives reject input-domain values/types under Types and
  retired hotkey aliases; the copied actual CRT/SDK/atomic corpus remains the
  positive control. Existing dependency/manifest/name/test boundaries remain.

## Executor qualification

Cached full-target x64/x86 builds and dual EXE publication pass. Final combined
evidence is exactly 193/193 distinct registered tests per architecture: 177
units/gates, eight background integrations, five native desktop and three real
audio integrations. No skipped/disabled/missing cases. Public units/gates are
121 per width. The final unit rerun includes left/right Alt regression rows.
Evidence: build/t94/s3 final-unit-*.xml, integration-*.xml, native-*.xml;
final.exit is zero. Executed names exactly match unfiltered CTest registration.

The initial x86 documentation gate failed because the executor prematurely
named an active-S record as a closed-S history filename. Correcting the name
restores the documented state invariant; the complete final unit/gate rerun
passes. Initial qualification.exit remains 1 rather than erasing that failure.
The last gate diagnostic-text edit changes no check predicate; its current
manifest was verified again. Existing x86 Ninja log recovery and original
mirror warnings remain disclosed; no zero-warning or cache-efficiency claim.

Production C/H: 20 changed paths (including old/new relocation paths),
+263/-271, net -8. Test C: 20 paths, +210/-189, net +21. Build/gate scripts:
four paths, +16/-5, net +11. Counted with git diff 7f64ec53 --numstat,
excluding documentation, manifests and binaries from code counts. Numeric
declarations/layout are identical after name normalization. No XP mirror diff.

Removed paths/vocabulary: types/input_values.h, lib_key/lib_key_input,
LIB_KEY_*/LIB_MOUSE_BUTTON_* and six KVM_HOTKEY_KEY/MODIFIER aliases. The only
neutral contract is base/input_interface.h; native decoding still belongs to
the same Base implementation, and KVM has the same sole delivery path.
The source API rename and lib_i32-to-lib_bool return require caller rebuilds;
they do not promise binary compatibility with previously compiled libraries.

Published SHA-256:
- x86: BF8CDD793C762FA275D25AFAB0ABEA5B001C239E18B88B5CB31EC0A1391777C9
- x64: 2EB65B25A4F5E8EBA1CCCC9233C61D024D7A175BBACB354A3AA92F88CEC37DDE

Root test infrastructure changes needed by receivers: none. App's two source
and three integration-test callers only adopt the renamed constants. X86 Debug
is unchanged. No NXVM read/import validation, real Linux host run or manual
RDP qualification was performed. Native audio classification and code are
unchanged; the passing PCM/MIDI integrations prove current device availability,
not a fix for disconnected-session endpoint failures. No Computer Use.

Executor actual-diff review confirms the request ledger A-E is fulfilled.
Complete delivery 3e5c61e4f3a0a4ccf540ebb11026ea5107c25362 is pushed to main.

## Coordinator review

After the executor push, reviewed the actual committed path list and critical
diff against the original request and A-E ledger. The neutral header is the
only definition, the same decode/delivery route remains, Window still translates
only UNMAPPED, and no native dependency or state owner moved outward. Reviewed
negative gate and delivery tests, all 193 executed names per width, binary
digests and the clean HEAD/origin agreement. No mirror, audio implementation,
user INI/media, root test infrastructure or X86 Debug change is present.
The result requires source consumers to adopt the Base spelling and rebuild;
no compatibility aliases are retained. No blocking finding remains. Owner
acceptance is pending; S4 and whole-T closure are not admitted automatically.
