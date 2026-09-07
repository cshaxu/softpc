# M9 T41：UX 调色板编码问题简报

## 现象

以 console 模式启动 SoftPC，Windows 3.1 从文字模式切入图形 splash
时，原应为蓝色的背景显示为黄色。

## 已确认的事实

- NXVM Core 的 `palette_rgb` 明确使用 `0x00RRGGBB`：红、绿、蓝分别在
  bit 16、8、0。导入的 `ux_frame` 是这份 RGB 值的共享表示。
- 共享 Win32 图形 presenter 将 `graphics_palette[pixel]` 直接写入
  32-bit `BI_RGB` DIB；这同样要求 `0x00RRGGBB`。
- SoftPC 的 `src/app/runtime.c` 当前从原始 `RGBQUAD` 取色时使用 Win32
  `RGB(r,g,b)` 宏。该宏产生 `COLORREF`，其数值布局为 `0x00BBGGRR`。
  因此 SoftPC 把 BGR 值交给期望 RGB 的图形 presenter，红蓝通道互换；
  蓝色 splash 随之成为黄色。
- 共享 presenter 的文字路径又把 `text_palette` 当作 `COLORREF` 后转换
  成 DIB 像素。这与其图形路径及 NXVM 的 `palette_rgb` 表示不一致；它是
  目前未由该 splash 暴露的 lib text-palette 缺陷。

## 归属

当前 splash 回归的直接根因属于 SoftPC 的 MVDM-to-lib 帧适配：导入后没有
把原有 `COLORREF` 外层表示迁移为共享 `RRGGBB` 表示。原始 MVDM 的 DIB、
调色板和任何 MVDM 文件均不应修改。

共享 lib 同时存在独立的文字调色板契约不一致；它应由 NXVM 修正，并由
SoftPC 以完整、原样的 lib 重导入取得，不能在 SoftPC `src/lib/` 中修补。

## 修复建议

1. **SoftPC（本任务）**：在 `src/app/runtime.c` 的 graphics 和 text
   palette 复制边界显式打包 `((r << 16) | (g << 8) | b)`，不再使用 Win32
   `RGB` 宏。为 palette adapter 增加断言式单元测试，至少验证纯蓝
   `RGBQUAD{b=255,g=0,r=0}` 变为 `0x000000ff`，纯红变为 `0x00ff0000`。
2. **NXVM lib**：以 `ux_frame` 的 `RRGGBB` 契约为唯一规范，令 Win32
   text presenter 像 graphics presenter 一样直接写入 DIB 像素；移除把
   `text_palette` 重解释成 `COLORREF` 的额外通道转换。补充 graphics/text
   各一条红、蓝、黄像素映射测试。
3. **回归验收**：NXVM 先提交上述 lib 修复和 manifest；SoftPC 重新完整
   导入并验证 byte-identical。随后在 x86/x64 package 上手动验证 Windows
   3.1 splash 蓝色，以及 text/graphics 两种 presenter 的红蓝黄映射。

在第 2 步之前，SoftPC 不应为了绕开 text 缺陷而让 graphics 和 text 使用
两种不同的 `ux_frame` palette 编码。
