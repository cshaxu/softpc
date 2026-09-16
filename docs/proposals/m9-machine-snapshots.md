# 整机快照：暂停保存、跨进程加载、继续执行

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

## 产品合同

目标是同一时刻的 CPU、内存、设备与媒体一致恢复，支持退出进程后再次加载。
不是截图、RAM dump、宿主进程 dump，也不是重新开机后自动重放安装步骤。

```text
SoftPC> pause
SoftPC> save "setup-before-failure.spcs"
... 可以 resume 继续试验，也可以退出程序 ...
SoftPC> load "setup-before-failure.spcs"
Machine loaded and paused.

SoftPC> resume
```

| 命令 | init / stopped | paused | running / lifecycle 转换中 |
| --- | --- | --- | --- |
| save <path> | 拒绝：尚无运行状态 | 保存，成功或失败后仍暂停 | 拒绝，提示先 pause，不偷偷执行组合命令 |
| load <path> | 校验并加载，成功为 paused | 校验并替换，成功为 paused | 拒绝，提示先 pause；转换中不入队 |

- 保存失败不得改变机器语义或留下可误认为成功的目标文件；目标已存在时默认拒绝，
  不隐式覆盖。明确完成后才打印成功，保持现有空行及唯一 prompt 规则。
- 加载后不自动执行任何客户机指令。用户可先 debug 再 resume；既有 start/reset/stop、
  CAP、X、display 与 console_control 语义不因新增功能改变。
- 首版要求相同快照格式、机器实现兼容标识、x86/x64 宿主宽度和硬件配置/ROM。
  不承诺跨构建、跨宽度或跨平台迁移；不兼容在改动当前机器前拒绝。
  不用可变 HEAD 文案代替构建兼容标识，交付测试必须覆盖不匹配。
- DIRECT/READONLY 不保存磁盘内容，只保存媒体引用、模式、大小、内容指纹和几何；
  OVERLAY 保存基底引用/指纹以及全部 dirty pages，软盘、硬盘均包含，空驱动器也记录。
  文件内容指纹用于拒绝旧内存配新磁盘，不能仅验证路径/大小/修改时间。
  所以 DIRECT 保存后磁盘又被写入时，旧快照一般会因内容不匹配而拒绝加载；
  Win95 安装反复回退必须使用 OVERLAY。READONLY/OVERLAY 的基底也必须保持匹配。
- 整机状态及所有 overlay 修改都放在单个二进制文件，不产生快照 sidecar。
  每份快照独立保存当前全部修改页，不建立快照增量链、压缩、去重、后台保存或插件框架。
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
| `src/common/machine/machine.c` 的 `common_machine_executor_event` | PAUSED 在 driver 回调里阻塞，debug 已有同一 executor 的同步 rendezvous；没有 save/load driver 契约。复用调度所有权，不把快照塞进 debug opcode。 |
| `src/mvdm/softpc.new/base/ccpu386/c_main.c` | CCPU_GR/SR/CR/DR、隐藏段缓存、interrupt map、single_instruction_delay 等状态分散；还有 simulate_level、jmp_buf 和指令局部上下文。debug 寄存器读写不是完整执行恢复接口。 |
| `src/compat/platform.c` | timer callback 累加 pending ticks；暂停回调并未证明停止该生产者。`set_runtime_heartbeat` 管 pacing，不等于停止 native timer。不能把保存耗时算成客户机后来需补发的全部时间。 |
| `src/compat/ccpu/lifecycle.c`、原始 `host_simulate` | 存在嵌套模拟与宿主栈返回。保留的 C 栈不能搬到新进程；必须证明新执行入口能从明确的机器状态恢复。 |
| `src/compat/memory.c`、原始 SAS/C-VID | host RAM 只是 SAS 的一部分；现有物理读写走总线，视频映射可能有副作用。不能把 debugger 的 bus read/write 当无副作用的整机导入导出。 |
| `src/mvdm/softpc.new/base/ccpu386/fpu.c` | 有栈/控制/状态访问入口，但 `setNpxTagwordReg` 本身不恢复 tag；还需检查栈项精度表示、TOS、异常及宿主舍入环境。不能只用现有 setter 拼凑。 |
| `src/mvdm/softpc.new/base/system/quick_ev.c` | q/tic 队列含函数指针、param、句柄及链表；保存语义事件标识、顺序和相对期限，重建指针/关联句柄。未知回调必须拒绝保存，不能忽略。 |
| 原始 `ica.c`、`timer.c`、`at_dma.c`、`fdisk.c` 与 FLA/GFI | PIC 请求/屏蔽/服务状态、PIT 编程、DMA、磁盘命令缓冲和阶段不等于 RAM；需要原所属文件的窄状态出口。 |
| 原始 video/keymouse 与 `src/compat/cvidc/gdp_state.c` | VGA planes/latches/palette/banks、输入协议状态和 GDP 中标量/指针混合；禁止把 GDP slot storage 直接整体保存。 |
| `src/compat/hdd_media.c`、`gfi_image.c` | 两类媒体有各自独占 lease 与设备几何/当前 cylinder。要统一取同一快照边界，不改变原始控制器实现。 |
| `src/lib/storage/medium_interface.h`、`medium.c` | 已有有效字节 read_at 和独占 lease replace；overlay 为只读基底+4KiB页。现有公开接口不支持复制 dirty pages/读取基底视图；按新媒体合同需最小通用导出/重建能力，不暴露链表指针，不把整机格式放入 Lib。 |
| `src/lib/storage/file_interface.h` | 有二进制 writer，没有现成整机快照/事务容器。只在确有缺失时补最小通用文件原语；格式和机器状态不属于 Lib。 |

