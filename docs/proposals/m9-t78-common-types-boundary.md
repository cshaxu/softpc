# Common 类型边界收口

## 来源与目标

T78 由所有者在完成 Lib/Common/x86 代码质量审计后直接准入。审计没有发现
功能性或架构级缺陷，但确认两处可以一次性收口的类型边界不一致：

1. `common/session/control.c` 直接包含 C 标准库 `<limits.h>` 并使用
   `UINT_MAX`；Common 应只消费 Lib Types 已定义的类型及常量词汇。
2. Common Machine 和 UI 对外使用 `lib_u32 run_generation`，内部却把它存为
   `lib_atomic_i32`，以强制转换读写。高位被使用时该转换不再是明确的无符号
   环绕语义。

目标是在不改变产品行为、公开 Common ABI、队列/线程模型或 KVM 语义的前提下，
让 Common 仅使用 Types 词汇，并使 generation 的存储与公开语义一致。

## 最小设计

Types 是标准 C 整数限制和原子类型的唯一适配边界。因此：

- 在 `types_interface.h` 暴露 `LIB_UINT_MAX`，由它包装 `UINT_MAX`；Session
  删除直接 `<limits.h>` 依赖，继续使用原有 `unsigned int` 容量和溢出检查。
  不把队列容量改成 `lib_size`，避免无必要的队列布局、格式和测试范围变化。
- 在 `types/atomic.h` 补齐已有 `lib_atomic_u32` 的 load、store、exchange
  操作，Windows 与 C11 实现形状相同；不引入新原子类型、锁或内存序策略。
- Machine 和 UI 将 `run_generation` 的私有字段改为 `lib_atomic_u32`，使用
  原有 `SEQ_CST` 内存序并保留现有 fetch-add 环绕行为。删除每一处 `i32/u32`
  强制转换。

这不是行为重构：generation 仍从零递增，零的既有“未绑定/未过滤”含义、帧
归属检查、Session 过滤、UI 事件投递和并发关系均不变。

## 范围与非目标

生产范围仅限 `src/lib/types/`、`src/common/machine/`、`src/common/ui/` 和
`src/common/session/`；测试仅在 `test/lib/`、`test/common/`。不改 App、Core、
x86、快照、配置、媒体、公共 Common 函数签名或 Lib 组件依赖图。

不借此清零图形帧未使用的字符表、不改 frame locking、不限制 Session 动态队列，
也不重构 Types 的 runtime facade。这些均非本任务确认的问题。

## S1：类型词汇与无符号 generation 收口

在一个实现 S 内完成上述最小替换，补充 Types unsigned atomic 及 Common
generation 边界测试。扫查 `src/common` 的原始 C 标准头/整数上界引用和所有
generation 的 signed atomic 使用；每个命中要么迁移，要么记录其独立原因。

预计生产约 +18/-16 行、测试约 +20/-4 行，净增加约 18 行；实际以
`git diff --numstat` 分别报告生产和测试。该预测不包括 manifest、文档或 EXE。

完成后运行 Types/Lib、Common 的针对性测试、结构门禁及双宽度后台回归，重建
两份 EXE，提交并推送一个完整 P。S1 完成后等待所有者测试；T78 不因构建通过
自动收口。

## 有限验收台账

| 项目 | 证明 |
| --- | --- |
| Common 不直接包含整数限制头 | 静态扫描及 Common 依赖门禁。 |
| Types 统一包装边界 | `LIB_UINT_MAX` 和 u32 原子 API 的 Windows/C11 编译及 Types contract。 |
| 无符号 generation 一致性 | Common Machine/UI 使用 u32 原子；高位值、递增和事件值测试。 |
| 行为未漂移 | 既有 Session、Machine、UI 测试及 x64/x86 后台回归。 |
| 共享 corpus 完整性 | Lib/Common manifests、DAG/corpus 检查和干净工作区。 |

## S1 实际交付

实现严格保持在已准入范围内：Types 新增已有 `lib_atomic_u32` 家族缺少的
load/store/exchange；Machine/UI 的唯一 generation 字段改为 u32 atomic，
Session 用 `LIB_UINT_MAX` 完成原有容量上界检查。没有新增锁、队列、对象、
线程、状态字段或 Common 公共函数。

相似问题扫查结果：`src/common` 中仅有 `session/control.c` 的 `<limits.h>` /
`UINT_MAX` 命中，已迁移；所有 `run_generation` 的 signed atomic 字段、读写和
cast 仅在 Machine/UI，共 12 个命中，均已迁移。没有其他生产命中需要转入
TODO 或扩大任务。

实际 C/H 源码：生产 `+42/-16`，净 `+26`；测试 `+10/-1`，净 `+9`；合计
`+52/-17`，净 `+35`。四份 manifest、proposal、CURRENT 和两份 EXE 不计入
该代码统计。增加量来自 Windows/C11 两种 atomic 实现形状对齐以及无符号边界
证明；删除量来自 Common 的 signed 转换和直接标准库 include。

验证：两份 Release package 均重建成功。双位宽 Lib/Common 非 desktop 单元集
均为 `44/44`；完整后台回归拆为 `109/109`（排除唯一长启动/重启项）加该项
直接执行：x64 与 x86 均退出码 0。Lib/Common manifest、Common DAG/corpus、
Types layout、文档治理和 diff 检查均通过。x86 初次编译失败经确认是调用环境
漏掉 `D:/programs/msys64/mingw32/bin` 的 `PATH`；补齐同一既有工具链路径后，
重编译及测试全部通过，非源代码问题。

双 EXE SHA256：x86
`3B39B56551BA705003E2B7394C84AC9905A325E4DE9372BA56E8E6126663E929`；x64
`904EB3E8DF8BE7D740F5B068E115A437D071C69533F3029A55F5956BAD4B8874`。
S1 已具备实现交付条件，待提交推送与所有者测试；T78 保持开放，不能因
本节测试通过自动收口。
