# T72：KVM 文本属性中性化设计稿

## 准入与审阅边界

原始请求：“好嘞。准入队列第一条，KVM文本属性中性化，先来个设计稿，我审一审”。

基线 bea66d3f，生产基线为已验收 T71。T72 S1 只调研和交付设计；
S1 设计已由 owner 批准，保留平行数组；随后授权“批准执行到t收口标准
然后等我审核后再正式收口”。S2 实施、S3 审计准入，正式 T 收口仍待审。
下文估算是 S1 基线；实际交付另行记录。不修改 INI、介质或快照格式。
当前状态唯一来源是 [CURRENT](../states/CURRENT.md)。
S1--S4 的设计/结果保留为历史；后续 owner 准入的 S5（第十二节）将
平行数组换成单一字符格数组，其余中性语义和组件边界不变。

## 一、建议：只搬走“解释属性位”的职责

T71 已完成：VM 拥有 CP437；基础文本和两种 leaf 扩展分离；
图形只归 Window；Base 的 control FIFO/frame latest-wins 不解释内容。
本 T 不重复这些工作，不新增线程、对象、缓存、转换层或资源注册表。

当前只有两个真实文本生产者：VM 的机器帧、Common UI 的状态文字。
它们直接提供四项事实：字形索引、资源 bank、前景色索引、背景色索引。
Window/Console 不再知道 VGA 属性位和全局 attribute_font_select。

建议暂时保留平行数组，不改成 cell struct。两种布局都需要四字节/格；
改 struct 不节省空间，却会扩大 text[] 的所有调用点迁移。本次消除的是
语义耦合，不是为了排版对称而改数据组织。

## 二、实际源码与有限范围

| 位置（相对仓库根） | 当前行为 | 拟处置 |
| --- | --- | --- |
| src/lib/kvm-base/frame_interface.h | text[u8]、attributes[u16] 和 attribute_font_select | 换成四个明确数组；移除全局选择开关 |
| src/vm/driver.c::vm_driver_copy_text | 复制原属性，先构造文本后读取字体选择 | 先成功取得字体与选择事实，在原逐格循环内解码一次 |
| src/common/ui/ui.c::common_ui_status_frame | 每格属性 0x07 | 直接写 fg=7/bg=0/bank=0，不解释 PC |
| src/common/machine/machine.c::common_machine_text_frame_changed | 比较 text、attributes、全局开关 | 比较四个数组，保留 palette/font/map 比较 |
| src/lib/kvm-window/render.c | 用 bit3 选字体、低/高 nibble 选颜色 | 直接读取 bank/fg/bg；字形位图的像素位运算仍保留 |
| src/lib/kvm-console/console.c | 同样解析 bit3，原属性直传 Console | 按 bank 查 map，传递明确颜色 |
| src/lib/kvm-console/frame_interface.h | map 注释依赖 attribute selection | 明确由 glyph_bank 选择相应 map |
| src/lib/console/console_interface.h | 另一个独立的 u16 attributes 数组 | 同步改成 fg/bg 索引，避免原生属性越过公共接口 |
| src/lib/console/console.c | 验证尺寸和 BMP 字符 | 在现有验证循环内验证颜色范围 |
| src/lib/console-broker/win32/console.c | 原属性直接赋给 CHAR_INFO.Attributes；缓存原属性 | 仅此处编码 Win32 属性；缓存改存中性色值，保留成功才确认规则 |

生产调用扫描确认 lib_console_write_text_frame 的外部生产调用只有
kvm-console；其余直接调用在测试。Broker 的 Linux 路径仍是原有能力范围，
不为本设计增加平台实现。Console/Broker 不得反向依赖 kvm-base。

Compat 的 attribute_select 输出仍是原设备事实，应保留；
VM 用局部变量接收，不能把它重新带进共享帧。MVDM、Compat 不改。

## 三、结构与转换

保留 kvm_text_frame 其余元数据、容量和固定 80 格 stride，仅替换：

旧：
```c
lib_u8 text[2000];
lib_u16 attributes[2000];
/* ... */
lib_u32 attribute_font_select;
```

