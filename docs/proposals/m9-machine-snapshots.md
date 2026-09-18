# 整机快照：运行中请求保存、停止时加载、暂停后继续执行

## 原始请求与准入边界

> 我想实现新功能可以保存机器运行镜像，然后直接在console里面加载后直接resume。比如，我现在想测试win95安装，在很长一个流程的某个地方现在坏了，要reproduce的话 每次启动setup跑到损坏位置要非常久
>
> 没错！基本上就是pause机器的时候 所有运行状态必须一致 请你审计并写好proposal 产品设计和工程设计 然后准入任务

本提案以 `ea7e982` 为源码基线。此次交付是源码审计、产品/工程设计和任务拆分，
不是快照实现或恢复成功证明。用户随后批准：
“批准收口T六十二提交推送然后继续进行这个调研。”
T62 已在 `54b2009` 独立收口提交推送，之后准入 T63 S1。
活动状态只由 [CURRENT](../states/CURRENT.md) 记录；本文件保留设计及审计依据。
与未启用 NTVDM 异常 hook 分支无关，不夹带其修复或其他 TODO。

媒体合同后续原文：
> 如果是direct和readonly的snapshot，不需要保存整个磁盘；否则也需要保存软盘 硬盘的overlay快照。整个快照就是一个binary文件

共享组件边界后续原文（取代 S1 初稿的扩展设想）：
> 原则上不得修改lib；common只允许在vm增加读出机器状态和写入机器状态的两个接口

本仓库该 Common 机器接口层实际为 `common/machine`，不新增 `common/vm`。
Lib 零修改；Common 仅允许这两个状态读写操作及其在原有 executor 上执行所必需的
接线、测试和 manifest 更新。其他 Common 组件不改，不增加 save/load 命令、路径、
文件格式、媒体接口、快照专用事件或独立状态机。若此边界不足，先报告，不自行扩大。

最新 owner 决策（取代下述初始请求中的暂停保存设想）：
> init、paused 和 stop，我们都不允许机器保存快照。
> 写入机器状态，我们只允许在init、stop的状态下执行。
> 如果机器已经是 paused 或者 running 的话，我们也就不允许机器的状态的写入。
> 我们可以先设个，比如说一秒钟。
> 加载完成后是 paused，用户仍可 reset，效果与普通 paused 后 reset 一样。

Owner 已批准更新 proposal 并开始实施。普通 pause、单步、断点的精确停止语义不变；
只有显式保存请求允许继续执行到快照边界。Common 管生命周期，不理解 CPU 安全条件；
VM/Compat 管安全条件和单调时钟期限，不增加指令预算或第三个公共准备接口。

## 产品合同

目标是同一时刻的 CPU、内存、设备与媒体一致恢复，支持退出进程后再次加载。
不是截图、RAM dump、宿主进程 dump，也不是重新开机后自动重放安装步骤。

```text
SoftPC> save "setup-before-failure.spcs"
Machine saved and paused.
... 可以 resume 继续试验，也可以退出程序 ...
... 若仍在原进程，先 stop，再 load ...
SoftPC> load "setup-before-failure.spcs"
Machine loaded and paused.

SoftPC> resume
```

| 命令 | init / stopped | paused | running | lifecycle 转换中 |
| --- | --- | --- | --- | --- |
| save <path> | 拒绝：须先运行 | 拒绝：提示 resume 后再保存 | 请求安全停止，再导出，成功为 paused | 拒绝，不排队延期执行 |
| load <path> | 校验并加载，成功为 paused | 拒绝：提示先 stop | 拒绝：提示先 stop | 拒绝，不排队延期执行 |

- 保存的是到达安全点时的状态，不是命令输入瞬间的状态。准入拒绝不推进机器；
  已开始寻找安全点后，失败不回滚已执行的指令。目标以现有二进制 truncate writer
  直接覆盖；写入失败可以留下截断或部分的新文件，也可以毁掉旧同名快照。明确完成后
  才打印成功，保持现有空行及唯一 prompt 规则。
- 加载后不自动执行任何客户机指令。用户可先 debug 再 resume；既有 start/reset/stop、
  CAP、X、display 与 console_control 语义不因新增功能改变。
- 加载后的 paused 是普通暂停状态：resume 继续恢复现场，reset 放弃现场并走原有
  reset 完成后暂停的路径，stop 走原有退出路径；不新增 loaded-paused 状态。
- 首版要求相同快照格式、机器实现兼容标识和硬件配置/ROM。
  不承诺跨构建或跨平台迁移；相同机器配置的 x86/x64 包必须互通；不兼容在改动当前机器前拒绝。
  不用可变 HEAD 文案代替构建兼容标识，交付测试必须覆盖不匹配。
- DIRECT/READONLY 不保存磁盘内容，只保存媒体引用、模式、大小、内容指纹和几何；
  OVERLAY 保存基底引用/指纹以及全部有效差异块，软盘、硬盘均包含，空驱动器也记录。
  文件内容指纹用于拒绝旧内存配新磁盘，不能仅验证路径/大小/修改时间。
  所以 DIRECT 保存后磁盘又被写入时，旧快照一般会因内容不匹配而拒绝加载；
  Win95 安装反复回退必须使用 OVERLAY。READONLY/OVERLAY 的基底也必须保持匹配。
- 整机状态及所有 overlay 修改都放在单个二进制文件，不产生快照 sidecar。
  每份快照独立保存当前全部有效差异，不复制 Lib 内部 dirty-page 链表；不建立快照
  增量链、压缩、去重、后台保存或插件框架。
  安装程序可能在写盘中途被暂停：必须保存控制器缓冲与未完成命令，不能要求客户机
  文件系统已卸载，也不能偷偷把未完成命令执行完后再取快照。
- 宿主窗口/Console/鼠标捕获、剪贴板、线程、锁、句柄、CLI 行、debug 命令历史不是
  客户机运行镜像。加载后由现有 UI 路径重建，无自动捕获，monitor 可用。
  客户机输入控制器内已经接受的数据必须保留；不能拿“清宿主输入”清掉 8042/FDC 状态。
- 首版不能回滚外部串口/打印输出。存在这些已连接的外部副作用端点时明确拒绝保存，
  而不是承诺整机一致后遗漏它们。设备内部状态仍纳入清单，普通扬声器重建宿主输出。
  不承诺与外部世界的确定性重放；快照后的宿主输入和时间节奏仍可能不同。

## 当前源码审计：能复用什么、缺什么

这是子系统级可行性审计；不是“所有 static/global 均已覆盖”的宣告。
S2 必须完成实际构建选中状态的逐字段账本，才能允许后续实现宣称完整。

