# Common Machine 同步请求发布与收敛

## 请求与状态

T65 的复核结论是：唯一必要改动是 state read/write 的完成事件发布顺序。现有
Common 公共契约已经规定唯一 control thread 串行提交同步请求；因此不为违反该
契约的并发调用增加 request slot、mutex、union 或通用阶段状态机。原讨论的
生命周期/输入分离、VM 安全点所有权、Session/UI 结构均为保持不变的约束，不是改造项。

Owner 已批准 T64 收口并准入本项为 T65；唯一 S 是 S1，状态由
[CURRENT](../states/CURRENT.md) 维护。本项已移出 Queue。随后 owner 要求按技术
事实复核；结论及修订范围由当前 packet 确认。原始准入请求：“可以，批准收口提交推送T64，接下来准入T65”。

## 范围与不变边界

主要生产范围为 `src/common/machine/machine.c` 及必要组件内部声明；测试归
`test/common`，并更新受影响的 README、manifest 与独立验证。
遵循现有 [架构](../design/ARCHITECTURE.md) 和 [源码规范](../design/CODING.md)。

- 保持公共 API、driver 契约及现有成功/失败的用户可见语义。
- 保持单 executor、现有 lifecycle 控制、普通输入队列、debug lease、run generation。
- 保持双缓冲、帧发布和 Session/UI 的对象所有权、提示符与展示策略。
- Common 不判断 CPU 安全点、不编码设备、不操作快照文件；VM 仍负责安全点及期限。
- 不修改 Lib、VM、Compat、MVDM，不增加线程、锁、request slot、通用动态任务框架或新的并发调用保证。
- `snapshot_finish()` 失败处理属于原架构 proposal 的 VM 工作，不纳入本任务。
- 有效帧复制属于独立 Lib proposal，本任务不重复实施。

## 拟定 S1：修正同步请求发布顺序

准入复核 `df5df14`：当前 state read/write 已先复制 payload，但仍在 executor
可观察 `requested` 后 reset 完成 event。running 的 executor callback 无需等待
`command_event`，可在两句之间完成并 signal，随后 reset 会丢失完成通知。
这是真实并发边界，和第二调用者无关。

修复固定为：复制 payload → reset completion event → 发布 requested → 原有 wake/wait。
state write 虽通常经 command worker 才消费，也采用相同顺序以保持同一操作契约。
media/debug 已是 payload → reset → requested；保持不动。

### S1 有限收敛台账

单位是四类同步请求的调用方准备、executor 消费、完成与中止路径。
每项必须有修复或带源码依据的保留决定；未完成项不能静默漏过。

| 类别 | 准入观察 | 后续证据要求 |
| --- | --- | --- |
| state read | payload → requested → reset | 修为 payload → reset → requested；running 即时完成路径 |
| state write | payload → requested → reset | 修为 payload → reset → requested；load 完成路径 |
| removable media | payload → reset → requested | 保留；唯一 control-thread caller |
| debug | payload/lease → reset → requested | 保留；唯一 control-thread caller |

调用线程契约审计确认：`common_machine` 的同步调用都从唯一 control thread 进入；
接口也禁止从 driver/sink callback 重入。因此不建立第二层“独占准入”，也不把
不同生命周期的 media/debug/state 机械合并。验收以现有 Common machine fake 的
running 状态即时 state-read 覆盖及源码顺序审查为准；不使用 Sleep 猜测。

## 执行、规模与退出

每个 S 开始前报告实际受影响文件、所有权、方案与 diff 估算；结束后以 Git
numstat 分别报告生产和测试增删，并证明必要语义未因删代码丢失。
本任务尚无可信净减行数承诺，目标是减少重复事实和完成路径，而非压缩排版。

本 S 执行共享独立测试、聚焦 machine fake 测试和 x86/x64 全量回归，构建两份
正式 EXE，更新受影响 manifest，按 P 提交推送并给 owner 测试链接。
不改用户配置、媒体。T 级退出需此次顺序修复的证据齐全；不存在待删除的重复路径。

若未来需要支持多调用线程，必须另行准入并定义公共同步契约；本任务不默默扩展。
