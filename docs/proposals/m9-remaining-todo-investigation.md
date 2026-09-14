# M9 T59: remaining TODO investigation and product boundary cleanup

## 后续架构整理准入：S2–S5

原始请求：“准入为当前T任务的多个s任务开始执行，记得先更新t任务的proposal”。
所有者批准沿用 T59，按此前四项方案顺序实施；不重复分配 S1。
S1 的已验证交付及尚未收到人工验收的事实保留，后续工作不宣称其手测通过。

目标是不改变产品体验、控制流和设备语义，清楚表达现有六组件职责。
App 仅 composition 组装 VM/Common；Common 通过注入的 driver 调用 VM；
VM 负责适配，Compat 提供原始宿主边界，MVDM 仍拥有机器状态。
设备 I/O 保持 MVDM → Compat → Lib，不经过 Common 二次转发。
本轮不修改 src/lib、src/common 或 src/mvdm，不支持多 VM，不新增转发层。

冻结收敛台账（每项须有实际 diff、同类扫描及测试证据）：

| S | 范围与方案 | 退出标准 |
| --- | --- | --- |
| S2 | 根 CMake：所有 VM 源统一归属 VM target；保留原机器 OBJECT 组合和链接需要；exe 仅 App/资源，测试链接 VM target。 | 无重复编译路径；双宽度完整构建、全量测试、固定 EXE；实际 target/source 审查。 |
| S3 | VM/Compat 的原始 ABI 声明：优先复用准确已有声明；缺失声明按所属边界集中内部头文件；删除重复声明，不增加包装函数。 | 声明与定义核对；debug、输入、启动/reset 及双宽度全量回归；所有手写 extern 命中有处置。 |
| S4 | VM 创建/销毁的单实例契约：检查现有保护；缺失时统一入口最小准入，重复创建在改动全局资源前拒绝，失败/销毁释放资格。 | 重复创建、失败后重试、销毁后重建；已有生命周期不变；双宽度全量测试和 EXE。 |
| S5 | 文档和静态门禁：App→VM 仅 composition；Compat 不反向引用 App/VM/Common；共享层不依赖产品；VM 私有头不被产品组件外引用。 | 非法依赖样本被拒绝；消除本轮旧路径；双宽度全量回归、构建、干净提交推送。 |

每个 S 一个活跃 packet；完整实现、验证及证据作为 P 提交推送后，切换协调者角色审查实际改动再收口。
若原 ABI/静态链接存在必要回调耦合，明确记录并保留，不以目录整齐为由改产品行为。
每个 S 输出两份固定 EXE；保护用户 INI/媒体。诊断仅使用当前 S 的 build 子目录，
单次沿用测试超时，清理自有临时产物，不创建跨项目依赖。
超出上述范围或需要修改受保护三棵 src 子树时暂停并请求决定。

### S2 交付证据

`softpc-vm` OBJECT target 唯一编译四个 VM C 文件；exe 只含 App/资源。
所有原 machine 与 runtime 测试改为链接该 target；原 CCPU/设备 OBJECT 组合保持不变。
扫描根 CMake 的 src/vm 命中只剩唯一四项源清单，无测试复制；检查实际生成链接命令。
双宽度 tests 构建通过，ctest test-x64/test-x86 均 86/86（96.93s/81.20s），
包括完整 debug、生命周期、package 和边界测试。生产 C/H 0 改动；根 CMake +41/-41，
共享三棵源树及用户 INI/介质未改；两份固定 EXE 已重建。
原 VGA 测试的 TEXT 宏重定义警告保留，此次不修改受保护源或测试语义。

### S3 声明整理与全量命中处置

扫描 `rg -n 'extern ' src/vm src/compat -g '*.c'`，包括函数内部声明。
冻结单位为命中文件/职责组；不是清空所有 extern 或扩大原始头文件暴露面。