| 已查生产位置 | 事实与工程影响 |
| --- | --- |
| `src/common/machine/machine.c` 的 `common_machine_executor_event` | PAUSED 在 driver 回调里阻塞，debug 已有同一 executor 的同步 rendezvous；增加两个状态读写操作的必要接线，不新增 Common save/load 系统或 debug opcode。 |
| `src/mvdm/softpc.new/base/ccpu386/c_main.c` | CCPU_GR/SR/CR/DR、隐藏段缓存、interrupt map、single_instruction_delay 等状态分散；还有 simulate_level、jmp_buf 和指令局部上下文。debug 寄存器读写不是完整执行恢复接口。 |
| `src/compat/platform.c` | timer callback 累加 pending ticks；暂停回调并未证明停止该生产者。`set_runtime_heartbeat` 管 pacing，不等于停止 native timer。不能把保存耗时算成客户机后来需补发的全部时间。 |
| `src/compat/ccpu/lifecycle.c`、原始 `host_simulate` | 存在嵌套模拟与宿主栈返回。保留的 C 栈不能搬到新进程；必须证明新执行入口能从明确的机器状态恢复。 |
| `src/compat/memory.c`、原始 SAS/C-VID | host RAM 只是 SAS 的一部分；现有物理读写走总线，视频映射可能有副作用。不能把 debugger 的 bus read/write 当无副作用的整机导入导出。 |
| `src/mvdm/softpc.new/base/ccpu386/fpu.c` | 有栈/控制/状态访问入口，但 `setNpxTagwordReg` 本身不恢复 tag；还需检查栈项精度表示、TOS、异常及宿主舍入环境。不能只用现有 setter 拼凑。 |
| `src/mvdm/softpc.new/base/system/quick_ev.c` | q/tic 队列含函数指针、param、句柄及链表；保存语义事件标识、顺序和相对期限，重建指针，保留关联句柄数值。未知回调必须拒绝保存，不能忽略。 |
| 原始 `ica.c`、`timer.c`、`at_dma.c`、`fdisk.c` 与 FLA/GFI | PIC 请求/屏蔽/服务状态、PIT 编程、DMA、磁盘命令缓冲和阶段不等于 RAM；需要原所属文件的窄状态出口。 |
| 原始 video/keymouse 与 `src/compat/cvidc/gdp_state.c` | VGA planes/latches/palette/banks、输入协议状态和 GDP 中标量/指针混合；禁止把 GDP slot storage 直接整体保存。 |
| `src/compat/hdd_media.c`、`gfi_image.c` | 两类媒体有各自独占 lease 与设备几何/当前 cylinder。要统一取同一快照边界，不改变原始控制器实现。 |
| `src/lib/storage/medium_interface.h`、`medium.c` | 已有 read_at、write_at、open(OVERLAY) 和 lease replace；可比较有效视图与只读基底保存差异，再在新 overlay 上写回。不需导出内部页，也不需改 Lib。Win32 只读打开允许 READ sharing；DIRECT 指纹使用原有独占 lease。 |
| `src/lib/storage/file_interface.h` | 复用二进制 writer 和媒体读取接口。原子发布/拒绝覆盖不是现有 writer 的已证能力，S3 须在不改 Lib 的边界下验证；有缺口就提交设计问题，不绕过或偷偷扩展接口。 |

结论：可沿现有唯一 executor 设计，但并非窄小补丁。最大未知是可序列化暂停边界，
其次是原始设备私有状态完整性。先证明这两点；不能以“保存文件成功”替代恢复证明。

## 工程设计

### 1. 唯一所有者，不新增执行器

```text
Common 既有 command provider -> App save/load + 既有 Lib 文件 I/O
                                 -> Common machine read_state / write_state
                                     -> 既有 executor -> 注入的 VM 状态读写
                                                         -> MVDM/Compat
结果：接口 status + 既有机器状态/完整帧通知；不新增 Session 快照事件
```

App 解析路径、管理文件读写与完成文案；composition 仍是唯一 VM 组装入口，
命令代码不直接访问 VM/Compat。现有 command provider 处理返回结果，Session 不修改。
Common 的两个操作为 read_state/write_state（工作名）：只传递不透明状态字节及结果，
不接收文件路径，不解释格式。字节传递采用有界缓冲或读写回调，具体签名在前审确定；
不增加 begin/end/size/free 等一组管理接口，不提供通用“任意任务执行”逃逸口。
读操作仅在 running 准入，写操作仅在 init/stopped 准入；Common 内部目前以 stopped
  表示尚未启动的执行器，产品 init 区别由现有 App 状态维持，不新增 Common INIT 枚举。
两个操作映射到注入 driver 的对应能力，实际访问仍由唯一 executor 完成；必要的
请求参数/结果存储及唤醒复用现有串行 rendezvous，不直接从 control 线程读写原始机器。
回调不得重入 machine/session。Common machine 仅承担这些操作必要的状态准入、
代际隔离和现有状态/帧发布；不扩张 Session/UI/debug/xasm32，不加专属完成事件。
两个操作参与 Common machine 的生命周期：成功后由 Common 完成普通 PAUSED 状态及
既有通知，不能由 VM 私自改状态，也不能先执行普通 pause 来代替寻找快照安全点。
VM 负责状态格式、兼容校验、保存/恢复顺序；Compat 提供宿主计时/媒体资源屏障；
原始文件保留设备状态所有权，只增加必要的窄导入/导出/重建接口。
Lib 不认识 snapshot/CPU；KVM 不改。不得复制设备实现到 VM 或改造全局为第二套机器。

MVDM 必要功能性状态出口属于本候选明确提出的 port-ABI 范围；S2 先列确切文件/字段
和预估原始 diff 再实施。保持原始格式，最小化相对 OpenNT 的 diff，不做格式重排、
构建期转换或全局裸内存登记框架。Common 的两项 ABI 变化须在前审中写清并更新
其共享测试/manifest；`src/lib` 与 `test/lib` 保持原样，不补页遍历器或文件原语。

### 2. 保存专用安全停止，不改变普通暂停

现有 PAUSED 只证明 CPU 没继续主循环，不证明所有状态已可迁移。
S2 必须从所有回调入口（普通指令边界、HLT、debug、嵌套 BOP/模拟）确认安全点。
目标是在无未表达宿主 continuation 的边界停住，保证恢复通过新 C 栈正常取指。
不保存 jmp_buf/栈，不靠执行 reset 再覆盖寄存器。显式 save 从 running 开始，允许原
执行器继续执行直到边界；不接受 paused 保存，所以无需从 Common 暂停循环推进旧栈。
VM 在原执行路径检查内部停止条件，不递归启动第二个 CPU invocation，不占用客户机
调试寄存器、不写入断点指令，也不修改普通 debugger 的停止位置。

