# T99 S40：110 个混合差异文件的保留代码审计

## 基线、范围和结论

SoftPC `f7537b1c`；XP SP1 `a2f6d7c93aa4f11efc51147566d5961c59a8ef89`。
只读取固定 XP 对象，没有读取 OpenNT 源码。全集是 S39 清单的 110 个 REVIEW
文件，不包括另外 365 个已分类文件。逐文件结果及当前 LF 归一化哈希见
[110 行台账](m9-t99-s40-retained-differences.tsv)。

本轮找到 **3 个精简点和 2 个检查条件缺口，合计 4 个文件**。其余 106 个文件
未找到有依据的进一步删除方案：有的直接复用已审查修复，有的保留必要的
状态、ABI 和宿主接线。这个数字不是“106 个文件重新逐行证明绝对最小”，
也不是“所有运行边界已经验证”。本轮是源码必要性审计，没有运行新的动态测试。

当前只交付文档，生产、测试、EXE、配置、媒体与快照均未改变。

## 覆盖方法与证据接收方

先对同路径文件提取条件结构，在同一父节点下配对条件组；已用 `#error`
替换的分支整体遮蔽，不进入其内部。忽略诊断 `#line`、注释和空白，只用于
寻找保留差异；不以这种归一化证明编译或运行等价。已审查的生成 ABI、
已验收的语义修复只登记接收方，不再次展开审计。

递归配对遮蔽 1915 个对应排除体。另有 11 个本地新增条件臂没有 XP 对应节点：
`cpu_vid.h`、`egacpu.h`、`gmi.h` 的已排除 ABI 备选；`ppi.c` 的旧宿主变量；
`ica.c` 的 IRET 及两处快照备选；`vga_mode.c` 的模式备选；`cfpu_def.h`、
`host_cpu.h` 的字序备选；`nt_graph.c` 的原生全屏检查。这些排除体仍不审计，
不把自动比较残留误计为待恢复功能。条件外的保留差异另行处理。

台账的证据组不是按目录宣判必要，而是复用以下具体职责证明，并检查残余
状态映射、声明、真实消费者及恢复顺序：

- **A / ABI 与 CPU**：[XP 语义审计](xp-sp1-hunk-semantic-audit-zh.md) 的
  34 文件与 C-VID 小节；[当前差异登记](pristine-divergence-current.md)；
  S36/S37/S38 的既有读取、权限和 GDP 修复。声明对应真实 provider；
  `c-files` 仍被根 CMake 和边界门禁使用；延迟 NPX 状态由 FPU 与异常路径共享。
- **D / 设备状态**：[T63 快照实施](../../../history/M9-T63-machine-snapshots.md)
  加当前 `compat/devices/archive.c` 的实际调用链。补查各设备的回调编号、
  指针转索引、有效范围、字段复制及恢复顺序；接收测试为
  `test/app-softpc/unit/machine/checkpoint_smoke.c`、`device_contract_smoke.c`
  及 `test/app-softpc/integration/snapshot_transaction_smoke.c`。
- **V / 视频**：复用当前差异登记中 S14/S15 的光标/dirty、S33 的 CHAIN2
  布局与 BIOS 文本 aperture、S37 的虚拟 CRTC。控制器快照沿既有寄存器写入
  路径恢复，先解除保护，再恢复保护和索引；不是第二套寄存器实现。
- **S / 音频**：[T93 S6](../../../history/M9-T93-S6-xp-dsp-pcm-mixer.md)、
  [S8](../../../history/M9-T93-S8-xp-midi-fm-attachment.md)、
  [S15](../../../history/M9-T93-S15-portable-sound-archive.md) 的已验收职责；
  补查现存 Mixer 声明、FM 状态及宿主调用。原生 worker/WinMM 所有权已由
  Compat/Lib 承接，不能恢复旧线程来减少镜像 diff。

每行给出保留内容或候选。既有测试是接收位置与历史证明，不冒充本轮重新运行。
完整性限于冻结的 110 文件及保留差异归类，不承诺原版全部代码没有其他 bug。

## 可直接精简的三个点

### C1：硬盘命令的双向映射只维护一份

位置：`base/disks/fdisk.c:1924–1958`。
`snapshot_encode_active_command()` 的 if 链和 decode 的 switch 重复维护同一
组 0–9 映射，分别由 HDD capture/restore 使用。改为文件内只读函数指针表：

```c
LOCAL void (*const snapshot_active_commands[])() = {
    NULL, restore, seek, rsector, wsector, format,
    rverify, diagnose, setparams, bad
};
```

encode 顺序查找，decode 检查范围后索引；两者保留原有入口及失败值。
必须保持 0=NULL、1=restore … 9=bad，未知函数返回 -1，非法编号返回 NULL。
数组长度是唯一范围来源，循环/边界类型应避免新增有符号比较警告。
这只是本地快照编号表，不是新设备派发器、公共注册框架或第二条执行路径。

同风格草案把原 36 行区块缩为约 26 行，**净减约 10 行**。实施时在已有
checkpoint 夹具补 0–9 编号往返及非法编号检查；当前测试已有 HDD 往返，
但不能当作完整编号矩阵已覆盖。不能重新编号来追求更短代码。