拟：
```c
lib_u8 text[2000];        /* leaf 资源中的 0..255 字形索引 */
lib_u8 foreground[2000];  /* text_palette 的 0..15 索引 */
lib_u8 background[2000];  /* text_palette 的 0..15 索引 */
lib_u8 glyph_bank[2000];  /* 0: primary，1: secondary */
/* 删除 attribute_font_select */
```

实际声明继续使用 KVM_TEXT_COLUMNS * KVM_TEXT_ROWS，不新增 magic capacity。
颜色索引不意味着固定 CGA 颜色，颜色由本帧 palette 给出。
bank 只是两个现有资源组的选择，和颜色、强度、VGA 寄存器无关。

VM 在现有复制循环内做：
```c
foreground[i] = attribute & 0x0fu;
background[i] = (attribute >> 4u) & 0x0fu;
glyph_bank[i] = attribute_select && (attribute & 0x08u) ? 1u : 0u;
```

这是保留现有输出，不是“纠正 VGA”：即使 bank=1，前景色仍保留
bit3 带来的索引 8..15；不擅自清 intensity，不把 bit7 新解释成闪烁。
以后要修设备语义，应单独立项，不能混进此结构迁移。

Window 直接取对应 font、palette[foreground/background]；
Console 直接取对应 character map。字体和映射表仍随各自完整文本帧复制，
不逐格携带 Unicode，不引入资源生命周期或额外投递。

下游 lib_console_text_frame 的 BMP text[] 不动，attributes[u16] 换成
foreground[u8] + background[u8]，字节容量不变。仅 Win32 Broker 最终写出时：
```c
cells[i].Attributes = foreground[i] | (background[i] << 4u);
```
这是原生输出适配，不是 VGA 解码。其成功缓存保存两个中性数组，
不增加第二套缓存或更改原有屏幕/reader/事务逻辑。

以上是源码 ABI 变更，所有本仓库调用点同批迁移；不保留 attributes
别名、旧开关或新旧并行接口。外部采用共享 corpus 时须一起更新调用者。
原公共 Console 任意原生高位属性不再是新合同的一部分；当前生产者实际
可见格使用原机器的字节属性。若扫描发现真实生产高位用途，停止审阅，
不能静默删掉效果。原 u16 数组用 memset(0x07) 得到 0x0707 的初始化
不作为新合同保留；无效格不显示，缺省文字明确使用 fg7/bg0。

## 四、有效性、比较和失败语义

- Base 验证可见格 fg/bg <16、bank <2，沿用尺寸错误分类；无效值返回
  INVALID_ARGUMENT。不验证隐藏尾格，不新增不必要的失败条件。
- Window 继续验证自己字体范围；Console 继续验证 BMP map。
- 逻辑 Console 在已有可见格字符验证循环中检查 fg/bg；不另建总校验器。
- 发布前验证失败不改 mailbox、不唤醒、不覆盖成功缓存。STOP、退出、
  请求完成、frame transport、两种 presenter 的生命周期完全不改。
- Common 比较中性色值及全部既有资源。颜色不变而 bank 改变也能发布；
  palette-only/font-only/map-only 改动仍能发布。
- 生产者初始化全部固定容量格，避免未初始化尾部参与现有全数组比较。
  不新增 generation、不改变帧是否 ready 的判断。
- 字体必须先读取成功再逐格解析；只调整现有单 executor 函数的取值顺序，
  不建立设备快照缓存，不读取另一套实时寄存器。

## 五、容量与性能代价

以下是现有声明加本稿布局的静态推算，未编译测量；S2 必须用双宽度
sizeof/offset 测量核对。4 个字节数组无需 packing 或 bitfield。

| 对象 | 当前 bytes | 拟 bytes | 差值 |
| --- | ---: | ---: | ---: |
| kvm_text_frame | 6088 | 8084 | +1996 |
| kvm_window_text_frame | 14280 | 16276 | +1996 |
| kvm_console_text_frame | 7112 | 9108 | +1996 |
| kvm_window_frame（图形 union 主导） | 984084 | 984084 | 0 |
| common_machine_frame | 985112 | 985112 | 0 |
| lib_console_text_frame | 8084 | 8084 | 0 |