仅使用宿主单调时间：从执行器接受准备请求起，寻找边界期限为 1 秒；不含文件导出
耗时，不增加指令计数预算。超限后不导出，在下一个可响应检查点普通暂停并返回失败。
不强杀线程、不强制丢弃嵌套栈，不宣称不可取消的宿主调用也能在 1 秒内返回。
S2 必须核对阻塞调用、HLT 检查点和取消路径；尚未证明前不得宣称硬实时超时。
Common 不存 snapshot_safe，不轮询嵌套深度；只接收读写结果，维护现有状态和通知。

源码前审已确认三类入口：c_main 指令退休后但中断处理前、NEXT_INST、HLT 等待。
不能只用 depth==1 判定安全：前者还有待处理 trap/IRQ；HLT 已推进 EIP，但 halt 状态
仍由 C 控制流表达。应选择可重入的明确阶段并证明恢复，不能直接统一跳到 NEXT_INST。
嵌套 BIOS 调用保存局部返回现场；保存请求等待原调用自然完成，超限拒绝，不序列化
宿主 continuation。设备的未完成硬件命令仍需保存，不能因宿主栈已退出就忽略它们。

屏障完成条件：

1. 停止新宿主输入准入；界定已经交付给机器的输入与尚未交付的 UI 事件。
2. executor 到达可恢复点；所有设备回调在同一边界完成或其待续阶段已显式保存。
3. 暂停并等待 timer producer 的在途回调；保留边界前已接受 ticks/设备期限，
   保存期间不继续累加。保持现有暂停体验；若需要改变一般 pause 的计时处理，
   先在 S2 写清前后行为，不顺手改客户机时钟算法。
4. 媒体无宿主在途写者，设备内部尚未完成 I/O 不强制完成。暂停后每项序列化数据稳定。

这里停止的是新外部输入准入，不是先清空所有队列。寻找安全点期间，原始设备仍可
产生和消费内部事件；到达屏障后保存尚未执行的事件、计数和设备状态。定时器可以
周期性重入队，强行排空既可能永不完成，也会改变快照位置。产品命令/输入队列的
截止与归属和机器内部 q/tic 队列分别处理，不能用一个“清空队列”操作混为一谈。

恢复时重建 host clock 基准，不把跨进程/保存耗时补成 guest ticks；RTC/CMOS 的
客户机可见时间及待触发事件随镜像恢复。pacing、宿主计数器绝对值不入文件。
q/tic 队列恢复直接重建内部节点，不重新调用 `add_q_event_*` / `add_tic_event`：
后者会重新分配句柄，且零延迟会立即执行回调。文件保存每个活动节点的回调标识、
参数、event_type、time_from_last、original_time、原句柄及队列顺序；另存两个句柄
分配游标和实际剩余 q/tic counter。队头的 time_from_last 不是实时递减计数器，
不能单独用于恢复期限。同期限节点保持原顺序；free list、hash 链和宿主地址重建。
原始句柄分配会回绕；活动节点若重号，hash 链的原遍历顺序也须用节点索引保留，
不能按期限排序后重新插入 hash，改变后续取消的目标。
设备持有的取消句柄与队列一起恢复，加载期间不触发事件。回调集合和参数逐生产
调用点核对后编码，不能按函数地址或函数表位置自动编号；未知标识拒绝加载。
尚未交付的产品输入须在边界前归类为丢弃或完成，不把宿主 source handle 放入镜像；
已被设备接受的字节/IRQ/按钮状态逐字保存。加载清除旧 run 的宿主排队事件、debug lease
和执行计划，通过现有 generation 过滤，不让旧输入进入恢复后的机器。

### 3. 文件和媒体：单个二进制文件，显式基底依赖

`.spcs` 是固定顺序、无版本号的内部容器：RAM 大小、core 长度与内容、device
长度与内容、media 长度与内容、外层 resume entry。明确小端固定宽度字段；所有
长度在消费前检查，非法枚举、超限长度与截断均拒绝。禁止直接 fwrite
struct/pointer/CRT stream。不引入动态 section、反射或兼容协商。

CPU/RAM/设备分别编码；callback 用本构建的固定语义 ID 与已审核参数，关联指针用
对象 ID/偏移重建。可重建的宿主解码指针、函数表、绘制资源不保存，恢复后重新建立。
S2 实测纠正初稿：CCPU 的硬件 TLB 条目和替换位置属于应保存的机器状态，清空它
会使尚未刷新 TLB 的页表修改提前生效。其 FAST_TLB 的索引和物理地址表也为固定
整数/字节数据，首版一起保存；不为省约 1 MiB 另造重建算法或改变 test-register 路径。
硬件 latches、隐藏段缓存、待 IRQ 同样不是可随意重建的缓存。
同类实测：保存 `c_debug` 实际断点表和计数，不从 DR7 重新计算；原始任务切换
清除局部使能后并不重建该表，加载不能偷偷改变原有行为。延迟 NPX 异常还须保存
`NpxIntrNeeded` 和独立的 `NpxExceptionEIP`，不能只保存当前 `NpxFIP`。
SAS 的页类型数组、20 位回绕、虚拟化 selector 标量和 CPU/SAS 共享的 RAM
共同构成机器状态；SAS/C-VID 的函数向量和宿主地址只在目标进程重建。SAS scratch
只作临时搬运，恢复时重新分配并让 Video 指向它，绝不编码指针或未初始化的 scratch
字节。惰性读取的 `SHIFTROT_OF_UNDEF` / `BEYOND_MEMORY_VALUE` 配置会影响客户机
结果，须保留已解析值及是否已解析，或在容器兼容性中拒绝不同配置。
视频保存的是 guest plane/font/register/latch/DAC 等语义数据以及 C-VID 的固定字段，
不是 DIB/Window 像素、dirty rect、函数表、GDP 内部指针或宿主地址。恢复后重建绑定并
发布完整帧；未知 GDP slot 用法拒绝 capture，不把兼容载体内存当作可移植的结构体镜像。
CPU continuation 明确区分取指准备与 HLT 等待；HLT 已前移 IP，且须保存原指令
进入时的 pending trap，不能用当前 EFLAGS.TF 重新猜测。恢复复用原循环的相应
阶段，不重复扣 quick-event 计数或提前接受 STI 阴影中的 IRQ；不序列化宿主栈。

