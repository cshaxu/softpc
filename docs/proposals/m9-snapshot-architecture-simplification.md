# 快照增量与全组件架构简化

## 请求、状态与基线

Owner 原始请求：

> 新增加的代码，有多少是可以删除或者精简的？mvdm里面，哪些新增diff是应该挪到 compat或者vm里面，以便减少diff？哪些diff其实不该存在或者简化，可以消除？非mvdm里面，同理？
>
> 对，允许你对当前所有组件的架构进行一次深入思考，包括lib和common，不改代码，看看怎样可以更好：优化数据结构、线程结构、组件结构、代码结构。
>
> 请你将以上全部写入proposal

本文件起初记录只读审计与候选设计；T66 准入后，它记录该任务的有限实施台账。
当前活动 S 仍以 [CURRENT](../states/CURRENT.md) 为准。D1/D2 与 C1 的 Common
部分已经分别由 T64/T65 处置，均不回流本任务。

历史审计基线为 `aa2bc0d`，T63 增量比较起点为 `ea7e982`。以下计数由
`git diff --numstat ea7e982 aa2bc0d -- src` 中的 `.c/.h` 汇总，排除测试、
文档、manifest、构建文件与 EXE：

| 范围 | 文件 | 新增 | 删除 | 净增 |
| --- | ---: | ---: | ---: | ---: |
| MVDM | 24 | 2626 | 1 | 2625 |
| 非 MVDM | 34 | 3118 | 50 | 3068 |
| 合计 | 58 | 5744 | 51 | 5693 |

非 MVDM 分布：App +149/-15；Common +188/-9；Compat +2322/-17；
Lib +43/-0；VM +416/-9。多数新增是必要状态枚举、编码与恢复，不能视为可删冗余。
外移减少镜像 diff，不等于全项目净减；测试行数也不得混作生产减量。

## 目标与非目标

减少必须同时维护的状态副本、字段顺序、分派和资源清理路径；保持现有体验、
单 executor、原始设备状态所有权、跨宽度快照及公共调用边界。
遵循 [架构](../design/ARCHITECTURE.md)、[源码规范](../design/CODING.md)、
[架构规则](../rules/ARCHITECTURE.md) 与 [编码规则](../rules/CODING.md)。

不以宏压行、取消错误检查、取消 static、暴露私有指针或增加转发层实现“减代码”。
不重写原始 CPU/设备，不建立通用序列化框架、任务框架、帧缓存框架。
Lib/Common 被允许纳入本次审计，不代表其 API 或生产代码已获修改授权。
实施前必须明确各阶段的共享 corpus 改动边界与兼容影响。

### 固定决策优先级

每项候选一律按下列顺序取舍，不能为了后项收益损害前项：

1. 维持或修复正确功能；
2. 维持正确组件边界与状态所有者；
3. 在前两项不受损害时减少 MVDM 镜像 diff；
4. 最后才考虑减少 VM/Compat 代码量。

因此“镜像 diff 更小”本身不是外移 CPU/设备状态的理由；同样，减少行数也不是
建立 descriptor、通用 serializer、转发层或第二状态机的理由。

## 候选审计台账

本轮覆盖单位是下列有限结构机会，不声称已证明全树不存在其他缺陷。
“确认”指源码事实；净减数字均为未实施估算，需准入前复核和实施后 numstat。

### A1：寄存器复制保持在镜像（保留决定）

[c_reg.c](../../src/mvdm/softpc.new/base/ccpu386/c_reg.c) 的新增 capture/restore
虽然只组合既有 `c_get*`/`c_set*`，但它描述的是 CPU 架构与隐藏段状态的完整恢复
顺序。它目前位于状态所有者旁，不需要 Compat 学习该顺序或引入一组跨层 accessor
ABI。按优先级 1/2，这 68 行镜像 diff 保留；不为优先级 3 的数字收益外移。

### A2：删掉快照复制的 Graphics Controller 分派

[vga_prts.c](../../src/mvdm/softpc.new/base/video/vga_prts.c) 的
`snapshot_write_graphics()` 另列九项 switch；原始 `vga_gc_outw()` 已按原表分派。
候选改为复用该入口，预计净减 15–18 行。必须验证初始化后的 handler 表、
动态 bit-mask handler 和全部四种写模式，不能仅凭名称宣布等价。
P9 的 V7 enable latch、真实掩码、零寄存器派生状态初始化是必要修复，保留。

