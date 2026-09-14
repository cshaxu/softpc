# 候选任务：从 SoftPC 抽取 canonical common

## 原始要求与基线

> 好的，你帮我做一下任务规划，目标是建立一个 common 组件，调用 lib，实现nxvm 和softpc虚拟机都需要的：debug，xasm32，session总控，ui 接管，machine机器接口层；原有cli作为session的inject；原有配置依然在app入口启动配置好；app负责组建所有所需线程实体；写入proposal，并且切分好任务，保证每个S任务有明确的边界和可供我验收测试的exe成果，每次移出的组件，原softpc内app/host必须清理干净不留第二实现；softpc当前用户体验和总体架构不变；nxvm的common作为组件划分参考。Td治理，提交推送后工作区必须干净。

SoftPC T55 已验收实现是行为基线：生产提交 `987d82e`，双宽度完整 CTest
各 58/58，严格 lib CTest 8/8。这些是既有证据，不是本次 Td 重新构建结果。
实施 S1 冻结准入时完整提交和产物哈希。行为以
[Product UI](../design/UI.md) 和现有回归测试为准。

本任务从现有代码搬迁、抽取和注入，保留唯一控制线程、唯一机器 executor、
已有队列分工与完成顺序。NXVM common 只作组件划分、接口需求及 debug/xasm32
来源参考；不以其当前 control/runtime/UI 替换 SoftPC 已验收实现。
最终 SoftPC common 成为 canonical corpus，供 NXVM 原样采用。
本提案在队首，以下 S 是实施顺序计划；本次 Td 不准入实施、不分配新 T。

后续批准的边界修订：common 之外仍允许直接调用 lib，例如 app 通过
storage 读取 INI。约束的是职责和资源所有权，不是将 common 作为所有
lib 调用的强制中转层；以下任务与验收按此修订执行。

## 五个组件及组装边界

| 所有者 | 负责 | 契约及禁止事项 |
| --- | --- | --- |
| app | 读取、解析、校验原有配置；选择 SoftPC driver、CLI、热键/标题/状态文案策略；组装启动全部所需线程实体，按序停止回收 | 可直接使用 lib 公共接口完成自身职责；通过 common 接口管理其拥有的实体，注入配置与 callbacks；不复制 reducer、broker 或 executor loop |
| common/session | 唯一总控事件队列、状态归约、命令准入和完成事件处理；推导机器请求与展示动作 | 注入原 CLI provider 和产品策略，调用 machine/UI 接口；不读取 INI，不解释 CPU/device，不绘制 |
| common/ui | 唯一拥有 monitor cooked Console、raw Console、Window 及 broker；执行创建/销毁、raw/cooked 切换、frame/title/freeze 和 monitor 输出/arm | 向 session sink 投递复制的输入/完成/失败事件；不决定产品生命周期、不解析 CLI |
| common/machine | 通用机器请求队列、executor 生命周期、运行代际、完整帧及完成事件发布；其运行所需中性资源 | 注入 SoftPC driver；CPU、设备、冷启动/复位具体步骤及原始宿主回调归产品 adapter；reset 只报告一次最终完成事实 |
| common/xasm32 | 原版汇编/反汇编能力 | copied bytes/text 接口，无线程，不接管 CLI |
| common/debug | 原版 debug command engine、调试会话及 machine 调试契约消费者 | 显式绑定 machine/debug driver，可作 session CLI provider；不自行建 executor、接管 Console 或绕过安全点 |

不增加 common/console 或 common/kvm。common/ui 是广义交互，
lib/kvm-* 仍只负责 keyboard/video/mouse。原 CLI 的命令、帮助、提示符、
拒绝文案及状态通知保持原样；原 command 实现作为 session 注入项，
不另写一份 common 版 SoftPC CLI。

允许的生产调用关系（箭头表示调用/依赖）：

```text
app -> common/session, common/ui, common/machine, common/debug, common/xasm32
app -> SoftPC CLI/provider + SoftPC machine driver
app/产品 adapter -> 自身职责所需的 lib 公共接口
common/session -> common/ui + common/machine
common/debug -> common/machine + common/xasm32
common/* -> 各自实际所需的 lib 公共接口
SoftPC driver -> MVDM/原始宿主兼容接口 + common/machine 中性资源接口
```

