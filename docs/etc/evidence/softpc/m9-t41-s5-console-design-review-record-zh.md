# M9 T41 S5 Console 设计问答记录与交叉核对

## 目的与范围

这是当前会话中 owner 对 S5 Console/UX 设计所作的原始中文决策摘录、
助手当时提出的方案结论，以及对当前
[S5 proposal](../../../proposals/m9-nxvm-library-adoption-and-softpc-binding.md)
的交叉核对。它是 supporting evidence，**不定义设计**；冲突时始终以当前
proposal 为准。

本文件保留对话中与 S5 有关的原始措辞摘录和逐项结果，不能替代聊天系统的
逐消息导出记录。摘录以 owner 的最终、更晚指示覆盖较早指示。

## 决策摘录与核对

| Owner 原始决定/纠正摘录 | 已收敛的结果 | Proposal 核对 |
| --- | --- | --- |
| “lib ux presenter只需要知道当前的一个boolean flag：是否own console the entire time, or only own console on presenter_type = console” | 已被更干净的 object model 取代：UX 不拥有 process Console；host 仅绑定 Current Console Object。 | “Shared-library boundary” |
| “UX和运行中的VM要独立切分开来，UX不管的，softpc负责激活ux的console与否” | UX 不决定 VM/monitor；SoftPC 为唯一组合层。 | “One SoftPC reconciliation path” |
| “host组件也要分win32和linux(placeholder)” | host/win32 实作，host/linux 保留同形 API placeholder。 | “Shared-library boundary”“Host internals” |
| “softpc.ini除了display有console, window以外，还要加一个console是否始终被当前活动vm独占的flag” | `console_control=0|1` 仅为 display=console 的 SoftPC 配置；缺失默认 1。 | “Three independent product facts” |
| “display设定window，那么console_control的数值我们不管；console此时永远总是softpc monitor cooked” | display=window 固定 `{W}` + monitor；不读取也不验证该键。 | 状态表及 `console_control=0|1` 段 |
| “app可以按需发送 setWindowTitle 的请求，它只会设定Window的Title；当不是window状态，这条请求会被静默处理成noop；console title永远不可以被设定。” | Window-only title API；Console title 不在 API 内。 | UX public API / Window mouse state 附近 |
| “window enabled和console enabled…改成…两个独立的mailbox。” | 对外是独立 Boolean 语义、内部原子 presenter-set snapshot；`{false,false}` 非 live set。 | “Presenter set” |
| “一个ux presenter可以只包含window presenter，也可以同时包含window+console presenters。” | `{W}`、`{C}`、`{W,C}` 三个 live set。 | “Presenter set” |
| “这个frame我怎么知道它是给console的还是给window的？…console frame mailbox + window frame mailbox?” | 独立 Window/Console frame mailbox；SoftPC 决定路由。 | “Console frame/output contract” |
| “mouse_capturable 全改成 mouse enabled…window相关。” | API 命名为 Window-only mouse enabled/release；不影响 Console。 | UX public API |
| “UX不创建、关闭、或选择 channel。SoftPC 从 host 取得 raw endpoint 后注入”随后改为“lib ux自己…负责创建和销毁 console…提供get console接口，让softpc去和lib host注册。” | 最新决定优先：UX 在 `console_enabled=true` 时创建/拥有 Console object；SoftPC 仅取得借用引用向 host 登记。 | `ux_presenter_get_console` 契约 |
| “同一时间只有一个console对象可以在host那边登记注册成功并真的使用。” | host 从 broker 创建到销毁始终恰好一个 Current Console Object。 | Invariant / host broker API |
| “同一时间必须有一个lib host注册的激活的console对象” | broker 创建要求 initial monitor object；无公开 unregister。 | host broker API / Invariant |
| “首先是要在lib host直接先切到softpc控制台的，然后再是让ux presenter去销毁它自己的ux console的。” | UX→monitor 一律 host replace 先、UX object 销毁后。 | “Reconciler-derived object order” |
| “cooked monitor…不接受任何预定义的热键，除非是windows terminal自己的热键。” | cooked monitor 只交付行；SoftPC 不把 CAP 注册为 monitor hotkey。 | SoftPC monitor command policy |
| “softpc控制台输入的命令应该和c+a+p是同一级别的。” | monitor `pause/resume/reset/stop` 变为 SoftPC FIFO lifecycle intent；只在适用状态接受。 | SoftPC composition / FIFO |
| “X…给input sink发送窗口已关闭的信号，然后不要做任何其他动作了。” | 精确事件为 `WINDOW_CLOSE_REQUESTED`：UX 阻止默认 close、只投递事件。 | Window X 段 |
| “window应该可以继续接收所有的指令，直到window真的关了。不要过度复杂化。” | 没有 closing filter；native Window 真正销毁前继续交付 Window events。 | Window X 段 |
| “window和console至少有一个是true，否则返回失败错误；调用方必须调用 ux_presenter的销毁函数才对。” | `{false,false}` 被拒绝；只有 terminal destroy 结束 presenter。 | “Presenter set” / UX lifecycle API |
| “endpoint 的旧事件必须带 epoch/generation。批准。” | host binding generation 丢弃旧 object 的 queued input；SoftPC run generation 处理旧 VM 输出。 | host replacement / Lifecycle FIFO and run generation |
| “lib ux不应该携带generation信息。” | `run_generation` 只在 SoftPC app envelope；不出现在任何 lib UX API/type/event/frame。 | Lifecycle FIFO and run generation |
| “你从lib ux拿到的event都没有run generation” | SoftPC UX binding 在 queue ingress 附加私有 input-generation；UX event 原样不变。 | Lifecycle FIFO and run generation |
| “cooked命令和Window热键…合并成一个队列…分清楚monitor输入还是ux presenter输入。” | 一条 SoftPC input queue，app envelope 标明来源/kind；host/UX 不解析命令。 | UX internals |
| “softpc后台总控thread…host console i/o worker…vm虚拟机…softpc主体控制线程” | UX control 是对象不是 thread；host 一 worker，UX 0–2 runner，SoftPC control 与 VM executor 分离。 | “Thread topology” |
| “仅当raw输入源被永久移除…才做 [INPUT_RESET]。” | 永久 retire 或 presenter destroy 才发 INPUT_RESET；普通 focus/切换不发。 | UX internals |
| “如果是切换到paused状态…保证有softpc控制台，然后ux console是disabled；…resume…确保ux presenter set正确恢复状态然后才是恢复vm状态。” | paused/stopped 导出 monitor + Console disabled；raw resume 先 UX ready、host replace、后 VM resume。 | 状态表 / reconciler path |
| “vm机器状态+配置+用户意图，才能推导出最终ux presenter的状态” | SoftPC 保存 config、frame route、FIFO intent、actual；纯 derive target，无第二 mutable desired state。 | “One SoftPC reconciliation path”“SoftPC composition” |
| “display=window…console永远总是softpc monitor cooked” | 已覆盖此前错误的 window/control=0 raw 设想。 | 状态表 |
| “vm图形模式 -> 文本模式…诸如此类，应该可以做成一个单元测试矩阵” | 稳态矩阵与 start/resume/pause/stop/X/frame-route test requirement 已进入 proposal。 | 状态表 / Deterministic concurrency verification |
| “这个测试是多线程…妥善构建单元测试” | completion-gated fakes、手动泵 queue、barrier 真并发边界测；禁止 sleep-based proof。 | “Deterministic concurrency verification” |
| “单一路径、简化处理、干净架构、清晰决策” | 单一 SoftPC reconciler；host/UX/VM 均只经 completion 推进。 | “One SoftPC reconciliation path” |
| “cooked 行不应靠 take_line 轮询。” | current object's generic sink 推送 copied cooked-line 到 SoftPC queue；无 take-line API。 | host Console API |
| “lib host从app启动开始就要负责这个维护工作。” | host broker 持有 native Console 生命周期、initial object、replace transaction、teardown。 | host API / Host internals |

## 交叉审计结论

截至本记录，所有上表最终 owner 决定均在当前 proposal 有一个且仅一个当前
规范位置。发现并在会话中修正的历史冲突为：

1. `display=window + console_control=0` 曾被误列为 UX raw；最终决定为固定
   monitor，已改正。
2. “目标 monitor 必先 paused/stopped”曾被写得过强；运行中的
   `display=console + graphics + console_control=1` 合法保持 VM running，已改为
   独立 reconciler path。
3. `run_generation` 曾表述得像会进入 UX；最终决定为 SoftPC-only ingress
   envelope，已改正。
4. 曾有 cooked command queue 与 UX queue 两队列表述；最终决定为单一
   SoftPC input queue，已改正。

后续实现/审查应以 proposal 的 “One SoftPC reconciliation path”、状态表、
Console-object API、Thread topology 和 Deterministic concurrency verification
为核对入口，并以本记录检索 owner 决策来源。
