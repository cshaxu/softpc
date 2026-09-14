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

### T56 S16：共享测试目录及物理键身份

原始准入：“准入。同时增补任务要求：common/test这个玩意要去掉。
我需要你在 test/目录下增加 common 和 lib 用于覆盖当前 common 和 lib 的单元测试。
src/lib, src/common, test/lib, test/common，这几个将来都要原样导入给nxvm的”。

基线 `4cf8629`。有限全集为当前 test 下所有 C 单元、支持文件、fixture、
src/common/test 以及根/共享 CMake 测试注册。按实际依赖分为共享 Lib、
共享 Common、SoftPC 产品集成三类；迁移用 git mv，保留断言，产品专用
adapter、固件/媒体测试留在原产品目录。两套共享 suite 各有独立 CMake
入口，仅使用这四棵目录和编译器/系统库；不复制测试、不依赖 app/MVDM。
生产 corpus 保留 manifest/DAG 自证，测试由外部 suite 注册；整套目录
隔离复制后构建运行，证明未来 NXVM 无需改动即可采用。

Common 的 pressed ledger 用来源、scan（缺失时 key）、EXTENDED 区分
物理键，与 Lib matcher 一致；不新增状态机/API。覆盖左右 Ctrl/Alt、重复
make、分别释放及 source retirement。双宽度完整回归、独立 suite、
manifest 和文档门禁全部通过后提交推送，单人双角色复核并给双 EXE。

增补批准：`machine.c` 私有实现的唯一 `size_t length` 改为 `lib_size`；
没有 ABI 或尺寸行为变化，不扩大到 imported debug/xasm32 源码重写。

#### S16 迁移账本及验证

基线测试翻译单元按实际依赖穷举归类：27 个纯 Lib 单元由 test/unit
搬到 test/lib；Common 六个原 unit、两个 xasm32 单元，加原 common/test
sync，统一由 test/common 注册。新增 physical-key identity 单元；共
27 个 Lib / 10 个 Common 行为测试，不复制旧实现。原测试断言保留。
Common 的 mouse FIFO 测试直接调用真实 common queue，去掉其旧 app
测试别名依赖；Lib writer 的 cleanup helper 原件迁到 test/lib，原产品
测试修订 include 继续用同一 helper，不另造删除实现。

Lib 两个 DAG fixture、KVM 命名 fixture、Linux wait fakes、types-layout
反例和 support runners 全迁入 test/lib；Common negative verifier 迁入
test/common。根 CMake 删除对应 target/注册，不再给共享测试附加 SoftPC
firmware.rc。纯 keyboard 测试原链接 softpc-machine 是多余依赖，改为
仅 kvm-base；其余 fixture 注入 .c 的测试仍保留原边界而非重写。

仍在产品测试：原始 CPU/控制器/BIOS/媒体、runtime/driver、CLI/debug
binding、presentation shutdown/cursor 和真实 package/restart。它们调用
app/host/MVDM 或固件，不能作为原样共享 Common/Lib 单元，未削弱或删除。
共享源码的 manifest/门禁仍在 src，各 suite 自带单独 manifest 与构建。
Common DAG 删除原 test 例外；Lib CI 改从 test/lib 配置，并监听其改动。

新物理键测试在未修复代码上确定性失败（两个 make 的退休只产生一个
break）；修复后 Ctrl/Alt、无 scan 的 fallback、重复 make、分别释放、
错误来源退休及重复退休通过。生产比较与 Lib matcher 的身份定义相同，
不改变 matcher、hotkey policy 或 paused 输入屏障。

已通过：产品 x86 79/79、x64 串行 79/79；独立 Lib 严格构建及 37/37，
Common 14/14。仅复制四目录后的隔离构建再次通过 Lib 37/37、Common
14/14，无 app/MVDM/产品资源依赖。x64 package stage 14 的 ver 可见性
出现两次失败（一次并行、一次串行）；完整串行重跑也曾通过，不能归因为
并行或声称已修。用 4cf8629 原 EXE 和当前 EXE、相同 overlay 配置和原
package test 各对照三次，均通过；没有放宽断言或修改输入/focus 逻辑。
诊断只在 build/s16-baseline 下放归档 EXE、临时相同配置及原测试包装，
各三次、有界原超时，日志不足 16KiB；原 INI/媒体未改。归档目录运行
结束后删除，保留短日志。该未定因观察归 package acceptance 台账。
用户同时将 console_control 改为 0，原样保留提交，构建没有改写 INI。

计数（相对 4cf8629，git diff --numstat，迁移不算新增源码）：生产 C/H
仅两文件 +6/-3，净 +3；新增一份 68 行物理键回归，其余测试只迁移或
改 include/真实 queue 调用。五个 CMake 文件合计 +159/-265，净减 106
行；没有测试聚合转发层、产品固件或额外 Lib API。43 个既有文件用 git mv
迁移；三个测试/fixture runner 的原路径依赖全部收回所属 suite。

