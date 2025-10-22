# 创世区块挖矿 - 使用指南

## 🎯 概述

现在可以通过**多种方式**启用创世区块挖矿，不再需要修改源代码！

## 📋 四种启用方式

### 方式 1: 环境变量 ✅ 推荐

**最简单、最灵活的方式**

#### Linux/Mac:
```bash
# 设置环境变量
export MINE_GENESIS=1

# 编译并运行
./zcutil/build.sh -j$(nproc)
./src/kotod

# 或者一行命令
MINE_GENESIS=1 ./src/kotod
```

#### Windows (PowerShell):
```powershell
# 设置环境变量
$env:MINE_GENESIS=1

# 运行
.\src\kotod.exe
```

#### Windows (CMD):
```cmd
# 设置环境变量
set MINE_GENESIS=1

# 运行
src\kotod.exe
```

---

### 方式 2: 命令行参数

**适合调试和测试**

```bash
# 编译
./zcutil/build.sh -j$(nproc)

# 运行时添加参数（需要 -help-debug 才能看到此选项）
./src/kotod -help-debug  # 查看帮助
./src/kotod -minegenesis  # 启用挖矿（功能待实现）
```

**注意**: 由于 chainparams 在参数解析前就初始化了，命令行参数方式需要额外的实现。目前建议使用环境变量方式。

---

### 方式 3: 编译时宏定义

**适合持久化配置**

```bash
# 方式 A: 使用 CPPFLAGS
make clean
./zcutil/build.sh CPPFLAGS="-DMINE_GENESIS_BLOCK=1" -j$(nproc)
./src/kotod

# 方式 B: 直接传给 configure
./configure CPPFLAGS="-DMINE_GENESIS_BLOCK=1"
make -j$(nproc)
./src/kotod
```

---

### 方式 4: 修改源代码（传统方式）

**不推荐，但仍然支持**

编辑 `src/chainparams.cpp`，在文件头部添加：

```cpp
#define MINE_GENESIS_BLOCK 1
```

---

## 🚀 完整示例

### 示例 1: 使用环境变量（Linux）

```bash
# 1. 修改创世区块参数
vim src/chainparams.cpp
# 修改第 64 行的时间戳文本
# 修改第 245-248 行的网络魔数
# 修改第 86 行的币种符号等

# 2. 编译
./zcutil/build.sh -j$(nproc)

# 3. 设置环境变量并运行
export MINE_GENESIS=1
./src/kotod

# 4. 等待输出...
# Mining genesis block with yespower algorithm...
# ...
# Genesis Block Found!

# 5. 复制输出的创世区块信息，更新到 chainparams.cpp

# 6. 取消环境变量
unset MINE_GENESIS

# 7. 重新编译
./zcutil/build.sh -j$(nproc)

# 8. 正常运行
./src/kotod
```

### 示例 2: 使用编译时宏（Windows）

```powershell
# 1. 修改创世区块参数（同上）

# 2. 使用宏定义编译
./zcutil/build.sh CPPFLAGS="-DMINE_GENESIS_BLOCK=1" -j4

# 3. 运行
./src/kotod.exe

# 4. 等待挖矿完成...

# 5. 更新 chainparams.cpp

# 6. 正常编译（不带宏）
./zcutil/build.sh -j4

# 7. 运行
./src/kotod.exe
```

---

## 📊 优先级顺序

如果同时使用多种方式，优先级为：

```
编译时宏定义 (#define) > 环境变量 (MINE_GENESIS) > 默认值 (false)
```

示例：
```bash
# 即使代码中 #define MINE_GENESIS_BLOCK 0
# 环境变量仍然可以启用挖矿
export MINE_GENESIS=1
./src/kotod  # 将会挖矿
```

---

## 🔍 验证是否启用

### 方法 1: 查看启动日志

如果启用了挖矿，启动时会看到：

```
Mining genesis block with yespower algorithm...
Genesis Block Parameters:
  Time: 1512879054
  Bits: 0x1f07ffff
  Target: 0007fff...
```

### 方法 2: 检查环境变量

```bash
# Linux/Mac
echo $MINE_GENESIS

# Windows PowerShell
$env:MINE_GENESIS

# Windows CMD
echo %MINE_GENESIS%
```

### 方法 3: 检查编译宏

```bash
# 查看编译时是否定义了宏
./src/kotod --version
# 或检查编译日志
```

---

## ⚙️ 配置示例

### 完整的新币种创建流程

