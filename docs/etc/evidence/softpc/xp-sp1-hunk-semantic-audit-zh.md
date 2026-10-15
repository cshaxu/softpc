# XP SP1 SoftPC 逐文件语义审计

## T92 S1 源差异审计与迁移建议（2026-10-15）

当前任务状态以 CURRENT 为准。以下是重新读取固定 Git 对象取得的新证据；
本节之后的 T40 表格是历史研究材料，不再代表当前逐文件结论或导入许可。
原始目标：审计当前 softpc.new 与 XP 的差异，以 XP 为镜像基底升级；
IBM PC 能力升级要纳入，非 IBM PC 的能力差异继续排除。

### 有限审计范围与完成条件

- 固定当前提交 `087e512fc4bb02225d3d8cb519a12ef87aa489ac` 的 498 个保留路径；
  本次未改变这些文件。
- XP 固定 `a2f6d7c93aa4f11efc51147566d5961c59a8ef89`，其 softpc.new 有 997 个路径。
- OpenNT 固定 `5e4619ab61c2aa76151e03973cce340be2933e61`，对应目录有 1148 个路径。
- 以每个保留路径、每个 XP 新增源码候选和每个相关语义 hunk 为覆盖单位。
  每项最终必须有 proposal 四种去向之一、所有者、必要本地修正和测试接收方。
  文件内容相同只证明没有基线迁移差异，不证明所有硬件行为正确。
- XP 另外有 499 个当前未保留路径。它们不能被默认导入；需区分历史构建、
  非 IBM PC、NTVDM 宿主，以及应补入的 IBM PC 能力。完成条件是没有未解释的
  候选或丢失的既有修复，而不是仅取得一个能编译的子集。

### 已复算的内容关系

只把 CRLF 转为 LF 后比较，保留其他空白和内容差异：

| 三方关系 | 文件数 | 当前处置 |
| --- | ---: | --- |
| 三方相同 | 338 | 可直接继承 XP 基线身份，无需源码替换 |
| 本地等于 OpenNT，XP 不同 | 45 | 审查 XP hunk，不预设整文件安全 |
| XP 等于 OpenNT，本地不同 | 74 | 逐项保留/审计本地修正，XP 不会自动替代它们 |
| 三方均不同 | 41 | 必须三方合并 |

全部 498 路径都存在于 XP；没有本地独有路径或本地已等于 XP、但不同于
OpenNT 的第五类。原始字节相同数则为 XP 4、OpenNT 5；这受换行影响，
不能当成 494 个功能修改。记录同时保留 raw SHA-256 与 LF SHA-256。
旧 A=58/B=28 已变为 A=45/B=41，总共仍是 86 个 XP 有变化的保留文件。

按重要子系统进一步核对：

- CCPU 269 个保留文件中，249 个三方相同、19 个只有本地修正，唯一 XP
  变化文件是 fpu.c，XP 仅修改一句注释。不能把基线升级宣传成新版 CCPU
  算法或性能升级；本地 CPU 修复仍必须保留。
- C-VID 52 个保留文件中，12 个三方相同、40 个只有本地修正；XP 不会
  自动替代此前的指针宽度、selected-marker、嵌套调用修复。
- 现有四个 ROM/CMOS 文件与 XP 原始字节相同，无需为此次升级换一套固件。
- cmos_bis.c 的四处 outb 显式 IU8 转换、gfi_mpty.c 的 SHORT 返回转换已
  对照 hunk 确认是表示层候选，不是新增 CMOS/FDC 功能。

采用 git ls-tree 与 cat-file --batch 读取两边固定提交，不读取 XP 暂存区作为
源码基准。XP 参考工作区存在暂存删除及未跟踪替代文件，因此目录存在并不
证明它是 pristine。审计未修改任一参考仓库。

### 已纠正的历史结论

1. **Pause/NumLock 的位置错误。** 固定 XP 的 virtual-key 判断位于
   `host/src/nt_keycd.c::KeyMsgToKeyCode`：ScanCode 0x45 且 VK_PAUSE 时返回
   key number 126。`base/keymouse/keyba.c` 不含这个 virtual-key 判断。
   应按当前 Machine -> Compat -> 原 host keycode 翻译链验证，而不是向
   8042 控制器无依据添加一份修复。本检查点尚未运行按键行为测试。
   当前 Machine 已填 wVirtualKeyCode，但 KeyMsgToKeyCode 只用扫描码及
   ENHANCED_KEY 查表；因此 XP 判断有真实的现有接入点，且必须同时保留
   本项目已修复的数组 >= sizeof 边界检查。
2. **reset 不能把已有调用误称为新能力。** OpenNT 已有 `npx_reset()` 和
   清 CMOS shutdown 的调用；XP 的 extern 声明和缩进变化不能被统计为
   新增 FPU/reset 修复。其他 reset 初始化顺序仍需继续审查。
3. **AT DMA 的声卡接线不是 PC-98 专属。** XP `at_dma.c` 无条件包含
   sndblst.h，并在普通 AT 地址/计数读取分支调用 SbGetDMAPosition。
   必须与声卡批次一起处理，不能在早期机械批次引入未满足的宿主依赖，
   也不能把整个差异归入 PC-98 后丢弃。

### 普通 IBM PC 路径的新核实结果

本组直接对照固定 XP/OpenNT 对象的 diff、当前源码和现有测试入口；
是源码结论，不是已实施或已运行的修复。

1. **AT DMA 自动重装：应采用 XP 修复。** `base/system/at_dma.c` 在
   terminal count 到达且 auto-init 开启时，OpenNT/当前版本连续两次复制
   base_count 到 current_count，完全未恢复 current_address。XP 把第二组
   两字节复制改成 base_address -> current_address。当前文件 695–698 行
   仍有重复计数复制；这是普通 AT DMA 路径，不依赖声卡或 PC-98。
   可以独立采用这一四行替换，保留当前 snapshot 捕获/恢复接口。测试接收方
   应覆盖递增/递减、自动重装/单次、连续两次传输，以及重装前后 snapshot；
   现有 checkpoint_smoke 的寄存器往返和 fdc_smoke 的常规 DMA 读写不能
   代替自动重装证明。声卡读取位置 hook 则仍留给声卡批次，不混进此修复。
2. **V7 指针启用位：应采用 XP 括号修复。** `base/video/v7_ports.c`
   当前 235 行仍是 `value&0x80!=old_value&0x80`。按 C 运算符优先级，
   中间比较产生 0/1，再与最后 0x80 相与，整个条件恒为零。XP 改为分别
   取两侧 bit 7 后比较，才能在启用位发生变化时画出/擦除指针。这是现有
   V7 寄存器处理器内的一行修复，不需要第二条呈现路径。验证必须覆盖
   0->1、1->0、重复值及只改变低位；保留现有主体绘制和指针发布顺序。
3. **鼠标实例清零：XP 的修复已在本地实现。** `mouse_io.c` 把
   host_memset 的 value/size 参数从颠倒顺序恢复为 `(address, 0, size)`；
   当前 819 行已正确。迁移时可以收敛这一处 XP-to-local 差异，但不能删除
   同一文件中的 InPort、DOS 鼠标 snapshot 和现有绘制修复。
4. **键盘表不是只有 Pause 一个变化。** `keyba.c` 还修改了 trans_8042、
   三种扫描码表、set-3 默认属性和反查表，并取消多处 BRAZILIAN_ABNT_KBD
   表项条件。这是普通 PC 键盘的扩展，不等同于 NEC_98。另有 KOREA 宿主
   输入 workaround，不能与键盘表一并笼统判断。扫描码表须逐项列出索引、
   make/break 序列和受影响键后再导入；表项静态比对如下，端口动态验证留给 S3。

#### 键盘表的精确变化与一个 XP 遗漏

按普通 AT 分支（NTVDM/NEC_98/JAPAN/KOREA/BRAZILIAN_ABNT_KBD 均未定义）
解析固定对象数组，保留 C 的尾部零初始化。七张表共 60 个元素不同：
keytypes 3、trans_8042 38、set_3_reverse_lookup 3、set_3_default_key_state 4、
三张 most_set_N_make_codes 各 4。默认状态表显式写了 126 项，声明容量
127，最后一项隐式为 0；不能把它误统计成表容量改变。

三种 make 表新增的 key number 均为十进制 14、56、94、107：

| key number | Set 1 | Set 2 | Set 3 | XP Set 3 反查 |
| --- | --- | --- | --- | --- |
| 14 | 0x7d | 0x6a | 0x5d | **0，未接通** |
| 56 | 0x73 | 0x51 | 0x51 | 56 |
| 94 | 0x7c | 0x68 | 0x68 | 94 |
| 107 | 0x7e | 0x6d | 0x7b | 107 |

本地 nt_keycd 的宿主扫描码表已有上述四个 key number；并不是添加无人可达
的表项。历史注释对两个 Brazilian 键的字符标签不一致，所以迁移以 key
number/扫描码为权威，不根据注释交换按键。XP keytypes 将 56/94/107 从
特殊类别 6 改为普通类别 0；Set 3 默认状态为 14/56/107=1、94=3。

XP 的 8042 翻译表改变以下 38 个输入字节（十六进制），其余 218 项相同：
08、0f、10、13、17、18、19、1f、27、28、2f、30、37、38、39、3f、40、
47、48、4f、50、51、53、56、57、5c、5e、5f、60、62、63、64、65、67、
68、6a、6d、6e。后续通过控制器端口覆盖全部 256 输入翻译及 make/break，
不能只测试四个新键；这些变化也不是 CP437 字符映射。

**新增 key 14 的反查遗漏必须与升级一起修复。** XP Set 3 make[14]=0x5d，
但 reverse[0x5d]=0，且 0x5d 没有其他新键占用。现有命令路径在收到
0xfb/0xfc/0xfd 后，用下一字节查 reverse，再直接修改 set_3_key_state。
因此向新键 0x5d 设置属性会错误修改 key 0，不能把 XP 表整套视为完整正确。
最小接法是在同一个反查表把 0x5d 项设为 14，保留原控制器路径；验证四个
新键的反查和属性更新、未涉及键不变、reset 后默认属性恢复。不增设第二表。
这里是固定源码解析与调用链证明，没有宣称已经运行键盘端口测试。

### 未保留文件：同名比较之外的迁移接收方

499 个未保留路径进一步分成 315 个 C/H/ASM 与 184 个其他文件。315 个源码
路径中，22 个是 XP 相对 OpenNT 新增，60 个是已有文件发生变化，233 个
与 OpenNT 经 LF 归一化后相同。此分类缩小语义差异审阅范围，但不把“未变”
当成无用或已验收的证明；未引入路径仍需按职责给出处置。

已审阅下列未保留实现的变化，并对照本项目接收边界：