最终补充 lib_size 后：x64 79/79（41.02s），x86 79/79（53.56s）；
最终 Common 隔离 14/14（3.57s），四目录逐文件 SHA-256 对照零差异。
命名 gate 的产品范围也跟随 test/lib、test/common 搬迁，故意错误的 fixture
不进入正例扫描，仍由独立反例测试检查。无 MVDM/app/host 源码改动。
本次临时 Ninja 探测、独立构建、四目录副本和基线诊断目录均已清理，
只保留 ignored build 内短日志及两个固定 EXE。

最终 EXE SHA-256：x86
`1713DB04F0EFEFDDBBDA671CFA075500873C393EFAE885F84AA03D9BC0872AC4`；
x64 `7E82E1458D25510E013BAED3023EDF2264F448C361E260029ECDB28CECC86ED0`。

### T56 S15：Common 平台边界及独立证明

原始准入：“批准，准入一个新的S任务修复common，并对lib做最小增补修改以服务common的需求。”
硬约束：“common组件不得自带任何 win32, linux”。

有限全集是准入基线 53667ab 的全部 Common C/H/CMake，以及新增的独立
验证文件。逐项扫描平台类型/API/条件编译和跨组件私有依赖；每项须迁移
到已存在的 Lib 公共契约，或在 Lib 增补实际缺失的最小原语。

1. Host 提供不透明、阻塞、非递归 mutex 的 create/destroy/lock/unlock；
   平台实现仍在 Host 内，Common 不保存任何 Win32/Linux 类型。
2. machine/input_queue、session/control 共用既有 Host event/wait 及新 mutex；
   machine 原子字段改用 Types，保留原先的原子 RMW 和内存序语义。
   删除整段 Windows 条件实现，Common 不另写 Linux 分支。
3. Common 自带完整 manifest、standalone CMake/CTest 和 component DAG 门禁，
   只接受显式 Lib 路径或已有 Lib targets；不依赖 SoftPC 根项目或测试路径。
   公共 ABI 统一 Lib types，禁止借 Types 平台头绕过 Host。

验收：mutex 互斥、队列唤醒/清空和 frame 发布的确定性测试；独立 corpus
及 DAG 正反例；双宽度全回归及 EXE，strict Lib 与文档门禁。平台构建
证据单独报告，不能以去掉 ifdef 冒充 Linux 已跑通。保留线程数、队列容量、
命令/调试/输入/展示语义。完成单人双角色实际 diff 审计并提交推送。

#### S15 P1 实施、自证与同类扫描

生产唯一资源所有者未变。Common machine 的 13 个原子字段在发布对象前
逐一初始化；load/exchange/fetch-add 保留顺序一致内存序，不以普通 volatile
或弱内存序替代。frame 与 input queue 各自持有 Host mutex；session 保留
原 FIFO、64 起始动态容量、固定无分配失败记录和锁内 signal/reset。
Host mutex 在对应平台中直接分配原生锁，不新增包装对象、线程或 Common
平台分支；创建失败保留原始 status，经已有唯一销毁路径清理。

有限全集：Common 全部生产 C/H、四个 CMakeLists 和新增 corpus 自证入口。
`rg windows.h|_WIN32|Interlocked|CRITICAL_SECTION|HANDLE|pthread` 的生产命中
均为 machine/input_queue、machine/machine、session/control，已迁移；
额外 control_state 的标准整数残留统一 Lib 类型。xasm32 的 DWORD 文本是
汇编语法，不是平台类型；门禁区分字面量与实际声明，不能删掉正确产品输出。
保留标准 C 处理及 Types 的中性头，禁止平台 Types 绕过和 sibling 私有头。
原产品测试直接编译 input_queue 的链接补 Host，没有另一份锁或队列。

Common 自带完整 LF manifest、独立 configure/build/CTest 与 common-verify；
门禁检查源 include、实际 CMake 直接边及平台条件实现，负例覆盖 Win32 头、
隐藏 HANDLE、pthread、平台 Types、非法 sibling、private/KVM support、
相对路径、native link 和未列入 manifest 的文件。Common 文件按 .gitattributes
统一 LF；原 CRLF-only 规范化不改变指令、debug 或 source 语义。

验证记录：x64 全量 68/68（71.19s），x86 68/68（90.24s）；严格 Lib 8/8
（3.32s）。Common+Lib 单独复制到 ignored 隔离目录，无 SoftPC 源码/测试，
独立构建、common-verify 和 CTest 4/4（最终门禁复核 1.98s）通过。最后补充 Linux mutex
的初始化失败/清理测试后，两宽度重新构建并 focused 5/5 通过；生产/EXE
不因此改变。Linux Host 实现已被现有可控 POSIX fake 编译/测试，但本机没有
WSL，未声称完成真实 Linux 运行或 KVM 原生显示 parity。

统计方法为基线 53667ab 的 `git diff --numstat`，排除文档和产物：
生产 C/H 12 路径 +296/-202，净 +94（Common 净 +7，Lib 净 +87）；
测试 3 路径 +159/-0；CMake/自证脚本 5 路径 +231/-1。
增加的生产代码是实际缺失的 Host mutex/atomic 原语和初始化/错误清理，
不新增业务状态机。两个 manifest 与文档不计生产代码。

