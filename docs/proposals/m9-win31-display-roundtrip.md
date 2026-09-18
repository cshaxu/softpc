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
- 同一 Compat 还吞掉了原始 `SetConsoleScreenBufferSize()` 与
  `SetConsoleWindowInfo()`，并把后续 `GetConsoleScreenBufferInfo()` 固定伪装成
  `80x50`。原始 `textResize()` 在 Win3.1 全屏/窗口往返时正是借这组 Console
  操作改变逻辑文本 buffer；固定几何会让 fill 以错误行跨度写入共享 surface。

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
- 修复仅可在 Compat 的 standalone text surface 模型内完成：维护原始 renderer
  请求的逻辑 Console buffer 与 viewport；fill 以该逻辑几何裁剪、计数和走行，
  再映射到固定 `80x50` 共享 text surface 的可见交集。字符与属性分别修改，
  报告实际逻辑写入数量；错误参数或不可用 surface 必须失败，不能假成功。
  原始 MVDM、Lib、Common、VM 不改。
- 不特判 CLS，不修改客户机镜像，不直接写宿主 Console，不引入强制 repaint。
- 与本缺陷无证据关联的焦点、输入时序、横向滚动视口等 TODO 不合并处理。

## 未来实施验收

冻结的验证范围包括：对 Compat 逻辑 Console buffer 的字符/属性整面 fill、起始
坐标与边界裁剪、buffer/viewport geometry 以及映射到共享 surface 的行跨度；直接
Window 打开、全屏后恢复 Window、Win3.1 `edit` 往返、普通 DOS `cls` 对照及重复
模式往返。验证画面、光标及输入
可用性没有因修复退化。
完成后编译双 EXE、执行 x86/x64 全量测试，记录真实模式往返验证结果，
提交推送供用户复核。无法复现时记录环境与观察，不声称修复完成。

S1 已完成并保留文本 surface 修复。其后图形发布的两个提交曾试图通过来源
分类压住症状；所有者已拒绝该方向。它们不是完成方案；仍未收口的 T69 S1 将删除
并以统一事务取代。

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

S1 P2/P3 的错误在于，把“谁写入了 surface”变成发布决策：原始 painter、V7
pointer 与 palette 被拆成不同入口，`painter_ready` 又把首个 dirty 当成完成信号。
这既是来源特判，也解释了用户看到的残余跳宽和卡顿；它必须整体移除，而不是继续
补条件。

## S1 修订方案：唯一 display-update transaction

原始图形更新已有通用边界：`host_start_update()` 和 `host_end_update()`。所有正常
文本/图形更新在原始 `gfx_updt.c` 中以这对 callback 包围绘制；原始 NT host 的
`nt_end_update()` 恰为空，因此 standalone Compat 可以在不解释任何客户机模式的前提
下接上它。

唯一模型如下：

```text
host_start_update  -> 开始未公开的 staging transaction
bind / painter / palette / pointer mutation -> 更新同一 staging surface，合并 damage
host_end_update    -> 唯一提交点：发布当前 geometry + pixels + palette + damage
```

bind 从不发布，任何写入者也不能直接发布；没有 `overlay` API、没有
`painter_ready`、没有按 dirty 面积/来源/V7 模式决定可见性的条件。未发生 update 的
bind 保持私有。`host_flush_screen()` 已走原始 update 算法，因而仍由同一结束边界提交。
必要的镜像改动只能是原有 `nt_start_update()` / `nt_end_update()` 各调用一次 Compat
bridge；这是一项机械 host-ABI 接线，不变更客户机控制器、绘制排序或产品 API。

少数原始 host callback 可以在这对边界之外直接改变 surface，例如硬件 pointer 的
port write。`damage()` 对所有调用者一律采用同一规则：若已有 transaction，只累计；
若没有，则自己创建并结束一个单次 transaction。它不检查调用者、模式、尺寸或 dirty
形状，因此不是 pointer/palette 的发布特判；每次可见发布仍都由同一个 end 逻辑完成。

本轮同时审计目前 `check_win_size()` 中的 standalone V7 宽度分支。若它只是为掩盖
错误的输入几何，须删除并由统一 transaction/正确的原始几何得到答案；若原始模型
确实缺少该 host geometry，则必须以可证明的一般 host geometry contract 取代，不能
保留 mode-number 条件。

## S1 第三轮实施核对

实现收敛为一个 Compat transaction record：`pending dirty` 只在嵌套深度归零时并入
可消费 dirty；若前一完成事务尚未被 VM 复制，两个 damage 的外接矩形继续合并。这使
frame consumer 始终读取最新 surface，并覆盖自上次复制以来的全部变动区域。bind、
palette、V7 pointer 与 original painter 都只调用同一 `damage` 入口；不存在来源标签、
`ready` 位或 overlay 入口。

