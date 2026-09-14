# M9 T59: remaining TODO investigation and repairs

## 原始请求与准入

“overlay 页查询 O(n) 这个加入一个队列proposal，td提交；然后所有其他todo，请你准入一个s任务予以处理”。

T58 已关闭；本任务使用下一全局编号 T59。S1 实现、双宽度全量回归及交付审查完成，等待所有者测试；S/T 尚未收口。
最新所有者授权：“验证T59的潜在问题 有确切证据的可以修复 没有证据的可以关闭不再关注”。
因此完成有界复测仍无证据的条目可关闭观察、不再转回 TODO；不称为已修复。
基线为 `bea5f8b`。Overlay 独立排队，不属于本任务；Win3.1 模式往返显示异常也保持独立候选。

## S1 有界台账

冻结范围是下列七条原始债务，每一条必须有独立结论，不能用合并调查隐藏遗漏。

| ID | 对象 | 初始状态及处理路径 |
| --- | --- | --- |
| A | make Error -1 / types-layout 无诊断退出 | 未归因；核对构建/测试进程与日志，重放原并发条件，不猜测为产品故障。 |
| B | package stage 17 debugger help | 未归因；核对输入、换行、视口与断言观察方式。 |
| C | compact Console stage 5 启动 | 未归因；串行基线后重放双宽度并发条件。 |
| D | 横向视口恢复 | 精确重建 30×30 buffer、20×10 viewport，验证 raw/cooked 往返几何。 |
| E | x87 FILD/FISTP qword | 独立有限值加载/存储判定，对照原始实现和双宽度；不把“写入一致”当算术正确。 |
| F | package stage 7 CAP 返回 monitor | 未归因；检查输入提交、状态完成、交接和提示符观察。 |
| G | package stage 14 ver 可见性 | 多次历史发生；优先保存输入/显示阶段证据，与 B/C/F 共用调查设施但不假定同因。 |

## 执行顺序与设计约束

1. 固定环境、EXE 哈希及原始断言；为七项建立可重复的最小证明。
2. 先调查 B/C/F/G 的公共测试路径，再处理 D、E、A 的独立验证。
3. 确认故障后仅修改责任层，扫描同因路径并补回归；不放宽原断言、不加重试掩盖错误。
4. 对未复现项报告尝试次数、环境、覆盖缺口；归档/延期须明确给所有者批准，不能标为已修复。
5. 修复后双宽度构建到固定 EXE、全量测试、逐项审计、提交推送；S/T 不自动收口。

原生 Console 状态仍由 Lib broker 管理，Common 管生命周期，测试观察不得改变产品语义。
保护用户配置和镜像。所有者随后明确“批准修改”：允许 cfpu_def.h 的小端宿主布局窄修，
包括 FP32/FP64/FP80 和按实际使用契约验证的整数高低字布局；保留大端定义，
不改 CPU 算法、舍入策略或 Lib/Common。补布局及真实指令双宽度回归。

## 验证预算与退出条件

首次包测试按宽度串行各十轮，之后有界重放历史并发条件；每个子进程沿用现有超时。
诊断只写本任务拥有的 build/t59-s1 子目录，每项运行最多 90 秒，文本日志总量不超过 20 MiB；
达到预算停止该轮采集、汇报证据，不无限打印或以等待代替归因。退出时清理自有进程和无用诊断产物。
新浮点探针使用内存或可丢弃输入，不写用户介质。

允许最终处置：已证实修复及回归；证据证明为测试问题并修复；所有者批准的未复现归档/延期。
七项均有处置才满足 S1 收口条件。最终处置见下表；检查点保留调查过程，不覆盖最终结果。

准入前只读复测：x86/x64 的 package 普通/compact、host_console_display、
types-layout 与其 selftest 各三轮通过；不覆盖 D 的精确横向视口或 E 算术判定，
也未重放 A 的原并发构建。这是基线观察，不是七项退出证据。

## S1 验证检查点（尚未交付）

- G：x64 普通 package 首轮 stage 14 失败；另一次重复前三轮通过、第四轮
  同样失败，最终画面只有 DOS 提示符、没有 ver 输出。确切复现，仍需归因。
- C：x64 compact 连续十轮通过；B/F 未在这些已执行路径中复现。
- D：新增 30×30 buffer/20×10 viewport 回归，x64 一轮、x86 三轮均通过。
  尚未修改 Lib 行为。