每个媒体 section 记录插槽、插入状态、模式、尺寸、几何和基底内容指纹。
DIRECT/READONLY 没有磁盘 payload；OVERLAY 逐块比较有效内容与只读基底，保存差异块
索引及内容，块大小写入格式，验证末块有效长度、索引范围和重复索引。未修改块来自
已校验的基底；曾写过但又恢复成基底内容的块可省略，恢复后的字节语义相同。
保存的是当前全部有效差异，不是自上次 save 以来的变化，也不维护第二套 dirty 标记。
无文件的零基底可显式记录零基底及尺寸；
其他内存基底必须有可重建表示，否则拒绝，不写无法解析的宿主地址。
只用现有 medium read_at 读取、open(OVERLAY) 新建、write_at 恢复差异；VM/Compat
编排媒体访问及指纹校验。差异扫描需要读全盘，但不建立第二套运行时 overlay 实现。
DIRECT 校验需在持有同一稳定媒体 lease 时进行；指纹可能需要读全盘，
但不把全盘写进快照。保存期间不能有另一个写者改变被校验内容。
文件发布由 App 负责，Common/VM 不拥有用户路径。owner 明确接受现有 truncate writer
的普通覆盖语义：写入失败时，同名旧快照可以已被截断或替换为不完整文件。App 不得把
它称为原子发布、排他创建或掉电持久性；仍不得在 App/VM/Compat 偷藏 Win32 文件调用。
所需空间主要是 RAM、设备和 overlay 修改页，预检不能替代实际写失败处理。

#### S3 corrected Storage boundary

The current public Storage surface is sufficient without expansion. Snapshot
loading opens the file as `LIB_STORAGE_MEDIUM_READONLY`, checks its existing
`byte_count`, and reads exact bounded chunks through `read_at`; it never uses
the whole-file `read_owned` helper. Saving uses the existing binary
`lib_storage_file_writer_open(..., TRUNCATE)`, `write`, and `close` calls.

The owner explicitly accepts the resulting ordinary overwrite contract: a
failed save may leave a partial target and may destroy a prior snapshot at the
same path. Therefore no temporary file, commit/abort protocol, atomic replace,
or Lib API is needed. This is not permission for App/VM/Compat to call native
file APIs or create a private file layer. VM retains the fixed little-endian
format and local media fingerprinting; Lib remains unaware of snapshots.

No overlay-page enumeration API is needed.  Compat already exclusively owns
the live FDD/HDD leases.  At the successful S2 barrier it can read each
effective 4-KiB page through the existing medium interface and compare it to a
separate readonly lease opened from the attached base path.  It writes only
different pages; direct/readonly record only path, mode, byte count and the
fingerprint.  On load Compat prepares replacement leases from the validated
base and page records, then commits them through its existing owner-local
lease replacement path.  This preserves the single runtime overlay model.

加载首先验证所有外部基底，缺失/改变则在修改机器前失败，不静默使用另一磁盘。
DIRECT 重新接原文件，保持直写；READONLY 仍只读；OVERLAY 重建内存 dirty pages，
不修改原文件也不修改快照文件。恢复到较旧 overlay 时替换整套页集合，不与当前页合并。
这不是磁盘回滚工具：不倒写 DIRECT 文件，不为它暗中创建工作副本。
快照加载后的运行依然遵守既有 stop/start/reset 媒体生命周期，不为快照增加另一套
持久分支；要回到保存状态就再次 load。恢复所挂载媒体与配置不一致时首版拒绝，
不隐式改 INI。用户保存的快照文件不随退出或 stop 自动删除。

### 4. 加载：校验/准备与提交分离，不宣称魔法回滚

先解析整个快照、校验所有状态约束和媒体，准备内存及工作文件，不改变当前机器。
仅 init/stopped 可加载；running/paused 必须先 stop。Common 必须确认旧执行已退出，
不能仅检查一个提前写入的状态值。当前架构是进程单机器，
不能启动第二个原始 CCPU 用于 staging；staging 是数据和资源，不是另一台 VM。

进入提交前取得上述屏障，并确保所有会分配/打开/验证的操作已完成。
优先让 commit 仅做已验证状态安装、指针重接和缓存失效，在唯一 executor 上完成。
不得重放设备端口写入来“恢复寄存器”，它会有命令/IRQ 副作用。
若仍存在不可消除的提交失败，必须明确进入 ERROR、禁止 resume、报告未完成恢复；
不承诺旧机器仍可用，不以 reset 掩盖，也不自动重试。普通文件/校验/准备失败保持旧机
处于原 init/stopped 状态，临时资源清理。S7 的故障注入证明每个边界。

写入状态成功后，Common machine 在该接口必要接线内更新 run generation，并复用
现有 PAUSED 与完整帧通知；App 从接口结果产生加载文案，不增加 load completion 类型。
VM 强制生成完整帧，
现有控制路径推导 UI，不等待未来 dirty 才看到画面。恢复的 paused Window 不捕获鼠标。
之后 resume 走原有 Console 交接和 Window 激活顺序，不能添加第二条 focus 路径。
必须建立真正可继续的唯一 executor 现场，不能仅设置 PAUSED 枚举。首次 resume 不得
经过 cold-run 的 driver.reset；用户显式 reset 仍必须执行原有 reset，不得在 driver
中用隐式 loaded 标志吞掉 reset 请求。

## 全量收敛账本与 S 拆解

有限审计域：基线 CMake 实际选中的 CPU、SAS、FPU、设备、Compat 和 VM 源；
以及 Common 的执行/输入/帧边界。S2 固定逐文件的 mutable globals/statics、堆对象、
在途回调和宿主 continuation 清单。每项仅可标记为：保存、重建（附理由）、宿主外部态
（附隔离策略）、未启用（附构建证据）。任何未知项或无法恢复的启用路径禁止最终收口。
不是把所有 498 个镜像文件都修改；新发现只补账本并调整对应 S 的明确范围。

下列行数是规划量级，不是源码核算承诺；均为生产 C/H 增删总量，不含测试/文档/EXE。
每个 S 开始前必须重新审计实际文件、预估原始 diff；结束按 Git numstat 报告实际值。