静态调用审计发现原始 `gfx_updt.c` 的 19 个 `host_start_update` 与 20 个
`host_end_update` 调用仍使用既有 callback table；其中 cursor 分支的额外 end 是在其
外层已开始的 transaction 内提前返回。Compat bridge 因而只接到原已有的一对
`nt_start_update`/`nt_end_update`，不复制或重新排序原始调用。所有 standalone DIB
mutation 的生产路径均归入该 transaction；测试中绕过 original callback 的 painter
直接调用则验证同一 `damage()` 的通用单次 transaction 规则。

`check_win_size()` 的 V7 `0x60..0x69` width 计算未作为本轮发布条件使用，也未新增。
现有 `vga_frame_smoke` 覆盖这些原始 controller mode 的 DIB 尺寸；它是已证明的 host
geometry 适配，暂不与本次 publication repair 混合改动。

## S2 新复现对照：首次窗口化初始化

S1 关闭后，所有者确认宽度跳变仍存在，但边界已显著收窄：只有 Win3.1 内 MS-DOS
提示符**首次以窗口化方式启动**时发生。把同一提示符切入客户机全屏、再恢复窗口后，
宽度稳定。这个对照排除“Window 只要收到 Prompt 帧就会跳”的解释，并优先指向首次
display-mode 初始化时建立的原始几何/cache/DIB metadata，而非 KVM Window resize、鼠标、
普通 dirty 更新或后续模式切换。

S2 将先记录首次窗口化与全屏往返两条路径的 completed surface geometry、stride、bind
顺序与 copied frame geometry；只在最早发生差异的 owner 修复。不得以保留最后宽度、
Window debounce、模式编号或 Prompt/PIF 名称判断来掩盖该差异。

### S2 当时的解释（S3 已纠正，见下文）

审计确认 Compat、VM copied-frame extraction 与 KVM Window 都不推导几何：Compat 仅绑定
原始 DIB，VM 逐字节复制其 width/height/stride，KVM Window 只绘制 frame。完整 surface 的
唯一宽高写者是镜像 `host/src/nt_graph.c::check_win_size()`；因此下游不存在应当保留的
“第二宽度事实”。

V7 的 `0x60..0x69` 专有 byte-painter 宽度必须取 `vd_video_mode` 与 V7 extension latch
所表达的**当前控制器模式**。此前 standalone DIB bridge 却以
`Currently_emulated_video_mode`（最后一次 BIOS 请求）选择该表。该变量在 Win3.1 首次
窗口化 Prompt 所采用的直接 controller 重编程期间可以与当前 controller 分离；于是同一
完成 surface 会交替经过标准 VGA 公式和 V7 table，产生 640/1280 两种错误 geometry。

修复把这份 V7 controller 解码收为 `v7vga_current_mode()`，由 V7 实现拥有；`ega_vide.c`
原有三份相同转换共同复用它，`check_win_size()` 也使用同一当前模式。它不是针对
Prompt/fullscreen/PIF 的分支，也不改变寄存器、BIOS、painter、dirty 或 KVM 发布路径。
`vga_frame_smoke` 将历史 BIOS mode 故意改为无关值后仍验证当前 V7 controller 的 640x400
surface，直接证明 geometry 不再从历史请求取得。后续人工路线仍须覆盖“首次窗口化”和
“fullscreen → windowed”两种 Win3.1 Prompt 路径。

### S3 文本 clear/roundtrip 的独立 host-state 缺口

审计证明 Standalone 的原始 Console API 调用已全部转接到 Compat 的一个 text surface；它不
存在第二个 native Console 文本副本。然而 `nt_clear_screen()` 仍保留原 NT host 的
`sc.ScreenState == FULLSCREEN` 早退。对 detached DIB/text host 来说，客户机的
fullscreen/windowed 只是客户机显示状态，不能决定是否把清屏写入唯一 text surface。

因此 standalone 仅移除该 native-host early return：所有状态继续调用同一 Compat
`GetConsoleScreenBufferInfo` / `FillConsoleOutput*` 转接；非-standalone 保留原行为。这不
改变客户机模式、清屏算法或 Console presenter，且不以 Win3.1、PIF 或任一模式编号判断。
VGA smoke 将 Compat text surface 置满、设置 guest `FULLSCREEN` 后调用原
`nt_clear_screen()`，断言 80x25 全部恢复为空格。它覆盖 Window/Console 两种 presenter
共用的机器文本输出事实。

