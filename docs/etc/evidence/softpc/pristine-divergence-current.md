# MVDM / OpenNT 当前差异账本

## 冻结范围与复算

T61 S1，2026-09-16，SoftPC `893db1ef790548de4d391dc71a9e5df9cb4e05c6`
（生产内容与 T60 `3df951c` 相同）；OpenNT
`5e4619ab61c2aa76151e03973cce340be2933e61`。原始树所选路径工作区无改动。
比较 `src/mvdm/softpc.new/` 中全部 tracked 文件与只读 OpenNT
`nt/private/mvdm/softpc.new/` 同路径文件。旧表归档为
[M1 历史快照](pristine-divergence-m1-snapshot.md)，其中 overlay/旧任务号不是当前决定。

复算入口：`tools/Audit-MirrorDiff.ps1 -OriginalRoot <OpenNT softpc.new>`。
脚本只读、JSON 输出，不进入构建/测试依赖，也不生成源码。输出两端每文件 SHA256、
每个 hunk 的原始坐标和增删数；以冻结 revision 和路径即可重建完整 498 文件全集。
Git 2.49.0.windows.1，`core.autocrlf=false`、`core.safecrlf=false`、
`diff --no-index --no-ext-diff --no-textconv --unified=3`。

| 口径 | 文件 | +行 | -行 | 区块 |
| --- | ---: | ---: | ---: | ---: |
| 原始文本（不忽略空白） | 97 | 46,241 | 44,893 | 5,112 |
| 辅助内容口径（另加 `--ignore-space-at-eol`） | 97 | 23,779 | 22,434 | 5,138 |

第二行同时忽略 CRLF/LF 和行尾空格，不可称为仅忽略换行；不是替代第一行。
格式差异可将多个内容区块合并为整文件差异，因此原始区块数反而较少。
498 保留文件中 401 字节相等、97 不同；无同路径原始文件者 0。
原始 1,148 文件中的 650 未选入文件单列在脚本输出，不计作修改/删除。
原始增删合计 91,134，净增 1,348；辅助合计 46,213，净增 1,345。
这解释了旧报告约 46,219 的口径差异，不能据此声称生产代码又增加数万行。

十二组互斥汇总（辅助内容口径）：

| 组 | 文件 | +行 | -行 | 区块 |
| --- | ---: | ---: | ---: | ---: |
| C-VID 规则 | 34 | 22,025 | 21,985 | 4,891 |
| 其余 C-VID | 7 | 73 | 37 | 18 |
| CCPU | 15 | 241 | 65 | 75 |
| base/inc | 13 | 87 | 17 | 20 |
| BIOS | 2 | 18 | 7 | 7 |
| 通信 | 1 | 4 | 3 | 4 |
| 键鼠 | 4 | 68 | 21 | 14 |
| support | 1 | 0 | 50 | 5 |
| 系统设备 | 5 | 50 | 15 | 16 |
| 视频控制器 | 2 | 203 | 145 | 34 |
| host 头 | 6 | 136 | 43 | 15 |
| host 实现 | 7 | 874 | 46 | 39 |
| 合计 | 97 | 23,779 | 22,434 | 5,138 |

## 覆盖及处置规则

以下表中路径均相对镜像根。A/D/H 为辅助口径增加/删除/区块数，便于识别内容。
每行涵盖该文件全部 hunk；按源码顺序列出的原因覆盖其新增/删除内容。
格式与功能混在同一 hunk 时分别处置，不因 hunk 属于功能文件而保留无意义空白。
全体文件均另受 F 规则覆盖：与原版不同的换行、行尾、空白、纯缩进由 S2
验证并恢复；不重排原版、不删原版本来有的空行。原始格式可能增加当前行数。

- **R**：保留必要 ABI/已验证功能；S6 复核既有回归。不是声称每个注释字节必要。
- **F/S2**：无行为清理，须证明选中预处理 token 等价。
- **S3**：原分支恢复，先核对真实编译选项与双宽度预处理结果。
- **S4/S5**：独立宿主分支已知用途，但最小性尚未证明；对应阶段评估，不能提前标为必要。

## C-VID 规则：34 文件的完整模式证明

