# Win3.1 MS-DOS 提示符显示模式往返异常修复

## 来源与状态

从 T55 S11 遗留的高优先级显示/兼容层 TODO 提升为队列候选。当前所有者
要求补充新的触发对照、提升到队首并准入调研。缺陷仍未修复。

## 已知复现与对照

1. 在 Win3.1 图形界面直接打开 MS-DOS 提示符；宿主处于 Window 显示模式。
   也可先进入客户机全屏文本状态、再恢复为 Window。
2. MS-DOS 提示符窗口内的字符残留，行/区域没有正确清理。
3. 在同一提示符中运行 `edit` 时，编辑器画面干净；退出 `edit` 后，提示符
   后续画面也持续保持干净。

普通 DOS 下 `cls` 已通过用户及原生包测试；而且 Win3.1 下运行 `edit`
能够改变后续结果。因此不能把此问题等同于通用 CLS 清屏故障，也不能用
“强制清屏”掩盖问题。优先调查文本 surface、dirty/完整帧发布、Window
路由与 Edit 触发的显示状态变化之间的失配。这里的全屏是客户机显示状态，
不是要求新增宿主无边框全屏功能。

## S1 第一轮结论：Compat 文本 Console 模型缺口

最早的已证实失配在 `src/compat/graphics_console_compat.c`，不在 Lib、
Common 或 presenter：

- 原始 `nt_graph.c::nt_clear_screen()` 与 `prepare_surface()` 已在文本模式调用
  `GetConsoleScreenBufferInfo()`、`FillConsoleOutputCharacter()` 和
  `FillConsoleOutputAttribute()` 清除整张 Console 画面。
- Compat 的两个 `softpc_compat_fill_console_*()` 目前返回成功、报告全部完成，
  却不写入 `textBuffer`；`textBuffer` 正是 `nt_cga.c::nt_text()` 与 VM frame
  适配共同使用的唯一文本 surface。
- 因而清屏操作被静默吞掉；VM、Common、KVM Window 与 raw Console 只会忠实
  复制同一份残留 surface。这解释了两条 presenter 路径都出现花屏/残字。
  `edit` 的完整文本重绘随后覆写该 surface，故能掩盖缺口。
- 同一 Compat 的 `GetConsoleScreenBufferInfo()` 错把 `sc.PC_W_Width/Height`
  （像素几何）报告为字符格尺寸。现有 fill 为 no-op 时这个错误被遮蔽；修复
  fill 前必须将它改为 text surface 的可见字符格合同，不能把像素数当作
  80xN 文本 cell 数。

这是一项独立宿主 Compat 修复，不修改保存的 MVDM 镜像，也不引入对真实宿主
Console 的旁路。`ScrollConsoleScreenBuffer()` 与 `WriteConsoleA()` 同样是
“成功但无操作”的不完整模拟；当前原始 `nt_scroll_up/down()` 均已提前返回，
故不把 scroll 泛化实现冒充为本缺陷的必要修复。实施前应逐一核实仍可达的
调用者，只补确有消费者的语义。

## 拟议调查与修复边界

- 先用上述精确流程复现，并保留普通 DOS 清屏与 Win3.1 `edit` 往返作为
  对照。
- 按同一帧追踪原有机器输出、VM 适配后的完整帧、Common 路由和
  Lib Console/Window 实际绘制，定位最早出现失配的边界。
- 区分源画面错误、已发布帧错误、显示模式路由错误及宿主缓存/绘制错误。
  在证实的责任层做最小修复，保留单一画面状态所有者，不增加刷新旁路。
- 修复仅可在 Compat 的 standalone text surface 模型内完成：以 text surface
  的字符格边界裁剪 fill，分别修改字符与属性，准确报告实际写入数量；错误
  参数或不可用 surface 必须失败，不能假成功。原始 MVDM、Lib、Common、VM
  不改。
- 不特判 CLS，不修改客户机镜像，不直接写宿主 Console，不引入强制 repaint。
- 与本缺陷无证据关联的焦点、输入时序、横向滚动视口等 TODO 不合并处理。

## 未来实施验收

冻结的验证范围包括：对 Compat 文本 surface 的字符/属性整面 fill、起始坐标
与边界裁剪、字符格 geometry；直接 Window 打开、全屏后恢复 Window、Win3.1
`edit` 往返、普通 DOS `cls` 对照及重复模式往返。验证画面、光标及输入
可用性没有因修复退化。
完成后编译双 EXE、执行 x86/x64 全量测试，记录真实模式往返验证结果，
提交推送供用户复核。无法复现时记录环境与观察，不声称修复完成。

当前 S1 已完成文本 surface 修复，正按下述第二轮结论修复图形帧发布边界；在
所有者复核前不关闭该任务。

## S1 第二轮结论：临时 DIB 被错误发布为完成图形帧

文本 surface 修复后，所有者在纯 Window 模式复现了更深一层问题：Win3.1
MS-DOS 提示符设为窗口启动时，KVM Window 在正常宽度和约两倍宽度之间反复
跳动；提示符窗口切到客户机全屏以及 Win95 Setup 的加载阶段也会花屏或跳宽。
追踪已证明这不是 KVM Window 的尺寸计算：在同一 graphics 模式内，VM 上游
交替发布 640x480 与 1280x480 帧。

责任边界在 Compat：原始 `graphicsResize()` 为绘制器准备 DIB destination 时会
短暂重绑几何。`softpc_standalone_dib_bind()` 把每次绑定都标成全幅 dirty，等价
于把尚未由原始 painter 写入的零化临时 DIB 发布为真实画面。KVM Window 正确地
按这些源帧调整，所以不能在 Window 侧加过滤或尺寸特判。

最小正确修复的第一步已完成：DIB bind 只分配/替换原始 painter 的 destination，
并清除上一 destination 遗留 dirty。所有者的后续复现证明这还不是完整边界：
V7 hardware pointer 是 Compat 内的独立 overlay，移动鼠标会直接以 32×32 dirty
发布 DIB；palette 更新也可发布整幅 dirty。若它们发生在 bind 后、原始 painter
完成新 surface 前，仍会把临时 640/1280 DIB 当成完成帧。

最终合同是每次 bind 开启一个 private painter generation：只有覆盖整张 surface 的
原始 painter dirty 才打开它；此前 V7 overlay 和 palette 只能更新其本地状态，不能
发布。bind 的 generation 改变还必须使旧 pointer background cache 失效，避免把旧
DIB 像素恢复到同尺寸的新 DIB。快照重建已在完整原始刷新之后显式 `invalidate_all`，
保留它作为强制提交路径。这不改变 MVDM、Lib、Common 或 VM，也不把 1280 宽度武断
视为错误；它只保证 overlay 或半幅原始画笔不会把未完成的 geometry 对外发布。测试
须证明 bind/partial-painter/overlay/palette 的准入、pointer generation 失效及显式
全帧发布。
