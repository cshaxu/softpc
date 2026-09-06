# M9 T39：NXVM 共享 Lib 准入审计

## 结论

未准入。候选范围是 NXVM 固定 revision
`9ef5e899a8e540cf9def51c9e70ae3397586477c` 的完整 `src/lib/`；SoftPC
只能逐字节导入，不得在 `src/lib/` 内产生本地改动。

共同库只承载两个产品都需要、且不含产品或机器语义的能力。SoftPC
`src/mvdm/softpc.new/` 不得 include、调用、持有或适配 lib 类型；它是
不可变的原始机器边界。所有 SoftPC 接入只能发生在 `src/host/` 或
`src/app/`。

## 当前缺口

`src/lib/MANIFEST.sha256` 未覆盖完整导入树。它遗漏
`host/clock.c`、`host/clock.h`、`host/clock_internal.h`、
`host/linux/clock.c` 与 `host/win32/clock.c`；因此当前不能证明完整、
可复现的原样导入。

## 组件处置

| 组件 | 处置 | NXVM 需要收口的通用能力 |
| --- | --- | --- |
| `base` | 保留 | 仅固定宽度类型、状态和原子词汇；禁止产品/机器依赖。 |
| `ux` | 保留 | 明确 copied-frame、mailbox 并发/wake、输入、action、native presenter 和可配置 router 契约。不得加入 DIB、热键含义、machine 或 guest 指针。 |
| `host` | 保留 | 补足 wait-any、event reset 类型、cancel/signal/timeout 竞态和 Windows/Linux 一致性。不得加入 guest time、timer tick 或 device callback。 |
| `session` | 保留 | lifecycle 仅经 product callback 驱动；safe point 由产品确认，lib 不得拥有 CPU、machine thread 或 guest/device 语义。 |
| `storage` | 保留 | image lease、safe-point replacement 和 commit failure 语义；不得加入 CHS、FDC、BIOS、INI 或媒体策略。 |
| `observability` | 保留但保持小 | 只发布 bounded copied outcome；只有存在两个真实 consumer 时才增加通用 diagnostic event。 |

SoftPC 的原始 host ABI、CCPU/C-VID、BIOS、ROM、controller、renderer、
device media 以及产品 monitor 不是共同库候选，应各自留在项目外层。

## 再次准入条件

1. 完整 manifest、固定 revision 和独立验证 target；
2. 可独立消费的 lib build entry，不能要求 consumer 复刻 NXVM 根 CMake；
3. 每个保留组件有两个无产品语义的 consumer/conformance 证明；
4. lib 全树不含 `softpc`、`mvdm`、`ccpu`、`cvid`、`bios`、`rom`、
   `guest`、`controller`、`profile` 或 VM 产品语义；
5. Windows 与 Linux 的 lib-only 构建及测试证据；
6. SoftPC 接入方案证明 `src/mvdm/softpc.new/` 的内容和编译输入均不变。

达到上述条件后，SoftPC 才可重新进行原样导入审批。
