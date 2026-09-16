# MVDM / OpenNT 当前差异账本

当前结论见文末 **S6 最终复核**。S1 数字和待处理标记是冻结基线；
S2–S6 的处置记录覆盖其历史状态，不把已完成的候选继续算作欠账。

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

## S2 实施证据

已恢复 keyba.c 两处原始缩进；删除 nt_graph.c 文件头空行、禁用的 standalone
DIB 分支及匹配 #else/#endif。未改真正执行的 graphicsResize 内容。
两宽度使用当前 CMake flags.make 的 compiler/defines/includes/flags（含 keyba
的原有 -iquote）执行 `-E -P`，保留字符串/字符及 token 边界、去掉空白后 SHA256：
修改前后均一致。

| 宽度/文件 | 前后同值 SHA256 |
| --- | --- |
| x64 keyba.c | B33E33F6E9C6B92548E20C9ADFDF596E8F4376AD415B4AE0B2DC9D6788529B6B |
| x64 nt_graph.c | 57E8D36C20A767BBA77197A54D8B658A7E0058A8A174180227044E65BFBE1B09 |
| x86 keyba.c | C266EFD8324F8454D2AC1E738067729E3F5670049D56B145633271496153ABF8 |
| x86 nt_graph.c | 88D27DE4CCAEBCD2FA15B5EB62DF84DA6C2CC754FB5716641841296A0CCFF053 |

临时只读预处理探针位于 build/t61-s2，证据保留后删除。首次探针的 PowerShell
单字符串 splat 错误、x86 DLL PATH 缺失已修正后重跑；不作为产品构建失败。
keyba.c 已恢复原始行尾及第一行原有尾空格，SHA256 与 OpenNT 完全相同：
`D3326210BE154866924CA99F1E44E0A76E9A912E15975305804F6B59C4CF7953`。
又恢复 com.c 两处缩进/末尾空行、keybd_io.c MONITOR 分支缩进、mouse.c 空行及
原有 toggle 初始化缩进。两宽度预处理 token 指纹前后相等：

| 文件 | x64 | x86 |
| --- | --- | --- |
| keybd_io.c | B9C74FE2B584B3DA9EB5D306A16808696E1B5C98030DDF38FB8E16C23B0164F8 | 41F0835F74535FDD1706495F7065F0E703083CB1FC9B15AB90F1378EAC2B97F2 |
| mouse.c | 47FC8BD0EA97204FE7DE53B6CE6C41A3C3DAE6A0B3C8ADAD9724CD6BF9193272 | DAA2CFBB8348429C9C8128BC8240F858858574180865CE9CA6192D3D3F06BE98 |
| com.c | 59750A835947E2120B7AC1DFC3294E701933786A0714907216380EED11B4C135 | 34F78A89C895C8206DE0D0DCD23A8F21823A8BD3D047B0FADD29F52021CD13BF |

全镜像 C/H 扫描：原版没有混合换行文件。48 个当前文件按其各自原版换行格式
还原，使用单字节无损编码，逐文件断言去掉 CR 后内容完全不变。Git 的自动换行
规范会隐藏其中大多数工作区字节差异；此项不能冒称 48 个新的 tracked 内容修复，
也不能保证其他 checkout 配置仍有相同原始字节统计。S6 会同时给出辅助内容口径。
后续 S2 清理：恢复 cfpu_def.h 原有注释/空行、host_com.h 注释内缩进、ica.c
原有空行和单调用外括号；删去 ica.c 已由 host_def.h 提供的重复 stdlib include；
删去 nt_graph.c 不改变代码的新增 VRAM 说明，恢复原有调色板更新后空行。

ica.c 删除括号是唯一选中 token 变化；作用域内仅一条无局部声明的函数调用，
无宏展开多语句。两宽度 `-S -o -` 生成汇编前后相等：
x64 `249B3DA64C14E1C008ECDE23EFCA4A4E49644B245FB53E9D6E880E37D8701A0E`；
x86 `62851A486344075259F5BBDA1B77513B60269588AB392D7A9F15A4FF542ED726`。
删去重复 include 后预处理结果相对括号清理后不变；FPU、nt_graph、com 的
注释/格式修改预处理结果不变。全部 C/H 用 `--ignore-space-at-eol -U0` 扫描
纯空白区块，最后两处命中为 ica/nt_graph 已恢复的空行。