EXE SHA-256：x86 `64B12A8D82B176B98110ECA53AC4E3A480C664DA2F75308EC777D029119B403B`；
x64 `38D71AFB9B0050E07E8B05306F1EE38D3F02371DFA5142D8523CB0935B88A589`。
MVDM、app、原始 host、INI 和媒体均未改动。T56 总审计不在本次收口声明内。
最后门禁反例还覆盖同组件前缀下的 parent traversal 和大写 CMake 调用，
已在独立树重跑；临时独立构建树、复制 corpus 和空 probe 目录清理，
只保留 ignored build 中的短构建/测试日志，双交付 EXE 保留。

#### S15 P2 实际提交复核

Executor P1 `864a74f` 已提交并推送 main；随后切换 coordinator 角色复核
实际提交，而非只看工作区说明。确认 Common 的锁范围、原子顺序、队列容量、
worker 数量和产品请求/完成路径未变；创建失败初始化与唯一清理路径配套。
Host 平台实现承担锁资源，Types 仅包装原子原语；Common 无平台实现目录。
独立构建入口、manifest、DAG 正反例与测试不依赖 SoftPC 根项目，双 EXE
哈希与上述测试交付相符。无待处理的本 S 阻塞项；真实 Linux 与用户 GUI
测试不冒充已完成。S15 交付等待用户测试，T56 和 S13 总审计仍未收口。

### T56 S14：raw → cooked 显示交接修复

原始反馈：“cooked console，从 raw console回到cooked console之后屏幕光标和显示就不正常了，换行也不会清理本行残余字符，而且光标始终好像在倒数第二行”。
原始准入：“你帮我准入一个新的S任务修复一下试试看，记得提交推送后让我测试。”

S13 总审计暂停而非收口。本 S 只修 lib host 的原生显示交接：查明 raw
frame 对 viewport、cells、palette、cursor 的修改及 cooked 激活缺失的恢复。
显示恢复纳入既有串行交接，不在 CLI 或 session 添加逐命令清行，不改变
reader 取消/join、焦点和 VM 生命周期，不新增公开 API 或线程。
正常 cooked→cooked 不应清掉交互，失败回滚不得重新武装已完成的行。
通过 native display probe 与现有可控测试证明缺陷和修复，再做双宽度全量
CTest、严格 lib、manifest/文档门禁，提交推送双 EXE 后等待用户视觉验证。
同类扫描限于 host 原生 frame/stream 输出、显示元数据和全部绑定/销毁出口；
旧 Win3.1 CLS debt 不在本次声明的修复范围。

实施设计：Win32 broker 懒创建一个 raw screen buffer，保留原 cooked
screen buffer；没有新增 native Console 或 reader。既有输出事务内完成
选择、必要的 palette/geometry 恢复，再启动 reader；同模式不切屏。
原生测试发现 raw 强制 80x25 视口会使 cooked 再激活时截断较宽历史，因此
raw 只滚回原点而不缩小宿主窗口。Win32 palette setter 的右/下边界转换
同时纠正；切换失败复用 broker 原有回滚，恢复失败的半状态不覆盖快照。
新增原生测试保存 120x30 显示（含 80x25 外文字），验证反复交接、短行
输出、同模式、分配/选择/读取器/metadata 失败回滚和 raw 下销毁。
package 观察端每次重新打开 CONOUT$，只观察当前可见屏幕，不读旧句柄
绑定的隐藏 cooked 缓冲区；产品输出路径未因此增加第二实现。

#### S14 P1 证据与同类扫描

旧版 `ca810bf` backend 的隔离原生探针已复现：120x30 cooked 画面经过
raw frame 后变成 80x25，光标由 (0,2) 变 (0,0)，cells/palette/cursor
style 同时改变。该探针只用于基线诊断，失败为预期；未修改用户 Console。
最终 `host_console_display_smoke` 使用隐藏的独立原生 Console、真实
屏幕 API 和可控 reader/startup 故障，不用 Sleep。验证跨模式、同模式、
宽屏历史保留、短行空白、native query/allocation/selection/metadata/reader
失败后的旧屏恢复、初始 raw 失败和 raw 下销毁。原生 Console 的 palette
与窗口几何并非由分开的 handle 自动完全隔离，故保留两个 native metadata
快照和一个“恢复已完成”标记；后者防止失败回滚覆盖原先有效快照。

`rg set_console_screen_buffer|set_console_window|set_console_cursor|write_console|CONOUT`
扫描 common/app/host/KVM，所有真实显示修改仍在 host Win32 backend；
common 只使用既有 broker/Console API。初始激活、正常 replace、失败
回滚及销毁都复用唯一 screen-selection helper；没有清屏回调、额外 reader
或第二输出队列。Linux 无对应 Win32 screen-buffer 逻辑，不改其现状。
旧 Win3.1 CLS debt 与本 cooked 交接无同一原因证明，保留原 TODO。