| S | 目标及边界 | 初步规模 | 退出证据 |
| --- | --- | --- | --- |
| S1 | 源码可行性、产品/工程设计；明确限制及全量审计域 | 0 生产行 | 当前报告与请求交叉审计、文档门禁；不声称恢复已实现 |
| S2 | 逐字段账本及运行中保存的安全停止/计时屏障；1 秒 VM 超时；限定两个状态接口的执行接线 | 数百行，须先报告实际文件和原始 diff | 原嵌套自然返回或超时失败；明确 CPU/HLT 恢复阶段；普通 pause/debug 不变；Lib 不改 |
| S3 | VM 状态容器/媒体差异，App 文件 I/O；使用已有 Storage | 数百行 | direct/readonly 无磁盘 payload；overlay 有效字节相等；基底变化拒绝；目标文件采用明确的普通 truncate 覆盖语义 |
| S4 | CPU/隐藏缓存/FPU/RAM 状态出口与恢复 | 数百行 | 非平凡 FPU/tag/TOS、分页/A20/段缓存、IRQ/shadow 及内存 roundtrip；不是只比较通用寄存器 |
| S5 | PIC/PIT/RTC/DMA、q/tic 队列、磁盘控制器待续状态 | 数百至千行级 | 待中断/待事件/半条 I/O 的恢复等价，回调参数和句柄重建；不能遗漏未完成传输 |
| S6 | 视频/键鼠及剩余启用设备状态；重建宿主绘制/声音资源 | 数百至千行级 | planes/latches/banks/font/palette 与 8042/InPort 保真；恢复即有完整帧；账本无未知设备 |
| S7 | Common machine 两个状态读写接口及必要执行接线+VM 单一恢复事务 | 数百行 | running 读、init/stopped 写；成功为普通 paused；超限读失败暂停；准备失败写保持原状态；跨进程重建 |
| S8 | App `save <file>`/`load <file>` 命令、帮助、既有 provider 结果/prompt 接线 | 百行级 | save 仅 running、load 仅 stopped（包含刚启动的 monitor）；命令矩阵、失败输出、paused debug/resume 与既有两类 display/console_control 路径 |
| S9 | Canonical width-free stream container and Storage reader | 百行级 | 删除 host/IA-32 位宽字段和 RAM+8 MiB/devices 4 MiB 人为限制；App 流式读取，VM 两遍计数后流式写入/有界 section 读取；恢复到 paused 只保留已有 Window、不因图形 frame 新建 Window；x86/x64 新格式互通 |
| S10 | 全量账本复核、安装长流程与回归、最终交付 | 测试为主 | 以下验收矩阵全通过，x86/x64 EXE，owner 手测后才关 T |

### T63 收口前：MVDM / VM / Compat 增量审计

在所有快照实现 S 完成、T63 收口前，必须对**本任务相对 T63 基线**的
`src/mvdm/softpc.new`、`src/vm` 与 `src/compat` 进行独立的实际 diff 审计。
这不是历史 OpenNT 镜像总差异统计，也不能以“x64 移植已有大量 diff”掩盖本任务新增量。

- 按文件报告新增/删除行、职责、对应 snapshot receiver 或事务阶段，以及是否为
  preserved-mirror 的 port-ABI hook；
- 每一个新增 MVDM hook 必须证明：所需状态不能由既有 Compat/VM 边界取得，且 hook
  仅导出或恢复原有机器状态，不另建 standalone 行为分支；
- 对本任务新增的 MVDM 逻辑，凡是本质属于 snapshot 编排、宿主资源重建、文件/状态
  格式、平台适配或产品事务，而非原始机器语义，必须优先归属 VM 或 Compat，并从镜像
  移出。目标是以正确职责边界自然减少镜像 diff；不得为了降低 diff 而搬走原始机器
  状态、复制机器逻辑，或引入第二条恢复路径；
- 审计 VM/Compat 是否重复实现了镜像已有逻辑；能回收至原始实现或已存在 Compat
  适配层的重复代码必须在收口前处理，不能仅记录为后续债务；
- 报告累计与各 S 的实际 numstat，并区分生产、测试、文档和固定 EXE；
- 未能证明必要性的 MVDM 增量、或无法归属到 VM/Compat 单一所有者的实现，阻止 T63
  收口。仅在该审计、验收矩阵及 owner 手测均完成后，才可关闭 T63。

### S6 receiver ledger

This ledger freezes the S6 receiver universe before implementation.  It does
not authorize a raw-structure copy: every `payload` row requires a fixed-width
field map and validation before it can enter the private archive.

| Receiver family | Disposition | Required boundary |
| --- | --- | --- |
| `keyba.c`, `keybd_io.c` 8042/keyboard controller | Payload (P4 implemented) | P4 encodes the 8042 FIFO in logical order, translation/scan-set, typematic/key-down, command/latch and held-key state. It rebuilds scan-code tables from the scan set, restores no host callback or pointer, and maps both pending `do_int(scancode)` and cancellable `allowRefill()` as queue semantic IDs. `keybd_io.c` native re-entry depth remains a safe-boundary invariant, never payload. |
| `mouse.c` InPort hardware | Payload (P3 implemented) | P3 encodes physical deltas/buttons, data/status/mode/address registers, edge baselines, the ID/diagnostic handshake and the finite startup-interrupt count in a fixed-width map. It restores fields directly without replaying port I/O or IRQ effects. |
| `mouse_io.c` DOS driver | Payload (P5 implemented) | P5 encodes the fixed-width semantic field map for cursor, motion, callbacks, handler segment:offset values, cursor backing bytes, saved callback registers, EGA register values and installed-state. `mm_handle`, EGA derived addresses and host cursor callbacks rebuild; no pointer or historical `MOUSE_CONTEXT` byte image enters the archive. |
| `ppi.c` speaker/PPI latches | Payload + rebuild (P2 implemented) | Preserve guest-programmed PPI gate/data state. PIT state is already S5 payload. P2 archives the port latch and both edge baselines in fixed width, restoring the host speaker on/off state without replaying a Timer gate transition. It does not preserve an audio task, event, phase buffer or native audio handle. |
| EGA/CGA/VGA/V7 registers, planes, latches, fonts and palette | Payload (P6/P8 implemented) | P6 encodes all four plane/font byte banks and the 256-entry programmable DAC. P8 adds the fixed register/index/attribute flip-flop/DAC cursor map and the two live C-VID latches. Restore replays original handlers then invalidates host rendering; no raw controller structure or renderer state is copied. |
| C-VID GDP slots and rule state | Rebuild except audited latches | P8 preserves only the two live scalar latches. GDP allocation, generated rule entry, function vector, pointer, scratch/screen route and dirty bookkeeping rebuild. Any future live slot that cannot be placed in a fixed semantic map blocks capture. |
| `nt_graph`, `dib_surface`, graphics console, KVM frame resources | Rebuild | Exclude DIB, dirty regions, Window pixels, host cursor backing, UI mailboxes and frame generations. Archive the V7 current-mode byte because it selects original host geometry; restore clears only the old host geometry cache, reselects the original painter and emits one ordinary complete frame. Standalone rebuild must not let the retired NTVDM fullscreen state skip DIB recreation. |
| `nt_sound`, `compat/audio` | Rebuild | Preserve only the guest PPI/PIT inputs above. Reopen/recreate host audio resources; no native worker/event/handle or host sound buffer is payload. |
| `com.c`/`serial.c`, `printer*.c`/`parallel.c` | Payload for virtual endpoints; reject external endpoints (P9 implemented) | UART/LPT registers, virtual host queues/buffers and all selected delayed callbacks are fixed semantic payload. A configured serial/printer output path rejects capture before any host output is serialized. No file handle, output path or external-world state enters the archive. |
| `quick_ev` callbacks from keyboard/COM/printer | Deferred semantic IDs | S5 already rejects unknown callbacks. S6 adds an ID only when the complete receiver payload and restoration proof exist; it must not blanket-allow the current callback pointer. |