### A3：PIT 同一状态解码只保留一份

[timer.c](../../src/mvdm/softpc.new/base/system/timer.c) 为 current/prior/gate
状态分别恢复，后两者创建临时 COUNTER_UNIT 只取函数指针。
候选改为一个有校验的状态 ID 解码，各字段直接赋值，预计净减 15–30 行。
原函数指针属于原始内部状态，仍留镜像；wait-action 与 gate-action 的合法集合
不同，不能无条件合并或放宽验证。非法 ID、NULL 规则和 PIT 相位需测试。

### A4：无行为残留清理

[com.c](../../src/mvdm/softpc.new/base/comms/com.c) 的新增重复 recv_char /
do_wait_on_send 声明、timer.c 新增无关空行，以及
[Compat video.c](../../src/compat/video.c) 未使用的 Currently_emulated_video_mode
声明可清理。保留必须早于首次引用的声明；只恢复本次新增差异，不格式化原始周边。
验收：预处理/双宽度编译、原始 diff；按调用点扫描同类残留。

### A5：不得为了镜像数字外移私有状态访问

c_main 续执行位置、TLB、FPU、quick-event 链表、DOS 鼠标驱动等私有状态，
不通过取消 static 或几十个新 getter 整体搬出。其窄复制/恢复 hook 保留在状态
所有者旁；文件编码、宿主资源和产品编排分别归 Compat/VM/App。
继续保持一套原始状态，禁止第二设备实现。这是保留决定，不是未完成外移。

### B1：archive 读/写字段顺序保持直接表达（保留决定）

[CCPU archive](../../src/compat/ccpu/archive.c) 与设备 archive 的直接读/写字段序列
就是固定二进制格式的可审计定义。为消除文字重复而建立 descriptor、union 或通用
serializer，会添加格式解释层和方向相关的失败路径，优先级 1/2 不成立。保持直接
表达，不纳入 T66 实施。

### B2：操作临时数据归局部作用域

[VM driver](../../src/vm/driver.c) 的 captured_image 仅在一次 capture callback
内创建、写出和释放，却作为长期成员并在 destroy 再清理。
候选移成该操作的局部拥有对象，统一清理出口；staged_image 跨 load/executor，保留。
[snapshot](../../src/vm/snapshot.c) 的 entry 副本没有生产读取，实际使用 callback
entry，应删除重复状态并相应修正只验证此冗余字段的测试。
收益主要是缩短所有权，不宣称大量净减。停在 callback 中等待期间对象仍须有效。

### C1：Common 同步请求收敛及发布顺序

Owner 已将本项转入独立 [Common 同步请求任务](m9-common-machine-request-simplification.md)。
本候选不再实施该 Common 改动；发布、请求槽及验收由该 proposal 唯一维护。

### C2：VM 快照收尾失败处理

[VM driver](../../src/vm/driver.c) 还忽略 snapshot_finish 的失败返回；应纳入已有
操作/执行终止路径，明确已发完成与后续时钟恢复失败的关系，不新增旁路通知。
本项仍由本候选负责，不属于独立 Common 任务。Common 不判断内部安全点，
VM 仍负责安全点与一秒期限。改动规模和净减需独立设计，不以删除必要等待/
错误分支达成行数指标；验证时钟恢复失败与原有完成/执行终止语义。

### D1：按有效内容复制帧

本项已由 [T64 S2](../history/M9-T64-S2-frame-copy.md) 按收窄复制方案完成。
本候选不再实施；复制契约、范围与验收以该 proposal 为准。

### D2：Console worker 复用 Base task

本项经 [T64 S1](../history/M9-T64-S1-lib-simplification.md) 审计后由 owner 取消，
保留各自 worker，不再安排统一 task 实施。
本候选不再实施；线程生命周期与验收以该 proposal 为准。

### D3：Storage overlay 查询保持独立任务

[medium.c](../../src/lib/storage/medium.c) 的 O(n) 页查询是性能机会，但新索引
会增加结构维护，不属于可承诺的净减代码。沿用已有
[overlay proposal](m9-overlay-page-index.md)，本候选不重复实现或登记。

## 线程与组件的保留决定

