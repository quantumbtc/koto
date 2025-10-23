# 线程数配置指南

## 📊 环境变量

### MINE_THREADS

设置挖矿使用的线程数。

**语法**:
```bash
MINE_THREADS=<线程数>
```

**范围**: 1 - 256

**默认值**: 自动检测CPU核心数

## 🎯 使用示例

### Windows PowerShell

```powershell
# 1. 使用4个线程
$env:MINE_GENESIS=1
$env:MINE_THREADS=4
.\src\kotod.exe

# 2. 一行命令
$env:MINE_GENESIS=1; $env:MINE_THREADS=8; .\src\kotod.exe

# 3. 使用单线程（调试）
$env:MINE_GENESIS=1; $env:MINE_THREADS=1; .\src\kotod.exe
```

### Linux/Mac

```bash
# 1. 使用4个线程
MINE_GENESIS=1 MINE_THREADS=4 ./src/kotod

# 2. 分步设置
export MINE_GENESIS=1
export MINE_THREADS=8
./src/kotod

# 3. 使用单线程
MINE_GENESIS=1 MINE_THREADS=1 ./src/kotod
```

## 💡 如何选择线程数

### 1. 根据CPU核心数

```bash
# 查看CPU核心数
# Linux/Mac
nproc
# 或
lscpu | grep "^CPU(s):"

# Windows PowerShell
(Get-WmiObject Win32_Processor).NumberOfLogicalProcessors
```

**推荐配置**:
- **2核CPU**: `MINE_THREADS=2`
- **4核CPU**: `MINE_THREADS=4`
- **8核CPU**: `MINE_THREADS=8`
- **16核CPU**: `MINE_THREADS=16`

### 2. 根据内存大小

每个线程需要约 **512 MB** 内存。

| 可用内存 | 推荐线程数 | 内存占用 |
|---------|-----------|---------|
| 2 GB | 2-3 | 1-1.5 GB |
| 4 GB | 4-6 | 2-3 GB |
| 8 GB | 8-12 | 4-6 GB |
| 16 GB | 16-24 | 8-12 GB |
| 32 GB+ | 32+ | 16+ GB |

**计算公式**:
```
最大线程数 = 可用内存(MB) / 512
```

示例：
```bash
# 4GB内存 → 最多8个线程
# 保留一些内存给系统，使用6个线程
MINE_THREADS=6
```

### 3. 根据CPU使用需求

如果需要同时运行其他程序：

```bash
# 保留一些CPU给其他程序
# 8核CPU，使用6个线程，保留2核
MINE_THREADS=6
```

## 🔧 性能调优

### 场景 1: 最快挖矿速度

```bash
# 使用所有CPU核心（默认）
MINE_GENESIS=1 ./src/kotod
```

### 场景 2: 内存受限

```bash
# 只有2GB内存，使用2个线程
MINE_GENESIS=1 MINE_THREADS=2 ./src/kotod
```

### 场景 3: 后台挖矿

```bash
# 使用一半CPU核心，不影响其他工作
# 8核CPU使用4个线程
MINE_GENESIS=1 MINE_THREADS=4 ./src/kotod
```

### 场景 4: 调试模式

```bash
# 单线程，方便调试
MINE_GENESIS=1 MINE_THREADS=1 ./src/kotod
```

## 📊 性能对比

### 示例：8核CPU，不同线程数的性能

| 线程数 | 算力 | 内存占用 | CPU占用 | 推荐场景 |
|-------|------|---------|---------|---------|
| 1 | 50 H/s | 512 MB | 12.5% | 调试 |
| 2 | 95 H/s | 1 GB | 25% | 低内存 |
| 4 | 180 H/s | 2 GB | 50% | 平衡 |
| 8 | 350 H/s | 4 GB | 100% | 最快 ⭐ |
| 16 | 340 H/s | 8 GB | 100% | 过度 ❌ |