每个文本副本多约 2 KiB；Window/Common 最大分配和图形传输不变。
Console 待处理帧及 worker 文本副本相应增大，不声称全程序只多 2 KiB。
Machine 颜色/bank 比较多读约 2 KiB；Base 增加最多 2000 格的验证循环，
现有多次 validate 调用会重复这项检查。S2 须计入实际调用次数并作有界
测量；不为省这点工作引入验证状态缓存或 unchecked 第二 API。
VM 的位运算移入原复制循环；Window 不再每条字形扫描线重复解析 bank。
净速度变化暂不下结论，不承诺性能提升；图形热路径没有新工作。

不选压成另一个 flags 字：它节约约 2 KiB，却重新引入共享位布局和
解码规则。也不选每格 RGB/Unicode/扩展 atlas：本任务没有这种需求。

## 六、估算与任务拆分

实施前估算（不是保证，C/H，不含文档/manifest/二进制）：

| 文件 | 预计新增 | 预计删除 |
| --- | ---: | ---: |
| src/lib/kvm-base/frame_interface.h | 18 | 3 |
| src/lib/kvm-console/frame_interface.h | 1 | 1 |
| src/lib/kvm-window/render.c | 7 | 8 |
| src/lib/kvm-console/console.c | 8 | 5 |
| src/lib/console/console_interface.h | 3 | 1 |
| src/lib/console/console.c | 4 | 0 |
| src/lib/console-broker/win32/console.c | 13 | 8 |
| src/common/machine/machine.c | 7 | 5 |
| src/common/ui/ui.c | 4 | 1 |
| src/vm/driver.c | 18 | 8 |
| 合计 10 个生产文件 | 83 | 40 |

预估生产净增约 43 行，主要是明确字段、初始化、校验；
不是本次减少代码数量的项目，而是减少共享层对产品编码的知识。
如需要显著超过此量或新增对象/状态/层次，先重新审设计。

测试预计 10--12 个文件，约 +180/-35，净 +145；现有属性 fixture 主要在
kvm_frame_damage_mouse、kvm_console_retirement_barrier、
console_broker_display、runtime_cursor、vga_frame；另扩展
kvm_frame_copy、lib_console_smoke、lib_console_io_contract、
common machine_wait/composition 及必要 diagnostics fixture。
无属性的 text[] 消费者保持原样。实际范围以新字段边界矩阵为准，
不能为了兑现估算漏测。manifest、说明、静态门禁增量另列。

- **S1（已接受）设计审计**：冻结上述有限调用链和前后合同，交付供 owner
  审阅；代码/测试增删均 0，不重编译。Owner 已批准平行数组设计。
- **S2（已完成）一次性迁移**：同一个交付迁移这条链所有生产者、消费者、
  验证、缓存、比较和测试；不分成能编译但语义混合的多个过渡版本。
  双宽度构建、后台回归、真实 Win3.1 往返及 snapshot 覆盖后，交付两份
  EXE 和实际增删统计，继续已批准的 S3，再等待 owner 手测。
- **S3（审计和复审已完成）收口审计**：核对全部旧属性解码去向、实际大小/成本、
  Shared corpus/DAG 和用户测试结果；无新功能，无无限“顺手重构”。
  T 关闭另需 owner 批准。

## 七、验收矩阵与禁止语义漂移

有限矩阵：256 个旧字节属性 × 2 个旧字体选择状态，共 512 组。
Window 检查实际前景/背景像素和选中的不同字形；Console 用不同 primary/
secondary 字符检查映射结果；原生 mock 检查最终颜色字与旧低字节一致。
另加中性合同允许、旧编码无法独立表达的 fg1 + bank1，证明不再隐含耦合。

覆盖颜色 0/15/16、bank 0/1/2、palette-only、font-only、map-only、bank-only、
80x25 与小尺寸、隐藏尾格、默认字体、光标扫描线/相位、状态说明画面、
latest-wins 跳帧及非法帧不覆盖已接受内容。新测试使用真实生产入口；
不以测试自写转换代替 VM/leaf 实现。