两固定 EXE 完成构建；全量 x64 CTest 64/64（63.72s）、x86 64/64
（83.47s）；严格 lib 构建和 CTest 8/8（3.41s），manifest、DAG、文档与
diff gates 通过。最后增加的初始 raw 故障断言单独重建并通过双宽度
focused test；生产及 EXE 没有再改。真实 package 测试同时保留输入、
DOS cls、CAP、debug 及 stop/start 行为，不冒充用户 Terminal/RDP 视觉验收。

以 `ca810bf` 为基线，生产 C/H 两路径 +87/-3（净 +84）；测试源码两文件
及 CMake 共三路径 +212/-4（净 +208），用 `git diff --numstat` 计数。
lib manifest 与 README、
设计/状态文档单列；common/app/MVDM/用户 INI/media 零修改。
只保留短构建/测试日志，隔离 baseline probe 退出后清理。

EXE SHA-256：x86 `C188518695143C3D9B12CDA4949F9418FAA2A3287C87FAF9AB2683D74D8F16FF`；
x64 `9354F747CFECD34EC47F0B8DDFA5E4C84BA3522CD616E8515CE8C0005C4BEF43`。
手测：console display 下 start → DOS → CAP 回 monitor，输入 help、
空回车及短命令，再 resume/CAP 往返；确认光标、残字和历史恢复。
S14 保留等待用户反馈，不冒充 T56 总审计完成。

#### S14 P2 独立复核

Executor 已提交推送 `07b278e`。同一代理切换 coordinator 角色，读取
该提交的完整生产/测试 diff、统计、任务原始要求和实际日志后复核：
新增原生资源仅归 host backend，全部首次/切换/回滚/退出入口有处置；
公共 ABI、common/app/MVDM、INI/media 未改。测试没有放宽旧断言，
只将 package 的屏幕读取绑定到真正的 active buffer，并增加原生失败
与恢复证明。独立确认 64/64 双宽度、8/8 strict、最终 focused tests、
manifest/文档门禁与产物哈希。接受实现交付；保持 S14 等待用户视觉
反馈，S13 的总审计仍未完成。诊断 probe 目录已安全删除，无残留 probe
进程或新增源/构建依赖；短构建/测试日志保留在 ignored build。

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

### S9 实施范围与同类扫描

有限全集为 `common_machine_debug_operation` 的 17 种操作、两处反汇编
调用、monitor/debug 两种 CLI 的全部 provider callbacks。唯一入口分别是
app command binding、session lifecycle dispatch、machine paused executor
rendezvous；无第二输入循环、executor 或 MVDM 源码改动。

| 操作集合 | SoftPC adapter 处置 | 验证接收者 |
| --- | --- | --- |
| READ/WRITE_REGISTER | 八个通用寄存器读写；IP、flags、六个段寄存器、CR0/2/3 只读；其他明确 unsupported | real debug binding smoke：AX 写入、读回及恢复，R 输出 |
| READ/WRITE_REAL、READ/WRITE_LINEAR | 分页关闭时经过已有 physical-memory copy；分页开启明确 unsupported，不自行实现地址翻译或伪造读取 | real smoke：E/D、A/NOP/D、无效范围失败 |
| GET_CODE_DEFAULT_SIZE、GET_CODE_BASE | 原始 CCPU CS 属性和缓存基址 | R/U、汇编/反汇编实际调用 |
| READ/WRITE_PORT、GET_CPU_SNAPSHOT | 未接安全产品能力，明确 unsupported | real smoke 逐操作拒绝 |
| SET/CLEAR/GET_WATCH | 未接内存访问观察点，明确 unsupported | real smoke 逐操作拒绝 |
| SET_EXECUTION_PLAN、GET_EXECUTION_RESULT | 未接逐指令 trace/break，明确 unsupported，不递归执行 CPU | real smoke 逐操作拒绝，T 不发 RESUME |
| CLEAR_EXECUTION_PLAN | adapter 无可安装计划，清除为空操作；允许普通 G 通过 session 请求 resume | real smoke：G 只返回 lifecycle request |

同类扫描检查了 debug 的集中 `command_execute`、read/write helpers、
U/XU、文件 L/W、异步 observation、续行 prompt 和原有全局地址记录。
失败状态统一阻止后续机器调用并替换伪造的结果文本；read failure 不留下
未初始化数据，W 不写失败读取的内容；复制请求先验证容量。地址记录改为
每个 debug 对象自己的状态。进入和重开 CLI 不读取机器。

真实 U 测试曾在 `f000:fff0` 卡住：原导入 debugger 将 xasm32 返回的
文本长度当作指令长度，且零长度/16-bit wrap 未保证循环前进。修复在同一
反汇编 API 增加独立的 instruction-byte count 输出，同步更新两处生产
调用和所有测试；没有添加第二 decoder。U 对零进展立即退出，先以较宽
整数计算下一地址再截断。S6/S7 导入哈希是历史来源，不再用于声称当前
修改后的 common 与 NXVM 字节相同。

