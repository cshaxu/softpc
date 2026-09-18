# Common Machine 同步请求发布与收敛

## 请求与状态

Owner 确认本任务只有两项改动：修正同步请求发布顺序；收敛重复的同步请求
参数、状态和完成机制。原讨论的生命周期/输入分离、VM 安全点所有权、Session/UI
结构均为保持不变的约束，不是改造项。

本文件独立为未编号 T 候选，接续 [Lib 优化](../history/M9-T64-lib-frame-copy-proposal.md)
之后、[快照与全组件架构简化](m9-snapshot-architecture-simplification.md) 之前。
顺序由 [Queue](../states/QUEUE.md) 唯一维护；正式准入才分配递增 T 编号。
不改变 [当前活动任务](../states/CURRENT.md)，本次仅编写候选设计。

## 范围与不变边界

主要生产范围为 `src/common/machine/machine.c` 及必要组件内部声明；测试归
`test/common`，并更新受影响的 README、manifest 与独立验证。
遵循现有 [架构](../design/ARCHITECTURE.md) 和 [源码规范](../design/CODING.md)。

- 保持公共 API、driver 契约及现有成功/失败的用户可见语义。
- 保持单 executor、现有 lifecycle 控制、普通输入队列、debug lease、run generation。
- 保持双缓冲、帧发布和 Session/UI 的对象所有权、提示符与展示策略。
- Common 不判断 CPU 安全点、不编码设备、不操作快照文件；VM 仍负责安全点及期限。
- 不修改 Lib、VM、Compat、MVDM，不增加线程或通用动态任务框架。
- `snapshot_finish()` 失败处理属于原架构 proposal 的 VM 工作，不纳入本任务。
- 有效帧复制属于独立 Lib proposal，本任务不重复实施。

## 拟定 S1：修正同步请求发布顺序

原审计基线 `aa2bc0d` 中，`common_machine_read_state()` /
`common_machine_write_state()` 先设置 requested，再写 payload、重置完成事件。
executor 可能提前观察请求，或完成信号被调用方随后重置。这是源码顺序风险，
尚未以故障注入复现，不将其归因于已经修复的显示问题。

实施前扫描所有同类同步请求。正确顺序为：确认独占准入，准备完整参数与完成
等待，再通过明确同步边界发布请求，唤醒 executor，等待结果。保留重复请求
拒绝；不能把用于准入的占用标志同时当成可消费标志。按现有调用线程契约选择
最小同步实现，不仅机械交换语句，也不增加第二请求路径。

验收用 controllable fake/barrier，不用 Sleep 猜测：

- 参数未准备完成时 executor 不消费；发布后只能看到本次完整参数。
- executor 立即完成也不会丢失通知。
- 重复请求、错误状态及不支持的操作按原契约拒绝。
- 失败、stop/shutdown 交错不会使同步调用永久等待。

## 拟定 S2：收敛同步操作请求槽

审计 media、debug、state 操作的参数、结果、等待与取消条件；证明现有单控制
调用方下这些同步操作不能同时占用请求，再统一为一个 Machine 内部请求槽。
槽只表示操作类型、对应参数 union、必要阶段、结果和一个完成事件。
executor 仍是操作执行者；调用方按已有同步 API 取结果。

保留快照保存等待 driver 完成等真实阶段，不将多个 boolean 机械替换成复杂
大状态机。不同操作的准入状态、debug lease、driver 失败和取消处理必须明确，
不因共用容器而放宽条件。请求槽保持占用直到结果安全交付，避免下一操作覆盖。
删除被替代的各套字段/事件/完成路径，不保留过渡实现。

start/pause/resume/stop/reset 与普通输入不并入这个同步请求槽；stop/shutdown
必须能终止等待，不能排在阻塞请求后面。不得将所有机器指令改成动态任务队列。

验收：各操作正常及失败矩阵、请求串行/重复拒绝、debug lease 失效、快照
成功后的 paused 完成顺序、stop/reset/shutdown 交错，且 S1 发布边界测试持续通过。

## 执行、规模与退出

每个 S 开始前报告实际受影响文件、所有权、方案与 diff 估算；结束后以 Git
numstat 分别报告生产和测试增删，并证明必要语义未因删代码丢失。
本任务尚无可信净减行数承诺，目标是减少重复事实和完成路径，而非压缩排版。

每个 S 执行共享独立测试、聚焦 barrier 测试和 x86/x64 全量回归，构建两份
正式 EXE，更新受影响 manifest，按 P 提交推送并给 owner 测试链接。
不改用户配置、媒体。T 级退出需两项证据齐全、旧重复路径已删除或有明确保留理由。

若请求不能按现有契约互斥，或需要修改公共 API、Lib/VM、用户语义或线程职责，
先停止报告并请求新的范围决定，不默默扩展。排队不等于实施准入。