### S6 P7: video-controller and C-VID reconstruction boundary

P6's plane/font bytes and DAC entries are necessary but do not by themselves
restore the active video device.  The selected V7 build keeps the EGA/VGA
register files in legacy bitfield structures and keeps C-VID's working state
in `GLOBAL_VGAGlobals`, which contains both semantic values and process
bindings.  Neither carrier is an archive wire format.

The fixed map therefore has three parts:

1. guest-visible EGA/VGA/V7 register bytes, selected register indexes, the
   attribute flip-flop, and DAC read/write cursor phase are payload;
2. the C-VID/V7 data latches that affect the next guest memory operation are
   payload;
3. C-VID plane/scratch/screen pointers, generated read/write/mark vectors,
   `sr_lookup`, renderer dirty fields and host drawing resources rebuild.

Restore must begin from a known controller baseline, replay the existing
register handlers in a documented order so they recreate chain/bank/mode and
rule routes, then install the saved latches and issue one complete refresh.
It must not replay guest port I/O, serialize a controller structure, or copy
GDP slot allocation bytes.  A field that cannot be placed in one of those
three classes blocks capture until its fixed semantic representation is
identified; it is not silently reset or treated as a cache.

### S6 P8: controller register replay

P8 implements this fixed receiver in the existing private device archive.
The preserved mirror hook converts the selected controller's semantic bytes
to and from the fixed map and deliberately calls the original sequencer, V7,
CRTC, graphics and attribute handlers on restore.  It uses the existing C-VID
`getVideo*`/`setVideo*` accessors for the two latches; it does not dereference
or copy the GDP carrier.  The hook then issues the normal complete refresh.
The archive has no product command, file format, Common API or Lib change.

### S6 P9: serial and parallel virtual endpoints

P9 completes the remaining selected COM/LPT receiver family without treating
the standalone virtual endpoint as an external device. `com.c` exports the
UART register and interrupt state; `printer.c` exports parallel port
registers, state and delayed-event handles. Compat separately owns the finite
virtual RX/TX and printer-buffer state. Queue callback pointers translate only
to semantic serial-receive/send and printer-out/ACK identifiers. A configured
output file is irreversible host state, so capture explicitly rejects it. No
file, `FILE` pointer, endpoint path or raw controller structure is archived.

每一实现 S 都是可构建交付：双宽度编译/全套与针对性测试、固定 EXE、完整 P 提交推送，
然后切换审计角色核对实际 commit 后收口。S8 前不暴露残缺 save/load，EXE 仍可验证
旧体验；不能把仅内部 roundtrip 的 S4 当成用户功能交付。逐阶段避免一次改所有设备。
S1 文档交付无需伪造重编译；已有 EXE 保持不变。

### S6 closure

S6 closes after the independent receiver review recorded in
[M9 T63 S6](../history/M9-T63-S6-device-archive.md). The frozen receiver
ledger has no unknown selected receiver: each is a fixed semantic payload,
rebuild-only host resource, or explicit external-endpoint rejection. The
result remains VM/Compat-private; it exposes neither a Common operation nor a
product save/load command. S7 is the separate, bounded Common/VM execution-
transaction step.

### S7 execution contract

S7 adds exactly two Common operations:

```c
lib_status common_machine_read_state(common_machine *machine,
    const common_machine_state_writer *writer);
lib_status common_machine_write_state(common_machine *machine,
    const common_machine_state_reader *reader);
```

`common_machine_state_writer` and `common_machine_state_reader` are copied,
bounded byte-transfer callbacks with caller-owned opaque contexts. They carry
no file handle, path, media, CPU, device or presentation type. The injected
driver receives the matching direction only through its specifically named
`begin_state_read`/`take_state_read_result`/`write_state` callbacks. This is a state-transfer contract,
not a generic executor-job interface.

Common admits read only from `RUNNING`, and write only from `STOPPED`. It owns
one synchronous request slot and one completion event, exactly as it already
does for paused debug and removable-media rendezvous. The worker/executor is
the sole driver caller. For read, Common asks the driver to begin its private
safe-boundary capture, then waits for its one executor completion. For write,
Common starts the existing executor only after driver preparation succeeds,
lets the driver restore at that executor boundary, and publishes the existing
ordinary `PAUSED` fact and complete frame on success. Common never tests CCPU
depth, pauses a device, interprets a section, or selects an image layout.

VM owns the one-second CCPU checkpoint deadline, clock stop/restart, image
capture/restore and `softpc_ccpu_entry` resume state. A read deadline/failure
returns a status after the VM has reached ordinary `PAUSED`; a rejected write
leaves stopped state unchanged; a successful write reaches ordinary `PAUSED`.
No new Session/UI fact is introduced.

### S7 canonical byte contract

The byte callbacks carry one VM-owned canonical stream, not any live archive
object. Its fixed header identifies the stream revision, machine configuration
and complete section count. Each section has a fixed
identifier and bounded byte length. Integers are written little-endian at
their declared 8/16/32/64-bit width; fixed byte arrays are copied verbatim;
every variable array is preceded by its element count and validated before
allocation or multiplication. No caller receives a pointer into the archive.

The stream uses one section for each already-reviewed semantic owner: CCPU
register/execution/debug/TLB/FPU maps, SAS map and its RAM/page-type bytes,
event records, every device map, video data and the CCPU resume entry. A
section is encoded by its owner from fields, never by `sizeof` of an existing
C structure. This excludes compiler padding, host-width values, function
addresses, CRT streams, native handles and all archive allocation pointers.
Unknown, duplicate, missing or oversized sections reject the whole incoming
image before restore. The VM first decodes and validates into a private staged
image; only then may it replace the stopped machine state. The codec does not
know a file path or write prompt text.

#### S7 P5: implemented CCPU/SAS canonical slice

The CCPU archive now has a private, callback-driven canonical slice for the
already-reviewed register, execution, debug, TLB, FPU and SAS owners. It
writes each scalar little-endian, writes only fixed byte arrays verbatim, and
allocates decoded RAM/page-type/TLB-index backing only after reading the
declared sizes. The slice deliberately leaves `valid` clear after decode: a
device section is still mandatory before an archive can be restored. It is not
an App-facing save format or a partial machine-state operation.

`checkpoint_smoke` captures a real private image, serializes and decodes this
slice, then verifies register/execution/debug/TLB/FPU metadata plus complete
RAM, page-type and fast-TLB-index bytes. The test runs at both host widths.

## 验收矩阵

#### S7 P6: implemented device canonical slice