common 不 include 产品目录、不依赖 MVDM 类型；callback 由 app 注入。
UI/machine 只调用注入的 sink，不反向依赖 session 实现。
UI 独占展示路径的 Console/broker/KVM lib 调用；machine、session、debug
在自身职责内使用 host/storage/types，不把所有 lib 调用塞入 UI。
app/host 可按自身职责直接 include、调用和链接 lib 公共接口，例如 app
通过 storage 读取 INI；不得直接操作 common 已拥有的 broker/KVM 对象、
executor 或队列，绕过其生命周期协议。common 公共头和产品均可复用 lib
的 copied value 定义，不复制一套标量 ABI，也不新增同义转发接口。

## 线程组装与单向数据流

app 决定创建哪些实体、注入谁、何时启动/停止。通过 common 的公开
create/bind/start/run/stop/join 等实际需要的生命周期接口组装；
不要求每个组件都有全套同形 API，不增加统一线程框架。
session 可继续运行在当前主线程，不因搬迁增加线程。

common/machine 管理已有 executor 活动，调用唯一 SoftPC driver。
common/ui 管理 broker 和 KVM 对象；lib 内部 native worker 仍由 lib
创建、唤醒与销毁，app 通过 common 对象生命周期控制它们，不接触内部线程句柄。
CLI、debug、xasm32 默认不新增线程。S1 逐个记录既有线程、入口、context、
唤醒、join、销毁所有者；每步核对线程数和职责不漂移。

app 是全部线程实体的组装点：它创建 common/session、common/machine、
common/ui 与产品 adapter，并以其生命周期接口启动和回收它们。组件可在自己的边界内调用 lib
创建 native worker，但不得自行从产品回调或惰性 I/O 路径额外生成一条未被
app 组装、停止和等待的生产线程。S1 现状账本已识别两条 worker：runtime
executor（S4 转 common/machine）和 speaker worker（S5 改为由 app 显式
组装的 SoftPC audio adapter 生命周期）。

```text
cooked line / KVM input / machine fact / UI completion
  -> session 唯一 control queue -> reducer
  -> UI API：执行展示/Console 交接，投递完成事实
  -> machine request queue：executor/driver 执行，投递完成事实
```

CLI 在 session 控制线程被调用，返回文字和请求；拒绝命令不入机器队列。
状态文案仅由完成事件触发，monitor 当前可用时输出状态并保证提示符。
接受不等于完成。run 代际留在 common/session-machine 交互内，不进入 lib。
启动、退出和部分创建失败沿既有生命周期处理；app 停止生产者、等待完成，
再释放 sink/context，不能形成第二读取者或新的 callback 重入。

## 冻结迁移账本与删除标准

S1 的有限全集是所有 tracked src/app、src/host 生产 C/H/资源、相关 CMake
target/调用点、对应测试和全部 lib 消费点。每个文件按职责片段登记：
来源、目标、唯一状态/资源所有者、调用者、迁移 S、替代测试、旧实现删除证明。
lib 调用点另注明：随职责迁移、保留原所有者直接调用，或删除重复/越权调用。
保留的直接调用须有明确职责与资源所有者，不作为待清零欠账。
最终处置只有：移入 common；保留纯 app/CLI；保留 SoftPC driver/兼容实现；
删除。混合文件必须拆清职责，不把整个 host 目录冠名 common。

每个迁移 S 在同一交付中修改全部调用者，删除已转移函数、队列、状态字段、
旧头声明、CMake 编译入口及测试旁路。保留的文件可含尚未迁移的独立职责，
但须列出后续所属 S；不得保留 common 已覆盖的第二实现或永久转发兼容层。
纯搬迁用 git mv；机械改名与职责抽取可辨识，不从零重写行为。

S5 按职责复核 media、clock、sync、配置文件读取和 prompt trace，而非
强制搬走这些调用。common 职责所拥有的资源随实现迁移；app/产品 adapter
自身拥有的文件、媒体或计时等资源可继续直接使用 lib。配置读取、解析和
校验留在 app，底层 storage 调用不需要 common 包装。SoftPC driver 保留
CHS/InPort/BOP/设备计时等产品语义。每个资源仍只有一个生命周期所有者，
不能绕过 common 管理的实体，不能保留重复实现；也不能为了“隔离 lib”
添加逐函数同义 wrapper 或改用原生 API。MVDM 保持不动。

