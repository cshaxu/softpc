# NXVM `src/lib` 共同能力范围反馈

## 结论

本反馈审计 NXVM revision `ecc88d35435c5c46773c132040669c14f3e8ade7`
的完整 `src/lib`。共同库的准则收紧为：能力必须是两个项目现在都实际
需要的，或是 owner 已明确要求 SoftPC 近期采用的共同能力。后者只包括
Linux UX/host 能力、零初始化可写媒体和 overlay 的单文件落盘；不能借此
保留 NXVM debugger、FDD sidecar 或单步控制。

因此当前 corpus **尚不可作为最终的原样导入 revision**。NXVM 先按本报告
收紧接口和本地化产品功能；随后 SoftPC 才导入新的完整 corpus，且导入后
`src/lib/` 必须保持逐字节相同。SoftPC 的所有绑定仍只能位于 `src/app/` 或
`src/host/`，`src/mvdm/softpc.new/` 不得 include、调用、持有或适配 lib 类型。

Windows 是当前准入平台；Linux 功能保留为双方的共同目标，但 SoftPC 的
Linux configure/build/run 证明另立后续任务，不阻塞本次 Windows 导入准备。

## 已决定保留

- `base`、Windows UX、Windows `host`、copied outcome 继续保留。它们是两边
  当前的共同宿主能力，且没有产品或 MVDM 语义。
- `ux/linux/**` 必须保留在 lib。SoftPC 目前未构建 Linux，不代表 Linux
  presentation、terminal input、mailbox wake 或 host wait 只属于 NXVM；owner
  已要求 SoftPC 达到相同级别。Linux 的运行验收延后，不应通过把实现移回
  NXVM 来规避共同能力。
- `lib_storage_file_read_owned()` 保留。NXVM 已用于镜像、catalog、BYOB 和
  session 输入；SoftPC 现有 overlay 也手写了“文件读入拥有内存”的同类机制。
- `lib_storage_image_create_overlay()`、字节 lease replacement、只读字节访问和
  可写 overlay 字节访问保留。SoftPC 当前 `SOFTPC_MEDIA_OVERLAY` 与 NXVM 的
  非只读镜像都需要私有可写副本。
- `lib_storage_image_create_zero_overlay()` 保留，但应重命名为能准确表达
  `create_zeroed_overlay` 的名称，并在 API 注释中说明：它只创建全零的**内存**
  overlay，不创建文件。要创建一份零初始化镜像文件，调用方随后用
  `lib_storage_image_commit()` 写入一个路径。SoftPC 后续媒体创建入口应采用
  这条组合能力。
- 单文件原子落盘能力和 `lib_storage_image_commit()` 保留。后者目前没有生产
  调用，但 owner 明确要求保留，作为 overlay 的受控保存能力；失败不得改变
  byte lease。应以它作为公共 API，原始 bytes-to-path helper 可降为 storage
  内部实现。
- session 仅保留 start/stop/fault/reset/pause/resume、paused/active/reset 查询
  及其线程安全状态。执行线程、safe point、机器回调和产品 action 始终留在
  两个项目各自的外层。

## 必须移出或删除

| 项目 | 实测事实 | 对 NXVM 的要求 |
| --- | --- | --- |
| session 单步：`request_step`、`take_step`、`step_requested` | 仅 NXVM 的 `vm/composition/session/{control,runner}.c` 使用；SoftPC 没有单步产品能力。它只是一个原子 flag 和三个 accessors，迁移成本低。 | 从 `lib/session/state.*` 删除。把 flag 与读取/消费逻辑置于 NXVM session/control，由 NXVM runner 使用。 |
| session `flip` | 仅 NXVM `vm/composition/session/control.c` 读取；它是在 start 时翻转的产品 session generation，不是 lifecycle 本体。 | 移入 NXVM control。 |
| pause reason | `request_pause(state, reason)` 与 `pause_reason()` 只服务 NXVM 的 `vm_session_pause_reason`；SoftPC 当前没有相同的数值 reason 合同。 | 共同状态改为无 reason 的 pause request；reason 由 NXVM control 本地保存。 |
| `lib_storage_image_take_direct_writable()` | 无 NXVM 生产调用，只有 lib 测试/neutral consumer。它接管一块 malloc 内存，绝不是文件直写。 | 删除。 |
| `lib_storage_file_reader_{open,next,close}()` | 无 NXVM 生产调用，SoftPC 也无对应需求。 | 删除，不以测试维持公共 API。 |
| `lib_storage_file_writer_{open,write,close}()` | 仅 NXVM `vm/machine/debug.c` 使用。它服务 `record start/stop` 命令，在每条指令时写寄存器、反汇编和内存访问记录。 | 移入 NXVM debugger/debug 层。 |
| `lib_storage_commit_pair_atomically()` | 仅 NXVM FDD remove 使用；它同时保存 raw image 与 `.json` address-mark sidecar。 | 从 lib 删除，作为 NXVM FDD 持久化策略的一部分本地实现。 |
| `lib_storage_image_mode_of()`、`lib_storage_image_discard()` | 仅测试使用；前者暴露实现模式，后者只是 destroy 后置空。 | 从公共 API 删除；测试改验可观察行为，调用方使用 `destroy()`。 |