**注意**: 线程数超过CPU核心数后性能反而下降！

## ⚠️ 注意事项

### 1. 不要设置过多线程

```bash
# ❌ 错误：4核CPU使用16线程
MINE_THREADS=16  # 性能反而降低

# ✅ 正确：4核CPU使用4线程
MINE_THREADS=4
```

### 2. 确保足够内存

```bash
# ❌ 错误：2GB内存使用8线程
MINE_THREADS=8  # 会导致系统卡死

# ✅ 正确：2GB内存使用3线程
MINE_THREADS=3  # 留一些内存给系统
```

### 3. 无效值会被忽略

```bash
# 无效值会回退到自动检测
MINE_THREADS=0      # 忽略，使用自动检测
MINE_THREADS=-1     # 忽略，使用自动检测
MINE_THREADS=1000   # 超过256，忽略
MINE_THREADS=abc    # 非数字，忽略
```

输出会显示警告：
```
Warning: Invalid MINE_THREADS value (0), using auto-detect
Auto-detected CPU cores: 8
```

## 🎯 实际示例

### 示例 1: 2核笔记本，2GB内存

```powershell
# Windows
$env:MINE_GENESIS=1
$env:MINE_THREADS=2
.\src\kotod.exe

# 预期：算力 ~80 H/s，内存占用 1GB
```

### 示例 2: 8核台式机，16GB内存

```bash
# Linux/Mac
MINE_GENESIS=1 MINE_THREADS=8 ./src/kotod

# 预期：算力 ~350 H/s，内存占用 4GB
```

### 示例 3: 服务器，32核，64GB内存

```bash
# 使用24个线程（留一些资源）
MINE_GENESIS=1 MINE_THREADS=24 ./src/kotod

# 预期：算力 ~1000 H/s，内存占用 12GB
```

### 示例 4: 虚拟机，4核，4GB内存

```bash
# 使用3个线程（留1核和内存给系统）
MINE_GENESIS=1 MINE_THREADS=3 ./src/kotod

# 预期：算力 ~130 H/s，内存占用 1.5GB
```

## 🔍 验证线程配置

程序启动后会显示实际使用的线程数：

```
# 指定了线程数
Using user-specified thread count: 4

Multi-threading configuration:
  Threads: 4
  Memory per thread: ~512 MB
  Total memory: ~2048 MB
```

或

```
# 自动检测
Auto-detected CPU cores: 8

Multi-threading configuration:
  Threads: 8
  Memory per thread: ~512 MB
  Total memory: ~4096 MB
```

## 💡 最佳实践

1. **首次使用**: 让程序自动检测
   ```bash
   MINE_GENESIS=1 ./src/kotod
   ```

2. **观察性能**: 查看内存和CPU占用

3. **调整优化**: 根据实际情况设置线程数
   ```bash
   MINE_GENESIS=1 MINE_THREADS=<优化值> ./src/kotod
   ```

4. **记录配置**: 将最佳配置写入脚本

## 📝 配置脚本示例

### Windows (mine.bat)

```batch
@echo off
set MINE_GENESIS=1
set MINE_THREADS=8
echo Starting genesis mining with %MINE_THREADS% threads...
src\kotod.exe
```

### Linux/Mac (mine.sh)

```bash
#!/bin/bash
export MINE_GENESIS=1
export MINE_THREADS=8
echo "Starting genesis mining with $MINE_THREADS threads..."
./src/kotod
```

使用：
```bash
chmod +x mine.sh
./mine.sh
```

## 🎉 总结

- ✅ 使用 `MINE_THREADS` 环境变量控制线程数
- ✅ 默认自动检测CPU核心数
- ✅ 有效范围: 1-256
- ✅ 推荐值: CPU核心数
- ✅ 考虑内存限制: 512MB/线程
- ✅ 无效值会回退到自动检测

根据您的硬件配置选择合适的线程数，获得最佳挖矿性能！🚀