硬边界：不改文字容量、默认字体/光标、图形、输入/鼠标/热键、Console
交接/焦点、生命周期、CPU/设备、定时、媒体、快照格式。生产原设备属性
仍在设备/Compat 内；共享代码中不应再有 VGA bit3 选择和 nibble 解码。
字形 bitmap 的像素位运算及 Broker 的 Win32 编码是明确保留项，
不能用笼统禁位运算检查误删。

本 S1 仅源码审阅与设计推算，没有动态效果/性能通过声明。

## 八、S2 有限收敛账本

冻结范围为第二节 10 个生产 C/H 路径及其现有调用者，不扩大为全树无缺陷声明。
单位为生产／比较／验证／输出边界；处置只允许迁移并验证，或明确保留及原因。
完成条件为下面各行证据通过、双位后台回归及 corpus 门禁通过、实际提交复审；
T 正式收口另待 owner 审核。以下测试名称对应现有 CTest 项，不新建测试框架。

| 边界 | 处置与证据 |
| --- | --- |
| VM 属性和字体选择 | vga-frame-smoke 经真实 driver.copy_frame 检查 512 组；设备、Compat 不动 |
| Common 状态文字 | composition 检查全容量 fg7/bg0/bank0 |
| Common 比较 | machine_wait 检查 fg/bg/bank/palette-only 发布与重复抑制，既有字体/map-only 保留 |
| Base 字段及尺寸 | kvm_frame_copy 检查合法上界、越界、隐藏尾格、小尺寸、两种帧互换和 sizeof |
| Window 消费 | kvm_frame_damage_mouse 检查 512 组像素、独立 fg1/bank1，既有光标/dirty 测试不变 |
| Console 消费及准入 | kvm_console_retirement_barrier 检查 512 组 map/颜色、独立 bank，非法值不更改 generation、不唤醒、不输出 |
| 逻辑 Console | lib_console 检查颜色越界不调用 sink；容量 sizeof 保持 8084 |
| Win32 输出和缓存 | lib_console_io_contract 检查 256 个原生颜色字及重复帧不重复写；既有失败/裁剪缓存测试保留 |
| 其余调用者 | runtime_cursor、console_broker_display fixture 迁移；无属性的 text[] 消费者不改 |
| 保留项 | 字形 bitmap 位运算、Broker 原生颜色编码、VM/Compat 设备事实各有唯一职责 |
| 历史诊断 | test/support/diagnostics/runtime_boot_smoke.c 已在 TESTS.md 标为不编译的历史程序；旧帧字段早于本 T 已失效，不作为当前 ABI 消费者或通过证据，也不扩大本任务重写它 |

S2 编译实测更正 S1 的逻辑 Console 算术笔误：8084 而非 8080；前后大小不变。
其他五项布局推算由双位测试断言核验。最终通过结果和实际 diff 记入 S2 历史。

## 九、S2 实施差额与成本审阅

生产 C/H 实际 10 文件 +50/-32，净 +18；测试 C/H 10 文件 +132/-7，净 +125。
复算方式为 git diff --numstat e0ad4e8f，并按 src/test 下 C/H 分组。
未扩大预估的生产路径；比预估少的行来自原循环、初始化、比较与失败出口复用，
没有省略任何规定验证。四份 manifest +25/-25；文档、两份 EXE 单列。
历史诊断没有作为一个可用程序迁移，因此实际测试文件为 10 个，而非上限 12 个。

有界性能探针采用 O3、真实 kvm_text_frame_validate 函数、volatile 函数指针，
100 万次 80x25 校验，每轮改变一格 fg 但保持合法，QPC 计时：
x64 1.931 us/次，x86 1.709 us/次；两次均返回 100 万次 OK。
这是单机瞬时校验成本，不是新旧整机 benchmark，不证明整机加速。
探针只存在于 build/t72-proof，记录后删除，不引入产品接口或测试依赖。

现有调用次数按具体输出路径而非“每帧恒定”计：VM 产文本时一次；
Common 交付复制一次；Window 发布、消费尺寸、文本渲染各一次；
原生 paint 一次，显示光标时 cursor geometry 再一次，通常合计 6--7 次。
额外重绘／光标定时独立发生，不能把 7 声称为全局上界。
Console 路径为 VM、Common 复制、Console 发布三次 Base 检查，之后逻辑
Console 原有 BMP 循环顺便验证两个颜色值。Common 状态文字无 VM 检查。
不缓存验证结果，不新增 unchecked 接口；图形帧不进入新增逐格循环。