结论：可沿现有唯一 executor 设计，但并非窄小补丁。最大未知是可序列化暂停边界，
其次是原始设备私有状态完整性。先证明这两点；不能以“保存文件成功”替代恢复证明。

## 工程设计

### 1. 唯一所有者，不新增执行器

```text
app command -> common session -> common machine 的既有 worker/rendezvous
                                  -> 注入的 VM snapshot 操作
                                      -> MVDM 状态出口 + Compat 资源/媒体
                                      -> Lib storage 文件 I/O
完成结果     <- 同一 control queue <- 同一 executor
```

App 只解析路径、发请求并显示完成结果；composition 仍是唯一 VM 组装入口。
Common 仅增加产品无关的可选保存/加载操作与完成合同，不知道寄存器、磁盘格式、
设备列表；不另建一个 snapshot manager 或线程。优先扩展现有序列化请求通道，
不复制另一套 event/lease/轮询状态机；确需新增字段逐一证明职责。
VM 负责文件格式、兼容校验、保存/恢复顺序；Compat 提供宿主计时/媒体资源屏障；
原始文件保留设备状态所有权，只增加必要的窄导入/导出/重建接口。
Lib 不认识 snapshot/CPU；KVM 不改。不得复制设备实现到 VM 或改造全局为第二套机器。

MVDM 必要功能性状态出口属于本候选明确提出的 port-ABI 范围；S2 先列确切文件/字段
和预估原始 diff 再实施。保持原始格式，最小化相对 OpenNT 的 diff，不做格式重排、
构建期转换或全局裸内存登记框架。Common/Lib 若需公共 ABI 变化，也必须在对应 S 的
前审中写清并更新共享测试/manifest，不能混作私有补丁。

### 2. 可恢复的暂停屏障是第一项验收，不是附加条件

现有 PAUSED 只证明 CPU 没继续主循环，不证明所有状态已可迁移。
S2 必须从所有回调入口（普通指令边界、HLT、debug、嵌套 BOP/模拟）确认安全点。
目标是在无未表达宿主 continuation 的边界停住，保证恢复通过新 C 栈正常取指。
不保存 jmp_buf/栈，不靠执行 reset 再覆盖寄存器，不默默执行更多客户机指令凑安全点。
如果嵌套路径无法用窄接口表达，停止并修订设计，不能交付只支持偶然暂停点的正式 save。

屏障完成条件：