高级 CPU 能力不是本次伪实现的范围。端口需要规定访问宽度和原始设备的
副作用边界；trace/break 需要已验证的逐指令停点；watch 需要真实访问
观察；分页内存和特殊寄存器写入需要各自完整的地址/异常/恢复证明。
这些返回 unsupported 的能力仍须后续明确准入，不能以本 S 的 CLI 接通
宣称全部 DEBUG 命令已实现。

### 后续 S10–S12：调试机器能力完成设计

原始请求：
> 你得告诉我哪些操作没有安全接线，怎样才能安全接通而不是半途而废？
> 准入，请你设计一下。是否需要加开S任务？

以 S9 P5 `d193638` 为设计基线，继续 T56，不新开 T，不扩大 S9。
本轮批准设计和后续顺序；S9 当前等待反馈，未自动宣布收口，也不同时
激活 S10。以下为后续实施简报，进入实施时须更新唯一 CURRENT packet。

#### 共同契约

- 沿用 copied debug request/result、暂停 executor rendezvous 和现有
  session control queue，不增加 debugger worker、CPU 副本或第二消息通路。
- CLI 始终独立于机器状态。同步访问只在 PAUSED 执行；异步 trace/break
  是安装计划后恢复唯一 executor，完成才发事件，不能让 control 阻塞等停点。
- 原始 CPU/device 是唯一语义所有者；产品 adapter 复用其函数。common
  只表达请求和完成，不 include MVDM。lib 不变。
- 每一项 unsupported 必须区分架构不支持与尚未接通。后者不能靠拒绝测试
  宣称完成；若必须改 MVDM，列出准确挂钩、调用频率、异常行为和最窄
  port-ABI diff，等单独批准。此次设计不是修改保留源码的授权。
- 全集继续采用 17 种 debug operation，另按寄存器 ID、地址模式、执行
  计划种类逐项细分。结案只接受真实通过、架构不适用（说明依据）、owner
  批准延期；实现困难不自动转成永久 unsupported。

#### S10：同步机器访问完成

范围：`host/debug.c`、产品 driver、必要的 common debug request 字段及
调用者、测试。复用当前通用 executor，不改变生命周期或 UI。

1. CPU snapshot：在一次 executor 请求中复制段缓存、表寄存器和控制
   寄存器；验证读快照不加载描述符、不修改 CPU。不能仅从 selector 推算缓存。
2. 特殊寄存器写：复用原 setter，分别验证 EIP/EFLAGS、段加载、CR 写入
   的失败和后续执行；不直接写全局状态。非法 selector 不伪成功，CR1 等
   架构不存在项继续明确拒绝。逐项确认 setter 的返回及异常边界。
3. 端口：给现有请求明确访问宽度，标准 I/O 命令固定 byte；扩展命令只
   按实际已有语法支持，不新增猜测。调用原设备分发，验证一次请求仅有
   一次预期副作用，不能失败后自动重试端口读取/写入。
4. 内存：明确 real segment:offset 与 linear 的地址契约；复用原有
   `xtrn2phy` 候选接口前核对权限及 accessed/dirty 行为，按页拆分，
   不产生客户机 #PF、不误改 CR2。RAM、ROM、显存/A20 分别核对已有
   访问入口。跨页写先验证可验证部分，不承诺设备副作用可回滚；失败
   返回须明确，禁止将部分成功冒充整请求成功。

退出：寄存器写后恢复执行、快照无副作用、端口宽度/次数、分页跨页与
未映射/只读/特殊区域均有真实 adapter 测试；同步调用仍只在 executor。
交付双 EXE，可测试 R/寄存器修改、D/E/A/U、I/O；明确具体支持矩阵。

#### S11：单步、执行断点及完成通知

先验证当前构建实际启用的逐指令挂钩：`c_main.c` 的 `check_I`、相关
stub、异常路径和编译条件；仅发现函数名不算可用停点。不得用帧回调、
定时器、另起 CPU 循环或客户机 TF/#DB 冒充宿主调试。

沿用 execution plan，产品 adapter 唯一拥有正在执行的计划及结果。
安装成功后经 session 派发 resume；真正达到停点后，executor 在安全
指令边界停下，machine 投递 copied debug completion（原因、位置、
执行数及现有 run 身份）。session 消费后经注入 provider 通知 debug，
debug 生成文本/后续请求；不得在 CPU callback 中输出或重入同步 API。
GET_EXECUTION_RESULT 查询同一结果，不保留第二状态副本。

定义并测试：trace 计数的指令完成边界、REP/异常/中断；断点停在目标
指令执行前；从断点继续不能原地无限再命中。CAP/stop/reset/q 对未完成
计划统一取消，清理归 adapter；迟到完成不得恢复机器或污染新 CLI/run。
无计划的 CLEAR 幂等，取消不得留下补丁字节或占用客户机 DR/TF。