其余候选明确保留：ntstubs.c 空行是删除旧 stub 后保留的原始行，继续删会增加
原始删除 diff；evidfunc.h 四条重复声明若恢复仍必须改成新 ABI，每条都会新增
一行差异而不会减少原始删除量，因此不恢复；34 规则文件必要替换同行内的空白
不另做无意义全量改写；host_cpu.h 非 CCPU 声明及 ios.c 留 S3；键盘/声音留 S4/S5。
必要功能解释注释保留，不以删去理由来伪造简洁性。

S2 构建前复算：402 同字节，96 差异文件；原始口径 +23,770/-22,442、5,131 区块；
辅助内容口径 +23,749/-22,421、5,127 区块。辅助口径相对 S1 减少 43 增删行、
11 区块、1 文件。原始口径的大幅降低主要是 checkout 行尾恢复，不能混为功能精简。
实际 tracked 生产改动为 8 文件 +17/-34，净 -17；测试源码 0。保护目录零改动。
`cmake --build --preset tests-x64/tests-x86 -j 8` 双宽度通过；串行 CTest 各
97/97 通过（x64 92.25s，x86 71.41s），包括 PIC、键码、鼠标、VGA、x87、BOP
和固定包测试。保护目录、用户 INI/媒体零改动，外部只读参考工作区仍干净。
`git diff --check` 唯一发现是恢复 keyba.c 第一行原版尾空格：这是原始镜像
逐字节一致所需的例外；排除此文件后无发现。两份临时预处理脚本已删除。
文档门禁不允许提前创建当前 S 的收口历史，故实施证据先留本账本，实际提交
审查完成后才登记历史/下一 S。不能把仅实施验证称为协调者收口。

固定 EXE SHA256：
- x86 `4D065ECBAF5CBD6B14E5B07876502CEBC343CEBE93E318FC2CE6CC1F571DEBD8`
- x64 `4D72F82800CAD5326217AB5F960611621F2990194904D2737FCB69DE9536D424`

## S3 实施证据

仅恢复 ios.c 的 50 行原始 NTVDM 条件块，以及 host_cpu.h 非 CCPU 分支的
原始声明排版（+32/-10）。当前生产净 +72，不是新增实现；与原始内容差异
则减少 92 增删行、4 区块。ios.c 已无内容差异；未选中的 NTVDM/非 CCPU
分支不宣称运行验证。必要的 CCPU 原型及小端寄存器布局保持不变。

真实 CMake 命令使用 CCPU、不定义 NTVDM；ios.c 的 `-E -dM` 再次确认。
遍历当前 build.make 的真实编译规则及其 .obj.d（不是残留旧对象），两宽度
各找到 57 个 ios.c/host_cpu.h 受影响编译单元；按各自 flags.make、includes
响应文件及原有 custom options 运行 `-E -P`。字符串/字符保留，token 间以
LF 分隔后 SHA256；逐项前后 57/57 相等。输出记录按探针遍历顺序、无末尾 LF
的整体指纹（内容为 `width object-path SHA256`）：

- x64 `A7D46BD531EFB60A97C2129350425C15C86321CD09B4FD1D2141D998F64D07DC`
- x86 `583342F8652694DD84C5698B72BB50FE6309B323BA664EB5992D20C97FE3ACF1`

双宽度构建通过；新 EXE 与 S2 6df4104 的 `.text`、`.data`、`.rdata` 各段
SHA256 全部相同，不仅是测试未发现变化。x64 首次全测 96/97，package-smoke
stage=14（启动后的 ver 未出现 Version，最后屏幕已有 C:\\>）失败；随后同一
EXE 连续五次包测试通过。保留此观察，不把失败归为本次新增代码，也不宣称
已修复包测试时序。最终完整套件 x64 97/97（55.15s）、x86 97/97（69.59s）通过。
探针初次 custom option 被 PowerShell
展开为字符已改为显式数组后重跑；这是探针错误，不是源码失败。

同类删除条件指令全树扫描命中 reset.c、c_main.c、ica.h、ios.h、ios.c、
cfpu_def.h。除本次 ios.c 外，其余是仍生效的设备初始化/TLS 模拟栈/CPU 类型
冲突/64K I/O 范围/浮点环境边界，均保留，不能作为“未选中原始分支”恢复。
直接 host_cpu.h 引用只有 cpu4.h；实际消费者由编译依赖覆盖。保护共享目录不变。