- **CMOS 有跨文件可用修复。** XP `base/system/cmosnt.c` 给扩展内存
  高低字节比较的右操作数补齐括号。当前编译的是 `base/system/cmos.c`
  （根 CMake 源清单），其 1052–1055 行仍有同样错误：`byte != value &
  0xff` 按 `(byte != value) & 0xff` 执行，而不是与截取后的字节比较。
  例如扩展内存为 15 MiB 时，低字节应为 0；原式却拿 0 与 15360 比较，
  错报 BAD_XMS。应把这两处括号修复迁入现有 cmos.c，不能为采用修复另建
  cmosnt 路径。测试接收方是原 CMOS 初始化/校验测试：覆盖小于和超过
  256 KiB 的扩展内存、正确字节及分别损坏的高低字节。这里确认的是表达式
  错误，不宣称已经动态复现用户启动故障，也不改变修复后的容量计算公式。
- **NT 内存分配器的修复没有本地同一实现。** `nt_mem.c` 的两处
  `flags & HDR_VALID_ALLOC != HDR_VALID_ALLOC` 同样补括号；本项目源码
  无 HDR_VALID_ALLOC/ADDRESS_TO_HEADER 分配器，也不编译 nt_mem.c。
  因此不引入这套 NT 虚拟内存实现，更不以此替换现有 SAS/Compat 内存。
- **原生磁盘句柄不是镜像后端。** `nt_fdisk.c` 增加 NT 文件读取权限、
  打开失败的 FALSE 返回与布尔归一化；`nt_rflop.c` 修改物理软驱锁和几何
  窄化。这些不是 WD/FDC 控制器升级。本项目不编译这两个物理媒体端点，
  保留既有 image/readonly/direct/overlay 和 Lib Storage 所有权。
- **宿主时钟不迁回 NT 线程。** `nt_timer.c` 用无符号乘法计算性能计数器
  余数，并调整 NT 心跳追赶算法。现有 Compat 的 softpc_clock_ticks 已用
  无符号 64 位余数计算，host_timer_event 保留机器侧设备/quick-event 调用。
  不复制 Win32_host_timer 的 DelayHeartBeat 调度线程，也不据此替换已验证
  的执行器事件等待；本结论不等于新的计时精度运行验收。
- **DMA 的重复修复只保留一个设备所有者。** XP `nt_vdd.c` 也在
  VDDSetDMA 的 terminal-count 后补自动重装地址/计数；VDD API 不属于当前
  产品。采用前述 at_dma.c 修复即可，不引入第二套 VDD DMA 更新入口。
  同文件的 0x201 -> VJOY.DLL 懒加载不是可独立复制的游戏端口模拟器。
- **NT 鼠标修复不能恢复旧宿主指针路线。** `nt_mouse.c` 增加超出标准
  BIOS 模式号的查表边界、默认虚拟中心，并把无效的 `iY >> 1` 改为
  `iY >>= 1`。这些作用于 NT Console 指针坐标/宿主捕获，不是当前 InPort
  硬件路径；不能把其模式号 fallback 搬入 Lib 或硬件。保留现有鼠标模型，
  后续输入批次验证 V7/非标准模式边界，不复制宿主菜单和 ShowConsoleCursor。
- **EMS/NTVDM 服务与新增硬件分开。** emm_mngr/emm_fncs 的类型调整、
  nt_emm 的 VdmMapDosMemory 声明、x86_emm 的失败返回、sim32 的 VdmMapFlat/
  VdmFlushCache API 是既有 EMS 或 VDD 宿主服务变化。当前编译清单不含这些
  实现；不要以“支持 IBM PC”之名将 NTVDM XMS/EMS 服务插在真实客户机驱动
  之外。`nt_bop.c` 的 DEM/XMS/WOW 分发顺序及 PIF idle 参数同理不导入。
- **其他宿主修复有明确而不同的接收边界。** nt_ntfun.c 检查同步串口
  IOCTL 的立即返回状态和完成状态；本产品没有该 FastComm 路线，不引入 NT
  串口直通。nt_rez.c 修复外部 ROM 文件 seek/read/close 失败处理；本产品
  host_read_resource 读取内嵌 RCDATA，不读取旁路 ROM 文件。nt_hosts/nt_nls
  的 Console 代码页、IME、GDI 字体及 Unicode 菜单属于宿主。nt_thred/stubs
  的 VDM_TIB 线程参数及 nt_cpu/nt_aorc 的替代 CPU ABI 也不进入当前单执行器。
- **声卡生命周期不能只搬端口。** 未保留的 base/support/terminat.c 新增
  SbTerminate，nt_reset.c 新增 SbCloseDevices。声卡批次必须把设备终止和
  宿主资源关闭分别接到当前既有机器/Compat 生命周期，而不是把原 NTVDM
  main、WOW session 或终止流程整套搬进来。此处是新增 S5 验收义务，不是
  声卡已实现的声明。
- **旧调试器的真修复不等于 CPU 修复。** yoda.c 把临时指令断点的两个
  `== 0` 改成 `= 0`，确实修正旧调试器清除断点的错误；但本项目不编译
  Yoda，使用公共 x86-debug，不引入第二个 debugger。dasm386/旧 dasm、
  btrace/trace 的显式窄化、返回类型和比较修正也保留为排除记录，不把它们
  归入 CCPU 执行能力升级。根 CMake 中没有这些旧调试实现。

#### 未保留宿主路径的排除理由补齐

以下分类来自固定 XP/OpenNT 源对象的实际差异，不以文件名中的 nt 前缀
代替语义判断；排除的是原宿主实现，不是相应 IBM PC 硬件能力。

- `nt_det.c`、`nt_fulsc.c`：RegisterConsoleVDM、物理全屏显存交接、
  error event、Console/桌面切换及 WOW 字体指针处理。不是当前 VGA 算法
  的替代实现；不把其中固定行数、字体选择或窗口冻结策略搬进本地呈现路径。
- `config.c`、`nt_pif.c`：PIF、WOW/会话标识、NT 配置和宿主 COM/LPT
  超时策略。配置继续由 App 解释，不引入 NTVDM 的配置所有者。
- `nt_event.c`：Shift/Alt 松开、锁定键同步、PrintScreen 和输入历史修正
  作用于原 Console 事件线程。不得复制第二个输入循环；本地输入仍由 Lib
  与 Machine 的既有入口负责。其阻塞前 `SbCloseDevices` 则是声卡生命周期
  义务，不能随整个宿主文件的排除一起漏掉。
- `nt_msscs.c`：NTIO.SYS 装载、VDM 标志、DPMI/EMS 初始化不导入；
  reset 后 `SbInitialize` 与上述关闭义务一起归 S5 的现有生命周期接线。
- `nt_com.c`：物理 COM 句柄、overlapped completion、宿主缓冲和自动关闭
  策略改变，并非全部只是类型转换。当前串口输出不采用此物理直通线程；
  普通 PC UART 模型的审阅仍归保留的 base/comms 文件，不能混为一类。
- `nt_error.c`：本地化消息、WOW 退出和原生错误对话框，不是机器设备功能。
  `obj.vdm/ntvdm.c` 的入口/VDM_TIB 变化也不引入另一套程序生命周期。
- `convert/convert.c`、`rename/rename.c`、`debugger/vdmdebug.c` 的调用约定
  变化属于旧工具；`bios/sysinit1.asm` 仅注释改变，不要求替换已核对相同的 ROM。

对五份筛选后无差异文件，另行读取未经筛选的原始 diff 核实：

- `base/bios/rtc_bios.c` 并非完全无源码变化：XP 增加 `outb` 的 IU8 转换。
  函数位于 `#ifdef NTVDM` 内；文件开头说明非 NTVDM 的功能已由 bios4.rom
  中的 Intel 实现替代。因此不为了这一转换新增 C RTC BIOS 路线。
- `base/debug/ega_dump.c` 的新增内容只服务 NEC_98，普通 PC dump 无语义变化。
- `base/inc/jformate.h` 仅更改错误说明注释。
- `host/src/nt_lpt.c`、`host/src/nt_wcom.c` 新增 NEC_98 的单打印口、8251
  状态及 153600 分频分支；普通 PC 的原宿主路线没有对应功能升级。

这次反查证明“筛选结果为空”必须保留原始 guard/职责证据，不能被报告成
“原文件未变”。全部未保留路径的处置清单见下节；以上不是运行验收声明。

#### 未变化候选不等于新的 IBM PC 升级

对 233 个未保留且 XP/OpenNT 相同的源码路径，先核对容易误判为新设备的
候选，避免用目录名代替实现证据：

- `base/disks/gfi_ibm.c` 与 `gfi_sflp.c` 是 SLAVEPC 分支，通过 RS232 RPC
  请求另一台真实 IBM PC 的软驱/FDC，不是更完整的本地 FDC 模拟；不引入，
  当前 FLA/GFI/镜像后端仍是唯一设备链。
- `base/inc/cdrom.h`、`cdrom_fn.h` 是旧接口和数据声明。固定 XP 的整个
  softpc.new 路径清单中，含 cdrom 的只有这两个头文件，没有其注释所说的
  cdrom_fn.c 实现。不能把导入两个头文件宣称为新增 CD-ROM，也不能把缺失
  实现误归成“非 IBM PC，故排除”；本基底没有提供可直接迁移的该能力。
- `base/system/qevnt.c` 是 NTVDM 专用调度器：其文件说明 x86 的 quick event
  立即派发、tick event 不支持，使用 host_ica_lock。它不是现有 quick_ev.c
  的新版替代；引入会改变时间和 snapshot 契约，因此不替换当前队列。
- `base/video/ga_glue.c` 明确是 a2cpu/旧 2.0 视频接口胶合；不是当前
  CPU_40_STYLE C-VID 的增强版本，不新增第二套读写胶合。

184 个非 C/H/ASM 路径主要是旧 NT 构建、资源、profile 和固件副本。
四个 data/{bios1.rom,bios4.rom,cmos.ram,v7vga.rom} 已逐字节核对，分别与
当前保留的 roms/ 同名对象相同；不需要新增第二份。`roms/bios2.rom`
与 bios4.rom 不同，不能说所有候选 ROM 都相同。固定 XP 的
base/system/rom.c:65–72 明确在 CPU_40_STYLE 或 ARCX86 下选择 bios4.rom，
否则才选 bios2.rom；当前 CPU_40_STYLE 因而继续使用已保留的 bios4.rom。
这是原选择契约，不是为了兼容现状新加的特判，不要求更换固件。
构建脚本和资源文件不因扩展名被默认认为安全；本项目仍使用自身 CMake、
入口、资源和品牌，而不是 NTVDM 的 makefile/sources/resource.rc。

### 视频迁移边界的补充核对

- `video.c` 把 vd_addr_6845 改用 M6845_INDEX_REG/M6845_DATA_REG，不是把
  CRTC 端口固定为 0x3d4：宏仍引用同一个 vd_addr_6845，XP 只是补 word 转换。
- vga_vide.c 的 DAC 循环改类型时同时保留独立的宽计数变量遍历 256 项；
  不能只搬 UCHAR 声明而漏掉循环变量拆分。BIOS 保存/恢复测试应覆盖全部
  DAC 项，而不是只验证第一个颜色。
- ega_prts/vga_prts 中的显式转换、符号比较和原型须与本地几何/dirty 修复
  三方合并；egawrtm0/egwrtm12 的旧写入器声明调整并不取代本地 C-VID 的
  selected-marker 接线。XP 没有提供新版生成渲染器。