| 命中组 | 处置 |
| --- | --- |
| VM debug、Compat machine 的 CCPU 寄存器/线程声明 | 集中 compat/ccpu/abi.h；与 cpu4gen.h、c_reg.c、ntthread.c 类型核对，不改变函数实现或 ABI。该窄声明避免把原始整套 CPU 宏带入 VM。 |
| lifecycle 与 keymouse 的 interrupt-map 声明 | 归入已有 ccpu/lifecycle.h，两处共用；只暴露原有地址 accessor，不新增状态。 |
| Compat machine 的宿主、媒体、视频/调试桥接；VM debug 的 executor wake | 集中 compat/platform.h，所有本地实现文件也包含它以校验定义；原 DIB/text surface 复用 dib_surface.h，GDP 销毁复用 gdp_state.h。 |
| VM input 的 KeyMsgToKeyCode | 复用原 nt_uis.h，删除手写声明。 |
| Compat keyboard 的初始化、video/v7 的 planes/palette/refresh；facade 的 Video | 复用已存在的 keyba/egaports/gfx_upd/egagraph/nt_graph/evidgen 声明，删除重复。 |
| platform 的 c_cpu_simulate、floppy config、executor event、sound gate | 复用 c_main.h、platform.h、audio.h；删除本地重复/前置声明。 |
| machine 其余原始 bootstrap、计时/设备全局；device_bop BIOS 入口注册 | 保留调用点窄 ABI，分别服务原机器初始化和有限 BOP 表；不引入宏污染较广的全套原始头，也不改原初始化顺序。 |
| debug 的 inb/outb；memory 的 xtrn2phy；keyboard 的 host_key_down/up/output_contents | 保留唯一该宿主消费者的原始端口/地址翻译/键盘协议声明，不增无消费者公共接口。 |
| cvidc Gdp；facade C_Video；platform soft_reset、nt_video_funcs、keyboard/error 表 | 保留原始全局绑定所需声明，不复制对象。error.h 在当前非 BUILDING_CPU_TOOL 配置没有 working_error_funcs 声明，不能仅凭文本搜索删除。 |

尝试把寄存器声明带入原 keyba 的旧式无原型编译域，会触发短整数默认提升的声明冲突；
已撤销该扩大引用，使用原有 lifecycle 头的唯一 accessor。没有因此改变参数或返回值。
中间一次 x64 CTest 进程在共享 DAG 检查期间无诊断退出 -1，不作为通过证据；
最终构建后重新运行完整 suite，禁止放宽断言或修改共享代码。

最终 S3 双宽度完整构建通过；x64 86/86（103.41s），x86 86/86（89.89s）。
生产 C/H 21 文件 +122/-107，净 +15（包含两个新声明头）；没有新增函数、状态或测试路径。
三棵受保护源码无 diff，EXE 已刷新。遗留声明按上表保留，不宣称原始全部头文件现代化。

### S4 单实例准入

`vm_create` 在任何机器/音频资源获取前通过一个原子 flag 取得资格；已有 owner 时返回
INVALID_STATE，输出为空。创建失败统一释放资格，销毁先完成资源清理再释放资格。
不增加机器状态机或等待锁，不改 Common。内部 vm_driver_create/destroy 仍是非 owning
测试适配器；全量搜索只有 public vm_create 和串行原始机器测试使用它，App 不可调用。
复用 command-provider 的可丢弃镜像验证失败后重试、重复创建、空销毁不释放别人的资格、
两轮销毁重建及 reset。生产 3 文件 +23/-8（净 +15）；测试 1 文件 +25/-0。
最终完整构建通过；x64 86/86（88.45s），x86 86/86（75.30s）；固定 EXE 刷新。

## 原始请求与准入

“overlay 页查询 O(n) 这个加入一个队列proposal，td提交；然后所有其他todo，请你准入一个s任务予以处理”。

T58 已关闭；本任务使用下一全局编号 T59。S1 实现、双宽度全量回归及交付审查完成，等待所有者测试；S/T 尚未收口。
最新所有者授权：“验证T59的潜在问题 有确切证据的可以修复 没有证据的可以关闭不再关注”。
因此完成有界复测仍无证据的条目可关闭观察、不再转回 TODO；不称为已修复。
基线为 `bea5f8b`。Overlay 独立排队，不属于本任务；Win3.1 模式往返显示异常也保持独立候选。

## S1 有界台账

冻结范围是下列七条原始债务，每一条必须有独立结论，不能用合并调查隐藏遗漏。