`sevid000..030.c`、`sinit011..013.c` 合计 +22,025/-21,985。
对每个文件在内存中逆向替换下列已枚举模式、去掉对应新增 include/两处解释注释，
再忽略空白比较完整文件：**34/34 完全相等**，无其他内容例外。
这证明分类穷尽，不证明替换本身行为正确；行为沿用既有 C-VID 双宽度测试。
脚本 `rule_patterns_match` 可复核；它没有写入原始/当前文件。

1. 固定 GDP 偏移解引用改为 `softpc_gdp_rule_slot`，保留原偏移及访问类型。
2. 动态 GDP 偏移同上，偏移显式 `unsigned int`，访问宽度 `sizeof(原类型)`。
3. 间接地址解引用改为 `softpc_gdp_rule_address`，保留原寄存器/类型。
4. LocalIUH 固定四字节字数分配改为 `calloc(原字节数/4, sizeof(IUH))`。
5. 仅 sevid019/020：移位字面量 `1` 改为 `(IUH)1`，匹配原生宽载体及原 sizeof 检查。

全部 R；GDP 槽位所有者为 `src/compat/cvidc/gdp_state.c`，访问声明为同目录
`gdp_rule_access.h` / `gdp_slots.h`；不能撤回而重新让 x64 指针挤入四字节槽。

| 文件（base/cvidc/） | A/D/H | 文件 | A/D/H |
| --- | --- | --- | --- |
| sevid000.c | 476/475/94 | sevid001.c | 451/450/86 |
| sevid002.c | 629/628/176 | sevid003.c | 718/717/195 |
| sevid004.c | 710/709/200 | sevid005.c | 690/689/163 |
| sevid006.c | 794/793/199 | sevid007.c | 992/991/214 |
| sevid008.c | 758/757/172 | sevid009.c | 445/444/128 |
| sevid010.c | 865/864/211 | sevid011.c | 617/616/158 |
| sevid012.c | 695/694/164 | sevid013.c | 753/752/214 |
| sevid014.c | 897/896/206 | sevid015.c | 715/714/207 |
| sevid016.c | 651/650/178 | sevid017.c | 462/461/127 |
| sevid018.c | 481/480/139 | sevid019.c | 608/604/131 |
| sevid020.c | 620/616/126 | sevid021.c | 741/740/131 |
| sevid022.c | 774/773/152 | sevid023.c | 510/509/109 |
| sevid024.c | 498/497/125 | sevid025.c | 733/732/160 |
| sevid026.c | 623/622/133 | sevid027.c | 565/564/126 |
| sevid028.c | 717/716/138 | sevid029.c | 582/581/134 |
| sevid030.c | 387/386/87 | sinit011.c | 441/440/21 |
| sinit012.c | 705/704/33 | sinit013.c | 722/721/54 |

## 其余 63 文件：内容原因与处置