The reviewed device archive now uses the same callback stream. A compact
field-table interpreter writes each fixed-width semantic field explicitly;
queue records are individually written as bounded `u32` values and reject more
than 65,536 pending records before allocation. The decoder constructs a new
private archive and exposes it only after the complete stream succeeds. This
remains an internal slice: media sections, container framing and the VM
transaction are still required before any public save/load operation exists.

#### S7 P7: implemented complete private image container

VM composes the private slices into one fixed-order image: declared RAM size,
bounded core, device and media payloads, then the outer CCPU resume entry.
There is no version, magic, section count or section identifier. Decode stages
all payloads and rejects an invalid RAM size, any malformed bounded payload or
resume entry before replacing an existing image. It remains VM-private: this P
adds neither the running safe-point transaction nor App file commands.

- 安全点到达后导出前后相同快照语义状态；导出期间状态/待事件稳定，不要求与请求瞬间相同。
- 全状态命令矩阵；嵌套自然返回、1 秒超限、HLT 无退休指令仍检查期限；普通单步不越过断点。
- 在进程 A 保存，退出；进程 B 加载。改变分配地址/正常 ASLR 下工作，不靠固定地址。
- DOS 提示符、实/保护/V86 模式、Win3.1 图文转换、Win95 Setup 中间阶段；加载后
  resume 能继续而非重跑 BIOS。安装介质由用户提供，仅用明确拥有的副本测试。
- OVERLAY 运行并修改 RAM/磁盘，再加载旧快照；二者同时回到保存状态；重复加载同一快照
  至少多轮，无跨 run IRQ1、timer backlog、卡键、媒体内容漂移或旧帧。
- DIRECT/READONLY/OVERLAY、FDD/HDD/空驱动器；媒体源后来改变/不存在明确拒绝。
  保存/加载不写原媒体或 INI；DIRECT 恢复后客户机正常写盘不受该断言约束。
  分别验证 direct/readonly 无磁盘 payload、overlay 恢复全部修改页及删除后来新增页。
  外部串/并口端点有清楚拒绝路径。
- PIC pending/in-service、PIT/RTC、DMA、FDC/HDD 传输缓冲、keyboard FIFO、VGA latch
  以及非空 x87 栈逐项覆盖，不能只靠引导冒烟。
- 截断/损坏/重复 section/超限长度/兼容标识不符、分配/打开/读写失败；失败不打印成功，
  不 resume 半恢复机。fault injection 和 controllable barrier 不靠 Sleep 猜竞态。
- `load -> debug -> resume`、`load -> pause/resume -> stop/start/reset`、Window CAP/X
  和 Console 交接均保留现有体验。VM 加载完成前没有 UI 自行启动机器。
- 单独证明 `load -> reset` 与普通 `paused -> reset` 相同，以及 `load -> resume` 不冷重置。
- 同一快照可跨 x86/x64 加载。镜像统一使用 IA-32 的 32-bit machine-word 标记；
  payload 仅使用固定宽度 wire 字段，宿主指针永不进入镜像。此前错误写出的 host-width
  镜像不兼容，必须重新保存。分别验证 x64→x86 与 x86→x64。完整回归测试和
  原始镜像 diff 账本及 Common 必要 manifest 更新；验证 Lib/test-lib 与其基线零差异，
  Common 除机器层两个接口必要接线/证明材料外零差异，既有边界门禁继续通过。

## 当前结论和停止条件

准入目标明确，不能承诺小改动：至少跨 Common 调度、VM、Compat、原始私有设备态。
第一阶段不修改生产文件、不改媒体。实现前遇到无法表达的宿主栈、未知事件参数、无法
原子安装的设备态、需要修改 Lib/其他 Common 组件或未解决文件安全原语，先给出证据
和设计修订，不用绕过检查
换取“能打开快照”。不需要在线机器状态迁移、第二执行器或改造 KVM。

### S7 P8: executor-owned state transfer

The driver binds the existing Common executor callback through one VM wrapper.
On a running read, Common calls only `begin_state_read`; VM starts the existing
one-second checkpoint. At the first outer CCPU boundary VM stops the producer
clock, captures and writes the canonical image, then invokes that same Common
executor callback. Common receives only its existing result and enters its
ordinary PAUSED wait. After the existing resume edge returns, VM disposes the
temporary image and restarts the clock. Timeout, archive, and writer failures
use the same result path and likewise leave an ordinary PAUSED machine.

For a stopped write, VM decodes the complete image into private staging before
Common changes lifecycle. The next executor invocation runs the original reset
only to rebuild original host resources, installs the validated image before a
guest instruction, then resumes the saved CCPU entry. Common's existing
paused-start edge parks it normally. A malformed stream leaves STOPPED
unchanged; no second reset path or persistent loaded-state behavior exists.

`snapshot_transaction_smoke` proves this real-driver contract: running read
produces bytes and PAUSED; corrupt stopped write rejects and remains STOPPED;
valid stopped write reaches PAUSED with a complete published frame; ordinary
resume/stop remain usable. Restore rebuilds the host video surface only after
both the video bytes and controller registers are installed. A graphics
route has no text fallback: if its painter is not ready at the restored
boundary, VM emits no frame until a complete graphics frame is available.
This prevents a text-shaped placeholder from creating a black, wrongly sized
Window after the user resumes. No host DIB, pixel allocation or window
resource enters the image.

### S7 P10: restore-completion callback boundary

The original reset and archive replay can invoke the driver's existing executor
callback before the restored CCPU entry has begun. VM suppresses only those
callbacks while its private restore transaction is active. It reopens the same
callback immediately before `softpc_ccpu_lifecycle_resume()`: that ordinary
re-entry callback is still required to reach Common's requested PAUSED wait,
publish the completed restored frame, and return from the parked executor.
This is one VM-local Boolean boundary, not a Common lifecycle state or second
completion channel.

### S7 P11: fresh-process staged decode

Staged decode must not inspect live SAS state: a stopped target has not yet
performed reset and therefore has no initialized CCPU RAM to compare. The
canonical header's declared RAM is first required to equal the target machine
configuration. The core then requires its SAS RAM and deterministically
derived page-type length to equal that declaration before allocating or
starting restore. The existing post-reset SAS restore remains the final live
state check. A two-process CTest saves, exits, creates a fresh stopped target,
loads, verifies its complete PAUSED frame, resumes and stops; a mismatched-RAM
decode is rejected before the restore transaction.


## S10: 软硬盘 overlay 完整交付（已准入）

Owner 明确指出：没有 overlay 内容的快照不满足原始整机恢复目标，因此
T63 不得收口；撤回尚未提交的收口及 T64 准入。S9 的显示恢复验收不替代
磁盘一致性验收。当前唯一实施任务见 [CURRENT](../states/CURRENT.md)。