```bash
#!/bin/bash

# 1. 设置币种参数
COIN_NAME="MyCoin"
COIN_SYMBOL="MYC"
NETWORK_MAGIC="0xaa,0xbb,0xcc,0xdd"
P2P_PORT=9999
RPC_PORT=9998

# 2. 修改源代码
# (这里应该用脚本或手动修改相关文件)

# 3. 启用创世区块挖矿
export MINE_GENESIS=1

# 4. 编译
./zcutil/build.sh -j$(nproc)

# 5. 挖掘创世区块
./src/kotod 2>&1 | tee genesis_mining.log

# 等待挖矿完成，Ctrl+C 停止

# 6. 从日志中提取创世区块信息
NONCE=$(grep "Nonce:" genesis_mining.log | tail -1 | awk '{print $2}')
BLOCK_HASH=$(grep "Block Hash:" genesis_mining.log | tail -1 | awk '{print $3}')
MERKLE_ROOT=$(grep "Merkle Root:" genesis_mining.log | tail -1 | awk '{print $3}')

echo "Found Genesis Block:"
echo "  Nonce: $NONCE"
echo "  Block Hash: $BLOCK_HASH"
echo "  Merkle Root: $MERKLE_ROOT"

# 7. 更新 chainparams.cpp
# (手动更新或使用 sed 脚本)

# 8. 取消挖矿模式
unset MINE_GENESIS

# 9. 重新编译
./zcutil/build.sh -j$(nproc)

# 10. 测试运行
./src/kotod

echo "Done! Your new coin is ready!"
```

---

## 🐛 故障排除

### 问题 1: 设置了环境变量但仍未挖矿

**解决方案**:
```bash
# 确认环境变量已设置
echo $MINE_GENESIS  # 应该输出 1

# 确认在同一个 shell 会话中运行
export MINE_GENESIS=1 && ./src/kotod

# 或使用前缀方式
MINE_GENESIS=1 ./src/kotod
```

### 问题 2: Windows 环境变量不生效

**解决方案**:
```powershell
# PowerShell: 确认设置
$env:MINE_GENESIS = "1"
$env:MINE_GENESIS  # 应该输出 1
./src/kotod.exe

# 或使用系统环境变量（永久）
[System.Environment]::SetEnvironmentVariable("MINE_GENESIS", "1", "User")
```

### 问题 3: 编译时宏定义不生效

**解决方案**:
```bash
# 确保 clean 后重新编译
make clean
./zcutil/build.sh CPPFLAGS="-DMINE_GENESIS_BLOCK=1" -j$(nproc)

# 或修改 configure.ac 然后重新 configure
./autogen.sh
./configure CPPFLAGS="-DMINE_GENESIS_BLOCK=1"
make -j$(nproc)
```

### 问题 4: 挖矿时间太长

**解决方案**: 降低难度参数（修改 `src/primitives/block.h`）

```cpp
// 从
.N = 131072,
.r = 32,

// 改为（测试用）
.N = 2048,
.r = 8,
```

---

## 📝 检查清单

创建新币种时的完整检查清单：

- [ ] 修改 `configure.ac` - 币种名称和版本
- [ ] 修改 `src/clientversion.cpp` - 客户端名称
- [ ] 修改 `src/chainparams.cpp` - 创世区块时间戳（第64行）
- [ ] 修改 `src/chainparams.cpp` - 币种符号（第86行）
- [ ] 修改 `src/chainparams.cpp` - 网络魔数（第245-248行）
- [ ] 修改 `src/chainparams.cpp` - P2P端口（第250行）
- [ ] 修改 `src/chainparams.cpp` - 地址前缀（第142-161行）
- [ ] 修改 `src/chainparamsbase.cpp` - RPC端口
- [ ] 设置环境变量: `export MINE_GENESIS=1`
- [ ] 编译: `./zcutil/build.sh -j$(nproc)`
- [ ] 运行挖矿: `./src/kotod`
- [ ] 等待创世区块生成
- [ ] 复制创世区块信息到 `chainparams.cpp`
- [ ] 取消环境变量: `unset MINE_GENESIS`
- [ ] 重新编译
- [ ] 测试运行

---

## 💡 最佳实践

1. **使用环境变量**: 最灵活，无需修改代码
2. **保存日志**: `./src/kotod 2>&1 | tee genesis_mining.log`
3. **验证结果**: 挖矿后确保节点正常启动
4. **备份代码**: 在修改前使用 git commit
5. **测试网先行**: 先在测试网测试所有功能
6. **文档记录**: 记录所有自定义参数

---

## 🎉 总结

现在您有**四种灵活的方式**来启用创世区块挖矿：

| 方式 | 优点 | 缺点 | 推荐度 |
|-----|------|-----|--------|
| 环境变量 | 最灵活，无需改代码 | 需要记住设置 | ⭐⭐⭐⭐⭐ |
| 命令行参数 | 清晰直观 | 需要额外实现 | ⭐⭐⭐⭐ |
| 编译时宏 | 持久化配置 | 需要重新编译 | ⭐⭐⭐ |
| 修改源码 | 最直接 | 不够灵活 | ⭐⭐ |

**推荐使用环境变量方式！**

```bash
export MINE_GENESIS=1
./src/kotod
```

简单、快速、无需修改代码！✨