- nt_cga/nt_ega/nt_vga 的矩形窄化不解决本地已修复的固定 80 格目标跨度，
  不得覆盖它；nt_graph 的 SwitchToFullScreen 及 DOS/V Console/IME/GDI
  字体路线是 NTVDM 宿主显示，不是新的 VGA 模式能力。

这些是源差异的审阅结论，不是导入或动态验收的证明。

### 499 个未保留路径的完整处置清单

[逐路径 TSV](xp-sp1-unretained-paths.tsv) 固定为 XP 全部 997 路径减去当前
498 路径；每个路径恰好一行，互不重叠。`xp_vs_opennt` 表示源码内容关系：
60 changed、233 same、22 new；其余 184 noncode 只表示不是 C/H/ASM，
并不表示无语义或无需核对。所有比较均读取本报告开头的固定 Git 对象。
该 TSV 是研究元数据，不包含外部源代码、固件内容或运行依赖。

| disposition | 路径数 | 迁移决定及接收方 |
| --- | ---: | --- |
| SB_DEVICE_HOST_SPLIT | 7 | 纳入 S5 IBM PC 声卡设计；XP 设备语义保留，NT 宿主绑定由现有 Compat/Audio 接口承接，见声卡审计；不是允许整文件直接编译。 |
| SB_LIFECYCLE_TO_EXISTING_OWNER | 4 | terminat/nt_reset/nt_msscs/nt_event 不导入；其声卡关闭、初始化及阻塞前释放义务转入现有 Machine 生命周期，不能随文件排除而丢掉。 |
| CMOS_FIX_TO_EXISTING_OWNER | 1 | cmosnt 不导入；两个内存容量字节比较括号修正由 S2 接入现有 cmos.c。 |
| PC98_EXCLUDED | 13 | 新增的 NEC_98 显示控制器、CG、系统和宿主文件；非 IBM PC，不进入源集或运行路径。 |
| NT_HOST_BUILD | 2 | nt_vdm/imlibdep 属 NT 产品入口/链接支撑；不导入另一产品宿主。 |
| NT_OR_LEGACY_HOST_SUPPORT | 94 | 原 NT/旧宿主服务及其头文件、NTIO 汇编、入口和虚拟实例支撑；不替换当前 App/Compat/BOP/SAS。变化项的具体功能已在前述审阅中分别判断，不能解释成“94 项都只是无关类型变化”。 |
| LEGACY_BUILD_RESOURCE | 179 | 原构建选择、消息/图标/资源、profile、生成描述及 NTIO include；本项目保留既有 CMake/资源/品牌，不把旧工具链变成依赖。 |
| BIOS4_EXISTING_IMPLEMENTATION | 2 | rtc_bios/prt_scrn 的非 NT 功能已由当前 bios4.rom 提供，不新增 C 实现。 |
| ALTERNATE_CPU_VIDEO_SUPPORT | 96 | 未选的旧 CPU/generated-header/胶合支持，包括 localfm/sascdef、旧 evid、90 个 cvidc 支撑路径及 ga_glue；XP/OpenNT 相同，不替换当前 CPU40/C-VID 路径。 |
| LEGACY_DEBUG_NOT_PRODUCT_DEBUG | 12 | 原 Yoda、trace、反汇编和调试工具，不是当前公共 Debug；保留现有 x86-debug 及 Machine 观察协议，不新增 debugger。 |
| LEGACY_PHYSICAL_REMOTE_FLOPPY | 5 | gfi_ibm/gfi_sflp/gfi_test 及对应接口；原物理/远端软驱链和测试，不是当前本地镜像 FDC 的新版。 |
| LEGACY_UNSELECTED_INTERFACE | 71 | 未选旧服务/宿主/CPU 接口声明；其余实现没有被纳入时不单独引入头文件，不能将声明数当新增能力。 |
| CDROM_DECLARATIONS_NO_IMPLEMENTATION | 2 | CD-ROM 是 IBM PC 能力，但 XP 该树只有声明，没有可移植实现；记录缺失，不把它谎称为已实现或非 IBM PC。 |
| NT_QUICK_EVENT_NOT_LOCAL_QUEUE | 1 | qevnt 的 NTVDM 立即派发/宿主锁路线不替换当前 quick_ev 队列。 |
| LEGACY_BUILD_TOOL | 3 | convert/rename/dat2obj 工具，不是运行设备。 |
| DUPLICATE_FIRMWARE | 4 | data/ 固件逐字节等于已保留 roms/，不复制第二份。 |
| ROM_ASSEMBLY_REFERENCE | 2 | bios1.asm/bios4.asm 可作原固件研究依据；当前不重建或替换固定 ROM。 |
| ALTERNATE_CPU_ROM | 1 | bios2.rom 为非 CPU40 选择；按原 rom.c 选择继续排除。 |

总和 499。其中“转入现有 owner”是语义迁移而非文件导入；目标仍保持唯一
设备/调度/生命周期实现。未变化的原支持代码没有被自动等同于非 IBM PC，
而是明确按旧路径、重复实现或只有声明排除。此清单不批准新增 IPX/TCP/IP、
宿主文件共享、DPMI/EMS 宿主加速等另一套产品服务；客体仍通过当前真实 CPU
和设备运行其自己的系统软件。

复核方法：固定提交分别执行 git ls-tree 和 git cat-file --batch；用当前
498 路径集合做差集；对 C/H/ASM 将 CRLF 规范为 LF 比较 OpenNT；按上述
互斥处置分组输出 TSV。生成时断言 499 个唯一路径，四份重复固件另断言
原始字节相同。清单只完成源选择覆盖，后续各 S 的运行证明仍不可省略。

### XP 新增转换也可能退化，不能按“消除告警”自动接受

对 86 个 XP 变化文件的审阅投影，扫描新增的“16 位寄存器 setter 前的
8 位强转”，得到以下四处。投影仅辅助定位，关键行已回查固定原始对象。

- `rom.c::do_search_for_roms` 新增 `setCS((UCHAR)((addr & 0xf0000) >> 4))`。
  当前 UCHAR 明确为 unsigned char，而 setCS 接收 IU16。以 0xC0003 为例，
  正确段值 0xC000 被截为 0；不能采用此转换。XP 基底必须保留 16 位段地址，
  并用有效 option-ROM 入口的捕获验证，不能仅依赖一般 DOS 启动成功。
- `diskbios.c` 新增 `setCX((UCHAR)(blocks / 256))`，可改变原有 16 位结果。
  例如 blocks=65536 时原表达式向 CX 传 256，新表达式传 0。不得归为
  等价清理。原有 CX/DX 容量拆分公式是否完全符合 BIOS 契约是另一问题；
  不因拒绝新截断而宣称旧公式已正确，也不在基线迁移中顺手改公式。
- 相邻 `setDX((UCHAR)(blocks % 256))` 对非负 blocks 结果等价：余数已经
  在 0..255 范围。应与上一个截断分别处置，不能见 UCHAR 就一律拒绝。
- `mouse_io.c` 光笔查询新增 `setBX((UCHAR)(position.x / cursor_grid.x))`。
  普通图形模式 6/E/F/10/11/12 的 cursor_grid.x 为 1，V7 图形表也为 1，
  因此不能依据文本模式默认 grid=8 排除大于 255 的横坐标截断。应保留
  现有寄存器宽度，后续验证光笔查询返回的实际 BX，不把它误称为新鼠标能力。

此外 `mouse_io.c` 新增 AH=0x4f 时调用 SwitchToFullScreen 的宿主路径。
这不是 VBE 硬件能力实现；独立产品不能通过空实现或强切 presenter 来
“支持”它，应从 XP 设备基底中明确剥离这一 NTVDM 宿主接线。

### Sound Blaster：有设备语义，但不是独立的完整音源

直接审阅了固定 XP 的接口和入口，进一步检查 DSP/FM 依赖：

| 候选路径 | 原文件行数 | 已确认的职责/限制 |
| --- | ---: | --- |
| base/inc/sndblst.h | 205 | SB 2.0 / DSP 2.01、IRQ5、DMA1、A220；DSP/mixer/MPU/AdLib 常量 |
| host/inc/nt_sb.h | 351 | 大量 WinMM 函数指针及宿主句柄，不是纯设备接口 |
| host/src/nt_sb.c | 988 | 端口分发、DSP/mixer/MPU 接线，混合 WOW、NtVdmControl 和 WinMM 初始化 |
| host/src/nt_sbdsp.c | 2962 | DSP 命令、DMA/IRQ；混合 waveOut 播放位置、线程、EOI hook 和 Sleep |
| host/src/nt_sbfm.c | 366 | AdLib 寄存器/状态与宿主 FM 设备；没有独立 PCM 合成器 |
| host/src/nt_sbmid.c | 896 | 已全文审阅：MIDI 字节组帧与 WinMM MIDIMAPPER 输出混合；没有 PCM 合成器 |
| host/src/nt_sbmxr.c | 329 | 已全文审阅：CT1335 寄存器状态与 waveOut/MIDI 宿主音量混合 |

七个文件合计 6097 行是候选原始体积，不是预估新增代码量或已批准复制量。
XP 声卡源码明确标识 Microsoft 1991-1996 All Rights Reserved；读取和用户
研究授权不构成再分发许可。首次复制前须按 Source And Research Policy
完成 notices/rights 记录与必要的 owner/legal 决策；本检查点未导入源码。

FM 路径查找 MOD_FMSYNTH，并经 NtVdmControl(VdmAdlibEmulation) 请求
宿主支持；没有宿主 FM 时，现有代码可模拟探测状态，但不能据此声称已经
产生音乐。DSP 输入命令也有明确未实现标记，不能宣传成完整 SB16/录音支持。

因此应纳入 IBM PC 声卡能力目标，但需明确三层：XP 可复用设备语义、
替换掉的 NTVDM 宿主接线、XP 未提供的独立发声能力。不得用空函数使构建
通过后宣称实现，也不得在 Compat 再造第二份 DSP/DMA 状态机。新增设备
状态必须进入唯一执行器、reset 和 snapshot 契约；宿主句柄不进快照。

#### MIDI 与 Mixer 的具体迁移约束

固定对象的 nt_sbmid.c、nt_sbmxr.c 已全文审阅，并回查 nt_sb.c 的 MPU
端口调用方以及本地 lib/audio/stream_interface.h，结论如下：

1. **MPU 不是完整 intelligent-mode 设备。** nt_sb.c 接收 reset 和 UART
   切换并提供 0xfe ACK；UART 数据进入 BufferMidi，intelligent mode 的数据
   被丢弃；输入方向除 ACK 外不支持 MIDI 接收。应保留并明确这一级能力，
   不能把文件名 MPU401 宣传成完整 intelligent-mode 或双向 MIDI。
2. **MIDI 不是音频样本。** BufferMidi 解释 running status、system common、
   realtime 和 SysEx，再向 MIDIMAPPER 发送短/长消息。Lib Audio 只接收
   i16 PCM，没有 MIDI 命令或音色库接口。独立实现需要明确 MIDI 输出目的地
   或合成器，不能把 MIDI 字节当 PCM enqueue，也不能以空回调假装完成。
   FM 的软件 OPL 缺口与 MIDI 的合成/输出缺口是两个不同问题。