| 路径 | A/D/H | 全部内容差异原因；处置 |
| --- | --- | --- |
| base/bios/reset.c | 14/6/6 | config 指针 ULONG_PTR；独立磁盘/设备拓扑；独立路径不安装旧 INT33 软件驱动，物理鼠标初始化保留。R |
| base/bios/tape_io.c | 4/1/1 | INT15 C0 配置表 ES 使用表所在段，不使用调用者 CS。R |
| base/ccpu386/c_bsic.h | 2/0/1 | c_getEFLAGS 声明。R |
| base/ccpu386/c_intr.c | 4/0/1 | T60 S1 拒绝 PIC -1，不把无效向量送 CPU。R |
| base/ccpu386/c_main.c | 115/16/25 | 独立模拟栈选择、声明；interrupt-map 接口；指令地址差值宽度；debug begin/retired；HLT 唤醒与时钟；执行节流/退出；clearHwInt；lifecycle enter/leave。R；空白 F |
| base/ccpu386/c_page.c | 22/6/12 | debug 成功内存读写观察点及声明。R |
| base/ccpu386/c_reg.c | 1/0/1 | stdio 声明。R |
| base/ccpu386/c_xcptn.c | 1/1/2 | yoda 声明；去除与 host_getenv 宏冲突的旧声明。R |
| base/ccpu386/ccpusas4.c | 4/0/1 | emm/host 及 host_sas_init/term 声明。R |
| base/ccpu386/cpu4gen.h | 14/13/14 | GDP 头；段寄存器 setter 和 flag getter 类型匹配真实 CCPU 定义。R |
| base/ccpu386/evid_c.h | 10/6/2 | 生成 callback 字段使用原生字宽 IUH。R |
| base/ccpu386/fpu.c | 12/6/5 | FPU_I64 参数指针；T60 S7 FIST64 符号/舍入及 memcpy 表示读取。R |
| base/ccpu386/ntstubs.c | 4/11/2 | 正确 cvidc include、stdio、GDP extern；移除会遮蔽真实 copyROM/NPX 实现的空 stub。R；遗留空白 F |
| base/ccpu386/ntthread.c | 41/3/3 | 多层模拟退出到最外层；类型化 null 返回及宽度格式。R |
| base/ccpu386/popf.c | 3/0/2 | stdio 与 note_486_instruction 声明。R |
| base/ccpu386/vglob.c | 2/1/1 | 选择真实 Evid_c 和 GDP 声明。R |
| base/ccpu386/zfrsrvd.c | 6/2/3 | 地址/中断头、FLDENV 声明及函数签名。R；D6 不变 |
| base/comms/com.c | 4/3/4 | ioctl 指针 intptr_t。R；最后空行 S2 |
| base/cvidc/ev_glue.c | 12/9/7 | stdio/inttypes/GDP；指针格式；真实 BIOS 写签名；GDP 创建入口。R |
| base/cvidc/evidfunc.h | 19/17/1 | mark callback 显式 IUH 四参数；删除重复声明。R；S2核对重复声明恢复是否可减少原始差异 |
| base/cvidc/evidgen.h | 18/8/5 | 函数指针调用契约代替 IHP；typecast 宏。R |
| base/cvidc/j_c_lang.c | 8/2/2 | stdio/inttypes 与原生宽度格式。R |
| base/cvidc/j_c_lang.h | 7/0/1 | CrulesRuntimeError 显式声明。R |
| base/cvidc/sascdef.c | 4/1/1 | SAS vector 空槽与 c_IOVirtualised 实际结构对齐。R |
| base/cvidc/vglfunc.c | 5/0/1 | IFN1 在仅声明处生成有效函数原型。R |
| base/inc/ckmalloc.h | 4/0/1 | stdlib 防止 malloc 隐式 int 截断。R |
| base/inc/cpu_vid.h | 15/2/2 | READ_POINTERS 选中 ABI；T60 S5 writer 声明。R |
| base/inc/egacpu.h | 17/2/3 | C_VID 字段类型及选中头，保留原有非 C_VID 分支。R |
| base/inc/egaports.h | 2/0/1 | T60 pel panning 查询声明。R |
| base/inc/emm.h | 6/3/1 | 宿主存储句柄 long→IHP。R |
| base/inc/gmi.h | 14/0/2 | C_VID 内存 handler 类型。R |
| base/inc/host.h | 4/1/1 | host_memset 参数与真实实现一致。R |
| base/inc/host_com.h | 3/2/2 | intptr_t ioctl 参数及对应注释。R；注释前空格 F |
| base/inc/ica.h | 2/2/2 | CPU_40 已提供中断枚举，避免重定义。R |
| base/inc/ios.h | 1/1/1 | 独立机器完整 64K I/O 地址空间。R |
| base/inc/sas.h | 5/3/2 | overwrite 声明及 Length_of_M_area PHY_ADDR。R |
| base/inc/timeval.h | 4/1/1 | 与选中 host_time 的 long 参数契约一致。R |
| base/inc/video.h | 10/0/1 | 非 NTVDM stream state 声明。R |
| base/keymouse/keyba.c | 4/4/2 | 仅缩进与行尾，整文件恢复 S2 |
| base/keymouse/keybd_io.c | 17/7/3 | 地址转为 Start_of_M_area 内宿主指针，SAS 操作仍用物理地址。R；MONITOR phy_base 纯缩进 S2 |
| base/keymouse/mouse.c | 34/1/5 | InPort 检测传输/复位支持 Win31。R；多余空行和 toggle 缩进 S2 |
| base/keymouse/mouse_io.c | 13/9/4 | host_memset 参数；guest mask 地址不用宿主 pointer 运算。R |
| base/support/ios.c | 0/50/5 | 原 NTVDM getIO/连接/断开/扩展 adapter 条件块被删；S3 验证后恢复 |
| base/system/cmos.c | 4/6/3 | config 指针与实际硬盘设备类型查询，不硬编码 C3/D4。R |
| base/system/ica.c | 25/2/5 | 独立 PIC INTR 撤销及 CCPU IRET bookkeeping。R；单调用花括号、空行 S2；stdlib 需核对用途 |
| base/system/idetect.c | 5/1/1 | vdm.h 经选中 include 路径。R |
| base/system/rom.c | 11/6/6 | config 指针、原生字符串长度。R |
| base/system/timer.c | 5/0/1 | 零延迟最小一单位，防止 quick-event 递归。R |
| base/video/gfx_updt.c | 189/145/31 | T60 S3 dirty 原始地址、split/wrap；S4 pan 前置依赖。R，不撤回已测矩阵 |
| base/video/vga_prts.c | 14/0/3 | T60 pan 查询及 split cancellation。R |
| host/inc/cfpu_def.h | 58/5/3 | 小端位域/字序；独立 fenv 宏接口。R；删掉的旧注释/空行 S2核对恢复 |
| host/inc/host_cpu.h | 33/32/2 | 小端 byte 顺序、CCPU 写函数精确类型 R；非 CCPU 压缩声明恢复 S3 |
| host/inc/host_def.h | 20/2/3 | stdlib/host_simulate；独立 host 选择、CPU_40 选择。R |
| host/inc/insignia.h | 12/1/2 | IHPE 指针宽、仅规则 IUH 原生宽、STRINGIFY 重定义保护。R |
| host/inc/nt_event.h | 4/1/1 | 隐式 extern 明确 BOOL。R |
| host/inc/nt_graph.h | 9/2/4 | 独立单执行器不取 NT mutex；pan 接口；stream BOOL。R |
| host/src/nt_cga.c | 19/2/4 | 标准 painter 选择、DIB出口、空矩形保护、非法尾逗号。R；缩进 F |
| host/src/nt_ega.c | 43/15/7 | DIB 出口；T60 bounds 和 pan。R |
| host/src/nt_graph.c | 67/12/11 | DIB 头、DAC 位宽声明、static 前置声明、独立 surface 初始化、V7 几何及调色板。R；文件头空行/中间空行、禁用旧 DIB 分支及尾部匹配 endif S2；VRAM 注释是否必要一并核对 |
| host/src/nt_keycd.c | 449/0/2 | standalone 重复映射表、KeyMsgToKeyCode、scan1 入口及分支闭合；S4，不先判定全部必要 |
| host/src/nt_munge.c | 29/1/5 | DIB/控制器头、T60 四平面 pan helper、两路径使用。R |
| host/src/nt_sound.c | 222/0/2 | standalone 声音状态机/clock/host tone、gate 入口及分支闭合；S5，不先判定全部必要 |
| host/src/nt_vga.c | 45/16/8 | DIB 出口、T60 四 painter 边界/单位/源界限。R |