1. 停止新宿主输入准入；界定已经交付给机器的输入与尚未交付的 UI 事件。
2. executor 到达可恢复点；所有设备回调在同一边界完成或其待续阶段已显式保存。
3. 暂停并等待 timer producer 的在途回调；保留边界前已接受 ticks/设备期限，
   保存期间不继续累加。保持现有暂停体验；若需要改变一般 pause 的计时处理，
   先在 S2 写清前后行为，不顺手改客户机时钟算法。
4. 媒体无宿主在途写者，设备内部尚未完成 I/O 不强制完成。暂停后每项序列化数据稳定。

恢复时重建 host clock 基准，不把跨进程/保存耗时补成 guest ticks；RTC/CMOS 的
客户机可见时间及待触发事件随镜像恢复。pacing、宿主计数器绝对值不入文件。
尚未交付的产品输入须在边界前归类为丢弃或完成，不把宿主 source handle 放入镜像；
已被设备接受的字节/IRQ/按钮状态逐字保存。加载清除旧 run 的宿主排队事件、debug lease
和执行计划，通过现有 generation 过滤，不让旧输入进入恢复后的机器。

### 3. 文件和媒体：单个二进制文件，显式基底依赖

首版 `.spcs` 为一个有版本的容器：header（magic/version/build compatibility/host width/
配置/ROM 标识）+ 有界 section（ID/version/length/checksum）+ 完成校验。
明确小端固定宽度字段；所有尺寸在乘加前检查，重复/缺失 section、非法枚举、超限长度、
截断与不兼容均拒绝。禁止直接 fwrite struct/pointer/CRT stream。
只采用固定、可审核 section 清单，不引入动态注册或反射。

CPU/RAM/设备分别编码；callback 用本构建的固定语义 ID 与已审核参数，关联指针用
对象 ID/偏移重建。可重建缓存（TLB、解码缓存、函数表、宿主绘制资源）不保存，
恢复后失效并重新建立；硬件 latches、隐藏段缓存、待 IRQ 不是可随意重建的缓存。

每个媒体 section 记录插槽、插入状态、模式、尺寸、几何和基底内容指纹。
DIRECT/READONLY 没有磁盘 payload；OVERLAY 额外保存页索引及复制页内容，页大小写入格式，
验证末页有效长度、索引范围和重复索引。未修改页来自已校验的原始基底；保存的是全部
当前 dirty pages，不是自上次 save 以来的变化。无文件的零基底可显式记录零基底及尺寸；
其他内存基底必须有可重建表示，否则拒绝，不写无法解析的宿主地址。
Lib 只提供复制媒体页/基底读取的最小通用契约，VM 编排编码及指纹校验；不建立第二套
overlay 实现。DIRECT 校验需在持有同一稳定媒体 lease 时进行；指纹可能需要读全盘，
但不把全盘写进快照。保存期间不能有另一个写者改变被校验内容。
临时文件写完、关闭并校验后再通过文件发布原语产生最终路径；磁盘满/写失败不能覆盖
原有快照。所需空间主要是 RAM、设备和 overlay 修改页，预检不能替代实际写失败处理。
不把“原子发布”夸大为已实现掉电持久性。

加载首先验证所有外部基底，缺失/改变则在修改机器前失败，不静默使用另一磁盘。
DIRECT 重新接原文件，保持直写；READONLY 仍只读；OVERLAY 重建内存 dirty pages，
不修改原文件也不修改快照文件。恢复到较旧 overlay 时替换整套页集合，不与当前页合并。
这不是磁盘回滚工具：不倒写 DIRECT 文件，不为它暗中创建工作副本。
快照加载后的运行依然遵守既有 stop/start/reset 媒体生命周期，不为快照增加另一套
持久分支；要回到保存状态就再次 load。恢复所挂载媒体与配置不一致时首版拒绝，
不隐式改 INI。用户保存的快照文件不随退出或 stop 自动删除。

### 4. 加载：校验/准备与提交分离，不宣称魔法回滚

先解析整个快照、校验所有状态约束和媒体，准备内存及工作文件，不改变当前机器。
新进程 init/stopped 可加载；running 必须先 pause。当前架构是进程单机器，
不能启动第二个原始 CCPU 用于 staging；staging 是数据和资源，不是另一台 VM。