先审计所有 FDD/HDD 媒体所有者、模式、lease、源身份以及容器读写入口，
报告文件/行数估算，再实现。复用已有 Storage 读写/替换能力；不另造 overlay，
不修改用户源镜像，不新增 Common API。

| 收敛单元 | 完成标准 |
| --- | --- |
| 所有软盘/硬盘槽位 | 每个槽位的媒体模式、配置身份、容量及 overlay 内容均有明确记录与恢复负责人 |
| OVERLAY 保存 | 与 CPU/设备同一安全暂停边界，修改内容进入同一个 binary，无 sidecar |
| OVERLAY 恢复 | 恢复保存时的完整有效视图，替换而非叠加后来修改；空 overlay 同样正确 |
| DIRECT/READONLY | 保持外部源引用语义，验证匹配条件；不声称 DIRECT 外部写入会回滚 |
| 容器与失败 | 长度/范围/源不匹配及截断明确失败，不把缺少媒体内容当完整成功 |
| 验证 | FDD/HDD 均有变更、无变更、跨进程和跨宽度测试，恢复后 resume/reset 正常 |

完成后双宽度完整构建、测试、提交推送并供 owner 手测。T63 整体收口必须
重新核对原始请求，不能把此项再以设计证明或延期记录替代实现。

### S10 实施前源码审计

已确认 `vm/snapshot_image.c` 只有 CORE、DEVICES 两段。`vm/driver.c`
在加载时先调用 `softpc_machine_reset()`；后者重新 attach 两类媒体，销毁
此前 overlay。因此即使 CPU/画面恢复正确，当前格式也不具备磁盘检查点能力。

| 所有者 | 当前事实 | S10 处理 |
| --- | --- | --- |
| Compat `hdd_media.c` | 两个槽位，产品只配置第一个；保存 medium/sector count，未保留 mode | 保存各槽位存在性和配置模式，复用原读写路径，禁止创建第二磁盘运行时 |
| Compat `gfi_image.c` | MAX_DISKETTES 槽位，产品只 attach A；保存 mode、几何、当前 cylinder | 归档插入状态与当前柱面；空槽明确记录，恢复不遗漏后端柱面 |
| VM `machine.c` | 冷 reset 重建媒体；换盘更新自身 floppy_path | 校验当前配置，恢复在 reset 后安装新 lease、设备/CPU 恢复前完成 |
| Lib Storage | OVERLAY 保留只读基底和稀疏页；已可 read/open/write/replace | 不改 Lib，不读取私有 page 链；差异比较和快照编码归 Compat |
| Win32 Storage | OVERLAY/READONLY 允许共享只读；DIRECT 不共享 | overlay 可另开只读基底；DIRECT 校验复用已有 live lease，不能重新打开导致共享冲突 |
| 原 device archive | 包含 HDD 控制器，但没有 Compat GFI cylinder 或任何媒体数据 | 保留控制器所有权，只补媒体后端状态，不能把寄存器归档当磁盘数据归档 |

实现采用一份 Compat 媒体归档算法。媒体对象不暴露给 Common/App；VM 把
媒体归档与现有 core/device image 组合。保存扫描发生在现有 CPU/timer 安全
屏障之后，一秒期限仍仅限制到达安全点，不拿磁盘扫描时间当执行超时。
文件只保存相对基底的有效差异块及完整基底内容指纹。读取对块索引/顺序/
长度/重复、媒体模式和基底匹配进行验证，构造新的 overlay 而非合并旧页。
DIRECT/READONLY 无磁盘 payload。不得以旧的仅两段格式返回完整加载成功。

预计生产涉及 8–12 个 VM/Compat/构建文件，新增约 450–750 行；测试约
250–400 行。这是实施前估算，不是最终 diff。不计划修改 Lib/Common/MVDM。
验证必须同时包含 FDD 与 HDD 实际字节；现有 snapshot transaction 的
overlay 配置没有写入后核对媒体内容，故原 105/105 不能证明此项已实现。

### S10 实施结果（待 owner 手测）

实现将 MEDIA payload 置于固定 container 顺序；没有版本字段或旧格式协商，
因此不猜测或混合缺少媒体内容的旧 snapshot。四个固定后端槽位（FDD 0–1、HDD
0–1）有统一 archive；当前产品只配置 FDD 0、HDD 0，其余必须显式为空。
每个非空槽位记录存在性、模式、长度、基底 SHA-256、FDD 当前柱面和按固定 4 KiB
排列的不同块；块大小不写入镜像。几何由原后端从已记录的长度
按既有规则重建，避免存两份可漂移的几何真相。最后一块写实际长度；读取拒绝
乱序、重复、越界、错误末块和截断。

OVERLAY 读取其有效视图并与新开的 readonly 基底逐块比较，只编码差异；恢复
先校验基底、构造新 overlay，再在 reset 后经原 FDD/HDD owner 置换 lease，故
后来写入不会与恢复页合并。DIRECT/READONLY 仅记录并校验外部内容，不复制整盘；
DIRECT 在运行期使用现有独占 lease，reset 后再验证实际重新挂载 lease。没有改动
用户镜像、Lib、Common 或 MVDM。

新增独立 `media_snapshot_smoke`：SHA-256 标准向量、跨 4 KiB 边界、末块、
FDD/HDD 替换、空 overlay、三种模式、基底变化、所有截断前缀及重复索引。真实
VM 事务和跨进程测试均向实际已挂载的 FDD/HDD overlay 写入不同内容，保存后追加
错误页，load 后证明保存页恢复、追加页消失，并检查 FDD 柱面。最终 x86/x64
各 106/106 通过；正式 package 已在提交前重建。Owner 手测接受前，S10 与
T63 均不得收口。

### S11：Window 存在时屏蔽 raw Console 鼠标

`display=console, console_control=0` 的图形路由会同时保留 raw VM Console
与 Window。当前两者都把 `KVM_EVENT_MOUSE` 交给 Common Session，因此 raw
Console 的鼠标可移动同一客户机鼠标、并在 Window 像素帧中可见。Owner 要求此
路由在 Window 显示时只接受 Window 鼠标。

唯一策略点是 `common/ui` 的输入转接：它在创建各 KVM leaf 时保存私有的输入
来源标签。Window 存在时，来自 VM Console 的鼠标事件直接以成功状态消费，不
进入 Session；Console 键、文本、热键、关闭与退休事件和 Window 全部输入仍走
既有 copied event sink。Window 销毁后，raw Console 鼠标立即恢复原路径。
这不读取 KVM 私有对象布局、不扩展事件 ABI，也不让 Session/VM 认识 surface
策略。组合测试覆盖 Window/Console 鼠标、Console 键/热键及销毁 Window 后的
恢复。x86/x64 全量测试各 106/106 通过；package 交付等待 owner 测试。
