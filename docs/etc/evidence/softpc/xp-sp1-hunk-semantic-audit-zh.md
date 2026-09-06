# XP SP1 SoftPC 逐文件语义审计

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
