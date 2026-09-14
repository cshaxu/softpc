# M9 T59: remaining TODO investigation and repairs

## 原始请求与准入

“overlay 页查询 O(n) 这个加入一个队列proposal，td提交；然后所有其他todo，请你准入一个s任务予以处理”。

T58 已关闭；本任务使用下一全局编号 T59。S1 已准入，尚未实现或收口。
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
保护用户配置和镜像。MVDM 只读调查；若需要改动保留的 CPU 源码，先报告根因与窄改动并取得明确授权。

## 验证预算与退出条件

首次包测试按宽度串行各十轮，之后有界重放历史并发条件；每个子进程沿用现有超时。
诊断只写本任务拥有的 build/t59-s1 子目录，每项运行最多 90 秒，文本日志总量不超过 20 MiB；
达到预算停止该轮采集、汇报证据，不无限打印或以等待代替归因。退出时清理自有进程和无用诊断产物。
新浮点探针使用内存或可丢弃输入，不写用户介质。

允许最终处置：已证实修复及回归；证据证明为测试问题并修复；所有者批准的未复现归档/延期。
七项均有处置才满足 S1 收口条件。当前没有任何一项被本次准入声称修复。

准入前只读复测：x86/x64 的 package 普通/compact、host_console_display、
types-layout 与其 selftest 各三轮通过；不覆盖 D 的精确横向视口或 E 算术判定，
也未重放 A 的原并发构建。这是基线观察，不是七项退出证据。

## 原始债务全文

- TODO(Low): local toolchain owner — T58 S3 observed make subprocess Error -1 and one diagnostics-free types-layout self-test exit; unchanged focused types-layout rerun passed. No compiler diagnostic or source change explains these transient exits. Investigate runner/toolchain process termination if reproducible; do not alter shared tests or product behavior to hide it.

- TODO(Medium): package acceptance owner — T58 S3 first full x64 suite failed startup debugger help at package stage 17; captured viewport was 30 columns and text wrapped. The unchanged focused package rerun passed. Root cause and relation to Console geometry are unproven; admit a bounded package/viewport investigation if this recurs. Do not weaken the help assertion or change shared Console behavior in an App relocation task.

- TODO(Medium): package acceptance owner — T58 S1 first full x64 run timed out in compact Console at stage 5 while x86 tests ran concurrently; the subsequent serial x64 full suite passed 85/85. Product src and both EXE hashes are unchanged from T57. Concurrency is context, not a proven cause; admit a bounded baseline-package investigation if this stage recurs, without weakening assertions or modifying runtime in the test-corpus task.

- TODO(Medium): Lib Host display owner — T56 S16 corrective probe with an artificial 30x30 buffer and 20x10 viewport observed cooked restoration as 30x10; restoring capacity afterward changed the viewport width instead. Normal full-width narrow/short/scrolled viewports pass. This is distinct from the fixed first-start frame-capacity ordering; admit a host-specific horizontal-scrollback preservation investigation if required, rather than add iterative resize compensation to this startup repair.

- TODO(Medium): SoftPC CPU owner — T56 S12 disposable FNINIT/FILD qword/FISTP qword probe observed an indefinite integer write after a finite read. The debug observation matches actual stored bytes; arithmetic correctness and pre-hook baseline equivalence were not this debug-ABI task's scope. Admit a separate original-x87 baseline comparison if requested; preserve media and do not alter CPU arithmetic in a debugger adapter.

- TODO(Medium): package acceptance owner — T55 S7 observed one x64 stage-7 CAP-to-monitor timeout after DOS; direct rerun, a full rerun and five consecutive package runs passed. Root cause is not established. Admit a baseline-comparison investigation if it recurs or the owner requests it; preserve the current assertion and do not label the symptom fixed.

- TODO(Medium): package acceptance owner — T56 S16 observed two x64 stage-14 DOS ver visibility timeouts, including a serial run. Full serial regression passed, and identical S15/current-EXE comparison passed three times each. S18 first full x64 run reproduced the compact-console stage-14 symptom; the subsequent full unchanged suite passed 83/83, as did x86. No input/focus change or weakened assertion was made; root cause remains unproven. This recurrence remains a separate bounded startup-input timing investigation, not a wake-allocation repair claim.