## 文件镜像模型：必须先澄清的差异

当前 NXVM 的 `lib_storage_image` 是**内存 byte lease**，没有文件句柄：

| NXVM API/模式 | 实际语义 |
| --- | --- |
| `take_direct_readonly(bytes)` | 接管已由 `read_owned()` 读入的 heap buffer；不复制、不可写、不是“直接读文件”。NXVM FDD/HDD 使用它。 |
| `take_direct_writable(bytes)` | 接管 caller 的可写 heap buffer；没有生产调用，应删除。 |
| `create_overlay(bytes)` | 复制输入 bytes，形成私有可写 heap buffer。 |
| `create_zero_overlay(count)` | `calloc` 一块全零私有可写 heap buffer；不创建磁盘文件。 |
| `image_commit(image, path)` | 把可写 image 的当前 bytes 以单文件原子替换写到 path；不改变内存 image。 |

SoftPC 当前 `SOFTPC_MEDIA_DIRECT` 则是 `fopen(path, "rb+")` 后按 I/O 直接
`fread`/`fwrite`/`fflush`；`SOFTPC_MEDIA_READONLY` 是 `fopen(path, "rb")` 后
按 I/O 读取；只有 `SOFTPC_MEDIA_OVERLAY` 才把完整文件读入私有内存。故两边
的 “direct/readonly” 名称目前不表示相同实现。

结论如下：

1. 不得把 `take_direct_writable()` 错当成 SoftPC 文件 direct 模式；删除它。
2. `take_direct_readonly()` 也不应继续以含混的 `direct` 命名保留。若 NXVM
   必须保留其已加载只读 byte lease，应改名为 `take_owned_readonly_bytes()`；
   但在“两个项目现在都调用同一 API”的严格标准下，它仍是 NXVM 本地能力，
   应移到 NXVM，直到 SoftPC 确实采用不可变内存镜像为止。
3. SoftPC 当前 direct/readonly 文件路径留在 SoftPC host；NXVM 当前也没有
   同类 file-backed lease，因此不得为了统一名称向 lib 新增文件句柄镜像 API。
   将来双方都实际需要时，再以独立任务抽取。

## 关于 FDD 成对提交

NXVM 的 pair commit 不是普通“写两个不相关文件”：FDD `.json` sidecar 保存
address marks、raw size、checksum 和 geometry，且 checksum 对应 raw image。
当前实现会 stage 两个临时文件、备份旧文件、替换两份新文件，失败时 rollback。
这解释了它为什么存在，也证明它是 NXVM FDD 私有持久化合同，而非共享存储
机制。

公共库应删除该 API。但 NXVM 不应无意中把它替换为“先写一个、另一个失败只
打印错误”的静默不一致状态；NXVM FDD 自己决定并记录下列之一：保留本地
rollback helper，或采用明确的顺序提交、错误报告及下次打开时的恢复/拒绝策略。
这项选择不应进入 lib。

## 关于顺序文本 writer

NXVM writer 是长寿命、覆盖创建的 recorder：`record start <path>` 打开文件，
每个指令观测写入寄存器/反汇编/内存访问，`record stop` 再关闭。它属于 NXVM
debugger 的诊断产品功能。

SoftPC 的 `app/prompt_trace.c` 则每次 trace 调用都以 append 打开、写一行并
关闭，且路径和启用策略是 SoftPC 自己的诊断合同。二者不能因都写文本就被当作
同一共同能力；NXVM writer 应本地化，SoftPC trace 不应为复用它而改语义。

## 额外收紧：公共文件 API

`file_write_exclusive`、`file_replace`、`file_remove` 只被 storage commit
实现使用；`file_exists` 除 commit 外只被 NXVM FDD sidecar 使用。pair helper
本地化后，这些不是跨产品的公共能力。应将单文件 commit 所需的 primitives
变为 `storage/internal`，FDD sidecar 所需的 exists/replace/remove 则随 FDD
本地化，避免以低层文件操作扩大共同 ABI。

## NXVM 完成条件

1. 完成上表的迁移/删除，并更新 `MANIFEST.sha256`、standalone CMake、tests
   与 Windows lib-only CI。
2. 保留 Linux UX/host；不要求本轮提供 SoftPC Linux build proof。
3. 把共同 storage 公共面收敛为：owned file bytes、copy/zeroed overlay、lease
   replacement、只读/可写 bytes、单文件 image commit，以及实现这些所必需但
   不公开的内部 primitives。
4. 记录新的 fixed revision。SoftPC 将对该 revision 做最后一次 unchanged-import
   审计，之后才开始完整替换本项目 `src/lib/`。
