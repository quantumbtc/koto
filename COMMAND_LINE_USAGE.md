# 命令行参数使用说明

## ✨ 新功能：命令行参数支持

现在可以直接通过命令行参数启用创世区块挖矿！

## 🎯 支持的格式（大小写均可）

```bash
# 格式 1: 大写带下划线（推荐）
./src/kotod --MINE_GENESIS

# 格式 2: 小写带下划线
./src/kotod --mine_genesis

# 格式 3: 小写无下划线
./src/kotod -minegenesis
```

## 💻 平台示例

### Linux/Mac

```bash
# 编译
./zcutil/build.sh -j$(nproc)

# 运行挖矿
./src/kotod --MINE_GENESIS

# 或者
./src/kotod --mine_genesis

# 或者
./src/kotod -minegenesis
```

### Windows PowerShell

```powershell
# 编译
.\zcutil\build.sh -j4

# 运行挖矿
.\src\kotod.exe --MINE_GENESIS

# 或者
.\src\kotod.exe --mine_genesis

# 或者
.\src\kotod.exe -minegenesis
```

### Windows CMD

```cmd
REM 编译
zcutil\build.sh -j4

REM 运行挖矿
src\kotod.exe --MINE_GENESIS
```

## 🔍 查看帮助

```bash
# 查看所有 debug 选项（包括挖矿参数）
./src/kotod --help-debug

# 输出将包括：
# -MINE_GENESIS
#       Mine a new genesis block (case insensitive: -MINE_GENESIS,
#       -mine_genesis, -minegenesis, also supports env var MINE_GENESIS=1)
```

## 📊 优先级顺序

如果同时使用多种方式，优先级为：

```
1. 命令行参数 (--MINE_GENESIS)     ← 最高优先级
2. 环境变量 (MINE_GENESIS=1)
3. 编译时宏 (MINE_GENESIS_BLOCK)
```

## 🎯 实际示例

### 示例 1: 完整的创世区块挖矿流程

```bash
#!/bin/bash

# 1. 修改币种参数
vim src/chainparams.cpp
# 修改创世区块消息、网络魔数等

# 2. 编译
./zcutil/build.sh -j$(nproc)

# 3. 挖矿（使用命令行参数）
./src/kotod --MINE_GENESIS 2>&1 | tee genesis.log

# 程序会输出挖矿进度...
# Nonce: 1000 | Block Hash: ... | PoW Hash: ...
# 
# 找到创世区块后会显示：
# ========================================
#    Genesis Block Found!
# ========================================

# 4. Ctrl+C 停止程序

# 5. 复制输出内容更新 chainparams.cpp

# 6. 正常运行（不带参数）
./src/kotod
```

### 示例 2: Windows 完整流程

```powershell
# 1. 修改参数
notepad src\chainparams.cpp

# 2. 编译
.\zcutil\build.sh -j4

# 3. 挖矿
.\src\kotod.exe --MINE_GENESIS | Tee-Object genesis.log

# 4. 等待完成，复制输出

# 5. 正常运行
.\src\kotod.exe
```

## ⚙️ 与环境变量的对比

### 命令行参数方式 ✅ 推荐

**优点**:
- ✅ 直观明确
- ✅ 不需要设置/取消环境变量
- ✅ 支持大小写
- ✅ 一次性使用方便

**缺点**:
- 每次运行都要加参数

```bash
./src/kotod --MINE_GENESIS
```

### 环境变量方式

**优点**:
- ✅ 一次设置，多次使用
- ✅ 可以在配置文件中设置

**缺点**:
- 需要记得取消设置
- 可能影响其他 shell 会话

```bash
export MINE_GENESIS=1
./src/kotod
# 使用完后需要
unset MINE_GENESIS
```

## 🛠️ 技术实现

### 参数检查顺序

```cpp
// src/chainparams.cpp
bool fMineGenesis = false;

// 1. 检查命令行参数（优先级最高）
try {
    if (GetBoolArg("-MINE_GENESIS", false) || 
        GetBoolArg("-mine_genesis", false) ||
        GetBoolArg("-minegenesis", false)) {
        fMineGenesis = true;
    }
} catch (...) {
    // mapArgs 可能还未初始化，继续检查环境变量
}

// 2. 检查环境变量（后备方案）
if (!fMineGenesis) {
    const char* env = getenv("MINE_GENESIS");
    if (env != nullptr && atoi(env) != 0) {
        fMineGenesis = true;
    }
}

// 3. 检查编译时宏
#ifdef MINE_GENESIS_BLOCK
#if MINE_GENESIS_BLOCK
fMineGenesis = true;
#endif
#endif
```

### Windows 参数处理

在 Windows 上，参数会被自动转为小写（见 `src/util.cpp`），所以：

```powershell
# 这些都等效
.\src\kotod.exe --MINE_GENESIS
.\src\kotod.exe --Mine_Genesis
.\src\kotod.exe --mine_genesis
```

## 🐛 故障排除

### 问题：参数不生效

**检查 1**: 确认参数格式
```bash
# 正确
./src/kotod --MINE_GENESIS

# 错误（缺少双横线）
./src/kotod MINE_GENESIS
```

**检查 2**: 查看启动日志
```bash
./src/kotod --MINE_GENESIS 2>&1 | grep -i "mining"
# 应该看到：Mining genesis block with yespower algorithm...
```

**检查 3**: 使用 debug 模式
```bash
./src/kotod --MINE_GENESIS -debug=all
```

### 问题：在 Windows 上参数不识别

**解决方案**: 确保使用 PowerShell 或 CMD

```powershell
# PowerShell 正确方式
.\src\kotod.exe --MINE_GENESIS

# CMD 正确方式
src\kotod.exe --MINE_GENESIS
```

## 📋 检查清单

创建新币种时：

- [ ] 修改 `src/chainparams.cpp` 中的币种参数
- [ ] 编译: `./zcutil/build.sh -j$(nproc)`
- [ ] 运行挖矿: `./src/kotod --MINE_GENESIS`
- [ ] 等待创世区块生成
- [ ] 复制输出到 `chainparams.cpp`
- [ ] 正常运行测试: `./src/kotod`

## 🎉 总结

**推荐使用命令行参数方式**:

```bash
./src/kotod --MINE_GENESIS
```

优势：
- ✅ 直观明确
- ✅ 支持大小写
- ✅ 一次性使用
- ✅ 无需清理环境变量

简单、快速、可靠！🚀

