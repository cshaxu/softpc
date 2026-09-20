# T72 S5 后 Lib/Common 只读质量审计

Owner：“测试通过 本s收口 然后对lib和common进行代码质量审计”。
生产基线 1de4fadf；S5 收口 0aebc12e 只改文档。此审计不准入修复，
不关闭 T72，不改变已通过验收的 S5 数据布局。生产代码、测试、二进制零改动。

## 有限范围与证据强度

范围为当前八个 Lib 组件、五个 Common 组件的职责、接口及关键生产链。
覆盖单位是下表的组件边界，不是宣称每个汇编 opcode、每个 OS 失败组合都已证明。
处置为：确认缺口、可选精简、保留现有边界、明确未覆盖。所有发现列于下节，
由 owner 决定是否准入；不擅自恢复已退役的 TODO 台账或移动队列。

| 组件 | 核对内容与处置 |
| --- | --- |
| types | Header-only 词汇与布局；现有 ownership/layout gates 通过，不搬入业务逻辑。 |
| base | 两平台 event/wait/task 的所有权与取消；保留平台薄实现，不新建线程管理层。 |
| console | 引用、事件/输出屏障、generation、文本校验；不把其 Unicode ABI 强制改成 KVM cell。 |
| console-broker | replacement/rollback、停 reader 后释放、输出缓存和调色板；自身保留活 reader 的失败规则不能在 Common 被破坏，见 A。 |
| storage | stream 所有权、范围减法校验、direct/overlay；已有 O(n) 页查询候选仍由原队列承接，不冒充新发现。 |
| kvm-base | 独立 frame/control 锁、关闭准入、matcher、UTF-16/TEXT；TEXT 的后续断点在 Session，见 C。 |
| kvm-window | 完整帧消费、surface/damage、冻结/捕获/清理、文本循环；可删除一次必被覆盖的清零，见 E。 |
| kvm-console | 字形映射、完整帧确认、重绑定 reset、输入退出；不添加第二绘制路径。 |
| common/machine | executor、挂起请求终止、帧比较及请求串行契约；create 失败输出见 D。 |
| common/session | completion、prompt、事件派发及源退休；见 C/D，不重新拆状态机。 |
| common/ui | 组装、Console/Window 路由、说明帧和 teardown；见 A/B/D。 |
| common/debug | 公共入口、解析参数存储、输出和 continuation 生命周期；见 F，保留原命令风格/语义。 |
| common/xasm32 | 公共长度/容量包装、paragraph 收尾和现有测试；未逐指令穷举 imported assembler/disassembler，不声称全面无缺陷。 |

## 确认的问题

### A / P1：Common UI 销毁失败仍释放回调上下文

`src/common/ui/ui.c:185` 收集 kvm_window_destroy/kvm_console_destroy 的错误，
但仍继续拆其他对象，最后 lib_release(ui)。Lib 的 join 失败有明确语义：
worker 可能仍存活，不能释放其 context。Window 的 input_context 指向
ui->window_input，failure_context 指向 ui。

后台 mock 在现有 composition fixture 中令 Window destroy 返回 IO_ERROR，
拦截 ui 的释放（不实际制造 UAF）：结果 status=5、ui_released=1，
window_context_still_points_into_ui=1。这是故障注入证明，不是正常桌面崩溃复现。
同类 Console join 失败也不能继续释放公共上下文。

建议按实际释放成功清空子对象指针；无法确认 worker 静止时保留其依赖对象，
立即传播终止性失败。不得为此建立恢复状态机。App 当前忽略 UI destroy 返回值
是需要同时审核的最终接收边界，但本轮不修改 App。基线 T72 前已有此路径。

### B / P1：Common 自己生成的说明帧调色板全黑

`src/common/ui/ui.c:281` 清零整帧，设置前景索引7/背景0，却未填 text_palette。
`src/lib/console-broker/win32/console.c:594` 在宿主支持时实际应用这些 RGB。
因此 display=console、console_control=0 的图形模式说明帧可能黑底黑字。
探针直接调用原 status 构造函数：fg_index=7、bg_index=0、两者 RGB=000000。
未执行可见 Terminal/conhost 复现；不把宿主不支持 palette 的情况混为同样结果。