| ID | 对象 | 初始状态及处理路径 |
| --- | --- | --- |
| A | make Error -1 / types-layout 无诊断退出 | 未归因；核对构建/测试进程与日志，重放原并发条件，不猜测为产品故障。 |
| B | package stage 17 debugger help | 未归因；核对输入、换行、视口与断言观察方式。 |
| C | compact Console stage 5 启动 | 未归因；串行基线后重放双宽度并发条件。 |
| D | 横向视口恢复 | 精确重建 30×30 buffer、20×10 viewport，验证 raw/cooked 往返几何。 |
| E | x87 FILD/FISTP qword | 独立有限值加载/存储判定，对照原始实现和双宽度；不把“写入一致”当算术正确。 |
| F | package stage 7 CAP 返回 monitor | 未归因；检查输入提交、状态完成、交接和提示符观察。 |
| G | package stage 14 ver 可见性 | 多次历史发生；优先保存输入/显示阶段证据，与 B/C/F 共用调查设施但不假定同因。 |

## 执行顺序与设计约束

1. 固定环境、EXE 哈希及原始断言；为七项建立可重复的最小证明。
2. 先调查 B/C/F/G 的公共测试路径，再处理 D、E、A 的独立验证。
3. 确认故障后仅修改责任层，扫描同因路径并补回归；不放宽原断言、不加重试掩盖错误。
4. 对未复现项报告尝试次数、环境、覆盖缺口；归档/延期须明确给所有者批准，不能标为已修复。
5. 修复后双宽度构建到固定 EXE、全量测试、逐项审计、提交推送；S/T 不自动收口。

原生 Console 状态仍由 Lib broker 管理，Common 管生命周期，测试观察不得改变产品语义。
保护用户配置和镜像。所有者随后明确“批准修改”：允许 cfpu_def.h 的小端宿主布局窄修，
包括 FP32/FP64/FP80 和按实际使用契约验证的整数高低字布局；保留大端定义，
不改 CPU 算法、舍入策略或 Lib/Common。补布局及真实指令双宽度回归。

## 验证预算与退出条件

首次包测试按宽度串行各十轮，之后有界重放历史并发条件；每个子进程沿用现有超时。
诊断只写本任务拥有的 build/t59-s1 子目录，每项运行最多 90 秒，文本日志总量不超过 20 MiB；
达到预算停止该轮采集、汇报证据，不无限打印或以等待代替归因。退出时清理自有进程和无用诊断产物。
新浮点探针使用内存或可丢弃输入，不写用户介质。

允许最终处置：已证实修复及回归；证据证明为测试问题并修复；所有者批准的未复现归档/延期。
七项均有处置才满足 S1 收口条件。最终处置见下表；检查点保留调查过程，不覆盖最终结果。

准入前只读复测：x86/x64 的 package 普通/compact、host_console_display、
types-layout 与其 selftest 各三轮通过；不覆盖 D 的精确横向视口或 E 算术判定，
也未重放 A 的原并发构建。这是基线观察，不是七项退出证据。

## S1 验证检查点（尚未交付）

- G：x64 普通 package 首轮 stage 14 失败；另一次重复前三轮通过、第四轮
  同样失败，最终画面只有 DOS 提示符、没有 ver 输出。确切复现，仍需归因。
- C：x64 compact 连续十轮通过；B/F 未在这些已执行路径中复现。
- D：新增 30×30 buffer/20×10 viewport 回归，x64 一轮、x86 三轮均通过。
  尚未修改 Lib 行为。
- E：真实 CPU 临时断言双宽度均把 0000123400001234 写为 8000000000000000。
  独立 cfpu_def.h 布局探针中 double 1.0 的 exponent 双宽度均为 0（应为 1023），
  hiword offset=0。已向所有者请求窄布局修正授权；未修改 MVDM 或算术算法。
- A：当前双宽度 -j8 目标重编译未出现无诊断退出；types-layout 与 selftest
  双宽度各三轮通过。未证实产品故障，按最新授权关闭观察，不修改测试来隐藏错误。
- B/C/F：x86 普通/compact 各十轮通过；x64 compact 十轮、带失败后诊断的
  普通测试十轮通过，未触发这三种症状。按最新授权关闭这些独立观察，不宣称修复。
  G 的复现仍独立处理，后续诊断轮次不拿来掩盖原失败。