同一 native-only `FULLSCREEN` guard 也包住原 Console cursor-size 更新。Standalone 的该调用
同样只更新 Compat cursor metadata，故采用相同规则：不因客户机 fullscreen 跳过它。测试先把
原 cursor cache 驱动到 hidden，再在 FULLSCREEN 下恢复可见 cursor，验证 metadata 更新；不新增
任何 cursor state、callback 或 UI 行为。

## S2 owner reopen: completed geometry still alternates (gate superseded by S3 P2)

The owner has now reproduced the initial-windowed Win3.1 MS-DOS Prompt width
alternation after the S2 package. The former closure is therefore incomplete:
the V7 historical-BIOS/current-controller mismatch was real but was not the
only periodic geometry source.

The audit began at the original renderer, not at KVM Window. The completed
frame trace proved that KVM was faithfully receiving `1280x480` followed by
`640x480`; the former is the original `80 * 8 * 2` generic 256-colour width.
The existing two-tick EGA/VGA mode-settle gate used one Boolean and therefore
did not restart when another controller write arrived during the wait. It could
publish that intermediate controller combination as a completed DIB.

S2 repairs the unique owner rather than filtering a consumer: the video core
keeps one private monotonic mode-change generation, and the existing host
settle gate restarts only when that generation changes. It selects geometry
only after the final controller update has remained stable for the original
settle interval. This is not a new timer, retry loop or timing policy: it
corrects the original controller transaction boundary. The focused VGA smoke
proves that a second controller invalidation between ticks keeps the gate
closed until two ticks after that second invalidation. No PIF, Win3.1,
application, mode-number, dirty-shape or downstream width condition exists.
Fullscreen roundtrip, initial Window and the Win95 Setup transition remain
manual evidence cases, not branches in production code.

## S3 admitted investigation after owner feedback

The owner closes S2 as an incremental improvement and admits S3. CLS now
clears correctly. Doubled-width excursions are less frequent but persist;
window-to-fullscreen remains corrupt, and return to window leaves malformed
text and newly typed characters invisible. See the
[S2 closure](../history/M9-T69-S2-video-settle.md) for the exact evidence limits.

The preceding explanation of the 1280-wide frame as a particular transient
256-colour combination is a hypothesis, not captured register evidence. The
settle change must also be reviewed for display starvation under sustained
register activity. No extra delay is authorized as a substitute for diagnosis.

S3 starts with a read-only audit, estimated production/test delta zero. Compare
VGA read/write mode, odd/even and chain addressing, latches and planar stride
with original source, following register writes through cached C-VID state.
Then distinguish incorrect video RAM from incorrect painter interpretation
and stale host publication. Retain the initial-windowed versus fullscreen-first
discriminator. Actual code estimates follow identification of the owner.

### S3 P1: packed painter geometry, not BIOS mode identity

The S2 description of `v7vga_current_mode()` as register-derived geometry was
incorrect. It reads the BIOS data byte at physical 0x449 (with a V7 latch for
extended text). It is suitable for the existing BIOS services, not for deciding
the physical painter's width. Changing only that byte in a 640x400 packed
display produced widths 1280, 752, 720, 800 and 1024 without any display-register
write. This is a deterministic host geometry defect; it does not yet establish
which bytes/registers Win3.1 changes during each reported excursion.

The existing painter selection already distinguishes standard VGA doubled
pixels from V7 sequential-chain4 packed pixels. `vga_graph_update()` passes
`get_bytes_per_line()` to that painter; `nt_v7vga_hi_graph_std()` emits exactly
one destination pixel per byte. The host therefore uses this same row width
when that packed painter is selected, deleting its BIOS mode-table override.
It checks the selected painter directly rather than duplicating register-based
selection; standard EGA/CGA/VGA painters retain their distinct pixel contracts.
No new mode identity, guest test, retry or downstream filter is added. Other
painters and the prior settle gate are unchanged in this bounded correction.

Estimated scope: one host bridge and one existing test; production net decrease.
Actual code: `nt_graph.c` +5/-9; `vga_frame_smoke.c` +17/-0. The regression varies
BIOS records 00h..1dh across the four packed modes, twice, without changing
registers, and verifies unchanged width; existing geometry and pixel tests stay.
The earlier x64 isolated probe returned 1280 for BIOS 12h/13h at the same
80 characters, 8-bit character width, 256-colour, seq-chain4/chain4 state.

This does not close S3: fullscreen corruption, windowed text corruption,
actual Win3.1 width excursions and sustained mode-write starvation still need
their own causal evidence. In particular, the earlier generation-delay test
proves rearming, not that two quiet ticks are a hardware transaction boundary.