失败及估算修正均已公开：初次 x64 严格编译发现原生颜色三目表达式的
整数提升符号警告，改为同型零值；首次尺寸测试发现 S1 算术少算 4 字节，
改正的是断言/估算，不是改变布局来迎合测试。不得把初次失败记成通过。

## 十、S2 P1 交付验证

两宽度均 Release 全构建通过；Lib 均启用 -Wall -Wextra -Wpedantic -Werror。
serial background presets: x64 105/105 (164.85s)，x86 105/105 (147.06s)。
其中 runtime-restart-boot 真实无桌面 Win3.1 PIF 往返通过（57.95s / 56.96s），
snapshot transaction/cross-process/media/boundary、restart、cursor、组件门禁通过。
每宽度 5 项 desktop 测试未运行；不宣称本次原生桌面视觉或 Linux runtime 验收。
文档治理及 diff --check 通过。无 MVDM/Compat、INI、media、snapshot 格式变更。

| 交付 | bytes | 对 S1 基线变化 | SHA256 |
| --- | ---: | ---: | --- |
| assets/binary/softpc32.exe | 3659288 | +4096 | 6CF6D5CCC83794B5AAFA62093DDF34BA42A1CFCA4727855CBE3105020B757977 |
| assets/binary/softpc64.exe | 3063343 | +4608 | 30C8FA052F3EDAF2B15B2BD3E82B2467D4D1ECE9335AA5910C25A189FA3E0A75 |

测试及有界测量证据已在本节和有限账本中保留；自建探针和临时日志清除，
既有构建树保留供增量验证。P1 推送后按实际提交复审 S2，再准入已批准的 S3。

S2 已交付 12bf7c96、复审 ee720d96 并收口。
[S3 技术收口准备审计](../history/M9-T72-S3-completion-readiness-audit.md)
覆盖整个 T 请求与 S1--S3；不代替 owner 的手测和正式 T72 收口批准。

## 十一、S4：原生鼠标位移修复（owner 扩展准入）

Owner 在文本迁移验收时发现 RDP 捕获位移异常，明确批准修改 Lib 内部输入。
此项与文本 schema 无关：T72 前后输入代码相同，问题来自 T70 引入的居中采样。
本机探针 514 个 raw 包全部 relative；RDP 341 个全部 absolute/virtual-desktop。
RDP 原生 X 连续减小，但旧中心差分输出正 X，证明旧路径不能正确解释该输入。

批准方案：删除所有 SetCursorPos，WM_INPUT 是唯一位移来源。relative 直接
按既有比例/余数换算；absolute 从 0..65535 转成桌面单位后做相邻差分，
首包只定位，设备/坐标空间/桌面或窗口几何变化后重新定位。WM_MOUSEMOVE
不发送位移，原有按钮、捕获/释放、冻结、热键和退休路径不变。

Raw mouse 注册是进程级资源：捕获只接受无现有注册的情况，拒绝抢占其他
消费者；释放只移除自身目标。调用者必须串行协调其他 raw 注册操作。
不新增 RDP 分支、边缘阈值、重试循环或第二条输入流。相对 raw 位移绕过
宿主指针加速，手感可能改变；绝对设备的有限坐标范围仍需 owner 验证，
不宣称拥有相对设备的无限移动能力。公共签名和 copied delta ABI 不变。

实施前估算：生产六 C/H +140--200/-45--80，测试 +130--200/-60--100。
实际生产七 C/H（含公共头注释）+148/-44，净 +104；测试一文件 +142/-34，
净 +108。增加量主要是原生 packet 解码及注册资源边界，非新的业务状态机。
没有改 Common、VM、Compat、MVDM、Console、INI 或介质。
构建、测试、交付及验收边界记录在
[S4 交付记录](../etc/evidence/softpc/m9-t72-s4-native-mouse-motion.md)。
Owner 已确认 desktop/RDP 测试均通过，批准 S4 收口；
[S4 收口与 T72 就绪汇总](../history/M9-T72-S4-native-mouse-acceptance.md)
核对原始目标及追加范围。T72 正式收口仍待明确批准。