- E：真实 CPU 临时断言双宽度均把 0000123400001234 写为 8000000000000000。
  独立 cfpu_def.h 布局探针中 double 1.0 的 exponent 双宽度均为 0（应为 1023），
  hiword offset=0。已向所有者请求窄布局修正授权；未修改 MVDM 或算术算法。
- A：当前双宽度 -j8 目标重编译未出现无诊断退出；types-layout 与 selftest
  双宽度各三轮通过。未证实产品故障，按最新授权关闭观察，不修改测试来隐藏错误。
- B/C/F：x86 普通/compact 各十轮通过；x64 compact 十轮、带失败后诊断的
  普通测试十轮通过，未触发这三种症状。按最新授权关闭这些独立观察，不宣称修复。
  G 的复现仍独立处理，后续诊断轮次不拿来掩盖原失败。
- E 临时断言红灯是故障证据，不是可发布的完成版本。
- G 后续诊断另一次捕获到 `8ver` 而非注入的 `ver`，原生输入缓冲已空，
  随后 CAP 可返回 monitor。证明该轮执行了意外命令，但不证明最初空白症状同因。
  测试创建可见 Console，raw 激活会前置它，因此自动化存在接收人工键盘的干扰入口。
  测试改为隐藏自有 Console，通过句柄观察，并断言启动及 raw 阶段仍隐藏；
  不改产品焦点规则，不增加输入重试，不放宽 Version 断言。
- 隐藏 Console 的 x64 普通/compact 各十轮通过；去除 VM 输入插桩、重新构建后，
  x86/x64 普通/compact 各三轮通过。MVDM/VM 源码与 HEAD 无差异。
  临时失败后 CAP/debug 采集已移除，保留原有失败报告。该证据支持测试隔离，
  不宣称已经解释所有历史 stage 14 超时；G 最终处置和完整交付仍未完成。

## S1 交付台账

| ID | 处置 | 证据及边界 |
| --- | --- | --- |
| A | 关闭观察，不再关注 | 双宽度 -j8 重编译、types-layout/selftest 三轮及最终完整构建/回归通过；没有确认的工具链或产品根因，不改实现。新增 CMake 目标后同一 make 多目标调用需重新进入生成后的 Makefile，不等同于历史 Error -1。 |
| B | 关闭观察，不宣称修复 | 普通/compact 有界重复及最终回归均通过原 help 断言。 |
| C | 关闭观察，不宣称修复 | 双宽度串行有界重复和最终回归通过；并发补验单独记录。 |
| D | 关闭观察，保留精确回归 | 30×30 buffer/20×10 viewport 用完整快照比较 raw/cooked 往返；双宽度通过，没有 Lib 行为修改。 |
| E | 已修复宿主布局 | 原失败整数恢复原值；真实头文件布局及实际 FNINIT/FLD/FSTP/FILD/FISTP/FBLD/FBSTP、四则运算双宽度通过。 |
| F | 关闭观察，不宣称修复 | 原 CAP/monitor 断言在有界重复和最终双宽度回归通过。 |
| G | 修复测试输入隔离；关闭未证实的剩余产品原因观察 | 意外 `8ver` 暴露可见自动化 Console 可接收外部输入；改为隐藏测试窗口并检查其隐藏状态。保留 Version、时限和所有原阶段断言；无重试、无产品补丁。不能将此归因为全部历史空白症状的唯一根因；按所有者有界验证授权归档剩余观察，不再推测产品故障。 |

### 修复与同类扫描

- 生产仅 `host/inc/cfpu_def.h`：小端 FP32、FP64HI、FP80SE 位域；FP64、FP80 字顺序；
  FPU_I64 对齐原 `fpu.c` 小端 BCD 表。保留大端定义、原算术及 Intel 内存转换。
  搜索原 host/inc 与 base 的 FP32/FP64HI/FP80SE/FP64/FP80/FPU_I64 定义，
  当前选中头文件是唯一该组布局定义；检查 fpu.c 的转换、BCD 常量和读写调用点。
  `softpc-x87-layout-smoke` 是永久布局检查，不引入第二套浮点实现。
- 复用 command-provider 的原 executor/debug rendezvous 执行 10 组正负值往返及
  4 组精确算术，不另造 executor。原 operand observation 断言保留并增加数值等式。
- 包测试只隔离测试进程的宿主输入；产品的 raw 激活、焦点和键盘归一化没有修改。
  临时 VM 输入插桩、失败后交互采集和独立布局探针均已清理。
- Lib/Common/VM/Compat 源码和用户 INI/介质无修改。共享 test/lib 仅补几何回归及 manifest。