进入提交前取得上述屏障，并确保所有会分配/打开/验证的操作已完成。
优先让 commit 仅做已验证状态安装、指针重接和缓存失效，在唯一 executor 上完成。
不得重放设备端口写入来“恢复寄存器”，它会有命令/IRQ 副作用。
若仍存在不可消除的提交失败，必须明确进入 ERROR、禁止 resume、报告未完成恢复；
不承诺旧机器仍可用，不以 reset 掩盖，也不自动重试。普通文件/校验/准备失败保持旧机
暂停且可 resume，临时资源清理。S7 的故障注入证明每个边界。

成功后 Common 发布新的 run generation 和 paused/load completion；VM 强制生成完整帧，
现有控制路径推导 UI，不等待未来 dirty 才看到画面。恢复的 paused Window 不捕获鼠标。
之后 resume 走原有 Console 交接和 Window 激活顺序，不能添加第二条 focus 路径。

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
| S2 | 逐字段账本及可恢复暂停/计时屏障；现有 executor 的最小入口调整 | 数百行，需先锁定嵌套执行可行性 | 普通/HLT/debug/嵌套路径可重新进入；无保存期间状态漂移；不满足则停止后续 |
| S3 | 单文件容器、媒体引用/指纹、FDD/HDD overlay 保存/准备、限长校验和原子发布 | 数百行 | direct/readonly 无磁盘 payload；overlay 有效字节相等；基底变化拒绝，损坏/磁盘满/目标存在无覆盖 |
| S4 | CPU/隐藏缓存/FPU/RAM 状态出口与恢复 | 数百行 | 非平凡 FPU/tag/TOS、分页/A20/段缓存、IRQ/shadow 及内存 roundtrip；不是只比较通用寄存器 |
| S5 | PIC/PIT/RTC/DMA、q/tic 队列、磁盘控制器待续状态 | 数百至千行级 | 待中断/待事件/半条 I/O 的恢复等价，回调参数和句柄重建；不能遗漏未完成传输 |
| S6 | 视频/键鼠及剩余启用设备状态；重建宿主绘制/声音资源 | 数百至千行级 | planes/latches/banks/font/palette 与 8042/InPort 保真；恢复即有完整帧；账本无未知设备 |
| S7 | Common 可选快照请求+VM 单一加载事务，跨进程恢复 | 数百行 | 准备失败保留旧机，提交故障禁止执行，旧事件隔离；新进程恢复不依赖旧地址/栈 |
| S8 | App save/load 命令、帮助、completion/prompt、UI 衔接 | 百行级 | 命令状态矩阵、失败输出、paused debug/resume、两类 display/console_control；不改既有命令语义 |
| S9 | 全量账本复核、安装长流程与回归、最终交付 | 测试为主 | 以下验收矩阵全通过，x86/x64 EXE，owner 手测后才关 T |

每一实现 S 都是可构建交付：双宽度编译/全套与针对性测试、固定 EXE、完整 P 提交推送，
然后切换审计角色核对实际 commit 后收口。S8 前不暴露残缺 save/load，EXE 仍可验证
旧体验；不能把仅内部 roundtrip 的 S4 当成用户功能交付。逐阶段避免一次改所有设备。
S1 文档交付无需伪造重编译；已有 EXE 保持不变。

## 验收矩阵

- 保存前后相同快照语义状态；保存期间等待一段宿主时间后，状态/待事件仍稳定。
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
- 同一快照不接受跨 x86/x64，分别测试两种构建的独立保存/加载。完整回归测试和
  原始镜像 diff 账本、Common/Lib manifest/边界门禁必须同步更新。

## 当前结论和停止条件

准入目标明确，不能承诺小改动：至少跨 Common 调度、VM、Compat、原始私有设备态。
第一阶段不修改生产文件、不改媒体。实现前遇到无法表达的宿主栈、未知事件参数、无法
原子安装的设备态或需要不在此范围内的产品语义，先给出证据和设计修订，不用绕过检查
换取“能打开快照”。不需要在线机器状态迁移、第二执行器或改造 KVM。