## 十二、S5：单一字符格数组（owner 追加准入）

Owner 原始请求：“我希望是代码更干净，更易懂。看起来我们应该合并。
准入一个新的S任务做这个。”此决定只替换 S1 的平行数组布局选择，
不撤销四项中性事实、leaf 资源所有权或原有输出语义。

在原 frame_interface.h 定义 kvm_text_cell，按 glyph_index、glyph_bank、
foreground、background 四个 lib_u8 排列。基础帧使用固定 cells[80*25]。
用编译期 sizeof/offset 断言保证恰好四字节且无 padding；不使用 packing。
每帧调色板、位图和字符映射仍位于现有位置。所有帧大小必须保持不变。
VM 和 Common 原有循环写 cell，Common 四次比较合为一次，两个 presenter
在原循环直接取 cell。底层 lib_console_text_frame 是独立 Unicode 契约，
不依赖 KVM、不迁移其数组、不新增类型转换对象。

有限范围：六个生产文件、九个测试文件的现存 KVM 文本消费者；每个
成员只能是已迁移并证明，或因属于独立 Unicode Console 而明确保留。

| 边界 | 修改及证明 |
| --- | --- |
| Base frame_interface.h | cell 定义、无 padding 断言、逐格验证；frame-copy 测试保证全部原 sizeof、可见格/隐藏尾格、拒绝不覆盖 |
| VM driver.c | 原数据源和读取次序不变，完整初始化和一次 cell 赋值；vga-frame 512 组和 runtime-cursor |
| Common ui.c | 状态文字按 cell 初始化和写字形；composition 校验每格及两行内容 |
| Common machine.c | 一个 cells 比较，其他元数据/资源比较保留；machine-wait 单独改变四个字段 |
| Window render.c | 原逐格渲染直接读 cell；512 组像素及独立 bank/颜色测试不变 |
| Console console.c | 同一转换循环取 glyph/map 和两种颜色；原 512 组、失败拒绝/退休测试不变 |
| 测试其他消费者 | win32-presentation 和真实 restart-boot 文本扫描改为取 glyph_index，不把 cell 内存当连续字符串 |

实现前估算：生产六文件 +45--70/-40--70，净接近零；测试九文件
+80--130/-60--100。此迁移不承诺明显加速；字节容量不变，主要收益是
每格责任聚合、减少关联四数组的心智负担。不为净减行数压缩排版。
不改鼠标、输入、图形、线程、mailbox、MVDM、Compat、INI、介质或快照。
双宽度后台验证后交付 EXE、提交推送和实际变更复审，等 owner 测试。

S5 实际生产六文件 +31/-33，净 -2；测试九文件 +94/-57，净 +37。
没有靠重排格式压行数：一次 cells 比较替代四次比较，原两个生产循环
直接构造完整 cell，原两个消费循环直接读取。低于估算来自复用原循环，
不是省略验证或增加兼容转发。尺寸断言保留，另验证 cell 各字段偏移、
相邻格/固定80跨度、四字段独立改变和完整复制。完整记录见
[S5 证据](../etc/evidence/softpc/m9-t72-s5-text-cells.md)。

Owner 已报告测试通过并批准 S5 收口；见
[S5 收口](../history/M9-T72-S5-text-cell-acceptance.md)。T72 保持开放。

## 十三、S6/S8：按组件拆分质量审计后续

Owner 后续决定将 Common 的原计划 S7 顺延为 S8，
原范围 A/B/C/D/F 不变。新 S7 见第十四节。

Owner 原始请求：“分成两个s任务 第一个先优化lib 第二个处理common”。
基线 c635988a；六项有限范围以 [审计报告](../etc/evidence/softpc/m9-t72-post-s5-quality-audit.md)
为证据，不重新扩大为全树重构。最新顺序为 S6 → 新 S7 → S8，每步等 owner 测试。

- S6 / Lib：只删除 Window 文本渲染前必被完全覆盖的清零。生产一个文件
  预计 +0/-2，测试一个文件约 +40--60；更新两份 manifest 和两份 EXE。
  用预填非零像素及前后哨兵验证 1x1、非标准小网格、80x25，默认及全部
  1..16 字体高度、双 bank、空白字形和无效输入不写出。无 API/布局变化。