## 顺序 S 任务及 EXE 验收

每个 S 交付 assets/binary/softpc32.exe 与 softpc64.exe，附产物哈希、
源提交、构建/测试结果及简短手测步骤。只交库单测或未接入产品的目标不足
以验收。每个 S 完成旧路径清理、双宽度验证、提交推送及独立审查后即进入
下一 S；用户可异步测试每个已推送的包，反馈的回归按对应任务边界修复。
每个 P 也必须完整提交并推送。

| S | 修改与清理边界 | 自动证明 | 用户验收的双 EXE 场景 |
| --- | --- | --- | --- |
| S1 基线/账本 | 冻结全集、线程/调用图、职责去向、命令/显示矩阵；逐项区分 lib 调用随职责迁移或保留直接调用；补确有缺口的外部行为测试，不改生产架构 | 源/产物哈希、全部既有回归；保留调用有唯一所有者；已有 TODO 不改写成成功 | 重建现有程序：monitor/help/start、DOS、Win3.1、pause/resume、stop/start，形成后续对照包 |
| S2 UI 提取并接通 | 将 app/presentation.c、monitor.c 及其头中的对象/显示/交接移入 common/ui；标题、热键表、help/status surface 内容注入；删除旧 UI 资源实现和声明 | handoff、retirement、X、frame、冻结热键、焦点顺序测试走真实 common；旧 app UI lib 调用为零 | display=console 两种 console_control、display=window；DOS/图形切换、CAP、X/resume、捕获/CAM、缩放及 monitor 即时输入 |
| S3 session 与 CLI 注入 | 移走 control.c、control_state、reconciler、presentation_plan、main 中 dispatch/reduce/prompt 调度；command.c/h 保留唯一产品命令 provider，拆走通用调度 | 命令矩阵、start 后三条命令排列、拒绝不派发、完成文案/prompt、迟到事件、paused 输入门禁；main 不再归约状态 | 所有命令及不可用提示；CAP 恢复、X 后 resume、Window running 时 monitor 命令可用且提示符完整 |
| S4 machine 与 driver | runtime.c、input_queue 的通用执行/请求/帧发布/线程生命周期移入 common/machine；keyboard.c、host/machine 和 runtime 的 MVDM 步骤归唯一产品 driver；删除旧 app runtime/input queue 实现 | 完整帧/sequence、reset 单完成、跨 run 8042 输入、冷启动、唯一 executor、停止/join 测试 | start-stop-start、pause-stop-start、start-reset-resume 回 DOS；Win3.1、CAD/CAF、RDP/本地输入；退出无残留 worker |
| S5 重复实现/越权调用收敛 | 复核 host/gfi_image、hdd_media、platform、audio、status、main 配置读取、prompt_trace；删除已迁职责的重复实现和越权调用；保留 app/adapter 自身职责的直接 lib 调用，不添加 common 转发层 | include/符号/CMake 门禁按职责检查：允许 app→storage 读 INI，拒绝绕过 common 管理 broker/KVM/executor；验证媒体模式、换盘、clock/sync、配置错误和 trace 字节契约 | 原 INI 启动、原配置错误行为、DOS/Win3.1、暂停换盘、reset、stop/start、媒体和退出 |
| S6 原版 xasm32 | 固定 NXVM 来源提交和目录哈希，逐字导入并接入 common 构建/测试；不删除 MVDM 有真实职责的解码器，不改 CLI | corpus 一致性、汇编/反汇编样例、边界/错误返回、双宽度链接 | 双 EXE 原命令和运行流程不变；新库能力由附带 focused tests 证明，不冒充新增交互 |
| S7 原版 debug | 固定 NXVM 来源并原样导入，接 common/machine 调试边界；原 CLI 默认入口不变，不新增 debug executor | 可控 machine driver 验证暂停访问、执行请求、lease/错误、CLI provider 注入/关闭；SoftPC 可支持项实测，缺失项明确 unsupported | 双 EXE 原命令、提示符、热键、暂停恢复不变；另附 debug 契约测试，不能以 dormant link 宣称调试器全验收 |
| S8 总体验收/handoff | 全账本完成处置，删除已迁职责的遗留源/target/API/过渡层，更新 README/架构；冻结 common manifest/接口/测试 | 所有职责唯一归属；双宽度全回归、严格 common/lib；合法直接 lib 调用保留且已审查，越权调用与第二实现为零 | 完整显示/命令/输入/媒体/焦点/退出矩阵总验收；用户通过后才收口 T |
| S9 monitor/debug corrective | 保留 monitor 空行修复；debug CLI 与机器状态解耦，仅同步机器访问经过 paused-state machine boundary | 四状态进入/保持/退出、CAP、惰性默认地址、访问错误及 lease/adapter 测试；双宽度完整构建测试 | debug 不自动暂停；运行时检查命令报错但不退出；暂停后可检查；q 不改变机器状态 |

