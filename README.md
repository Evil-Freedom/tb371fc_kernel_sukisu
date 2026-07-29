# TB371FC 内核文档

## 设备信息
- 设备: 联想小新Pad Pro 12.7 (TB371FC)
- SoC: 高通骁龙870 (SM8250-AC, Kona)
- 内核: Linux 4.19.157-perf+
- 基础源码: lss4/android_kernel_lenovo_paladin (TB-Q706F)

## 集成功能

### SukiSU-Ultra (Root)
- 基于 KernelSU 的内核级 Root
- 支持 non-GKI 4.19 (kprobe hook)
- 支持 KPM 模块系统
- 配合 Shamiko 可过环境检测

### EROFS 文件系统
- 从 lateautumn233/erofs_kernel_4_19 backport
- 支持 LZ4 压缩、xattr、POSIX ACL、SELinux 标签
- 可挂载 EROFS 格式的 system/vendor 分区

### 已支持的硬件
- 音频: AS33970 + ES7210 Codec
- 触摸: ST FTS + Goodix 指纹
- 传感器: SSC + Hall
- 背光: QCOM SPMI WLED
- WiFi/BT: QCA6390
- 电池: BQ27xxx + SMB1398

### Android 兼容性
- ✅ ColorOS 15.0.1.801 Final
- ✅ ZUI 16.0.474 底层
- ⚠️ HyperOS 3 (需要适配)
- ⚠️ Android 16 (需要框架层补丁)

## Android 16 支持说明

### 内核层面: ✅ 已支持
4.19 内核已具备 Android 16 所需的所有内核功能:
- eBPF (BPF_SYSCALL + JIT)
- Cgroups v2
- Namespaces (UTS/IPC/PID/NET)
- Seccomp
- FUSE + OverlayFS
- EROFS (已 backport)

### 框架层面: ⚠️ 需要补丁
Android 16 QPR2 强制要求完整的 eBPF 支持, 4.19 内核的 eBPF 不够完整, 会导致卡开机。

**解决方案: fuck-bpf 补丁**

项目地址: https://github.com/techyminati/fuck-bpf

该项目提供 34 个 AOSP 框架层补丁, 回退 Android 16 QPR2 中对 eBPF 的强制要求, 让 4.19 内核可以正常启动 Android 16。

已验证:
- ✅ Kernel 4.19 + Android 16 QPR2 → 正常开机
- ✅ Kernel 4.14 + Android 16 QPR2 → 正常开机

使用方法:
1. 在 ROM 源码中应用 fuck-bpf 补丁
2. 刷入本内核
3. 正常开机

### 音频 Codec 说明
paladin 内核源码不包含 WCD938x 和 WSA881X 驱动。TB371FC 的 dtbo.img 中引用了这些 codec, 但实际驱动需要从 Qualcomm BSP 内核移植。

当前使用: AS33970 + ES7210 (paladin 已有)

如需完整音频支持, 需要:
1. 从 Qualcomm SM8250 BSP 内核获取 WCD938x/WSA881X 驱动源码
2. 移植到 paladin 内核树中
3. 在 defconfig 中启用

## 已知限制

| 项目 | 状态 | 原因 |
|------|------|------|
| 小布语音唤醒 | ❌ | 联想硬件阉割DSP |
| WCD938x 音频 | ⚠️ | 驱动不在 paladin 源码中, 需移植 |
| 四扬声器 | ⚠️ | 依赖 WSA881x 驱动 |
| eBPF 高级功能 | ⚠️ | 4.19 内核限制 |
| GKI 模块 | ❌ | non-GKI 内核 |

## 构建信息
- 编译器: Clang (AOSP) + aarch64-linux-gnu-
- 构建系统: GitHub Actions
- 打包工具: AnyKernel3
- 输出: TB371FC_kernel.zip (可 TWRP 刷入)