- 保留 Session 与 VM executor：机器执行不能阻塞总控。
- 保留 Window worker 与 Console reader：原生消息、阻塞读行与取消约束不同。
- 保留 Console worker：不把绘制搬入 broker 输入回调。
- 保留 Session 控制输入队列与 Machine 输入队列：前者决策，后者交付机器输入。
- 保留 logical Console 与 broker：逻辑对象不同于独占原生资源所有者。
- 保留 Common UI：集中拥有 monitor、KVM、broker 及交接资源，不是空转发层。
- 保留 App 组装/产品策略、Common 中性协调、VM 具体驱动、Compat 原始宿主边界、
  MVDM 原始状态所有者的分工。不要按“目录少就是简单”重新合并。

这些保留项需在后续验收再次检查，不能为了候选优化偷偷改成相反设计。

## T66 阶段与准入要求

T66 只按下列顺序执行；每一个 S 开始前均须以当时 HEAD 复核文件、原始 diff、
调用方和预计 numstat。候选中的历史估算不能替代实际基线。每个 S 完成后均须
进行双宽度构建、完整回归、package EXE 刷新、P 提交推送和实际变更审计。

### 每个 S 的强制报告

开始实施前必须向 owner 报告：

- 精确组件/文件清单与所有权；
- 预计 production 与 test 的新增、删除、净变化；
- 对 OpenNT MVDM 镜像 diff 的预计新增、删除、净变化；
- 选择该实现而非外移、抽象或复制的边界理由与停止条件。

完成实施后必须以实际 Git 统计报告同一组 production/test、组件文件与 MVDM
镜像 diff 数字，逐项说明保留、移动或删除的原因；随后完成 x86/x64 编译、聚焦
和完整测试、刷新两份 package EXE、提交并推送。报告必须给出提交哈希与两个 EXE
链接，供 owner 测试；未满足这些记录与证据不得收口该 S 或自动准入下一项。

| S | 范围 | 退出证明 |
| --- | --- | --- |
| S1 | C2：将 `snapshot_finish()` 失败接入既有 VM 操作终止路径 | 时钟恢复失败注入、完成顺序、双宽度回归 |
| S2 | A2–A4：仅复用既有 VGA/PIT 原始机制并清理无行为残留；A1/A5 保留边界复核 | 原始 diff、双宽度 CPU/视频 roundtrip、handler/非法-ID 证明；前后逐路径行数 |
| S3 | B2：将仅一次 capture callback 使用的 VM 临时所有权收回局部，删除未读取 snapshot entry 副本 | snapshot roundtrip、失败清理、双宽度与所有权审计 |

此前把 A1 外移算入的“镜像减少约 100–125 行”估计已作废；A1 依正确边界保留。
S1 的正确性修复可能净增代码，允许发生；S2/S3 才追求在不伤害前两项前提下的净
简化。每阶段开始前重新审计基线、列文件/功能/估算及停止条件；结束报告实际
production/test numstat，分别计算镜像 diff 与全项目净变化，不用测试增加掩盖生产复杂度。

共享改动同步更新 src/lib、src/common、test/lib、test/common 中实际受影响的
manifest 和独立验证；当前 T66 不预设会修改 shared corpus。

停止条件：需要更改当前用户体验、原始设备行为、快照格式、公开 ABI，或使总复杂度
显著上升；先报告设计取舍，不悄悄扩大范围。无相关变更时不制造 ABI 修改。
本文件全部台账项在收口时必须有实现证据或明确的保留/独立任务归属，不能遗漏。

## S1 delivery

S1 delivery is recorded in [snapshot finish result ordering](../history/M9-T66-S1-snapshot-finish.md).
The actual VM production delta is +9/-1 (net +8); no test, Common, Lib or
OpenNT MVDM mirror path changed. The retained snapshot READY phase is the
single failure fact; no new driver latch or error channel was added. Both
widths passed their 107-test full suites and package builds.

## S2 delivery

S2 delivery is recorded in [original mechanism reuse](../history/M9-T66-S2-original-mechanism-reuse.md).
The snapshot video restore now enters the original `vga_gc_outw()` dispatcher,
and PIT state IDs have one validated private decoder rather than two temporary
`COUNTER_UNIT` wrappers. `com.c` declarations remain because they are required
before the snapshot helpers' first reference; moving code merely to erase that
textual duplication would enlarge the protected mirror diff. No public ABI,
Common, Lib, thread or snapshot-format path changed.