- E 临时断言红灯是故障证据，不是可发布的完成版本。
- G 后续诊断另一次捕获到 `8ver` 而非注入的 `ver`，原生输入缓冲已空，
  随后 CAP 可返回 monitor。证明该轮执行了意外命令，但不证明最初空白症状同因。
  测试创建可见 Console，raw 激活会前置它，因此自动化存在接收人工键盘的干扰入口。
  测试改为隐藏自有 Console，通过句柄观察，并断言启动及 raw 阶段仍隐藏；
  不改产品焦点规则，不增加输入重试，不放宽 Version 断言。
- 隐藏 Console 的 x64 普通/compact 各十轮通过；去除 VM 输入插桩、重新构建后，
  x86/x64 普通/compact 各三轮通过。MVDM/VM 源码与 HEAD 无差异。
  临时失败后 CAP/debug 采集已移除，保留原有失败报告。该证据支持测试隔离，
  不宣称已经解释所有历史 stage 14 超时；G 最终处置和完整交付仍未完成。

## S1 交付台账

| ID | 处置 | 证据及边界 |
| --- | --- | --- |
| A | 关闭观察，不再关注 | 双宽度 -j8 重编译、types-layout/selftest 三轮及最终完整构建/回归通过；没有确认的工具链或产品根因，不改实现。新增 CMake 目标后同一 make 多目标调用需重新进入生成后的 Makefile，不等同于历史 Error -1。 |
| B | 关闭观察，不宣称修复 | 普通/compact 有界重复及最终回归均通过原 help 断言。 |
| C | 关闭观察，不宣称修复 | 双宽度串行有界重复和最终回归通过；并发补验单独记录。 |
| D | 关闭观察，保留精确回归 | 30×30 buffer/20×10 viewport 用完整快照比较 raw/cooked 往返；双宽度通过，没有 Lib 行为修改。 |
| E | 已修复宿主布局 | 原失败整数恢复原值；真实头文件布局及实际 FNINIT/FLD/FSTP/FILD/FISTP/FBLD/FBSTP、四则运算双宽度通过。 |
| F | 关闭观察，不宣称修复 | 原 CAP/monitor 断言在有界重复和最终双宽度回归通过。 |
| G | 修复测试输入隔离；关闭未证实的剩余产品原因观察 | 意外 `8ver` 暴露可见自动化 Console 可接收外部输入；改为隐藏测试窗口并检查其隐藏状态。保留 Version、时限和所有原阶段断言；无重试、无产品补丁。不能将此归因为全部历史空白症状的唯一根因；按所有者有界验证授权归档剩余观察，不再推测产品故障。 |

### 修复与同类扫描

- 生产仅 `host/inc/cfpu_def.h`：小端 FP32、FP64HI、FP80SE 位域；FP64、FP80 字顺序；
  FPU_I64 对齐原 `fpu.c` 小端 BCD 表。保留大端定义、原算术及 Intel 内存转换。
  搜索原 host/inc 与 base 的 FP32/FP64HI/FP80SE/FP64/FP80/FPU_I64 定义，
  当前选中头文件是唯一该组布局定义；检查 fpu.c 的转换、BCD 常量和读写调用点。
  `softpc-x87-layout-smoke` 是永久布局检查，不引入第二套浮点实现。
- 复用 command-provider 的原 executor/debug rendezvous 执行 10 组正负值往返及
  4 组精确算术，不另造 executor。原 operand observation 断言保留并增加数值等式。
- 包测试只隔离测试进程的宿主输入；产品的 raw 激活、焦点和键盘归一化没有修改。
  临时 VM 输入插桩、失败后交互采集和独立布局探针均已清理。
- Lib/Common/VM/Compat 源码和用户 INI/介质无修改。共享 test/lib 仅补几何回归及 manifest。

### 验证与产物

P1 `2c05238` 已提交推送。协调者在推送后复核实际提交的全部路径、头文件差异、
测试输入边界、七项处置及双宽度结果：没有超出批准范围的生产改动，没有绕过失败断言；
原 VM/Common/Lib/Compat 和 INI 保持不变。此次只完成交付审查，不自动关闭 S1/T59。

