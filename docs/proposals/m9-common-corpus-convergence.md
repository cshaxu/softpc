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

## 五个组件及组装边界

| 所有者 | 负责 | 契约及禁止事项 |
| --- | --- | --- |
| app | 读取、解析、校验原有配置；选择 SoftPC driver、CLI、热键/标题/状态文案策略；组装启动全部所需线程实体，按序停止回收 | 调用 common 公共接口，注入配置与 callbacks；不复制 reducer、broker 或 executor loop |
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
common/session -> common/ui + common/machine
common/debug -> common/machine + common/xasm32
common/* -> 各自实际所需的 lib 公共接口
SoftPC driver -> MVDM/原始宿主兼容接口 + common/machine 中性资源接口
```

common 不 include 产品目录、不依赖 MVDM 类型；callback 由 app 注入。
UI/machine 只调用注入的 sink，不反向依赖 session 实现。
UI 独占展示路径的 Console/broker/KVM lib 调用；machine、session、debug
在自身职责内使用 host/storage/types，不把所有 lib 调用塞入 UI。
app/host 不直接 include、调用或链接 lib。common 公共头可以复用 lib 的
copied value 定义，产品通过 common 契约使用这些值，不复制一套标量 ABI。

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
最终处置只有：移入 common；保留纯 app/CLI；保留 SoftPC driver/兼容实现；
删除。混合文件必须拆清职责，不把整个 host 目录冠名 common。

每个迁移 S 在同一交付中修改全部调用者，删除已转移函数、队列、状态字段、
旧头声明、CMake 编译入口及测试旁路。保留的文件可含尚未迁移的独立职责，
但须列出后续所属 S；不得保留 common 已覆盖的第二实现或永久转发兼容层。
纯搬迁用 git mv；机械改名与职责抽取可辨识，不从零重写行为。

common-only lib 边界也覆盖 media、clock、sync、配置文件读取、prompt trace。
S5 将中性文件/媒体句柄、运行同步和时钟资源的实际生命周期转入 common；
SoftPC driver 保留 CHS/InPort/BOP/设备计时等语义。配置读取时机、解析和
校验仍归 app；底层读取使用 common 内有资源/错误契约的职责入口。
不得绕回原生 API，不得用逐函数同义 wrapper 通过门禁。无法同时满足中性
所有权、简化和行为不变时，先提交具体边界设计修订，不能暗中豁免调用点。
MVDM 保持不动。

## 顺序 S 任务及 EXE 验收

每个 S 交付 assets/binary/softpc32.exe 与 softpc64.exe，附产物哈希、
源提交、构建/测试结果及简短手测步骤。只交库单测或未接入产品的目标不足
以验收。每个 S 完成旧路径清理、双宽度验证、提交推送及独立审查后供用户测试；
用户确认该 S 后再进入下一 S。每个 P 也必须完整提交并推送。

| S | 修改与清理边界 | 自动证明 | 用户验收的双 EXE 场景 |
| --- | --- | --- | --- |
| S1 基线/账本 | 冻结全集、线程/调用图、职责去向、命令/显示矩阵；补确有缺口的外部行为测试，不改生产架构 | 源/产物哈希、全部既有回归；已有 TODO 不改写成成功 | 重建现有程序：monitor/help/start、DOS、Win3.1、pause/resume、stop/start，形成后续对照包 |
| S2 UI 提取并接通 | 将 app/presentation.c、monitor.c 及其头中的对象/显示/交接移入 common/ui；标题、热键表、help/status surface 内容注入；删除旧 UI 资源实现和声明 | handoff、retirement、X、frame、冻结热键、焦点顺序测试走真实 common；旧 app UI lib 调用为零 | display=console 两种 console_control、display=window；DOS/图形切换、CAP、X/resume、捕获/CAM、缩放及 monitor 即时输入 |
| S3 session 与 CLI 注入 | 移走 control.c、control_state、reconciler、presentation_plan、main 中 dispatch/reduce/prompt 调度；command.c/h 保留唯一产品命令 provider，拆走通用调度 | 命令矩阵、start 后三条命令排列、拒绝不派发、完成文案/prompt、迟到事件、paused 输入门禁；main 不再归约状态 | 所有命令及不可用提示；CAP 恢复、X 后 resume、Window running 时 monitor 命令可用且提示符完整 |
| S4 machine 与 driver | runtime.c、input_queue 的通用执行/请求/帧发布/线程生命周期移入 common/machine；keyboard.c、host/machine 和 runtime 的 MVDM 步骤归唯一产品 driver；删除旧 app runtime/input queue 实现 | 完整帧/sequence、reset 单完成、跨 run 8042 输入、冷启动、唯一 executor、停止/join 测试 | start-stop-start、pause-stop-start、start-reset-resume 回 DOS；Win3.1、CAD/CAF、RDP/本地输入；退出无残留 worker |
| S5 其余 lib 消费/入口收敛 | 处理 host/gfi_image、hdd_media、platform、audio、status 以及 main 配置读取、prompt_trace 的全部剩余 lib 调用；common 拥有中性资源，兼容 host 保留产品步骤；app 仅组装 | 全生产 include/符号/CMake DAG 门禁；媒体 readonly/direct/overlay、换盘、clock/sync、配置错误、trace 字节契约；不允许转发旁路 | 原 INI 启动、原配置错误行为、DOS/Win3.1、暂停换盘、reset、stop/start、媒体和退出 |
| S6 原版 xasm32 | 固定 NXVM 来源提交和目录哈希，逐字导入并接入 common 构建/测试；不删除 MVDM 有真实职责的解码器，不改 CLI | corpus 一致性、汇编/反汇编样例、边界/错误返回、双宽度链接 | 双 EXE 原命令和运行流程不变；新库能力由附带 focused tests 证明，不冒充新增交互 |
| S7 原版 debug | 固定 NXVM 来源并原样导入，接 common/machine 调试边界；原 CLI 默认入口不变，不新增 debug executor | 可控 machine driver 验证暂停访问、执行请求、lease/错误、CLI provider 注入/关闭；SoftPC 可支持项实测，缺失项明确 unsupported | 双 EXE 原命令、提示符、热键、暂停恢复不变；另附 debug 契约测试，不能以 dormant link 宣称调试器全验收 |
| S8 总体验收/handoff | 全账本清零，删除遗留源/target/API/过渡层，更新 README/架构；冻结 common manifest/接口/测试 | 所有职责唯一归属；双宽度全回归、严格 common/lib；产品无直接 lib 调用；无第二实现 | 完整显示/命令/输入/媒体/焦点/退出矩阵总验收；用户通过后才收口 T |

S6/S7 “原版”指生产源码字节一致，common CMake 接线属于集成改动。
若上游 debug 与抽出的 common/machine 不兼容，先列出签名/语义差异，
在真正的机器边界统一；不得私改原版源、改坏体验或添加第二 machine façade。
需要新 CPU 调试能力或 MVDM 修改时另取产品功能准入。默认启用 debugger
命令会改变产品体验，不在本次架构重整内；debug 必须完成真实契约测试，
不能只复制目录后称为已接通。

## 每个 S 的退出条件

1. 迁入职责在 SoftPC 生产路径实际使用（S6/S7 新能力按上表契约验收）；
   所有旧实现/调用/编译路径清理，未迁独立职责逐项登记。
2. 对照外部事实与完成顺序，复用现有 tests；必要时追加 controllable
   fake/barrier 测试，不以 Sleep、弱化断言或只测试实现内部替代行为证明。
3. 两个 EXE 构建并完成全量 CTest，受影响 common/lib 严格构建和
   manifest/DAG/governance 门禁通过。GUI/RDP 用户实测与自动测试分别报告。
4. 报告生产/测试文件新增、删除、搬迁及 added/removed/net 行数。
   common 新增与本地删除配对；xasm32/debug 新功能源单列，不掩盖重构膨胀。
5. 单人先以 executor 提交推送完整 P，再以 reviewer 审查实际 diff、原始
   要求及证据。状态/记录也提交推送，工作区干净后给 EXE 链接。
   INI 仅原样提交用户改动，构建不得改写；媒体、MVDM 字节不变。

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