最终辅助内容口径 +23,739/-22,339、5,123 区块，95 个文件有非行尾差异；
当前 raw 工作区口径 +23,857/-22,457、5,130 区块，402 同字节/96 不同字节。
ios.c 行尾仍受 apply_patch/checkout 影响，不将内容恢复冒称所有工作区字节恢复。
生产 2 文件 +82/-10，测试源码 0；无新路径。文档及 diff-check 通过。
临时探针在保留上述指纹后删除。固定包 SHA256：

- x86 `58DEB3A11B297A9A2288B73681478A65403432D79F51E952426C614B2394E32E`
- x64 `08D3F83AAE6D64BB1FF0BD73115874E6E40D4E03CE7FDE457284928B543700D2`

## S4 实施证据

键盘冻结全集为 nt_keycd.c 的两份 Scan1ToKeynum、Scan1ToKeynumExtended、
aNullCharScan、aNumPadSCode 以及 KeyMsgToKeyCode；四组表和转换函数逐项
确认与原始文件相同。删除独立分支的重复 corpus，直接使用原始定义；仅隔离
NT 头文件与未使用的 BiosKeyToInputRecord，并保留现有 scan1 窄入口。
原始 BIOS 转换代码没有删除，也没有启用 NT 服务。四组表现在各只有一个定义。

唯一生产调用为 vm/input.c 的 KeyMsgToKeyCode，以及 compat/keyboard.c 的
softpc_host_scan1_to_key；前者仅在 VM 边界复制扫描码/扩展标志，后者保持
0x7f 掩码和 make/break 分派。两者均未修改；不存在新的表、队列或转发层。
选中编译单元的真实 CMake 参数运行 -E -P，词法 token 指纹前后相同：

- x64 `DF06D4FE4669C2F15D4508E277532E0B58B706C2A16CC9667E60020089769122`
- x86 `ABD73385ECD02FDC5769F1EECA7B6836DAF386D3DA291DB92C21B51A0153A2DA`

原始内容差异由 +449/-0、2 区块降为 +13/-0、3 区块；减少 436 行差异，
区块增加是从大复制分支改为三个局部编译边界，不代表功能增加。raw checkout
仍有行尾造成的 +19/-6，最终 S6 单独对账。生产 1 文件 +11/-447，净 -436；
测试 1 文件 +34/-0：完整 133+84 表项的 FNV-1a 指纹、普通表窄入口一致性、
每项 8 种修饰组合与 make/break 等价。测试不依赖外部源码或重复映射表。

同类扫描发现原版 KeyMsgToKeyCode 的两个 `> sizeof` 判断允许等长索引越界，
这是原版和精简前均有的行为，未混入等价精简；已转 TODO，需独立输入安全准入。
未声称该边界安全或真实 RDP 手测通过。探针的 x86 初次因工具链 PATH 缺失失败，
补既有 mingw32 路径后重跑成功；指纹计算初版的 PowerShell 整数掩码错误已纠正，
没有更改生产算法。四组原始表的完整比较也独立通过。

双宽度构建通过，全量 x64 97/97（80.34s）、x86 97/97（69.35s）。
文档门禁及 diff-check 通过，共享四目录、INI、媒体未改动，外部树干净。
固定 EXE SHA256：
- x86 `29FBE2B2C13182D54520388239C4933431BEC8F2E2B3236A7733CDB567FB0133`
- x64 `CDCAF22C812B26122DB4EC3C24B4A6422F2284B22F8194848BA85AB95F914E14`

## S5 实施证据

有限对照全集为 nt_sound.c 的全部状态/函数、Compat 的 timer gate/heartbeat/
audio sink 以及原始 ppi/reset 调用者。按职责处置如下：

