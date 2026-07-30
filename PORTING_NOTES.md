# TB371FC 驱动移植交付说明（任务 2 产出）

> 本目录是 `tb371fc_kernel_sukisu` 仓库**待提交**的内容。请在确认无误后执行任务 1（开 PR / 提交）。

## 一、做了什么

针对原厂固件 dtbo #07（真·TB371FC，代号 spinel）里实际使用、但 paladin 内核未接好的硬件驱动，产出可合入的源码与配置：

| 硬件                                 | 驱动来源                                                         | 处理方式                                      | 状态                                                                             |
| ---------------------------------- | ------------------------------------------------------------ | ----------------------------------------- | ------------------------------------------------------------------------------ |
| 触摸 Novatek NVT-ts (SPI+笔)          | **paladin 源码已自带** `drivers/input/touchscreen/nt36xxx/`       | 仅补 compatible + 开配置 + 补 DTS 节点            | ✅ 已接线（paladin 的 Makefile/Kconfig 早已 source 该驱动，之前误判为"缺失"是用了错误关键字 `novatek` 搜索） |
| 背光 KTZ8866 (双片 a/b)                | 从 `PocoF3Releases/kernel_xiaomi_sm8250` (4.19.325，API 兼容) 提取 | 源码 vendored + 接线 + compatible 修复 + DTS 节点 | ✅ 已提取                                                                          |
| 面板 Novatek NT36532 3K (BOE/Tianma) | **无需专用 .c 驱动**                                               | QCOM 通用 DSI 面板框架靠 DTS 命令点亮；节点已从 dtbo 抽出   | ✅ DTS 已抽                                                                       |

## 二、两个"必然不工作"的 compatible 不匹配（已修复）

固件 DTS 的 compatible 带后缀，而驱动只认基础名，导致驱动**不会绑定**：

1. **触摸**：驱动 `of_match` 仅 `"novatek,NVT-ts"`，固件是 `"novatek,NVT-ts-spi"`  
   → 补丁 `patches/nt36xxx_compatible.patch` 在匹配表加 `"novatek,NVT-ts-spi"`。
2. **背光**：驱动仅 `"ktz,ktz8866"`，固件是 `"ktz,ktz8866a"` / `"ktz,ktz8866b"`  
   → 已直接改 `ktz8866.c` 的 `of_match_table`，加 a/b 两项。

> 属性名已核对一致：ktz8866.c 解析的 `ktz8866,hwen/enp/enn-gpio` 与固件节点完全对应。

## 三、工作流改动（`.github/workflows/build_tb371fc_sukisu.yml`）

在 EROFS 步骤之后、编译之前新增 **"Import TB371FC vendor drivers"** 步骤：

- 拷贝 `ktz8866.c` + `ktz8866.h` 进 paladin，并接线 Makefile/Kconfig；
- `git apply` 触摸 compatible 补丁；
- 拷贝 `tb371fc.dtsi` 到 paladin 的 `arch/arm64/boot/dts/vendor/qcom/`；
- 同时保留了上一轮已修的 **EROFS staging 冲突** 与 **SukiSU/KSU 显式启用** 两处修复；
- push 触发路径新增 `vendor_drivers/`、`patches/`、`port/`。

## 四、⚠️ 需你在 CI 构建中确认的（本地无法编译验证）

1. **ktz8866.c 在 4.19.157 (paladin) 上的编译**：源码取自 4.19.325，backlight/I2C API 一致，但需在 Actions 里实跑确认无报错。
2. **nt36xxx 补丁 apply 成功率**：用 `git apply --ignore-whitespace`，失败自动降级 `patch -p1`，仍失败仅告警（说明 paladin 可能已含该 compatible）。
3. **设备树节点的真正生效路径**：
   - 若刷机时**沿用原厂 dtbo**（dtbo #07），则触摸/背光/面板节点由 dtbo 提供，内核只需驱动+compatible 即可——`tb371fc.dtsi` 仅作参考/自带 dtb 用；
   - 若要内核**自带完整 dtb**，需一个 `#include "tb371fc_driver_nodes.dtsi"` 的 board dts（`paladin` 当前**没有 TB371FC 的 board dts**，这是下一步工作，不在本轮 scope）。

## 五、提交前请确认

- [x] `ktz8866.c` 来源合规（PocoF3 SM8250 内核，GPLv2，与 paladin 同许可）
- [x] 是否沿用原厂 dtbo（推荐，省去自写 board dts）
- [x] 确认无误后由我执行任务 1：开 PR / 提交到 `tb371fc_kernel_sukisu`