S6/S7 “原版”指生产源码字节一致，common CMake 接线属于集成改动。
若上游 debug 与抽出的 common/machine 不兼容，先列出签名/语义差异，
在真正的机器边界统一；不得私改原版源、改坏体验或添加第二 machine façade。
需要新 CPU 调试能力或 MVDM 修改时另取产品功能准入。上述原样导入与
不接 CLI 是 S6/S7 的历史范围；后续 S9 已明确批准接通 debugger，并允许
为以下契约修改 common/debug。保留来源记录，不再声称修改后源码逐字一致。

### S9 已批准的 debug 交互契约

原始补充要求：

> 是否可以进入和保持在debug cli，是用户决定 和机器状态无关；只有debug调用机器的同步读写接口，才要paused；如果此时没有paused状态便会报错。

- `debug` 在 INIT、STOPPED、RUNNING、PAUSED 均可进入，不隐式暂停。
  打开时只初始化 CLI，不读取寄存器；默认 CS/IP 等在实际需要的命令中惰性取得。
- `?`、`q` 和其他不访问机器的操作不要求暂停；`q` 只退出 CLI。
  机器状态变化不关闭 debug CLI，提示符由当前 CLI provider 提供。
- 同步寄存器、内存、端口等访问统一经过 common/machine 检查；非 PAUSED
  明确报错并保留 CLI，不返回伪造的零值，不暗中暂停或缓存为稍后执行。
- Window/raw Console 的 CAP 仍走 session 原有控制入口，与当前 CLI 无关。
  debug 活跃时可以暂停/恢复；恢复后机器访问立即受同一状态边界拒绝。
- common/debug 不触碰 MVDM，不增加 executor 或 Console reader。检查 driver
  的全部操作种类，逐项记录支持与明确 unsupported 的证据；trace、break、watch、
  port 不得因有命令解析就声称已支持。需要修改 MVDM 的能力另行准入。
- 验收覆盖上述四状态、续行提示符、失败后继续输入、CAP 与 lease 失效、
  monitor 空行回归及实际 SoftPC adapter；最终交付 x86/x64 EXE 和全量测试。
  本次准入不是实现完成或 S9 收口。

## T56 S1 冻结账本（`121de7c`）

账本的有限全集是该提交下 `git ls-files src/app src/host` 的 60 个路径：
`src/app` 24 个、`src/host` 36 个。路径是覆盖单位；混合职责文件按下列
receiver 拆分，后续 S 必须在同一提交删除已迁出的符号、状态、测试旁路和
CMake 输入。`retain` 不是永久豁免，而是仍有唯一产品职责或指定后续审查。