3. **现成 MIDI 队列有明确尾消息缺口。** 初始化时 NextCopyPosition 与
   LastCommand 都为 0。第一条完整 MIDI 命令仍位于偏移 0，完成后调用
   SendMidiRequest，而其循环只处理 `NextCopyPosition != LastCommand`。
   因此第一条完整命令不会发出；下一条占据新槽位后才可能发出上一条。
   这来自源码控制流，不是宿主听音结论。迁移应区分“完整消息末端”与
   “正在组装的消息起点”，用单条命令即输出、最后一条不滞留的测试证明。
4. **不能原样保留 32 位宿主指针与轮询。** ToBufferAddr、MidiHdrs 的地址
   算术把宿主指针转 ULONG，x64 会截断；ResetMidiDevice 在 INQUEUE 时每
   5 秒 Sleep，没有截止条件。设备字节解析可以保留，WinMM 缓冲/回收必须
   留在宿主边界，不能把这个等待塞进唯一执行器或快照安全点。
   嵌入 realtime 消息时，原文也明确承认没有检查缓冲耗尽；此入口需要
   容量/回绕测试，不能仅验证普通三字节 NoteOn。
5. **Mixer 寄存器与声音增益不能混为一个宿主滑块。** Master 与 Voice
   分别调用同一个 waveOut SetVolumeProc，后一次写入覆盖前一次，而不是
   同时体现两级寄存器；FM 音量直接修改宿主 MIDI 设备。独立接入应保留
   客体寄存器读回，把各通道和总增益用于本机生成的样本，不能改宿主全局音量。
6. **零音量存在确定的无符号下溢。** `level * 0x2000 - 1`（Voice 使用
   0x4000）在 level=0 时赋给 ULONG 得到 0xffffffff，随后左右声道拼接
   也不是静音。迁移不能照搬此公式；测试要覆盖零、最大和中间级，同时
   验证 master/voice 写入顺序不覆盖另一寄存器的效果。
7. **读默认值由原端口入口拥有，不能丢失。** MixerDataRead 在 MixerReset
   分支只 ResetMixer，没有写输出指针；回查 VsbByteIn 确认它在分发前
   已将 `*data = 0xff`。因此原公开端口路线的读值确定，不能把内部函数
   未赋值误报为未初始化读取。迁移保留这一入口默认值与 reset 副作用，
   用公开端口测试证明，不另造 helper 默认值或第二条读路径。

因此 S5 不是“复制 6097 行再换一个播放函数”的小移植。可复用的是设备
协议主体；独立 PCM 时基、FM/MIDI 输出、混音、缓冲完成与快照状态边界都
须在实施前定稿。上述缺口是源审计结论，没有新增源文件或运行测试。

### 本地独有修正：非 C-VID 的 34 文件保留清单

本组 XP 与 OpenNT 内容相同，不能靠升级 XP 消除差异。重新读取固定 XP
对象并与当前文件比较后，34 文件合计相对 XP `+1113/-96`，净 +1017 行；
口径为 LF 归一化后的 inventory.py SequenceMatcher（autojunk=False），
不是本次修改量，也不是 Git 默认算法的承诺值。迁移去向统一为
**XP file + local patch**；当前文件已经具有这一内容关系，无须先覆盖再恢复。

| 明确路径（相对 softpc.new） | 必须保留的职责／修正 | 后续验证接收方 |
| --- | --- | --- |
| base/ccpu386/c_bsic.h | c_getEFLAGS 正确声明 | 双宽度编译、checkpoint |
| base/ccpu386/c_debug.c | 指令/数据断点内部状态快照 | checkpoint、snapshot transaction |
| base/ccpu386/c_intr.c | PIC 拒绝应答的 -1 不作为中断向量 | irq_smoke |
| base/ccpu386/c_main.c | 指令地址计算、调试观察、执行器事件、HLT 等待、安全点、PIC 撤销和 TLS 模拟栈 | lifecycle、irq、checkpoint、runtime input continuation |
| base/ccpu386/c_page.c | 成功访存的调试观察，不另读客户机内存 | debug 集成、machine 回归 |
| base/ccpu386/c_reg.c | 寄存器及隐藏段缓存捕获/恢复 | checkpoint、跨进程 snapshot |
| base/ccpu386/c_tlb.c | TLB 和 fast-TLB 状态捕获/恢复 | checkpoint 的页转换状态验证 |
| base/ccpu386/c_xcptn.c | 正确声明来源，移除局部冲突声明 | 双宽度编译；不是新增异常分支修复 |
| base/ccpu386/call.c、iret.c、ret.c | 换栈后用 set_current_SP，按新栈宽度而非指令操作数宽度写 SP/ESP | CPU 跨权限级/混合栈宽度用例必须保留或补齐 |
| base/ccpu386/ccpusas4.c | SAS 内存、页类型及 A20 wrap 状态快照，补正确声明 | checkpoint、snapshot transaction |
| base/ccpu386/cpu4gen.h | 唯一 C-VID GDP 声明来源和真实 CCPU setter/getter ABI | 编译、ABI/边界检查 |
| base/ccpu386/evid_c.h | 本机宽度的规则代码地址和 marker 指针 | 双宽度 C-VID/CHAIN2 回归 |
| base/ccpu386/ntstubs.c | 唯一 GDP 定义；删除遮蔽真实 ROM/FPU 实现的空 stub | 链接、启动、x87 |
| base/ccpu386/ntthread.c | 最外层退出、正确 jmp_buf 指针失败返回和 sizeof 格式 | lifecycle、嵌套退出、编译 |
| base/ccpu386/popf.c | stdio 与 note_486_instruction 声明 | 编译 |
| base/ccpu386/vglob.c | 唯一 C-VID 类型和 GDP slots 来源 | 编译、C-VID 回归 |
| base/ccpu386/zfrsrvd.c | 正确函数声明、延迟 NPX 异常 EIP 快照 | x87、checkpoint |
| base/inc/ckmalloc.h | malloc 指针返回声明，避免隐式 int 截断 | 双宽度编译 |
| base/inc/cpu_vid.h、gmi.h | 真实地址/数值参数顺序与宽度，EVID 函数表 ABI | 双宽度 C-VID 回归 |
| base/inc/egaports.h | 既有 pel-panning 接口 | VGA frame 回归 |
| base/inc/emm.h、host_com.h | 宿主块/指针参数保持本机宽度 | 编译、serial 回归 |
| base/inc/host.h、timeval.h | 与现有 provider 一致的 memset4、host_time 契约 | 编译、quick_time |
| base/inc/quick_ev.h、base/system/quick_ev.c | 用语义 callback ID 保存队列，恢复时不执行零延迟回调 | checkpoint 队列捕获/恢复用例 |
| base/system/cmos.c | 实际介质拓扑、宿主指针转换、RTC 状态快照 | checkpoint、dual_media；另叠加前述 XP 跨文件括号修复 |
| host/inc/cfpu_def.h | 小端位域、浮点字序、BCD 整数字序 | x87_layout_smoke |
| host/inc/host_cpu.h | 小端寄存器布局、真实 CCPU vector 参数与返回类型 | 编译、CPU/寄存器回归 |
| host/inc/nt_event.h | PointerAttachedWindowed 的 BOOL 声明 | 编译 |
| host/src/nt_munge.c | EGA pel-panning 跨平面/回绕取值 | vga_frame_smoke |

这些是迁移保护项，不是声称本轮已经重新运行或逐项证明全部旧测试充分。
已读取 irq_smoke 的拒绝应答断言、checkpoint 的 HLT/IP、嵌套安全点、TLB、
设备与队列往返断言；它们是实际测试接收点，而非仅按文件名猜测。
特别是 CALL/IRET/RET 的混合栈宽度，不以普通 DOS 启动成功代替定向证明。
本组也不允许把 snapshot owner 重新搬到 Compat 去复制私有状态，或恢复
ntstubs 中的空 FPU 入口以换取链接通过。

#### C-VID 的可机械证明部分和不可省略的语义部分

40 个 local-only C-VID 文件分为 6 个胶合/声明文件和 34 个生成规则文件。
对后者采用现有 Audit-MirrorDiff 的逆变换规则在内存中比较固定 XP 对象：
只逆转 GDP slot/address 访问、本机字宽 calloc 和指定移位表达式，去除已知
支持头/说明注释后忽略空白；不修改源码，不以此替代运行正确性证明。

- **27 个完全匹配枚举转换**：sevid002–015、sevid021–030、sinit011–013。
  它们没有剩余未分类的文本变化；保留 GDP 宽度与局部存储适配即可。
- **7 个还有语义补丁**：sevid000、001、016–020。现有 source-boundary
  门禁明确记录 28 个 selected-marker 点位，以及这些文件中的局部 continuation
  标签；不能把它们当成“只是 x64 替换”覆盖回 XP。该门禁只检查指定代码形态，
  不能代替 CHAIN2 byte/word/string、嵌套激活及视频输出测试。
- **6 个胶合/声明文件已读完整 diff**：ev_glue.c 保留 GDP 创建、真实声明、
  诊断格式和 CHAIN2 到既有 EGA marker 的地址单位转换；evidfunc.h 保留生成
  marker 的四本机字参数契约；evidgen.h 保留类型化回调；j_c_lang.c/.h 保留
  真实声明及诊断宽度；vglfunc.c 保留仅声明文件的现代 C prototype 展开。

整个 C-VID 在 XP 与 OpenNT 间无内容升级；迁移策略是原位保留这些本地差异，
将固定比较基准改成 XP。S4 不需要为了“换基底”重新生成或机械重写这 40 个
文件。7 个语义补丁文件的进一步核对如下，27 个逆变换匹配不能代替它们。

在上述逆变换后，另外把已标注 T88 的局部 goto 在内存中还原为固定 XP 的
原始 continuation 调用文本：sevid000/001 分别 6/4 处，结果完整匹配 XP；
sevid016–020 分别 2/7/7/16/6 处，总计恰好 48。其余差异块落在以下 28 个
dirty 替换及被删除的 L25 dirty 标签/包装函数中；未出现另一类残差。
此检查是差异归类，不是重新证明原算法所有输入均正确。

| 文件 | 必须保留的 marker 所属规则编号（S_ 前缀） | 局部 continuation 数 |
| --- | --- | ---: |
| sevid000.c | 无 | 6 |
| sevid001.c | 无 | 4 |
| sevid016.c | 2625、2629 | 2 |
| sevid017.c | 2635、2643、2647、2653、2661 | 7 |
| sevid018.c | 2665、2671、2679、2683、2689 | 7 |
| sevid019.c | 2696、2697、2698、2699、2700、2702、2703、2706、2708、2712、2714、2718、2720、2724 | 16 |
| sevid020.c | 2726、2729 | 6 |

规则覆盖 ModeX 的 byte write、前后向 byte/word move，以及 Copy 的
byte/word/dword write/fill/move。byte、word、string 使用原有 marker 槽位；
word/dword 个数转换为字节范围，反向 move 计算起始范围，零长度不标记。
不要恢复原内联 CGA bounds，也不要让 continuation 再建立一次自动局部帧。
既有 `chain2_rule_smoke.c` 实际覆盖 5 种写入表、4 种对齐、12 个复合入口，
并检查重放一致性与后续标量调用；读取覆盖 2 种 read mode、5 个入口和边界
哨兵。测试自身明确不制造不存在的视频源 bulk callback，因此不得宣传成
“所有原始生成规则均已验证”。T88 的 48 点历史证据及当前静态形态检查一起
构成迁移保护依据；源升级不扩大或重做该修复。

