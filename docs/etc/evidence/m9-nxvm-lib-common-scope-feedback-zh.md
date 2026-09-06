# NXVM `src/lib` 反馈（简版）

当前 NXVM `src/lib` 还不能作为 SoftPC 的最终原样导入 revision；先按以下边界
收紧。之后 SoftPC 才整树逐字节导入，且所有绑定都在 `src/app/` 或 `src/host/`，
绝不触及 `src/mvdm/softpc.new/`。

## 原则

lib 是两个产品共用的**中立平台底座**：文件、窗口/console、线程、同步、时钟、
复制值和 byte lease。NXVM 与 SoftPC 的产品层都不直接使用宿主 SDK。

debugger、FDD sidecar 格式、单步、执行策略、CHS/FDC/BIOS、媒体策略和错误呈现
是产品/机器语义，留在各自项目。

## 保留在 lib

- `base`、Windows/Linux UX、host clock/sync、outcome；Linux 是 SoftPC 的后续
  共同能力，本轮不要求 Linux 运行验收。
- 平台文件能力：owned read、随机读写/flush/close、append/truncate stream、
  exists/replace/remove、单文件原子提交。产品只提供路径/内容/错误策略。
- storage byte lease：owned-readonly bytes（改名，不能叫 direct）、copy/zeroed
  overlay、lease replacement、`image_commit()`。

`create_zero_overlay()` 只创建全零内存 overlay，不创建文件；要创建镜像文件须再
调用 `image_commit()`。SoftPC 应在后续媒体创建入口采用该组合。

SoftPC 当前 direct/readonly 是 `FILE *` 随机 I/O；NXVM 的 direct-readonly 则是
已加载内存。两者不是同一实现。lib 应提供中立 opaque file API 供 SoftPC 使用，
并把 NXVM 的内存 API 改名为 `take_owned_readonly_bytes()`。

## 移出或删除

- NXVM session/control：单步、`flip`、pause reason；共同 session 只保留通用
  lifecycle state。
- NXVM FDD：raw image + JSON address-mark sidecar 的成对提交/rollback 策略；它可
  调 lib 文件 primitives，但不能作为 lib 的 pair-commit API。
- 删除：无生产调用的 `take_direct_writable`、逐行 file reader、测试专用
  `image_mode_of` 与 `discard`。

NXVM debugger 的记录格式、开始/停止和错误提示留在 NXVM；它通过 lib 的通用
文件 stream 写入。SoftPC trace 同样使用该 stream 的 append mode，但保留自身路径
和诊断语义。

## NXVM 收口

完成上述迁移/删除、更新 manifest/CMake/tests，并通过 Windows lib-only CI 后，
固定新 revision 供 SoftPC 做最后一次 unchanged-import 审计。