| 路径集合 | 当前唯一职责 | S/处置 |
| --- | --- | --- |
| `app/control.[ch]`, `control_state.[ch]`, `reconciler.[ch]`, `presentation_plan.[ch]` | 唯一 control FIFO、状态归约、desired/actual 和展示动作推导 | S3 移入 common/session；原实现删除 |
| `app/runtime.[ch]`, `input_queue.[ch]` | 唯一 executor、机器请求/输入队列、run generation、frame/state publish | S4 移入 common/machine；原实现删除 |
| `app/presentation.[ch]`, `monitor.[ch]` | broker、raw/cooked Console、Window/KVM 对象和展示执行 | S2 移入 common/ui；原实现删除 |
| `app/keyboard.[ch]` | SoftPC key/mouse injection adapter 与产品 hotkey 后果 | S4 保留为产品 driver 部分；仅通用入口迁入 common/machine |
| `app/command.[ch]` | SoftPC monitor CLI、帮助与产品文案 | S3 保留为唯一注入 provider；只拆走通用 session 调度 |
| `app/main.c`, `firmware.rc` | 配置读取/校验、标题/热键策略、实体组装、进程入口和资源 | retain app；S3/S5 删除已迁 reducer/资源管理，app 可直接 storage 读 INI |
| `app/prompt_trace.[ch]` | SoftPC trace 产品文件契约 | retain app；S5 复核其直接 storage 调用与 CRLF/NUL 行为 |
| `host/machine.[ch]` | SoftPC machine driver/原始 callback 组装，含当前 generic lifecycle 接线 | S4 拆出 common/machine 通用边界；保留唯一 SoftPC driver |
| `host/gfi_image.c`, `hdd_media.[ch]` | SoftPC FDD/HDD product media providers | retain product adapter；S5 审查 direct storage 所有权、无重复资源实现 |
| `host/audio.c` | SoftPC speaker adapter 和当前 speaker worker | retain product adapter；S5 使 app 显式组装/停止/join worker，保留 device 语义 |
| `host/device_bop.c`, `memory.c`, `keyboard.c`, `mouse_instance.c`, `serial.c`, `parallel.c`, `platform.c`, `video.c`, `v7_pointer.c`, `dib_surface.[ch]`, `input.h`, `status.h` | 原始 host callback、设备/呈现/端点适配和产品状态 | retain SoftPC driver/adapter；S4/S5 逐项删除任何已迁 generic lifecycle 或 lib ownership 重复部分 |
| `host/compat/**` 16 路径 | 保持 MVDM/CCPU/C-VID/BIOS/设备接口的 SoftPC compatibility adapter | retain product compatibility；不得移入 common 或修改机器语义 |

`src/app` 精确路径为：`command.[ch]`、`control.[ch]`、`control_state.[ch]`、
`firmware.rc`、`input_queue.[ch]`、`keyboard.[ch]`、`main.c`、`monitor.[ch]`、
`presentation.[ch]`、`presentation_plan.[ch]`、`prompt_trace.[ch]`、
`reconciler.[ch]`、`runtime.[ch]`。`src/host` 精确路径为上表顶层集合以及
`compat/bios/host_def.h`、`compat/ccpu/{facade.c,lifecycle.[ch],legacy/{gdpvar.h,PigReg_c.h,sas4gen.h}}`、
`compat/cmos/port.h`、`compat/conapi.h`、`compat/cvidc/{gdp_rule_access.h,gdp_slots.h,gdp_state.[ch]}`、
`compat/{edl_fast_bop.c,graphics_console_compat.c,keymouse/cpu4.h,system/error.h}`。

现有后台活动账本：

| Worker | 创建/停止现状 | 接收者与验收 |
| --- | --- | --- |
| runtime executor | `app_runtime_create()` 经 `host_sync_task_create()` 创建；`app_runtime_destroy()` 请求停止并 join | S4 common/machine；保留一条 executor，测试唯一启动/停止/join 与 run generation |
| speaker worker | `softpc_host_speaker_enable()` 懒创建；`softpc_host_speaker_stop()` join | S5 产品 audio adapter；app 显式组装和停止，禁止惰性第二生产路径 |
| lib Console/KVM native workers | common/ui 通过 lib 生命周期间接拥有 | S2；app 不持有 lib 内部句柄，lib 不理解产品状态 |

S1 lib-call 处置：presentation/monitor 的 console/host/kvm 调用随 S2；
runtime 的 host sync/clock 与 generic queue 调用随 S4；`prompt_trace` 的
writer 及 `main` 的 INI storage 调用保留 app；`machine`/media 的 storage
调用保留产品 adapter、在 S5 验证资源所有权；audio 的 host-sync 调用保留
adapter、在 S5 调整组装路径。所有其他 lib call 在 S1 的 CMake/source
inventory中按同一 receiver 复核；不以调用发生在 app/host 为错误。