### INT 15h 描述符长度：XP 变量改宽不能孤立导入

重新核对 `base/bios/tape_io.c`、`base/inc/base_def.h` 和当前
`compat/ccpu/facade.c::retrieve_descr_fields` 后，结论如下：

- 当前 `word` 是 IU16，`sys_addr` 是 IU32。当前调用方的两个 limit 局部变量、
  局部函数声明以及 Compat 实现均使用 `word *`，实现显式截取 descriptor.limit。
  因此不能把当前代码描述成“32 位写入 16 位局部变量造成栈覆盖”；源码不支持。
- 固定 XP diff 把 source_limit/target_limit 改为 sys_addr，却没有同步修改
  非 CPU_30_STYLE 分支的 `retrieve_descr_fields` 声明。当前配置为 PM、
  CPU_40_STYLE，这条调用路径实际保留。原样复制变量改动后，会传 IU32 指针给
  IU16 指针契约，且仅写入低 16 位，剩余位没有初始化保证。
- 正确迁移是保留 XP 两个 IU32 局部变量，并同步这一既有 helper 的声明与
  Compat 定义为 `sys_addr *limit`，去掉实现中的 `(word)` 截断。不增加 helper、
  状态或另一条描述符解码路径；仍调用 CCPU 的 read_descriptor_linear。
  相对 XP 镜像只需修正声明一行；Compat 预计 +2/-2，两个变量沿用 XP。
- PROD 下 limit 断言被移除，并不等于 ABI 可以不一致；也不能据此宣称当前
  DOS/Windows 故障由该问题造成。本次只证明迁移风险，没有运行 INT 15h 复现。

S2 验证接收方应补真实 helper 的 16 位与页粒度 32 位 limit 输出、相邻哨兵，
以及 INT 15h AH=87 的源/目标内容、A20 原状态恢复和返回标志。现有 bop_smoke
只确认 BIOS_CASSETTE 入口存在，不能充当内存搬运的行为证明。
XP 的 JAPAN+i386 NtAllocateVirtualMemory 独立内存路径仍排除：正常 IBM PC
已有 SAS/A20 机制，不引入第二份客户机内存来承接这项类型修正。

### 86 个 XP 变化路径的迁移接收账目

以下按固定 inventory 的 45 个 xp-only-change 和 41 个 three-way 路径登记，
每个路径恰好出现一次。此表是迁移设计，不是导入完成或运行通过的声明。
`XP+patch` 表示以 XP 为基底，保留本报告所列当前独立宿主、快照、宽度及
设备修正；`XP` 表示未发现需要额外本地语义补丁。非当前 profile 的受保护
代码可以保留为原镜像文本，但必须证明不编译、不链接、不激活；不为消除
未选分支而机械删除整段原代码。任何被列为排除的宿主语义均不能成为新依赖。

| 路径（相对 softpc.new） | 接收批次、去向及必须保留/排除事项 | 验证接收范围 |
| --- | --- | --- |
| `base/bios/cmos_bis.c` | S2 XP；端口值显式字节转换 | CMOS 读写、双宽度编译 |
| `base/bios/reset.c` | S3 XP+patch；保留介质拓扑、配置表地址及现有 reset；排除 NT/PC-98 路径 | 首启、cold reset、介质保留、snapshot |
| `base/bios/tape_io.c` | S2 XP+patch；按上节修正描述符 helper ABI，保留 BIOS 配置查询；排除 NT 私有内存 | AH=87、A20、描述符、BOP |
| `base/ccpu386/fpu.c` | S3 XP+patch；XP 仅改注释，不能覆盖本地 FPU 适配 | x87、snapshot |
| `base/comms/com.c` | S2 XP+patch；寄存器/端口宽度及 void 声明，保留串口状态快照 | UART、串口输出、snapshot |
| `base/comms/printer.c` | S2 XP+patch；状态/字节表示，保留现有打印输出及状态捕获 | LPT、输出失败、snapshot |
| `base/comms/printer_.c` | S2 XP；host_lpt 声明可见性 | 编译、打印 BIOS |
| `base/comms/rs232_io.c` | S2 XP；端口和返回位宽 | 串口 BIOS、UART |
| `base/disks/diskbios.c` | S2 XP+patch；拒绝 XP setCX 的 UCHAR 截断，不顺便改变原 /256 算法 | BIOS 磁盘返回寄存器边界 |
| `base/disks/fdisk.c` | S2 XP+patch；保留镜像介质、几何和 snapshot owner | HDD、媒体模式矩阵、snapshot |
| `base/disks/fla.c` | S2 XP+patch；保留当前 FDC/介质状态，接纳等宽赋值 | FDC、DMA、snapshot |
| `base/disks/floppy.c`、`base/disks/floppy_i.c` | S2 XP；PC-98 差异不激活，不改现有 AT FDC | FDC、双介质、冷启动 |
| `base/disks/gfi.c`、`base/disks/gfi_mpty.c` | S2 XP；配置索引/返回值转换，继续调用当前 image provider | 空盘、插拔、读写、错误返回 |
| `base/inc/base_def.h`、`base/inc/config.h` | S2 XP；注释/声明基底，不改变本地配置所有权 | 编译、源边界 |
| `base/inc/bios.h` | S2 XP；IPT0 声明须按实际 ANSI 分支核对 | BOP 链接、双宽度编译 |
| `base/inc/dma.h` | S2 XP；PC-98 bank 类型不激活，AT 仍为两控制器 | DMA 布局、snapshot |
| `base/inc/egacpu.h`、`base/inc/egagraph.h`、`base/inc/egamode.h` | S4；egacpu XP+patch，其余 XP；保留当前 C-VID 类型/声明，排除 PC-98 结构 | C-VID ABI、video modes |
| `base/inc/egavideo.h` | S4 XP+patch；保留真实字体/视频声明，NTVDM NativeFontAddr 变化不作为客户机修复 | 字体、文本、编译 |
| `base/inc/floppy.h`、`base/inc/gfi.h` | S2 XP；保留 AT FDC/GFI 布局，不启用 PC-98 通道 | FDC、介质 |
| `base/inc/gfx_upd.h`、`base/inc/gvi.h` | S4 XP；宏/声明调整，不增加 presenter | dirty、编译 |
| `base/inc/host_lpt.h`、`base/inc/printer.h` | S2 XP；不采用 PC-98 单打印口 ABI | LPT、编译 |
| `base/inc/ica.h` | S3 XP+patch；保留 IRQ/snapshot 接口，不采用 PC-98 IRQ 分配 | PIC、snapshot |
| `base/inc/ios.h` | S2/S5 XP+patch；保留现有注册契约；SB 槽位随 S5 接通，不重复分配 | 端口注册、DMA/声卡 |
| `base/inc/mouse.h` | S3 XP；不激活 PC-98/地区宿主入口 | InPort、DOS mouse |
| `base/inc/rs232.h` | S2 XP；com_save_* 的 void 返回与定义配套 | 编译、UART |
| `base/inc/sas.h` | S3 XP+patch；保留唯一 SAS 与状态捕获契约 | 内存、A20、snapshot |
| `base/inc/tape_io.h` | S2 XP；地区/NT 扩展声明不构成当前 BIOS 新服务 | 编译、BIOS |
| `base/inc/video.h` | S4 XP+patch；保留本地呈现/状态边界，不引入 DBCS 宿主 | 文本/图形、snapshot |
| `base/keymouse/keyba.c` | S3 XP+patch；60 个表项、set-3 反查修正及既有控制器状态 | 全扫描表、端口命令、snapshot |
| `base/keymouse/keybd_io.c` | S3 XP+patch；保留 BIOS 输入/空闲契约，排除 NT 地区输入旁路 | 键盘 BIOS、idle wake |
| `base/keymouse/mouse.c`、`base/keymouse/mouse_io.c` | S3 XP+patch；保留 InPort、DOS mouse、snapshot；拒绝 lightpen BX 字节截断 | Win1/Win3 mouse、端口、坐标边界 |
| `base/keymouse/ppi.c` | S3 XP+patch；保留 PC speaker/状态快照，排除 PC-98 PPI | speaker、PPI、snapshot |
| `base/support/ios.c`、`base/support/time_day.c` | S2 XP；端口字宽和已掩码的字节转换，不替换宿主时间 | I/O、BIOS time |
| `base/system/at_dma.c` | S2/S5 XP+patch；先修 auto-init 地址重装，SB 位置 hook 随 S5，不丢 DMA 快照 | DMA 自动重装、FDC、声卡、snapshot |
| `base/system/ica.c` | S3 XP+patch；保留 PIC 撤销/应答和状态快照，排除 PC-98 IRQ | IRQ、重置、snapshot |
| `base/system/idetect.c` | S3 XP+patch；XP 宽度表达不替代现有事件驱动空闲等待 | idle CPU、输入唤醒、停止 |
| `base/system/illegalp.c` | S2 XP；写入已经掩码的字节 | 编译、无设备端口 |
| `base/system/rom.c` | S3 XP+patch；拒绝 setCS UCHAR 截断，保留固定 ROM 与独立启动 | ROM 启动、段地址 |
| `base/system/timer.c` | S3 XP+patch；保留定时、音频和快照；显式字节转换不换时钟模型 | PIT、speaker、snapshot |
| `base/system/timestrb.c`、`base/system/unexp_nt.c` | S3 XP；所选语义无新增能力，排除 PC-98/文件尾控制字符影响 | 编译、时间回归 |
| `base/video/cga.c` | S4 XP+patch；保留当前 CGA 更新/快照，检查 cursor offset 的显式转换 | CGA 文本/图形、光标回绕 |
| `base/video/ega_mode.c`、`base/video/ega_vide.c` | S4 XP；保留普通 EGA 模式，地区 Console/native font 路径不激活 | EGA 字体/模式、BIOS |
| `base/video/ega_prts.c` | S4 XP+patch；保留 T88 几何/CHAIN2 接线；有符号 cursor 比较不能仅称机械 | 端口、cursor end=0、CHAIN2、模式切换 |
| `base/video/ega_read.c`、`base/video/ega_writ.c` | S4 XP+patch；保留现有地址、marker 和声明修正 | EGA read/write、CHAIN2 |
| `base/video/egawrtm0.c`、`base/video/egwrtm12.c` | S4 XP；显式存储宽度，仍使用原写入算法 | EGA 写模式/plane mask |
| `base/video/gfx_updt.c` | S4 XP+patch；保留文本 stride、dirty/split/wrap、发布时序 | 文本非标准 stride、图形、snapshot |
| `base/video/gvi.c` | S4 XP；不启用地区宿主视频路径 | 视频初始化、编译 |
| `base/video/v7_ports.c` | S4 XP+patch；采用 bit7 比较修正，保留 V7 几何/快照 | 硬件光标启停、重复端口写 |
| `base/video/v7_video.c` | S4 XP+patch；保留 V7 marker、绘制及状态语义 | V7 全模式、cursor、snapshot |
| `base/video/vga_mode.c`、`base/video/vga_prts.c` | S4 XP+patch；保留现有真实模式与端口修复 | VGA 模式矩阵、DAC、几何切换 |
| `base/video/vga_vide.c`、`base/video/video.c`、`base/video/video_io.c` | S4 XP；排除硬件 fullscreen/DBCS，不能把 AH=4f 宿主切换宣传为 VBE | VGA BIOS、mode set、文本往返 |
| `host/inc/host_def.h`、`host/inc/insignia.h` | S2 XP+patch；保留本机指针/平台定义；ARCX86、NT 编译选项不激活 | 双宽度 ABI、编译 |
| `host/inc/host_gen.h` | S2 XP；push_word 同宽赋值，不改变模拟栈 | CPU/BOP、编译 |
| `host/inc/host_inc.h`、`host/inc/host_rrr.h`、`host/inc/monregs.h` | S2 XP；NT/地区宿主声明不成为独立宿主实现 | 源选择、编译 |
| `host/inc/nt_cga.h`、`host/inc/nt_fulsc.h` | S4 XP；原生 Console/fullscreen 声明保持未选 | 编译、无原生 fullscreen 依赖 |
| `host/inc/nt_graph.h` | S4 XP+patch；保留独立 DIB/字体边界 | copied frame、字体、编译 |
| `host/inc/nt_pif.h`、`host/inc/nt_reset.h`、`host/inc/nt_uis.h` | S2 XP；PIF/WOW/资源属于未选 NT 宿主，不新增 App 配置 | 源选择、编译 |
| `host/src/nt_cga.c` | S4 XP+patch；保留固定目标 stride、裁剪、字体/表面更新 | 文本行布局、CGA 图形 |
| `host/src/nt_ega.c` | S4 XP+patch；保留已有 DIB 边界/绘制修正，坐标显式 SHORT | EGA 像素与边界 |
| `host/src/nt_graph.c` | S4 XP+patch；保留独立图形宿主，不引入 SwitchToFullScreen 或 NT Console 呈现 | 单一呈现路径、模式切换 |
| `host/src/nt_keycd.c` | S3 XP+patch；采用 Pause/NumLock 区分并保留数组 >= 边界 | 键盘映射、越界输入 |
| `host/src/nt_sound.c` | S3 XP+patch；保留已验收的 PCM speaker，不导入 NT SessionId/Beep 路由 | 冷/热启动首次发声、stop/reset |
| `host/src/nt_vga.c` | S4 XP+patch；保留已有图形高度、边界及表面修正 | VGA 像素、分辨率、snapshot |