| 成员 | 唯一所有者、处置及证明 |
| --- | --- |
| FreqT2/PpiState/T2State/LastPpi/FreqPpi/ET2TicCount/PpiCounting | 共用原始全局定义；不迁移或复制设备/声音状态 |
| BeepLastFreq/BeepLastDuration，PulsePpi 两个 static 计数 | 共用原始缓存与局部状态；重复请求、脉冲计数/超时验证 |
| host_alarm/host_ring_bell | 原版正文，仍为 MessageBeep/config 入口；受控测试覆盖开关 |
| PlaySound/host_timer2_waveform/HostPpiState/PulsePpi | 原版正文逐函数文本比较相同；音高/PPI/平滑算法未重写 |
| InitSound/LazyBeep/PlayContinuousTone | 共用原始状态逻辑，仅条件隔离 NT beep handle/create/ioctl/close |
| GetPerfCounter | standalone 保留原有 GetTickCount()*10 换算，局部函数使用原始调用名；未引入新时钟 |
| host_enable_timer2_sound/host_disable_timer2_sound/timer2_gate | 现有三个接入口原样保留，只操作上述同一状态；ppi/reset/Compat gate 消费 |
| OpenBeepDevice/hBeepDevice/BeepCloseCount | 原始 NT 路径完整保留，standalone 不编译；不声称 NT 运行验证 |
| Compat audio worker 与平台 heartbeat | 完全未改；仍仅呈现声音/调用 PlayContinuousTone，不新增线程或 state owner |

受控 sound-state 测试先在精简前两宽度通过，再验证精简后：真实 nt_sound.c
包含进独立单元测试，只替换宿主 clock/bell/tone 和配置查询；没有复制算法或
依赖外部源码。覆盖重复 tone 抑制、短 click 清缓存、200ms 脉冲累积/1008Hz
平滑结果、连续声音超时、PPI/Timer2 gate、频率边界、InitSound 和配置铃声。
特意保持原始 equal-tick 分支会清计数的行为，不借精简更改原有时钟语义。
现有 sound-smoke 继续走原始端口/PIT/reset；audio lifecycle/failure 测试保留。

六个上述原版正文函数逐一比对相同。剩余条件分支只隔离 NT 资源，不删除原始
实现；没有第二 LazyBeep/PlaySound/PulsePpi。内容差异 +222/-0 降至 +64/-0，
减少 158 行；生产 nt_sound.c +62/-220，构建测试接线 CMake +9/-0，新增测试
源码单列统计。Lib/Common 四目录、App/VM/Compat、配置媒体均未改。
测试接线初版碰到无 include guard 的原始 host_def 重复包含，以及 K&R 原型
默认提升，已只修正测试接线；未为测试改动原始头文件。首次配置误用了 build
preset 名称，改用现有 configure preset 后成功，不归为产品失败。

新增测试 77 行，构建接线 9 行；生产净 -158，三者合计净 -72。双宽度构建
成功，完整 x64 98/98（82.24s）、x86 98/98（71.34s）；文档门禁和 diff-check
通过。未新增临时源码/媒体，原始外部参考工作区仍干净。
固定 EXE SHA256：
- x86 `86F32C392BE284A14DE5025D3BCD34BDB33E1BB0266C9C902432AA7AF4911624`
- x64 `0D5C7D57412DB0DC89FF7FAC25155480FB084A07E0402819B56AE683C41525EF`

## S6 最终复核

同一 498 文件全集、同一 OpenNT revision 和同一工具参数复算。恢复已修改文件
自身的原版 LF/CRLF 风格后，raw 与辅助内容口径一致：403 文件逐字节相同、
95 文件不同、无来源文件 0、未选入原始文件 650。没有通过删文件降低差异。

| 口径 | +行 | -行 | 增删合计 | 净增 | 三行上下文区块 |
| --- | ---: | ---: | ---: | ---: | ---: |
| S1 raw（历史 checkout 行尾含噪声） | 46,241 | 44,893 | 91,134 | 1,348 | 5,112 |
| S1 辅助内容 | 23,779 | 22,434 | 46,213 | 1,345 | 5,138 |
| S6 raw 与辅助内容 | 23,141 | 22,335 | 45,476 | 806 | 5,132 |

以可比的辅助内容口径计算，减少 **737 行增删、6 个区块、2 个差异文件**。
不把消除 checkout 换行噪声算作数万行功能精简。区块合并/拆分会改变数量，
不代表功能数。剩余 34 个规则文件仍为 +22,025/-21,985、4,891 区块，
完整逆模式比较 34/34 通过、例外 0；T61 未修改这些文件。

### 原始请求、剩余差异和最终处置

S1 表的 97 个文件单元仍是完整归因全集。keyba.c、ios.c 已转为同字节；
以下 12 文件采用更新的 A/D/H，其余 83 个剩余文件沿用 S1 原因/数值：