- S8 / Common：A 销毁失败保留 live worker 依赖；B 状态帧提供可读颜色；
  C TEXT 走现有输入路径、由 VM 决定支持；D 三个 create 统一清空输出；
  F Debug 固定参数数组内嵌。保留原命令语义和风格，不新增状态机、缓存、
  队列或恢复框架。开始前核对具体变更量及调用方失败处理；若 App 的最终
  teardown 接收方需要同步调整，须先写入该 S 简报，不隐瞒跨组件边界。

每个 S 单独双宽度构建、后台全回归、代码增删统计、提交推送和实际变更
复审，等 owner 手测。S8 暂不实施；其精确文件/diff 估算在准入执行前汇报。
收敛账本：E 属 S6；A/B/C/D/F 属 S8；没有遗漏审计项，也不恢复 TODO。
S6 同类扫描保留图形 surface 初始化、鼠标/键盘状态 reset：它们不是随即
完全覆盖的文本输出，不能为净减代码而删除。没有测量整机速度提升的承诺。

S6 实际生产 +0/-2（净 -2），测试 +46/-1（净 +45），符合预估。
双宽度构建和后台验证已完成；文档门禁初次失败及复查结果如实记录于
[S6 证据](../etc/evidence/softpc/m9-t72-s6-text-render-simplification.md)。
P1 01a737b2 已推送、实际变更复审完成；owner 认可并准入新 S7，S6 已收口。

## 十四、新 S7：Window 统一像素差分

Owner：把 S7 推迟到 S8，准入新的 S7，统一文本/图形帧转成位图后的
dirty 比较和绘制，减少可能的闪烁。S6 随 owner 认可和后续准入收口。

保留 latest-wins 及现有 RGB surface。一个内部 render_frame 替代原
render_text/render_graphics；两种解码直接生成颜色，调用同一个可内联的
比较/写入/扩大 dirty 辅助函数。按 owner 后续要求删除中间行缓冲，
不分配整帧、不增加堆对象、线程、资源生命周期或公共接口。
surface_valid 替代 graphics_valid：首次和 surface 重建全量失效；同尺寸
模式切换按真实 RGB 差分，不因为模式名不同强制全屏刷新。

文本光标仍由原 native paint 反色叠加，不写入底图。消费前后计算实际可见
光标矩形；位置/形状/可见性改变时失效旧、新区域，防止光标残影。
原 blink 定时、resize/freeze/OS paint 和统一失败出口不改。失效矩形仍由
原 dirty 映射转换至客户区，系统累计到 WM_PAINT 后以其 clip 绘制。

代价：文本新增 RGB 比较；两种解码都直接比较，均保持 O(像素数)。
收益：相同文本零帧重绘，局部文字/字体/颜色变化只失效实际像素区域。
不声称普遍加速或消灭所有闪烁；不把 CPU/设备或窗口几何故障混作 dirty。

估算生产三文件 +60--90/-65--90；两个既有测试 +90--140/-25--45；
manifest/文档/EXE 另列。有限验收：首次/重建、重复帧、文本单格、字体/
palette-only、最新完整帧跳帧、同尺寸文本图形互换、光标移动/隐藏/闪烁、
S6 完整像素覆盖；所有边界必须由真实 renderer/native mock 证明。
双宽度构建和后台全回归后提交推送，等待 owner 测试；不开始 S8。

S7 P1 实际生产三文件 +49/-45（净 +4），测试三文件 +97/-27（净 +70）；
多出的测试文件是产品 runtime-cursor 对内部渲染入口的必要迁移（+3/-1）。
双宽度后台各105/105通过。实际证据和首次构建修正见
[S7 记录](../etc/evidence/softpc/m9-t72-s7-window-pixel-damage.md)。

Owner 要求去掉行缓冲后，P2 仅 render.c +26/-34（净 -8），测试不变。
S7 累计生产 +58/-62（净 -4）；双宽度后台重新各105/105通过。
光标旧、新区域补刷不变；等待 owner 测试，不开始 S8。