- `cmake --build --preset tests-x64 -j8`、`tests-x86 -j8` 完成，刷新两份固定 EXE。
- `ctest --preset test-x64 -j1 --output-on-failure`：86/86。
- `ctest --preset test-x86 -j1 --output-on-failure`：86/86。
- 两个 suite 均包括新布局测试、真实指令测试、包测试、完整 Lib/Common 测试、manifest、边界和文档门禁。
- 两宽度并发执行 `ctest --preset test-<width> -R '^softpc-package-' --repeat until-fail:3 -j1 --output-on-failure`，
  普通/compact 各三轮全部通过（共 12 次）；C/G 未复现。
- 对比交付前 HEAD 的 `git diff --numstat`：生产 1 文件 +35/-4（净 +31）；
  测试 C 源码 4 文件 +132/-7（净 +125）；根 CMake +7/-0。
  另有 test/lib manifest、文档和两份 EXE，不计为生产/测试源码行数。
- `softpc32.exe`：3,502,713 bytes，SHA256 `7DDA1F9C8631DBCB90AE7D39CFB94723625D7852A98F06A0FD6AE37B4CFCAC66`。
- `softpc64.exe`：2,849,636 bytes，SHA256 `792943A6DF298DFA199E5278ED26AE995B2C86D4F2EBAEF408FC97CA1C407778`。

## 原始债务全文

- TODO(Low): local toolchain owner — T58 S3 observed make subprocess Error -1 and one diagnostics-free types-layout self-test exit; unchanged focused types-layout rerun passed. No compiler diagnostic or source change explains these transient exits. Investigate runner/toolchain process termination if reproducible; do not alter shared tests or product behavior to hide it.

- TODO(Medium): package acceptance owner — T58 S3 first full x64 suite failed startup debugger help at package stage 17; captured viewport was 30 columns and text wrapped. The unchanged focused package rerun passed. Root cause and relation to Console geometry are unproven; admit a bounded package/viewport investigation if this recurs. Do not weaken the help assertion or change shared Console behavior in an App relocation task.

- TODO(Medium): package acceptance owner — T58 S1 first full x64 run timed out in compact Console at stage 5 while x86 tests ran concurrently; the subsequent serial x64 full suite passed 85/85. Product src and both EXE hashes are unchanged from T57. Concurrency is context, not a proven cause; admit a bounded baseline-package investigation if this stage recurs, without weakening assertions or modifying runtime in the test-corpus task.

- TODO(Medium): Lib Host display owner — T56 S16 corrective probe with an artificial 30x30 buffer and 20x10 viewport observed cooked restoration as 30x10; restoring capacity afterward changed the viewport width instead. Normal full-width narrow/short/scrolled viewports pass. This is distinct from the fixed first-start frame-capacity ordering; admit a host-specific horizontal-scrollback preservation investigation if required, rather than add iterative resize compensation to this startup repair.

- TODO(Medium): SoftPC CPU owner — T56 S12 disposable FNINIT/FILD qword/FISTP qword probe observed an indefinite integer write after a finite read. The debug observation matches actual stored bytes; arithmetic correctness and pre-hook baseline equivalence were not this debug-ABI task's scope. Admit a separate original-x87 baseline comparison if requested; preserve media and do not alter CPU arithmetic in a debugger adapter.

- TODO(Medium): package acceptance owner — T55 S7 observed one x64 stage-7 CAP-to-monitor timeout after DOS; direct rerun, a full rerun and five consecutive package runs passed. Root cause is not established. Admit a baseline-comparison investigation if it recurs or the owner requests it; preserve the current assertion and do not label the symptom fixed.

- TODO(Medium): package acceptance owner — T56 S16 observed two x64 stage-14 DOS ver visibility timeouts, including a serial run. Full serial regression passed, and identical S15/current-EXE comparison passed three times each. S18 first full x64 run reproduced the compact-console stage-14 symptom; the subsequent full unchanged suite passed 83/83, as did x86. No input/focus change or weakened assertion was made; root cause remains unproven. This recurrence remains a separate bounded startup-input timing investigation, not a wake-allocation repair claim.