| 路径 | 最终 A/D/H |
| --- | --- |
| base/comms/com.c | 3/3/3 |
| base/inc/host_com.h | 3/2/2 |
| base/keymouse/keybd_io.c | 16/6/3 |
| base/keymouse/mouse.c | 32/0/4 |
| base/system/ica.c | 19/0/2 |
| host/inc/cfpu_def.h | 58/1/3 |
| host/inc/host_cpu.h | 23/0/3 |
| host/src/nt_cga.c | 18/1/4 |
| host/src/nt_ega.c | 40/12/7 |
| host/src/nt_graph.c | 51/11/7 |
| host/src/nt_keycd.c | 13/0/3 |
| host/src/nt_sound.c | 64/0/10 |

所有历史 F/S2、S3、S4/S5 候选已作出明确处置：

- 18 个末尾空行文件在 T60 已恢复；S1/S6 哈希核验，不再次删除原版空白。
- S2 恢复 keyba、com、host_com、keybd_io、mouse、ica、cfpu_def 原始格式/注释；
  删除 nt_graph 禁用 standalone DIB 分支。保留正在执行的原始 DIB 路径和必要说明。
- ntstubs 的空行原版就存在；不删。evidfunc 重复旧声明的 ABI 不正确，恢复它们
  会破坏契约；重复加入修正后的声明也不能消除原声明删除，保留单份正确声明。
- S3 完整恢复 ios 的未选中 NTVDM 分支、host_cpu 非 CCPU 原版声明，保留必要
  CCPU 类型。选中预处理与双宽度代码/数据等价；未声称 NTVDM 运行验收。
- S4 复用原始键盘表/算法，仅保留 NT include 隔离和现有窄入口，原始 +449 降至 +13。
- S5 复用原始声音状态机，仅隔离 NT beep 资源、保留已有宿主出口，原始 +222 降至 +64。
- S6 对全部非规则差异文件按零上下文逐 hunk 比较恢复候选，补回 nt_cga 一行、
  nt_ega 三行原始缩进；不改非法尾逗号修复及 T60 painter 边界。实际 Git delta
  恰好四行前导空白替换，去掉前导空白后删除/增加行逐字相同；没有改 token。
- 必要编译/指针/C-VID ABI、CPU/debug/HLT、BIOS、鼠标、PIC、timer、FPU 和绘制
  差异按 S1 的 R 原因保留。未因缩减统计撤回 D6 或改用新模拟器。
- 原始 keycode 等长索引缺陷仍在 TODO，属于需另准入的行为修复，不混入等价精简；
  既有三个 Queue 候选未改变顺序，Win31 display roundtrip 不宣称本任务修复。

### 代码与所有权审计

按 `git diff --numstat 893db1e -- src test CMakeLists.txt tools` 计数：
生产镜像 14 文件 +176/-715，净 -539；测试 2 文件 +111/-0；构建接线 +9/-0；
三者合计净 -419。只读审计工具另计 +89，不参与构建、不生成源码。
S6 自身生产 +4/-4，净 0。各阶段原始 diff 变化见 S2–S5 的前后证据；
恢复 ios 增加当前源码 50 行，却消除原始 50 行删除，符合本任务而非压 LOC。

App/VM/Compat、Lib/Common 四目录、INI 和媒体相对 893db1e 无修改；
没有外移复制实现、第二状态 owner 或新适配层。外部 OpenNT 工作区干净。
全部保留路径及 650 个未选入路径范围未变。构建仍直接使用 src，未增加 overlay。

最终验证和固定 EXE 指纹见下方交付记录；手动 GUI/声音/RDP 验收仍由用户进行，
自动测试不替代这些人工体验结论。

### S6 交付验证

双宽度 tests-x64/tests-x86 构建通过；串行完整 test-x64 **98/98，82.54s**、
test-x86 **98/98，72.11s**。包含原有 IRQ、BOP、VGA、x87、重启、输入、
声音及包测试；文档治理和当前 diff-check 通过。零上下文 hunk 内唯一对应
删增行的纯空白候选全树扫描（规则文件另受完整模式证明）最终为 0；这不是
声称任意理论重构再也不能减少差异，而是已准入候选和同类格式问题全部处置。
本阶段无诊断临时目录遗留，未清理其他任务的 build 目录。

固定 EXE SHA256：
- x86 `33F5E099C169A9509F7A6D3C938A500BCFA17ECDE7220815917F380EFCA23D81`
- x64 `D05DFAC42A81D687CC3E979BF6C47222F3A65FF6C6F59FCC7D7D0C58FF91F862`