同类扫描：FDC 一项、串口/并口/键盘两项的回调映射不值得强行改表。
PIT 的 wait-complete 与 gate-fall 允许集合不同；共用无约束 decoder 会减弱
恢复校验，故保留。不存在将所有设备抽象为统一 registry 的建议。

### C2：删除 Mixer 的三个重复声明

`host/src/nt_sbmxr.c:20–22` 已包含 `nt_sb.h`，后者已声明
`MixerSetMasterVolume`、`MixerSetVoiceVolume`、`MixerSetMidiVolume`。
删除 implementation 内三行同签名声明即可，**净减 3 行**。函数定义、
消费者和公共 ABI 均不变，不删除其他必要的 static 前置声明。

相对 XP：原来的三行 BYTE 声明已被本地 IU8 声明替换；删本地声明会减少
本地新增三行，但原版三行的删除仍计入镜像 diff，不宣称总 diff 减六行。

### C3：删除恒等三元表达式

`base/system/quick_ev.c:1144`：

```c
/* 当前 */
ptr->next = i + 1 == count ? NULL : NULL;
/* 建议 */
ptr->next = NULL;
```

后续迭代通过前一节点连接当前节点，末节点本来就应为 NULL。净行数不变，
去掉没有语义的条件；保留原恢复顺序和不执行回调的契约。

## 相邻两处条件缺口：源码确定，客户机触发未证明

### F1：链表查找的 NULL 检查晚于解引用

`base/system/quick_ev.c:1041` 在 hash 指向的节点不属于 head 链时，查找会先
把 ptr 走到 NULL，下一轮执行 `ptr->next`，到不了后面的 `if (ptr == NULL)`。
最小修法是在原循环条件加入 `ptr != NULL &&`，沿用既有 FALSE 返回。
不新增恢复逻辑、队列或重试。净行数可保持不变。

这是内部 hash/list 不一致时的错误处理缺口。尚未证明正常产品操作能造成该
不一致；不能宣传为已复现的快照崩溃。实施回归应在同组件受控夹具构造该
不一致并断言失败，不能新增测试专用生产 API。

### F2：键盘快照检查不可能成立

`base/keymouse/keyba.c:2391`：索引函数只能返回 -1 或 0–133；检查 `< -1`
永远为假。活动 anomalous 状态若未找到对应数组，capture 会放过 -1，
而 restore 已明确拒绝活动状态的负索引。

建议只把条件改成：

```c
if (in_anomalous_state && state->anomalous_index < 0) return FALSE;
```

保留非活动状态用 -1 的正常表示。净行数不变。不应仅删检查：它原本意图是
阻止生成无法恢复的活动状态。正常键盘路径是否能产生未知指针未获动态证明；
实施测试应区分非活动 -1、合法活动索引及非法活动指针，不增加新状态机。

## 明确不建议做的“减行”

- 用整结构 memcpy 取代设备快照：其中有本机指针、回调、位域、句柄和稳定
  编码，结构尺寸相似不等于传输语义相同。
- 重建 quick-event 时逐项调用普通入队：可能执行零延迟回调，改变顺序和
  hash/handle 身份；现有恢复入口有独立而必要的用途。
- 合并 DOS mouse 的 point/vector helper：它们对应不同原生类型；为省几行
  引入类型擦除或宏不比现状干净。
- 删除 ROM 第二次 strlen：前面可能写入截断终止符，重新求长度有必要。
- 删除 guest FM 状态：Lib 没有 FM 输出，不等于客户机不探测 AdLib 寄存器。
- 恢复零偏移 CHAIN2 调用为通用指针：即时数值相同，但会破坏统一的存储
  表达，且已有明确用户决策要求保留。
- 为减少 diff 恢复 NTVDM 原生线程、原生全屏 UI 或第二套输入反向转换。
- 修改、展开或重复审计任何已排除的 #error 分支。

## 建议实施边界与验证

如批准，五点可在一个小 P 内处理：4 个生产文件，生产净减约 **13 行**，
无新公共接口、状态、线程、特判或依赖。精确 +/- 必须按实际补丁重新统计；
测试预计只扩展已有夹具，不能先承诺测试净减少。不要把本轮估计当作已实施。

验证包括双宽度编译、原有快照/设备回归、映射编号与两项失败条件的定向检查，
再按实施任务要求发布并运行规定回归。本轮只读，无需重建或刷新 EXE。

## 自审与限制

复核发现：初版通用条件配对可能把外层 else 配到内层 else，已改为父节点下
递归配对；11 个本地新增排除臂单独处置，未依据其误差提出代码回滚。
历史审计曾把 ntstubs 的重复空函数描述成遮蔽非空实现；本报告采用当前差异
登记的更正：原函数均为空，移除是解决同时编译的重复定义。

本轮没有证据支持大规模回滚。必要职责不等于每行都是唯一最短写法；上述
候选优先减少重复权威和无意义条件，避免仅把镜像代码搬到 Compat 使数字好看。

交付核验：110 个路径唯一且与固定 REVIEW 全集一致；逐文件当前哈希一致。
文档治理门禁及 git diff --check 通过；对 f7537b1c 的 src/test/assets 差异为空。
没有重跑构建或动态测试。临时比较数据在核验后按任务清理规范删除。