已存在、可承接迁移的自动证据：runtime restart/cursor/input、lifecycle、
command、control-state/reconciler、presentation plan/shutdown、KVM
admission/control/retirement、machine/device/media/boot smoke。S1 不新增
第二套行为测试；其 focused proof 是这些测试的测试目标与上述 receiver 的
一对一映射，随后两种宽度全 CTest 证明现状基线。任何发现的真实缺口才可在
本 S 增加最小外部行为测试。

### S1 P1 执行证据

构建只复用现有已验收生产源码；账本审查没有发现一个能以不改变生产架构的
最小外部行为测试填补的空白，故未新增测试或制造重复测试路径。2026-09-13
实际执行 `cmake --build --preset tests-x64`、`ctest --preset test-x64`，
以及 x86 对应命令：x64 58/58、x86 58/58 全部通过。独立
`cmake -S src/lib -B build/lib-strict -DLIBRARY_STRICT_WARNINGS=ON`、构建和
CTest 通过 8/8；文档门禁和 `git diff --check` 通过。

固定包 SHA-256：`softpc32.exe`
`FDE1E2BEF0802AE92FC6434A0F8B4524AC9AE53F57F8BAC9C55DCC335922797A`；
`softpc64.exe`
`8BC57B396190BC97994AF042E817E95C23A3E6E6E437AAC8D1D4EBD5D15D696C`。
构建没有改写 INI 或媒体，也没有产生生产源码 diff。用户手测以这两个包为
准：monitor `help/start`，DOS，Win3.1，pause/resume、stop/start；后续
S2--S8 均以相同体验作为对照。

### S2 P1 执行证据

`common/ui` 现为生产路径中 broker、cooked monitor logical Console、raw VM
Console、Window/KVM 实例及其创建、销毁、绑定、frame、title、freeze 和 monitor
输出/arm 的唯一所有者。`app/main.c` 只注入复制 hotkey、标题和图形 raw-Console
状态文本，并把 common 的复制事件桥接到既有 control queue；它不再创建或销毁
lib Console/KVM/broker 对象。旧 `app/monitor.[ch]` 与
`app/presentation.[ch]` 已删除，CMake 与测试的旧路径同步清理。

执行了两固定宽度完整 CTest（各 58/58）、strict standalone lib CTest（8/8）、
文档/DAG 门禁及 diff hygiene。两个 package EXE 只刷新
`assets/binary/softpc32.exe` 和 `softpc64.exe`，没有修改 INI 或 media。S2
结束后进入 S3；session reducer/CLI/machine executor 仍是 app 的唯一实现，
没有宣称已迁移。

### S3 P1 执行证据

`common/session` 现拥有生产 control FIFO、actual-state reducer、frame
sequence gate、presentation action 推导、prompt scheduling 以及唯一的
control-loop dispatch。`app/main.c` 仅读取配置、创建实体，并注入 SoftPC
runtime adapter 与 `command.c` CLI provider；它不再拥有 queue、reducer、
presenter action、prompt scheduler 或 control loop。原
`app/{control,control_state,reconciler,presentation_plan}.[ch]` 已同一交付
删除。app 对 runtime 枚举和 lifecycle request 的转换均是显式 composition
boundary，不依赖两个组件的枚举编号相同。

completed runtime 文案继续由 CLI provider 暂存，只有 cooked monitor 为
Current Console 时才输出；不会在 raw Console 路由中写入 monitor 状态文本。
新增 source-boundary gate 同时拒绝旧 app session 文件和 main 中重新出现的
queue/state/reconciler 实现。types-layout self-test 在每次运行前删除全部
自有 fixture probe，失败/中断后的残留不再污染下一次正向检查。

固定 package 构建、完整 CTest 各 58/58：x64 与 x86 均通过；strict lib CTest
8/8、documentation/DAG gate 与 diff hygiene 通过。两个交付 EXE 同步刷新：
`assets/binary/softpc32.exe` 和 `assets/binary/softpc64.exe`；INI 与媒体未改。

### S4 P1 执行证据