表中“验证接收范围”是后续测试的义务，不表示现有同名测试已经完整覆盖。
特别是 BIOS 截断边界和新增键盘表项，必须在迁移前后做定向值比较，
不能以编译无警告替代语义证明。

EGA cursor 的 signed/unsigned 比较已做一轮值域排查：两个寄存器字段均为
5 bit；在 `do_new_cursor` 中，end=0 的专用分支位于 `end-1` 比较之前，
所以不能把孤立表达式的下溢当作实际可达错误。对 start/end=0..31、
height=0..32 的 33,792 组分支选择枚举，原比较与 XP 比较结果全部相同。
这证明该范围内的比较转换不改变分支，不是整条 EGA 光标输出的运行测试，
也不覆盖非法负高度。迁移仍保留原分支次序，不需要为 end=0 添加新特判。

上述 86 路径以固定 inventory 的关系分类自动核对：每个预期路径在此表
出现恰好一次，遗漏/重复均为 0。此覆盖检查只证明账目闭合，不替代各行
语义和后续动态验证。

### S1 完成性与后续准入结论

[498 个保留路径清单](xp-sp1-retained-paths.tsv) 与前述 499 路径清单构成
固定 XP 的全部 997 路径，交集为空。338 个路径无需内容重写；44 个路径
按已审查 profile 采用 XP；116 个采用 XP 并保留/补充本地修正。其中 74 个
本地独有路径的 XP/OpenNT 基底相同，保留补丁即可，不需要为了换基底重写。
这与 86 个 XP 变化文件的语义表配套，不以 TSV 替代因果解释。

| 原始审计要求 | 证据与结论 |
| --- | --- |
| 当前镜像与 XP 差异 | 固定三方提交、498 路径关系/覆盖清单、86 变化路径表、74 本地补丁保护项。 |
| 以 XP 为镜像基底升级 | Proposal S2–S6 分批接收；比较基准改为 XP，OpenNT 仅作迁移血缘；不直接覆盖现有实现。 |
| 纳入 IBM PC 升级 | DMA、CMOS、V7、键盘及描述符 ABI 均有迁移/验证接收方；SB 七文件及四处生命周期义务保留为 S5 必需范围。 |
| 排除非 IBM PC 差异 | 13 个新增 PC-98 路径排除；保留文件中相应分支必须不选。NT 宿主、旧工具链与重复实现另有明确处置，不混称为非 IBM PC。 |
| 不丢本地修复 | CCPU、C-VID、InPort、绘制、snapshot、独立媒体/时钟/声音修复保留；28 marker 与 48 continuation 点位核对。 |
| 可执行建议与风险 | S2/S3/S4 分别 37/18/31 变化路径；参考 diff 量与盲覆盖风险已量化。S5 给出单一 owner、时间、PCM、生命周期/快照及测试边界，但须先选定缺失 OPL/MIDI 后端和解决源码准入。 |

源审计不等于已经实现升级。S5 无法仅凭 XP 树得到完整独立声卡，最终代码量
不能诚实地报成一个小补丁；该结论是本次审计的发现，不应通过排除 IBM PC
声音能力来伪造任务完成。首次复制前仍须按源码政策确认权利依据；版权声明
不构成再分发许可。后续源码变更必须分别准入和完成双宽度编译、定向及全量
测试、部署和提交推送，本轮不代做或宣称其结果。

地区宏不是架构证明：JAPAN 下可能有 IBM PC 地区输入行为，不能自动等同
NEC_98 硬件。当前审计仅选择已定义的普通 AT/V7 profile；不增加新地区产品。
本轮只有文档/研究元数据变更，没有生产、测试、二进制、配置或介质变更。

## 以下为 T40 历史审计（由上述 T92 检查点逐步替代）

## 目的与口径

本报告将 XP SP1 的 `mvdm/softpc.new` 与 OpenNT 和当前
`src/mvdm/softpc.new` 三方比较。它不是优先级表：`A`、`B` 仅说明三方
关系；每个文件的“语义类别”说明 XP 实际改变了什么。

- **A**：本项目内容仍等于 OpenNT，而 XP SP1 不同。
- **B**：OpenNT、XP SP1 与本项目三方内容均不同。
- **既有能力修复**：XP 修改当前 normal AT/V7 可达的既有设备/BIOS 行为。
- **既有能力不同实现**：目标功能相同，但 XP 依赖 NTVDM 宿主，或本项目
  已由 standalone 适配器承担实现。
- **表示/移植修正**：位宽、类型、显式窄化、声明或编译器表达；不改变
  客体机器语义。
- **非当前 profile**：PC-98、Japan、SoundBlaster 或其他未选择机器配置。
- **NTVDM 产品宿主**：PIF、VDD、会话、NT 内存、窗口、输入、声音或设备
  资源的产品宿主实现。
- **格式/注释**：没有编译后机器差异。

审计采用当前实际机器定义：`CPU_40_STYLE CCPU C_VID CPU_486 SPC386
V7VGA WDCTRL_BOP PROD SOFTPC_STANDALONE`；不定义 `NTVDM`、`NEC_98` 或
`JAPAN`。基线固定为 OpenNT `5e4619ab6` 和 XP SP1 `a2f6d7c93`。因此一个
XP hunk 即使位于普通文件中，只要被 `NEC_98`、`JAPAN` 或 `NTVDM` 排除，
也不算本项目机器 profile 的升级。

## T40 后的路径库存校正

本报告的 86 个 A/B 条目来自 T40 前的三方内容比较；T40 没有删除其中任何
一项，因此 A=58、B=28 及以下逐文件结论均不变。变化的是“OpenNT 有、XP
没有”的单边库存：初始 126 条中，本项目原已缺少 96 条，T40 删除最后 30
条 MIPS/PPC 头文件后，本项目现已缺少全部 126 条。

T40 还删除 `base/inc/get_env.h`、`base/inc/gfisflop.h` 与
`host/inc/monsim32.h`。三者在 OpenNT 与 XP 中都存在，故不属于这 126 条，
也不改变 XP-only 21 条的分类；它们只是当前 standalone 未选择的历史声明。
因此本地 C/H 库存从初始审计的 526 降至 493。T40 的双宽 23/23 CTest 与
package smoke 已通过，详情见
[T40 收口记录](../../../history/M9-T40-xp-obsolete-header-retirement.md)。

## 结论摘要

这 86 个 A/B 文件中，尚未发现 XP 为当前 normal AT/V7 profile 新增一个
独立的设备、BIOS 服务、CPU 或显示能力。XP 的主要增量是 PC-98/Japan
分支和 NTVDM 产品宿主演进。当前 profile 可达且有明确语义的 XP hunk 分为：

- **表示/移植修正**：CMOS、打印机、GFI/FDC 返回值以及 NT 绘图矩形坐标
  的显式窄化；它们不是客体功能升级。
- **既有能力修复**：`keyba.c` 将共享扫描码 `0x45` 的 Pause 与 NumLock
  区分开；这是普通 AT 键盘输入路径的实际修复，需与本项目输入适配三方合并。
- **既有能力不同实现**：reset、ROM、PIC、timer、鼠标、原始 `nt_*`
  renderer/keyboard/sound 文件。这些文件的 XP 改动不能整体采用，因为 XP
  使用 NTVDM 产品宿主或本项目已有 standalone 所有者。

除明确写作“可直接采用的窄 hunk”者外，以下结论均不是复制整个 XP 文件的
授权。

## A：本项目等于 OpenNT，XP 不同