建议由 Common UI 这个说明帧生产者明确设置自己使用的可读颜色；无需恢复 PC
映射到 Lib、无需读取客户机调色板或新增资源对象。Composition 测试应检查最终
颜色对比，而不只检查两个索引不同。T72 前 status 帧已同样缺失 palette。

### C / P2：Session 静默吞掉 KVM TEXT

`src/common/session/control.c:271` 转发 KEY/MOUSE，但 TEXT 落入 return 1。
`src/lib/kvm-base/input.c` 对无法物理映射的合法 Unicode 确实会产生 TEXT。
探针提交 U+4E2D：accepted=1、delivered=0；相同 running 状态 KEY 对照 delivered=1。

建议 TEXT 与无 held-key 生命周期的普通输入一样，在 running 时送现有 machine
sink；paused 继续不转发。字符支持由产品 VM 决定。SoftPC VM 目前也只消费 KEY/
MOUSE，因此修 Common 不等于凭空使 DOS 支持 Unicode；这里只恢复中立传输边界。
现有 physical-key 测试未覆盖 TEXT。此断点在 T72 前已经存在。

### D / P2：Common 三个 create 的失败输出不符合统一约定

`common/ui/ui.c:151`、`common/session/session.c:221`、
`common/machine/machine.c:517` 将 options/driver 校验放在清空有效 out 指针之前。
传入有效 out 地址但 options=NULL，会返回 INVALID_ARGUMENT 且保留旧值。
UI 探针证实 status=1、output_null=0；另外两处源码有相同顺序。

建议统一先检查 out 地址、清空 *out，再校验其他参数；不改成功路径。
当前产品调用均预先置 NULL，本轮未证明正常启动因此故障。Lib 大多数 create
及 Common Debug 已采用正确顺序，不需要建立新的公共构造 helper。

## 可选精简，不是产品故障

### E / P3：Window 文本像素清零随后被全部覆盖

`src/lib/kvm-window/render.c:20` 在确认目标大小精确匹配后清零整个 surface；
后续 row/column/scan/bit 循环覆盖每一个目标像素，包括空字形的背景色。
可删这次清零，80x25、16扫描线时省一次 1,024,000 字节写入，无新状态。
需用预填非零 surface 和不同尺寸/字形测试证明完整覆盖；本轮没有修改或基准测试。

### F / P3：Debug 固定参数指针表仍有独立堆生命周期

`src/common/debug/command.c:46,643,2970` 的 arguments 固定256项，只指向同对象
command_copy；每次 open 分配，close 释放，另有分配失败检查。可内嵌数组，
删除分配/释放及重复状态判定，保留原 parser、命令实现和输出风格。
代价：未 open 对象也占该数组，x86/x64 约1/2KiB；open 时总有效存储基本不变。
应验证 open/close/reopen、参数上限和所有 continuation，不为几行减少改命令语义。

## 验证与限制

- 本轮重跑 `ctest --preset test-x64 -R '^(library\.|common\.)'`：63/63，46.59s。
- 相同 x86：63/63，47.49s。包括 manifest、DAG、负例、布局和故障契约检查。
- 这些现有测试通过不推翻上述 probe：composition 的 destroy mock 固定成功，
  status 只检查属性索引，输入 lifetime fixture 只构造 KEY，create 未覆盖脏 out。
- 两个临时 C 探针复用当前 control.c 和现有 composition mock；原生产函数不改，
  只拦截分配释放/失败返回，不启动真实窗口、不使用媒体。所有进程已结束，
  临时源码/EXE 在记录结果后清除。探针第一轮的 KVM_KEY_A 测试符号错误已改为
  现有字符键值后重新编译成功，不是产品编译失败。
- 未重编译产品 EXE，未执行桌面/RDP、原生 Linux 或完整 xasm opcode 穷举；
  S5 的双宽度105/105和 owner 手测证据仍保留，不冒充本轮新跑全产品回归。
- A--D 建议下次准入修复，E/F 由 owner 权衡；无需再做一轮大架构重构。