`common/machine` 现在是唯一生产 executor、lifecycle/input queue、run
generation 与完整 frame publication 所有者。原
`app/runtime.[ch]`、`app/input_queue.[ch]` 已从生产树删除；`app` 仅创建
`app_machine_driver`，把 SoftPC reset/run/stop/wake、guest-input conversion、
MVDM frame capture 与 removable-media 操作注入 common。`common/session`
直接调用一个中性的 `common_machine`，不再通过 runtime adapter。prompt trace
仍在产品 driver 的 published-frame observation 中执行，未进入 common。

新增无 Sleep 的 fake-driver smoke，覆盖 cold start、run generation、完整 frame、
input wake、reset 的一次最终 completion、resume、stop 与 join；原
runtime/restart/input-continuation smoke 通过 test-only compatibility surface
实际执行 common/machine，不形成第二生产实现。静态 boundary gate 同时证明 app
没有遗留的 generic runtime/input queue 源。

`CMakePresets.json` 的 x86 preset 现在显式固定到
`D:/programs/msys64/mingw32/bin`，确保其 GCC/cc1/DLL 同属一套 32 位 runtime；
这修正了先前外部 PATH 混入异构 DLL 时只产生 x64 包的构建环境问题。S4 的每次
baseline 与交付均以 `softpc32.exe`、`softpc64.exe` 同时存在为必要条件。

本次固定 package SHA-256：`softpc32.exe`
`7C735F8AE9474BE5DFCCEC59C4BCFCCA87050BD245437CD5B2C99BEF5A6210E9`；
`softpc64.exe`
`26DC5A9FA1175A3D3B80B3969591285E8506D72B73D57EA5CCFC9483890378C0`。
双宽度完整 CTest 各 59/59；strict lib 8/8、documentation/DAG gates 与
`git diff --check` 通过。构建仅刷新两个 EXE，未改写 INI 或 media。

### S5 P1 执行证据

S5 逐一审计 app/host 对 lib 的直接调用。以下均保留在其唯一产品所有者，
不建立 common 转发层：app 的 INI byte read（storage）、prompt trace 的
writer（storage）、keyboard 的 copied KVM-to-SoftPC input adapter、machine
预检以及 FDD/HDD image provider（storage medium），以及 original host pacing
所需 clock/sync。它们没有碰 common/ui 的 broker/KVM、common/session 的
control queue 或 common/machine 的 executor/input queue，故不是绕过。

唯一发现的重复生命周期是 speaker worker：第一次 tone request 曾在 host
callback 中惰性创建线程，且原始 reset 的 timer shutdown 会顺带销毁它。现在
app 在实体组装时显式 `softpc_platform_audio_start()`，退出时显式 shutdown/join；
tone callback 只写 frequency 并唤醒已存在 worker，原始 reset 不再创建或销毁
该 worker。新增 lifecycle smoke 覆盖 start 幂等、shutdown 后 re-create 和
第二次 join。此改动不改变 original sound/PPI/timer2 状态机。

S5 的双宽度完整 CTest 各 60/60；strict lib 8/8、documentation/DAG gates 与
`git diff --check` 通过。package SHA-256：`softpc32.exe`
`58D3B35C91B07FF3A3E2C4E0A343BDF235B0C018BCEE7BD603AB800A57282230`；
`softpc64.exe`
`01DF2F0754A8B89944A26F871085C0C79B62BA42C9D33E5A8B436C4B0D352E5E`。
INI 和 media 保持不变。

### S6 P1 执行证据

从 NXVM `e894ef8949a0d92719678f9ebd6cec2793256ac0` 的
`src/common/xasm32` 逐字导入七个源码/头文件，并在
`docs/history/M9-T56-S6-xasm32-source-provenance.md` 记录逐文件
SHA-256。`common-xasm32` 只编译原始三份 C 源，原样接入两项 byte/text
contract smoke；不接 CLI、executor、UI 或 MVDM。x64/x86 完整 CTest 各
62/62，strict lib 8/8 通过；双 package EXE 保持同一已验证哈希，INI 和
media 未触碰。

### S7 P1 接口收敛审计

NXVM fixed source `e894ef8949a0d92719678f9ebd6cec2793256ac0` 的原版
`common/debug` 不能只依赖 xasm32：它要求 `common/machine` 提供一条
暂停态的 typed debug lease/execute 边界。SoftPC 当前 `common/machine`
是唯一 executor，却尚未公开此边界；SoftPC/MVDM 也没有已有 debug
adapter。因此不能直接复制 debug 后声称它已工作。