### 验证与产物

P1 `2c05238` 已提交推送。协调者在推送后复核实际提交的全部路径、头文件差异、
测试输入边界、七项处置及双宽度结果：没有超出批准范围的生产改动，没有绕过失败断言；
原 VM/Common/Lib/Compat 和 INI 保持不变。此次只完成交付审查，不自动关闭 S1/T59。

- `cmake --build --preset tests-x64 -j8`、`tests-x86 -j8` 完成，刷新两份固定 EXE。
- `ctest --preset test-x64 -j1 --output-on-failure`：86/86。
- `ctest --preset test-x86 -j1 --output-on-failure`：86/86。
- 两个 suite 均包括新布局测试、真实指令测试、包测试、完整 Lib/Common 测试、manifest、边界和文档门禁。
- 两宽度并发执行 `ctest --preset test-<width> -R '^softpc-package-' --repeat until-fail:3 -j1 --output-on-failure`，
  普通/compact 各三轮全部通过（共 12 次）；C/G 未复现。
- 对比交付前 HEAD 的 `git diff --numstat`：生产 1 文件 +35/-4（净 +31）；
  测试 C 源码 4 文件 +132/-7（净 +125）；根 CMake +7/-0。
  另有 test/lib manifest、文档和两份 EXE，不计为生产/测试源码行数。
- `softpc32.exe`：3,502,713 bytes，SHA256 `7DDA1F9C8631DBCB90AE7D39CFB94723625D7852A98F06A0FD6AE37B4CFCAC66`。
- `softpc64.exe`：2,849,636 bytes，SHA256 `792943A6DF298DFA199E5278ED26AE995B2C86D4F2EBAEF408FC97CA1C407778`。

## 原始债务全文

- TODO(Low): local toolchain owner — T58 S3 observed make subprocess Error -1 and one diagnostics-free types-layout self-test exit; unchanged focused types-layout rerun passed. No compiler diagnostic or source change explains these transient exits. Investigate runner/toolchain process termination if reproducible; do not alter shared tests or product behavior to hide it.

- TODO(Medium): package acceptance owner — T58 S3 first full x64 suite failed startup debugger help at package stage 17; captured viewport was 30 columns and text wrapped. The unchanged focused package rerun passed. Root cause and relation to Console geometry are unproven; admit a bounded package/viewport investigation if this recurs. Do not weaken the help assertion or change shared Console behavior in an App relocation task.

- TODO(Medium): package acceptance owner — T58 S1 first full x64 run timed out in compact Console at stage 5 while x86 tests ran concurrently; the subsequent serial x64 full suite passed 85/85. Product src and both EXE hashes are unchanged from T57. Concurrency is context, not a proven cause; admit a bounded baseline-package investigation if this stage recurs, without weakening assertions or modifying runtime in the test-corpus task.

- TODO(Medium): Lib Host display owner — T56 S16 corrective probe with an artificial 30x30 buffer and 20x10 viewport observed cooked restoration as 30x10; restoring capacity afterward changed the viewport width instead. Normal full-width narrow/short/scrolled viewports pass. This is distinct from the fixed first-start frame-capacity ordering; admit a host-specific horizontal-scrollback preservation investigation if required, rather than add iterative resize compensation to this startup repair.

- TODO(Medium): SoftPC CPU owner — T56 S12 disposable FNINIT/FILD qword/FISTP qword probe observed an indefinite integer write after a finite read. The debug observation matches actual stored bytes; arithmetic correctness and pre-hook baseline equivalence were not this debug-ABI task's scope. Admit a separate original-x87 baseline comparison if requested; preserve media and do not alter CPU arithmetic in a debugger adapter.

- TODO(Medium): package acceptance owner — T55 S7 observed one x64 stage-7 CAP-to-monitor timeout after DOS; direct rerun, a full rerun and five consecutive package runs passed. Root cause is not established. Admit a baseline-comparison investigation if it recurs or the owner requests it; preserve the current assertion and do not label the symptom fixed.

- TODO(Medium): package acceptance owner — T56 S16 observed two x64 stage-14 DOS ver visibility timeouts, including a serial run. Full serial regression passed, and identical S15/current-EXE comparison passed three times each. S18 first full x64 run reproduced the compact-console stage-14 symptom; the subsequent full unchanged suite passed 83/83, as did x86. No input/focus change or weakened assertion was made; root cause remains unproven. This recurrence remains a separate bounded startup-input timing investigation, not a wake-allocation repair claim.