## 收敛检查与限制

97 行文件单元 = 34 模式证明 + 63 逐文件内容审查；401 同字节文件无需功能归因。
每个原始 hunk 的格式差异由 F 覆盖，内容由以上同路径记录覆盖；不能把辅助区块数
误当作原始区块数。S2–S5 的待验证处置不是 S1 已实施修复。

18 个此前仅尾部空行文件由 T60 `3df951c` 恢复，已进入 401 同字节集合。
生产、测试、保护路径在 S1 不变；保留 T60 已手测 EXE，不宣称新构建：

- x86 SHA256 `4795C30678FCB092E728179D828B9A5A3B737A16E04AB160D19153A3C28874C9`
- x64 SHA256 `02A828C3CD9D99EDE9A1186C9B1B9F1A0B32AB1A36C8B9DD9B9BE8828437CB14`

S6 必须以同工具/口径复算；恢复文件由“差异”转为“同字节”时仍保留其处置记录。
S4/S5 若评估保留，须记录为什么比外移/复用更简单，以及行为验证；不可仅写“必要”。

S1 执行验证：工具复算两遍总数一致；完整规则模式 34/34；通过 T60 3df951c 的
18 个 mirror 变更路径逐一哈希确认全部与原始相等；双 EXE 哈希如上；
文档治理和 `git diff --check` 通过。源代码与测试零改动，未运行新的产品测试。