| 文件 | XP hunk 的实际语义类别 | 当前 AT/V7 可达部分与审计结论 |
| --- | --- | --- |
| `base/bios/cmos_bis.c` | 表示/移植修正；格式/注释 | 四处 `outb` 参数显式转为 `IU8`，保持 8 位 CMOS 端口值。可作为独立窄 hunk，非功能升级。 |
| `base/comms/printer.c` | 非当前 profile；表示/移植修正 | 大块为 PC-98 LPT 状态机；普通 AT 路径仅有 `IU8` 状态/控制位及 `state` 收紧。只审查这些窄化，排除 PC-98。 |
| `base/comms/printer_.c` | 非当前 profile；格式/注释 | XP 增量围绕 PC-98 打印/调试路径；没有确认的 normal AT 算法变化。保留本项目路径。 |
| `base/comms/rs232_io.c` | 非当前 profile；NTVDM 产品宿主 | XP 引入/包围 Japan、PC-98 和 NT 串口宿主假设；不是普通 UART 新能力。保留 standalone 通信端点。 |
| `base/disks/diskbios.c` | 非当前 profile；既有能力不同实现 | XP 扩充平台/宿主媒体发现与 BIOS 磁盘路径；固定映像 A:/C:/双媒体契约不能被替换。无确认可直接采用 hunk。 |
| `base/disks/fdisk.c` | 非当前 profile；表示/移植修正；格式/注释 | **可采用部分、但不能整文件采用。** XP 把 PC-98 的 `fdisk_outb`、`fdisk_ioattach`、`fdisk_iodetach` 分支包入 `NEC_98`，当前 profile 不编译；normal AT 路径只有驱动器几何字段和 `patch_rom` 的显式 `unsigned char`/`unsigned short` 窄化，以及 `C_HARD_DISK1_NAME + driveno` 的 `IU8` 索引窄化。它们保持现有 WD2010/FDC 算法与本项目映像后端，可作为独立表示修正审查；没有 XP 的普通 AT 磁盘行为修复可采用。 |
| `base/disks/fla.c` | 非当前 profile；表示/移植修正 | 主体是 PC-98 驱动器发现；普通路径出现局部状态字节窄化。仅可抽取已证明不穿过媒体宿主边界的窄化。 |
| `base/disks/floppy.c` | 非当前 profile；表示/移植修正 | XP 大量 PC-98 FDC/媒体代码；普通 AT 的 `sas_store`/状态窄化不是能力增加。不能将 PC-98 控制器带入。 |
| `base/disks/floppy_i.c` | 非当前 profile；既有能力不同实现 | XP 的扩展软盘 BIOS/物理设备处理依赖其宿主环境。当前 FDC 与映像端点保持独立。 |
| `base/disks/gfi.c` | 表示/移植修正；格式/注释 | 将 FDC 返回值转为 `SHORT`、设备配置索引转为 `UTINY`；保持原 GFI 算法。可单独审查。 |
| `base/disks/gfi_mpty.c` | 表示/移植修正；格式/注释 | 空 GFI 命令返回值显式转为 `SHORT`；可单独审查。 |
| `base/inc/base_def.h` | 表示/移植修正；非当前 profile | 共享基础声明包含多平台扩展。只在一个获准实现 hunk 确需声明时最小合并。 |
| `base/inc/bios.h` | 非当前 profile；既有能力不同实现 | XP 扩大 BIOS/平台声明，不能单独视为 BIOS 能力。只跟随获准 BIOS hunk。 |
| `base/inc/config.h` | 非当前 profile；NTVDM 产品宿主 | XP 暴露的配置面包含产品/平台选项；不改变 standalone 固定 machine profile。 |
| `base/inc/dma.h` | 非当前 profile | XP 的 DMA 声明服务 PC-98/扩展配置；normal AT DMA 无确认算法升级。 |
| `base/inc/egagraph.h` | 非当前 profile；表示/移植修正 | 图形声明伴随其他显示分支；不是独立能力。 |
| `base/inc/egamode.h` | 非当前 profile；表示/移植修正 | 同上；仅跟随已确认 normal EGA hunk。 |
| `base/inc/egavideo.h` | 非当前 profile；表示/移植修正 | 同上；不能单独复制。 |
| `base/inc/floppy.h` | 既有能力不同实现 | 声明与 XP 物理软盘宿主耦合；本项目映像 ABI 优先。 |
| `base/inc/gfi.h` | 既有能力不同实现；表示/移植修正 | 仅为 GFI 窄化提供必要声明时合并；不引入 XP 媒体契约。 |
| `base/inc/gfx_upd.h` | 既有能力不同实现 | 仅配合显示算法 hunk；本身没有可见能力。 |
| `base/inc/gvi.h` | 非当前 profile；既有能力不同实现 | XP 图形接口涵盖非选择路径；不单独采用。 |
| `base/inc/host_lpt.h` | NTVDM 产品宿主 | XP 并口宿主接口，不是客体 LPT 控制器逻辑。 |
| `base/inc/mouse.h` | 非当前 profile | XP 的地区/平台输入定义；normal AT 鼠标行为须从实现 hunk 而非头文件判定。 |
| `base/inc/printer.h` | 表示/移植修正；非当前 profile | 只服务打印机 AT 窄化或 PC-98 分支；无独立能力。 |
| `base/inc/rs232.h` | NTVDM 产品宿主；非当前 profile | 串口宿主接口差异；保持 standalone endpoint。 |
| `base/inc/tape_io.h` | 非当前 profile；NTVDM 产品宿主 | 对应 Japan/NT 扩展内存服务；不进入当前 BIOS 服务集。 |
| `base/keymouse/ppi.c` | 非当前 profile | XP 的主要增量为 PC-98 PPI 初始化与端口行为；normal AT 8042/PPI 不升级。 |
| `base/support/time_day.c` | 既有能力不同实现；格式/注释 | XP 依赖原 host 时间服务表达；本项目 host clock 是 standalone 所有者。无直接机器 hunk。 |
| `base/system/at_dma.c` | 非当前 profile | PC-98/SoundBlaster 条件支持；当前 AT DMA 不引入。 |
| `base/system/illegalp.c` | NTVDM 产品宿主；非当前 profile | XP 的异常/产品集成条件不属于固定 SoftPC 机器。 |
| `base/system/timestrb.c` | NTVDM 产品宿主；格式/注释 | 时间节拍宿主处理的 XP 差异不提供独立客体计时改进。 |
| `base/system/unexp_nt.c` | NTVDM 产品宿主 | NT 异常处理入口；standalone 不采用。 |
| `base/video/cga.c` | 非当前 profile；表示/移植修正 | XP 主体为多平台显示条件和类型清理；无确认的新 CGA 模式/算法。 |
| `base/video/ega_mode.c` | 非当前 profile；NTVDM 产品宿主 | 增量主要在非目标/NTVDM 显示路径；保留当前 EGA mode owner。 |
| `base/video/ega_prts.c` | 非当前 profile；表示/移植修正 | PC-98/显示端口条件及表示清理；不增加普通 EGA 端口能力。 |
| `base/video/ega_read.c` | 非当前 profile；表示/移植修正 | 同上；没有确认 normal AT 读算法修复。 |
| `base/video/ega_vide.c` | 非当前 profile；既有能力不同实现 | XP 显示调度依赖其产品呈现层；不整体采用。 |
| `base/video/ega_writ.c` | 非当前 profile；表示/移植修正 | 同上；仅有表示性质 hunk 才可独立审查。 |
| `base/video/egawrtm0.c` | 非当前 profile；表示/移植修正 | 同上。 |
| `base/video/egwrtm12.c` | 非当前 profile；表示/移植修正 | 同上。 |
| `base/video/gvi.c` | 非当前 profile | XP 图形视频接口含扩展机器路径；不是 standalone 新能力。 |
| `base/video/v7_ports.c` | 非当前 profile；表示/移植修正 | XP 变动主要包围/支撑扩展配置；未确认 normal V7 端口算法修复。 |
| `base/video/v7_video.c` | 非当前 profile；既有能力不同实现 | XP renderer 调用依赖其宿主；本项目仍有 V7 renderer 和独立表面。 |
| `base/video/vga_mode.c` | 非当前 profile；表示/移植修正 | XP 多配置模式差异；没有发现新增 normal V7/VGA mode。 |
| `base/video/vga_prts.c` | NTVDM 产品宿主；非当前 profile | XP 包含 NTVDM cursor/synchronization 与配置分支；不整体采用。 |
| `base/video/vga_vide.c` | NTVDM 产品宿主；表示/移植修正 | 产品绘图同步与类型清理混合；无独立客体能力。 |
| `base/video/video.c` | NTVDM 产品宿主；非当前 profile | XP 顶层 video dispatch 为多产品配置服务；保留 standalone 路由。 |
| `base/video/video_io.c` | NTVDM 产品宿主；非当前 profile | XP I/O routing 与 NTVDM/X86GFX 条件相关；不采用。 |
| `host/inc/host_gen.h` | NTVDM 产品宿主 | 原 NT host 通用声明；standalone 不以它为功能来源。 |
| `host/inc/host_inc.h` | NTVDM 产品宿主 | 同上。 |
| `host/inc/host_rrr.h` | NTVDM 产品宿主 | 同上。 |
| `host/inc/monregs.h` | NTVDM 产品宿主 | XP monitor/product register 合同；不属于 machine profile。 |
| `host/inc/nt_cga.h` | NTVDM 产品宿主 | XP CGA 呈现宿主声明；当前 DIB adapter 保留。 |
| `host/inc/nt_fulsc.h` | NTVDM 产品宿主 | XP 全屏产品服务；不导入。 |
| `host/inc/nt_pif.h` | NTVDM 产品宿主 | XP PIF 服务；不导入。 |
| `host/inc/nt_reset.h` | NTVDM 产品宿主 | XP host reset 约定；当前 reset topology 保留。 |
| `host/inc/nt_uis.h` | NTVDM 产品宿主 | XP UI/product 服务；不导入。 |

## B：三方均不同