S7 的收敛顺序固定如下：

1. 在 `common/machine` 添加中性 debug request/result、暂停态 lease 和
   可选 injected debug executor。lease 只在 `PAUSED` 时有效；每次
   start/reset/stop/destroy 都使旧 lease 无效。该层不读取 CPU 或 MVDM
   状态，不自建 executor。
2. SoftPC 的现有 machine driver 暂不提供 debug executor，因此 common
   边界返回明确 `LIB_STATUS_UNSUPPORTED`；这不是第二实现，也不假装
   SoftPC 已支持 debug。contract fake 覆盖 lease、有状态拒绝、失效和
   injected executor 的请求/结果复制。
3. 仅在上述边界编译通过后，逐字导入 NXVM `common/debug` 的五个
   文件；若来源需要一个中性 C-runtime/file declaration façade，则只
   补 types 的声明包装，不复制 storage 或产品逻辑。
4. debug 不进入 SoftPC monitor CLI，不改 MVDM，也不创建线程。S7 的
   proof 是原版 source equality 与 common fake contracts；真正的
   SoftPC debug adapter 是后续明确产品能力任务。

### S7 P2 执行证据

上述中性 boundary 已落地，NXVM 的五个 debug 源文件逐字导入并以 S7
history 记录 hash。现有 SoftPC adapter 未注册 debug executor，因此只会
返回 `LIB_STATUS_UNSUPPORTED`；没有 CLI 或 executor 路径变化。common fake
覆盖 paused lease、result copy、原版 help 与 resume 后 stale lease 拒绝。
x86/x64 全量 CTest 各 62/62，strict lib 8/8 通过；S7 package hashes 见
S7 provenance record。

## 每个 S 的退出条件

1. 迁入职责在 SoftPC 生产路径实际使用（S6/S7 新能力按上表契约验收）；
   对应旧实现/调用/编译路径清理，未迁独立职责逐项登记。
   common 外合法直接 lib 调用不计作残留；验收拒绝重复所有权与越权调用，
   不以直接调用数量清零作为目标。
2. 对照外部事实与完成顺序，复用现有 tests；必要时追加 controllable
   fake/barrier 测试，不以 Sleep、弱化断言或只测试实现内部替代行为证明。
3. 两个 EXE 构建并完成全量 CTest，受影响 common/lib 严格构建和
   manifest/DAG/governance 门禁通过。GUI/RDP 用户实测与自动测试分别报告。
4. 报告生产/测试文件新增、删除、搬迁及 added/removed/net 行数。
   common 新增与本地删除配对；xasm32/debug 新功能源单列，不掩盖重构膨胀。
5. 单人先以 executor 提交推送完整 P，再以 reviewer 审查实际 diff、原始
   要求及证据。状态/记录也提交推送，工作区干净后给 EXE 链接。
   无需等待用户同步手测即可进入下一 S；INI 仅原样提交用户改动，构建不得
   改写；媒体、MVDM 字节不变。

## 不变量与交付边界

现有 Product UI 的命令矩阵、display/console_control、先恢复展示再恢复
VM、X 暂停并关闭、CAP 保留窗口、frozen 热键、source retirement、
鼠标捕获/隐藏/释放、250ms 光标、等比例无黑边缩放、焦点顺序、状态与 prompt
契约全部保留。reset 内部 STOPPED 不成为公开状态；无新 frame 不改写
最后完整帧。lib control FIFO/frame mailbox/broker 事务不因抽取改变。

已有 [TODO](../states/TODO.md) 的 Win3.1 模式往返后 CLS 残留、
偶发 CAP timeout、overlay 性能债务分别保留，不宣称修复、不弱化检查。
迁移引入的新回归必须在本 S 解决。

最终交付为可独立验证的 common corpus、driver/provider 契约、manifest、
行为矩阵和删除账本。NXVM 实际改动与验收由其自身任务完成，本项目不写入
NXVM，不将外部仓库设为运行/构建/验收依赖。只有 NXVM 原样 adopt 并通过
验证后，才可宣称两个产品已完全复用同一 common。