退出：真实小程序证明 T 精确执行、G 到目标停止、取消/复位无遗留、
guest 自身调试机制不变；fake completion barrier 证明事件时序与拒绝。
若现有挂钩不足，停止相关实现并给出 port-ABI 决策，不用轮询替代。
交付双 EXE，可测试 T、带地址 G 和 CAP 取消；CLI 始终保持可用。

#### S12：watchpoint 与整体调试验收

复用 S11 的停止/结果/取消路径，只增加原执行/内存访问边界的观察。
SET/CLEAR/GET_WATCH 共用 adapter 一份登记表；公共接口按现有单地址
语义处理跨字节重叠，不预造任意范围容器。read/write/execute 的触发点
及命中信息明确；CPU 访问与 debugger 自己读取必须区分，调试检查
不能触发自己。DMA/设备访问是否在原命令承诺内须先核对，不能暗中
声称覆盖所有物理访问。保留客户机 DR0–DR7 的原有功能。

退出：真实读写/执行各命中一次，未命中正常运行，查询/清除一致，
debugger 读取不自触发；CAP、q、stop/reset 清理和迟到事件复用 S11
测试。全集每项有命令级正反例，未接项必须获得明确延期决定。
交付双 EXE，运行完整 monitor/debug、DOS/Windows、CAP/X/resume、
stop/start/reset 回归；不能只以新增单测通过宣布全部 debugger 可用。

每个实施 S 均要求：确定性测试 + 真实 adapter/EXE 测试、x86/x64 全量
回归、双 EXE/哈希、同类扫描、旧路径清理、提交推送和干净工作区。
新增状态只允许服务于真实请求/完成所有权；不因本设计引入能力框架、
额外线程、重复调度器或 lib 产品语义。

### S10 实施与 S11/S12 挂钩审计

S10 验证：x64/x86 全量 CTest 各 63/63，strict lib 8/8；最终声明
去重后双宽度重新构建并通过 debug-binding focused test。真实 executor
验证修改 CS:EIP 后执行指定程序；分页跨页失败不会部分写入，CR2 和
页表 A/D 保持不变。没有改动 MVDM、lib、用户 INI 或媒体。
生产四个文件 +142/-43（净 +99）；测试一个文件 +151/-4（净 +147）。
最终 package SHA-256：x86
`2DC7CEC7B417805B9672C4D6045002351657CD3D133287C3631A0738D08FBFA7`；x64
`C6363E7662817E38A26C9FD6D93B73EB7757D2C0F33721DF7A7413D40C411BE3`。

S10 保留唯一 paused executor，未改 lib 或 MVDM。实际完成：

- 原 setter 连接 IP/flags、段寄存器、CR0/2/3；段加载返回原异常编号
  转为失败。CR0 的 PG=1/PE=0 在调用原 setter 前拒绝，避免客户机 #GP。
- snapshot 复制原 CPU 有效缓存；原 getter 不提供 LDTR AR，所以其
  输出只显示 selector/base/limit，不能伪造 DPL/type。CR1/CR4 属本 CPU
  架构不支持，不是未接适配。
- I/O 请求显式 bytes=1，经原 inb/outb，非法宽度/越界 byte 在调用前拒绝。
- debug 内存使用 xtrn2phy 的无 A/D 更新模式、整请求预检查，再走
  phy_r8/phy_w8；保留 A20 和 SAS mapping，ROM 写明确失败。
  同类扫描发现原 host 的 physical 命名接口实际调用线性 SAS 操作；
  本次 debug 不复用该路径，防止分页后重复翻译，不扩大修改其他调用者。
- 真实 smoke 验证寄存器恢复、有效缓存、非法 protected selector、非法
  CR0、PIC mask byte I/O、跨页读写、未映射页不改 CR2/A/D、跨页写失败
  无前半段修改，以及修改 CS:EIP 后真实 CPU 写出预期程序结果。
  原 XD ffffffff 用例在 A20 包裹下是合法 ROM 读，改为验证实际总线语义；
  不再以旧 host 越界拒绝作为“正确”期望。

#### 已批准的窄 port-ABI 边界

2026-09-13 owner：“批准对mvdm的小范围必要改动”。S11/S12 可增加
必要的指令/访问通知和暂停后 refetch 接线；这是此前 MVDM 不动约束的
明确限域例外。原执行、分页和设备算法不重写，调试状态留在外部。

现有入口不足以直接证明计划中的完整执行/观察语义：

- `c_main.c:816` 的 check_I 是解码前入口，生产 stubs.c 为空；在此停下
  时已有本地取指指针，恢复并修改 CS:EIP 后必须重建，不能只调用 pause。
- `c_main.c:4361` 的正常指令完成路径可区分真实完成，但 MOV SS、
  POP SS、IRET、STI 等直接进入 NEXT_INST；异常返回也进入 NEXT_INST。
  因而现有 pace/budget hook 不是统一的“成功退休一条指令”通知。
- `c_page.c:437/469/497/525` 的 vir_read 路径没有 check_D；现有 check_D
  只在写路径。Intel check_for_data_exception 则属于客户机 DR/#DB，
  不能据此偷偷占用客户机调试寄存器来实现宿主 watchpoint。

