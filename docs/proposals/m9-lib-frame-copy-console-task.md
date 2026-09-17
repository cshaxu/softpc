# Lib 有效帧复制与 Console task 复用

## 请求与状态

Owner 要求将以下两项 Lib 优化独立为一个 T 任务候选，放到队首：

1. 文本帧不再复制无用的最大图形数组；统一按有效内容复制帧。
2. kvm-console 复用现有 Base task，删除重复的 Win32 线程管理。

本文件是未编号候选；顺序由 [Queue](../states/QUEUE.md) 唯一维护。
正式准入时才分配递增 T 编号，不改变 [当前活动任务](../states/CURRENT.md)。
来源审计见 [架构简化 proposal](m9-snapshot-architecture-simplification.md)；
本文件独占这两项的实施范围，原 proposal 不再重复安排。

## 边界与非目标

遵循现有 [架构](../design/ARCHITECTURE.md) 与 [源码规范](../design/CODING.md)。
保持用户体验、公开帧结构和现有公开操作签名，不增加线程、不改变线程职责。
不修改 MVDM、VM、Compat、快照格式、Console broker 交接或产品策略。
Common 仅允许有效帧复制的必要调用点适配，不包含 Machine 请求/状态机重构。

明确排除 tagged union、引用计数、零拷贝指针、缓冲池、storage overlay 索引，
以及其他 Common/Lib 清理。Base 只复用既有 task API，不预设新增能力。

## 拟定 S1：统一有效帧复制

主要范围：`src/lib/kvm-base/frame_interface.h`、`mailbox.c` 及经审计确认的
帧复制调用点；必要时包括 Common machine 的 copied-frame 交付路径。

在现有 KVM 值工具边界提供一份复制实现：完整复制必需元数据，文本帧复制
文本、属性、字体和光标等有效内容，图形帧复制调色板及 stride 对应的有效像素。
不再为了文本更新复制最大图形容量；不改变 `kvm_frame` 的公开布局。
实施前枚举所有复制与读取点，明确非活动字段的读取约束，尤其是 mailbox 的
palette 比较、文本/图形切换和未初始化目标，不能简单删掉 memcpy 字节数。

保留双缓冲、锁、sequence/run-generation、latest-wins、dirty 累积及成功后
acknowledge。只提供一份复制规则，不给各消费者建立自己的字段清单。

验收：文本/图形往返、不同尺寸/stride/palette、完整元数据、dirty 跳帧与旧
ack 测试；证明文本复制不搬运图形数组，输出与基线一致。报告实际复制量和
生产/测试 diff，不以净减行数代替性能与语义证明。

## 拟定 S2：Console worker 复用 Base task

主要范围：`src/lib/kvm-console/console.c`、组件内部声明和
`win32/component.c`。将现有单线程创建、join、释放接到 `base_sync_task`，
移除只包一个线程句柄的额外对象及重复资源管理。平台无关循环归组件自身；
平台输入解释仍留平台实现。不为迁移另造转发对象或线程抽象。

保留 Console worker、私有 mailboxes 和原先 input callback 所在线程。
不把绘制放进 broker 输入回调，不改变 Linux UNSUPPORTED 范围。
先核对 Base task 的取消、等待和销毁契约，不能让 task cancellation 绕过
STOP FIFO 或 detach/join 屏障。

验收：创建失败、FIFO 遇 STOP、故障唤醒、回调脱离、worker join、恰好一次
SOURCE_RETIRED、destroy 后无 event，以及 Console 重新绑定后的待绘帧行为。
Base API 若不足以保持这些语义，停止并报告，不顺势扩张范围。

## 执行与退出要求

每个 S 开始前重新审计实际基线、受影响文件和调用点，报告实现方案与 diff
估算；完成后报告真实新增/删除、资源所有权及类似问题扫查结果。
同步更新受影响的 `test/lib`、`test/common`、README 与 manifest，运行共享
独立验证、聚焦测试和 x86/x64 全量回归，构建两份正式 EXE 并按 P 提交推送。
不修改用户配置、媒体。T 收口需两项分别有证据，提供双宽度 EXE 供 owner 验证。

如需变更公开结构/既有 API 语义、用户体验、线程职责或明显增加系统复杂度，
先报告并取得批准。排队不是实施准入，本次文档操作不声称已有代码或测试成果。