Similar-issue sweep: `check_win_size()` remains the DIB-size owner; Compat bind
and VM copying are unchanged. The original non-packed BIOS-dependent fallback
is retained for now because CGA/EGA/text host scaling differs; this P does not
claim it valid for arbitrary direct register programming. BIOS-service callers
of `v7vga_current_mode()` retain their separate mode-reporting responsibility.
The exact current `nt_graph.c` versus OpenNT diff is +75/-20 (Git no-index),
four fewer added mirror lines than the previous bridge. No new source file,
state field, interface or timer was introduced.

Verification: both `tests-x64` and `tests-x86` builds succeeded, refreshing the
two package EXEs. Final VGA smoke passed independently on both. Final full
CTest runs each passed 106/108; `softpc-package-smoke` and
`softpc-package-compact-console` failed at Window lifecycle stage 16. No
controlled baseline comparison was made, so they are not dismissed as
unrelated. Documentation governance and whitespace checks passed. Owner INI
and HDD image were unchanged. S3 stays open; this is P1, not acceptance of
the remaining display-roundtrip symptoms.

### S3 P2: remove unbounded settle rearming

Source audit found that the generation-based restart added in S2 postpones
`choose_display_mode()` indefinitely when invalidation arrives on every tick.
Its test only asserted the new delay policy, not correct hardware behaviour.
A replacement regression issues 20 consecutive invalidations and requires the
original bounded two-tick selection. It failed on P1 at the second tick.

Restore the exact original OpenNT `nt_graphics_tick()` countdown branch and
remove the now-unused generation field, getter and declaration. No replacement
timer, retry, queue or state is introduced. P1's BIOS-independent packed width
remains. This proves bounded painter selection, not uninterrupted painting
under all register activity, nor that every reported Win3.1 pause is explained.

Estimated change: delete the S2-only generation mechanism in three production
files and replace its one test, net decrease. Actual production +7/-25 (net
-18); test +10/-12 (net -2). `gfx_upd.h` again has no textual OpenNT diff.
Similar-issue search found no remaining `mode_change_generation` reference in
src/test; the only retained mode-change gate is the original host countdown.

Verification: normalize CRLF only and compare the complete
`nt_graphics_tick()` against OpenNT: both 2457 characters, exactly equal.
Current mirror diffs are `nt_graph.c` +62/-13, `gfx_updt.c` +189/-145 and
`gfx_upd.h` unchanged. Both complete builds refreshed package EXEs. The new
VGA test passes on x86/x64. Full runs: x64 105/108, x86 106/108. Both retain
the two package stage-16 failures. x64 additionally failed
`machine_smoke.c:1048` (`bda_tick_low != 0`); the immediate isolated machine
and VGA rerun passed. This does not erase the failed full run or establish
its cause. Documentation gate and diff checks passed. No owner media/INI,
Lib or Common changes; S3 remains open for runtime display evidence.

### S3 post-P2 width audit

Owner feedback for `19533be`: neither initially windowed MS-DOS Prompt nor
the window restored after fullscreen now jumps width. This accepts those
observed width paths, not every display corruption symptom.

The bounded audit covers width selection, painter selection, invalidation
scheduling, Compat DIB binding and VM frame copying. P1 removes packed-width
dependence on BIOS bookkeeping; P2 restores the original bounded countdown.
Compat copies painter DIB geometry, and VM copies that geometry rather than
deriving it from damage rectangles. Existing x86/x64 VGA tests pass.

A temporary x64 probe refined the remaining chain4 hypothesis. Starting from
original mode 66h, unlock extensions through ports 3c4/3c5, select ERFC and
clear bit 5 (6ch to 4ch). Four normal host ticks each report seq-chain4=0,
pending-mode-change=0, width=640 and the existing packed painter. Only after
an explicit mode invalidation and the original two ticks does width become
1280 with the doubled VGA painter. Restore ERFC and reselect before continuing
the test. The probe was removed and the test target rebuilt afterwards.

This distinguishes a register write from a subsequent painter-selection
boundary: the earlier direct `choose_display_mode()` probe did not prove
that ERFC alone causes a visible resize. Source confirms ERFC updates CPU
read/write banking without requesting mode selection. `choose_vga_display_mode`
also accounts for the emulator's interleaved backing layout; therefore simply
removing chain4 from painter selection would be an unproven functional change.
No production repair follows from this probe. Remaining proof is a real driver
register sequence reaching that selection boundary, plus its CPU-access and
scanout interpretation. No PIF/mode exception, debounce, new renderer or
Lib/Common change is justified by the current evidence.