建议单独准入窄 port-ABI：在 c_main 的实际指令入口/成功完成边界增加
纯宿主通知，覆盖上述绕行和异常，停机返回时重新建立原取指状态；
在 c_page 的 CPU operand 读写边界增加带 linear address、字节数、方向
的通知。原指令、地址翻译、设备及异常算法不重写；计划、匹配、结果、
取消状态全部留在外部 adapter。暂停只能在完整指令边界执行，访问
通知仅记命中，不能在读写进行一半时阻塞 executor。页表遍历、描述符
内部读取、DMA 是否属于 watch 的覆盖范围须显式区别，不能声称全覆盖。

按上述限域授权执行，仍禁止链接拦截、轮询、客户机 TF/DR 或第二
executor 绕过。S11/S12 不得以拒绝操作测试收口。

### S11 有界收敛账本与 port-ABI 处置

Owner 允许小范围必要 MVDM 改动；本阶段只改 `base/ccpu386/c_main.c`。
新增两个观察声明、DO_INST 命中后 refetch、正常完成和五处成功绕行的
完成通知。没有新增 CPU 状态或改动原 opcode、异常、分页、TF/DR 算法。
这是本阶段原始源码差异的明确保留处置，不把差异称作 pristine。

| 冻结全集 | 唯一所有者及证明 |
| --- | --- |
| TRACE、BREAK_REAL、BREAK_LINEAR、CLEAR、GET_RESULT | product driver 的单份 state；真实程序验证精确 IP/count、同址重设前进、取消和 reset 清除 |
| 普通/quick 完成、POP SS、MOV SS、PIG POPF、IRET、STI | 原 CPU 相应成功路径通知；运行配置的绕行全部有真实 instruction proof；PIG 非当前配置但保留对称通知 |
| 异常/TF/中断/REP/HLT | 异常不计成功；REP 完成一次；真实 fault/TF handler 测试；HLT 不制造完成或中断，等待原 CPU 返回，原 host pause/stop 路径保留 |
| 完成、输出、自动续步 | 原 PAUSED copied event → session provider → paused GET_RESULT；无需新增每指令事件队列。真实 provider `t 2` 和 shipping EXE G/T 检查唯一 IP |
| 取消/关闭/销毁/复位 | 现有 command wake 处理取消；外部暂停取消、driver run 退出取消、reset 清零；close/destroy/reopen 共用关闭路径 |
| 旧路径 | host/debug.h 遮蔽原 CCPU debug.h，改名 machine_debug.h；不保留转发头。同步/异步 debug 输出共用转换函数 |

首次 shipping G/T 测试未隔离 DOS 定时中断，观察到正确进入 ISR 的 IP。
测试程序加入自身 CLI 指令以验证固定 IP；产品不屏蔽中断，单独 TF 测试
仍证明 guest trap 保留。所有测试代码只写一次性 RAM，随后 cold reset；
未修改媒体或用户配置。S12 的 watch/访问记录仍未交付，不作全 debug 收口。

S11 验证：x64/x86 全量 CTest 各 63/63；strict lib 8/8；最后追加的
real-break/reset 断言另经 x64 focused debug/package 通过，x86 全量包含它们。
以 `6483f63` 为基线，生产 11 个路径（含一次头文件改名）+217/-49，净 +168；
测试两个文件 +168/-4，净 +164。MVDM 只改一文件 +12/-4，净 +8。
SHA-256：x86 `658DFEB69B9312864C43B94F91224D88532AA2B8436BC03E07A70952F8A30A7E`；
x64 `72ED204622B6B615073C158921A9520A11022E1B7BF83764BA2C1FCFAAF87859`。

### S12 有界观察账本

冻结范围为 17 项 machine debug operation、现有 command 消费者，以及
CCPU 的 vir_read/write byte/word/dword/bytes 八条操作数访问路径。
栈和字符串指令复用该边界；spr/phy 内部翻译、描述符、DMA 与 debugger
检查不作为操作数监视。PIG cannot_write 不执行真实写入，不加通知。
访问通知只记录本指令候选；成功退休才可命中，异常指令的候选丢弃。
EXECUTE 在解码前停，继续跳过刚命中的同址一次；READ/WRITE 在成功
指令结束后的安全点停。watch 三个单地址槽，无新容器或执行线程。

访问记录通过现有 GET_EXECUTION_RESULT 复制，删除无人调用的
observe_instruction 第二入口。每指令最多 32 条记录；超出明确标记，
监视匹配仍检查全部访问。大块访问记录其完整宽度和最低 8 字节，
不为观察重读内存/设备。CLEAR_PLAN 不删除独立 watch 登记；外部取消、
q、stop/reset 清理所有调试状态。证明逐项覆盖命中/非命中/重叠、
故障隔离、检查隔离、清理、XT/XW/G 消费和双宽度 package。

#### S12 操作/命令处置与同类扫描