| 文件 | XP hunk 的实际语义类别 | 当前 AT/V7 可达部分与审计结论 |
| --- | --- | --- |
| `base/bios/reset.c` | 非当前 profile；既有能力不同实现；表示/移植修正 | XP 大块 IVT/POST 变更为 PC-98；normal AT 有 `npx_reset`、清 CMOS shutdown 等 reset 维护。local 已有 reset host-representation 修正，故只能按 reset hunk 三方合并，不能整体采用。 |
| `base/bios/tape_io.c` | 非当前 profile；NTVDM 产品宿主；表示/移植修正 | XP 的 Japan INT 15 扩展内存使用 NT 虚拟内存/XMS；normal 分支仅见地址类型表达。local caller-context 行为问题独立存在，XP 不解决。 |
| `base/ccpu386/fpu.c` | 格式/注释；表示/移植修正（local） | XP 对该文件的可见改动是注释去粗话；不构成 CPU/FPU 修复。保留 local CCPU ABI 修正。 |
| `base/comms/com.c` | 非当前 profile；既有能力不同实现 | XP 新增 PC-98 8251 UART；normal AT 16550 逻辑没有 XP 功能升级。local standalone serial ABI 保留。 |
| `base/inc/egacpu.h` | 表示/移植修正；既有能力不同实现 | XP 与 local 均影响 C-VID/CPU 声明；只有获准显示/CPU 实现 hunk 才能三方合并。 |
| `base/inc/ica.h` | 表示/移植修正；既有能力不同实现 | PIC 声明随 host ABI 演进；不能覆盖 local IRQ 适配。 |
| `base/inc/ios.h` | 既有能力不同实现；NTVDM 产品宿主 | I/O callback 声明跨 host 边界；保持 standalone ABI。 |
| `base/inc/sas.h` | 表示/移植修正 | XP 与 local 都涉及地址/表示；local x86/x64 SAS 适配为准。 |
| `base/inc/video.h` | 表示/移植修正；既有能力不同实现 | 只随获准 renderer hunk 合并。 |
| `base/keymouse/keyba.c` | 既有能力修复；非当前 profile；表示/移植修正 | XP normal 路径修复 ScanCode `0x45`：以 virtual key 区分 Pause 与 NumLock；这是当前 AT 键盘可达修复。Japan/PC-98 键表不采用。须与 local 输入转换三方合并并做 Pause/NumLock/Ctrl-Alt-Del 回归。 |
| `base/keymouse/keybd_io.c` | 非当前 profile；既有能力不同实现 | XP 主要为 Japan/PC-98 键盘行为；local 键盘/CCPU ABI 已独立。无确认普通 AT 新能力。 |
| `base/keymouse/mouse.c` | 既有能力不同实现；非当前 profile | local 有 Windows 3.1 InPort 诊断行为；XP 变动不能覆盖该已验证 standalone 行为。 |
| `base/keymouse/mouse_io.c` | 既有能力不同实现；非当前 profile | 保留 local mouse I/O/capture 适配；XP 非目标输入分支不采用。 |
| `base/support/ios.c` | NTVDM 产品宿主；既有能力不同实现 | XP I/O support 依赖原 host；本项目 external compat owner 保留。 |
| `base/system/ica.c` | 非当前 profile；既有能力不同实现 | XP PIC 扩展与 local IRQ/typing 适配冲突；只有可证明 normal AT IRQ 语义修复的 hunk 才三方合并。 |
| `base/system/idetect.c` | 既有能力不同实现；表示/移植修正 | XP 私有 MVDM include/layout 假设不同；local standalone declaration adaptation 保留。 |
| `base/system/rom.c` | 既有能力不同实现 | XP ROM/config 路径与当前固定 ROM、映像媒体契约不同；不可整文件采用。 |
| `base/system/timer.c` | 非当前 profile；NTVDM 产品宿主；格式/注释 | XP 功能增量是 PC-98 时钟/端口及 `NTVDM` 状态；normal AT hunk 实质为格式。local zero-delay quick-event 防护必须保留。 |
| `base/video/gfx_updt.c` | 既有能力不同实现 | XP 图形更新与 local copied-frame/DIB 路由修改同一呈现边界；不是新客体模式，需 renderer 三方合并。 |
| `host/inc/host_def.h` | NTVDM 产品宿主；表示/移植修正 | XP host 数据表示不是 local x64 host ABI 的替代品。 |
| `host/inc/insignia.h` | NTVDM 产品宿主；表示/移植修正 | 同上。 |
| `host/inc/nt_graph.h` | NTVDM 产品宿主；既有能力不同实现 | XP 图形 host contract 不取代 standalone DIB contract。 |
| `host/src/nt_cga.c` | 既有能力不同实现；表示/移植修正 | 同一 CGA dirty-rectangle 呈现能力，XP 是 NTVDM window host；local standalone adapter 保留。 |
| `host/src/nt_ega.c` | 既有能力不同实现；表示/移植修正 | 同一 EGA 呈现能力，XP 是 NTVDM host；不整文件采用。 |
| `host/src/nt_graph.c` | NTVDM 产品宿主；既有能力不同实现 | XP 图形资源/窗口生命周期不属于 machine profile。 |
| `host/src/nt_keycd.c` | NTVDM 产品宿主；既有能力不同实现 | XP Windows keyboard host mapping 不取代 standalone input adapter；但 `keyba.c` 的 Pause 修复可独立审查。 |
| `host/src/nt_sound.c` | NTVDM 产品宿主；非当前 profile | XP 为 Session 0/非 Session 0 选择不同 beep host，且含 PC-98 声音；不是客体 PC speaker 新能力。 |
| `host/src/nt_vga.c` | 既有能力不同实现；表示/移植修正 | XP 将 `RECT` 坐标显式转为 `SHORT`；它是 NT renderer 的类型清理，不是 VGA/V7 模式修复。 |

## 需要进入后续任务的实际候选

1. **表示/移植候选**：`cmos_bis.c`、`printer.c` normal AT 部分、`gfi.c`、
   `gfi_mpty.c`，以及已证明等价的局部 byte/`SHORT` 窄化。它们应各自有
   编译和设备 smoke 证明。
2. **既有能力修复候选**：`keyba.c` Pause/NumLock `0x45` 区分。这是目前
   唯一从 XP diff 中确认、会改变当前 normal AT/V7 可达行为的修复；需要
   三方合并和键盘回归，不可原样覆盖 local 文件。
3. **reset 维护候选**：`reset.c` 中 normal AT 的 NPX reset/CMOS shutdown
   维护需要单独核对调用点与 local reset topology；本报告不把它先验列为
   已确认修复。

所有其他 A/B 差异都应继续作为 XP-to-local 镜像差异记录，而非升级待办。

## 最终源文件去向账本

下列结论是每个文件的最终源代码去向，不是优先级。`全部采用`表示该文件
应以 XP SP1 原文件为基线；若当前文件带有必要的 x64 直接移植修正，必须在
XP 文件上重施该修正。它不启用 `NTVDM`、`NEC_98` 或 `JAPAN` 宏。`部分采用`
表示保持当前 OpenNT/local 主体，仅取得本报告点名的 XP hunk。`完全不采用`
表示 XP 文件/语义不进入当前 standalone 路线。

### 全部采用 XP SP1 文件；重施必要 x64 直接修正

这些 A 文件的 XP 变化在当前 profile 下是受条件隔离的非目标代码、格式，或
与 local host 无关的窄化；采用整个 XP 文件能让镜像直接以 XP 为基线，不会
改变 selected source model。

| 关系 | 文件 |
| --- | --- |
| A | `base/bios/cmos_bis.c` |
| A | `base/comms/printer.c`, `base/comms/printer_.c` |
| A | `base/disks/diskbios.c`, `base/disks/fdisk.c`, `base/disks/fla.c`, `base/disks/floppy.c`, `base/disks/floppy_i.c`, `base/disks/gfi.c`, `base/disks/gfi_mpty.c` |
| A | `base/inc/base_def.h`, `base/inc/bios.h`, `base/inc/config.h`, `base/inc/dma.h`, `base/inc/egagraph.h`, `base/inc/egamode.h`, `base/inc/egavideo.h`, `base/inc/floppy.h`, `base/inc/gfi.h`, `base/inc/gfx_upd.h`, `base/inc/gvi.h`, `base/inc/mouse.h`, `base/inc/printer.h` |
| A | `base/keymouse/ppi.c`, `base/support/time_day.c`, `base/system/at_dma.c` |
| A | `base/video/cga.c`, `base/video/ega_mode.c`, `base/video/ega_prts.c`, `base/video/ega_read.c`, `base/video/ega_vide.c`, `base/video/ega_writ.c`, `base/video/egawrtm0.c`, `base/video/egwrtm12.c`, `base/video/gvi.c`, `base/video/v7_ports.c`, `base/video/v7_video.c`, `base/video/vga_mode.c`, `base/video/vga_prts.c`, `base/video/vga_vide.c`, `base/video/video.c`, `base/video/video_io.c` |
| B | `base/ccpu386/fpu.c` |

`fdisk.c` 特别明确：全部采用 XP 文件。PC-98 路径仅在 `NEC_98` 下编译；
normal AT 路径的 XP 改动是几何/ROM 写入的窄化，故在 XP 文件上保持这种
表达即可，不保留 OpenNT 文件作为主体。

### 采用 XP SP1 文件为主体；重施 local standalone/x64 patch

这些文件同样以 XP SP1 原文件为镜像基线，**不是**以 OpenNT/local 为主体。
其 XP hunk 有的在当前 defines 下天然不编译，有的必须由一份显式 local patch
重设 standalone 所有权。最终审查对象始终是 `XP file -> local patch`：被拒绝的
XP 语义也必须以有原因的反向 local diff 表达，不能靠继续保留 OpenNT 文件隐藏。

| 关系 | 文件 | 仅采用的 XP 内容或保留原因 |
| --- | --- | --- |
| A | `base/comms/rs232_io.c` | XP 文件为主体；重施 standalone serial endpoint patch。PC-98/产品 hunk 保留在镜像但在当前 defines 下不可达，或由显式 local diff 禁止。 |
| A | `base/inc/host_lpt.h`, `base/inc/rs232.h`, `base/inc/tape_io.h` | XP 声明为主体；重施最小 local host-ABI 声明 patch，不接纳 XP product-host contract 为实现。 |
| A | `base/system/illegalp.c`, `base/system/timestrb.c` | XP 文件为主体；以 local patch 排除产品/计时宿主语义，保留已证明的 normal-AT 表示。 |
| B | `base/bios/reset.c` | XP 文件为主体；重施 local reset topology/host-representation patch，并保留 XP normal-AT NPX reset/CMOS shutdown 维护。 |
| B | `base/comms/com.c` | XP 文件为主体；PC-98 8251 在当前 defines 下不可达，重施 local normal-UART host ABI patch。 |
| B | `base/inc/egacpu.h`, `base/inc/ica.h`, `base/inc/ios.h`, `base/inc/sas.h`, `base/inc/video.h` | XP 声明为主体；重施 local x86/x64 ABI patch，禁止以 XP 32-bit host ABI 覆盖它。 |
| B | `base/keymouse/keyba.c` | XP 文件为主体；保留 XP Pause/NumLock `0x45` 修复，再重施 local 输入和热键适配。 |
| B | `base/keymouse/keybd_io.c`, `base/keymouse/mouse.c`, `base/keymouse/mouse_io.c` | XP 文件为主体；重施 local Windows 3.1/InPort、capture 与输入 ABI patch。 |
| B | `base/system/ica.c`, `base/system/idetect.c`, `base/system/rom.c`, `base/system/timer.c` | XP 文件为主体；重施 local IRQ、include ABI、ROM/media、zero-delay timer patch。 |
| B | `base/video/gfx_updt.c` | XP 文件为主体；重施 local copied-frame/DIB 路由 patch。 |

### 完全不采用 XP 文件；保留 standalone 实现或记录镜像差异

| 关系 | 文件 | 原因 |
| --- | --- | --- |
| A | `base/system/unexp_nt.c` | XP NT 异常产品宿主，不是 selected machine。 |
| A | `host/inc/host_gen.h`, `host/inc/host_inc.h`, `host/inc/host_rrr.h`, `host/inc/monregs.h`, `host/inc/nt_cga.h`, `host/inc/nt_fulsc.h`, `host/inc/nt_pif.h`, `host/inc/nt_reset.h`, `host/inc/nt_uis.h` | XP 原始 NTVDM host/product declarations；不构成 standalone machine source。 |
| B | `base/bios/tape_io.c` | XP 仅提供 Japan/NT 虚拟内存路径，且不解决 local 行为 blocker。 |
| B | `base/support/ios.c` | local external compatibility owner 取代 XP host support。 |
| B | `host/inc/host_def.h`, `host/inc/insignia.h`, `host/inc/nt_graph.h` | XP NTVDM host ABI 不可作为 local x64/standalone ABI。 |
| B | `host/src/nt_cga.c`, `host/src/nt_ega.c`, `host/src/nt_graph.c`, `host/src/nt_keycd.c`, `host/src/nt_sound.c`, `host/src/nt_vga.c` | XP 是 NTVDM 窗口、输入、声音与图形宿主实现；保持 standalone adapter，至多另开任务移植已证明自包含的算法。 |