全集按 operation 枚举 17 项分组：寄存器读写 2、real/linear 内存读写 4、
byte port 读写 2、code base/default-size 2、CPU snapshot 1、watch 三项、
execution plan 三项。全部由实际 SoftPC adapter 承接；架构不存在的
CR1/CR4 明确不适用，非法参数/非 paused/stale lease 仍拒绝。
`debug_binding_smoke` 的 synchronous_access、execution_plans、command_matrix、
trace_cli、watchpoints、access_boundaries 是逐组证明；原 xasm32 byte/text
测试和真实 A/U、XA/XU 证明汇编/反汇编接线。完整 suites 保留 lifecycle、
输入、frame、UI 和 shipping DOS/package 证明，不声称代替人工 Win3.1 验收。

命令族覆盖 E/F/M/C/S/D 与 XE/XF/XM/XC/XS/XD、寄存器/有效段/控制快照、
H/V、N/W/L 一次性 host 文件、A/U 和 XA/XU；T/XT/G/XG 包含真实 CPU
完成与多次自动续行；XW 包含三类设定/查询/清除及 G 命中文案。
无效 G/T 地址/计数、XW 参数及 watch kind 不派发执行/不安装错误状态。
四状态 CLI、CAP、q、stale lease 和端口 byte 范围沿用 S9/S10 真实测试。

扫描命令：`rg vir_read|vir_write|spr_|phy_` 覆盖 c_page/c_oprnd/c_stack/
c_main；byte/word/dword/bytes 都已接通知，stack/string 复用它们；PIG
cannot_write 与内部 spr/phy 不新增通知。真实测试含两种标量宽度、
byte、stack、x87 reverse-buffer、REP 40 次（记录截断但第 40 次监视
仍命中）、异常 handler 不继承故障指令候选、debugger 检查不自触发。
x87 写入观察与实际存入内存的 8 字节比较；不把原 CPU 的浮点结果
改造成测试期望，额外观察到的不定整数现象交 TODO 独立核查。

`rg observe_instruction|awaiting_pause` 旧入口/只写标记已清零；没有第二
观察 sink 或每指令 frontend 队列。命令矩阵发现 inline XE 错留续行，
现与 E 一样仅缺值时续行；XR 同类计数收紧。G/T/XW 在错误参数后
不再先改状态。结果输出仍经既有 PAUSED → session → provider 路径。

#### S12 P1 交付证据

最终 `tests-x64/tests-x86` 构建成功，CTest 各 63/63（43.42s / 44.52s）；
严格 lib 8/8，文档治理与 diff hygiene 通过。原 package smoke 已在
Console/Window 路由执行 G/T/XW 实际命中，再 stop/cold start 至 DOS。
未修改 lib、用户 INI 或媒体；只刷新两个固定 EXE。

以 S11 closure `53c6625` 为基线，`git diff --numstat -- src test`：
生产 8 文件 +176/-86，净 +90；测试 2 文件 +268/-10，净 +258。
本 S 的 MVDM 仅 `c_page.c` +21/-6，净 +15：八条访问的成功通知及
保留原地址/缓冲区的局部变量，没有访存/翻译/异常算法修改。

SHA-256：x86 `67BAFC9EE9A5BD6552FC423ED09FD47A51A8652BC02297D637F5E145607E4ED0`；
x64 `BB3886C042DBF4A6B01DE9EAF2A10C8C707373878D62E40B271662407E249D4F`。
保留忽略目录内的短 build/test 日志作为证据，无残留运行测试进程、
临时调试程序文件或新增构建树。人工 Win3.1/GUI 测试等待 owner；
本记录不以自动测试冒充该人工验收，也不提前收口整个 T56。

### S9 P5 交付证据

2026-09-13：`cmake --build --preset tests-x64/tests-x86` 均成功；
`ctest --preset test-x64/test-x86 --output-on-failure` 各 63/63 通过，
包含真实 adapter、四状态 CLI、CAP、G、续行、失败和 package 交互。
`build/lib-t56-s8-strict` 使用 `-Wall -Wextra -Wpedantic -Werror`
构建成功，独立 CTest 8/8 通过。文档治理和 `git diff --check` 通过。

本次仍保留 S9 等待 owner 测试，不作 S/T 收口。生产路径是
app command provider → common/debug → common/machine 同步 executor rendezvous
→ SoftPC adapter；CLI provider 从 main 移到 command_binding，原位置删除，
没有第二 CLI、executor、reader 或 MVDM 改动。

变更统计以 P4 `2ab3f1f` 为基线，使用 `git diff --numstat`，排除文档和
二进制：生产及 CMake 16 个路径 +590/-222（净 +368），测试 5 个路径
+219/-5（净 +214）。新增是实际 adapter、CLI 注入和执行线程交接；
main 中原有 provider 实现迁出，不保留转发副本。

交付文件 SHA-256：

- x86：`05A4DAD7E88D1369B909522C6AA9D3E886E123D438B895D6819225CDB93BE65C`
- x64：`8B1979A256E85E24034655D6B82A932E0A62603037E9A8BD58C8DA1797472272`

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
