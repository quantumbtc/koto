# Windows 使用说明

## ⚠️ 重要提示

由于技术限制，**命令行参数方式在当前版本无法使用**。必须使用**环境变量方式**。

### 为什么不能用命令行参数？

`chainparams` (链参数) 在程序启动时作为全局变量被初始化，这发生在 `main()` 函数执行之前，也就是在命令行参数被解析之前。因此，在 `CMainParams()` 构造函数中无法访问命令行参数。

## ✅ 正确的使用方法

### Windows PowerShell（推荐）

```powershell
# 方式 1: 一行命令（最简单）
$env:MINE_GENESIS=1; .\src\kotod.exe

# 方式 2: 分步执行
$env:MINE_GENESIS=1
.\src\kotod.exe

# 挖矿完成后，正常运行（不需要环境变量）
.\src\kotod.exe
```

### Windows CMD

```cmd
REM 设置环境变量
set MINE_GENESIS=1

REM 运行挖矿
kotod.exe

REM 挖矿完成后，正常运行
kotod.exe
```

## 📋 完整示例

### PowerShell 完整流程

```powershell
# 1. 进入 koto 目录
cd D:\btq\koto

# 2. 修改币种参数（可选，创建新币种时需要）
notepad src\chainparams.cpp
# 修改第 64 行: 创世区块消息
# 修改第 245-248 行: 网络魔数
# 修改第 86 行: 币种符号

# 3. 编译（如果已编译可跳过）
.\zcutil\build.sh -j4

# 4. 挖掘创世区块
$env:MINE_GENESIS=1
.\src\kotod.exe | Tee-Object genesis.log

# 等待输出：
# Mining genesis block with yespower algorithm...
# ...
# Genesis Block Found!

# 5. Ctrl+C 停止程序

# 6. 查看日志
type genesis.log | findstr "Nonce Block Hash Merkle"

# 7. 复制输出内容更新 chainparams.cpp

# 8. 正常运行（不需要环境变量）
.\src\kotod.exe
```

### CMD 完整流程

```cmd
REM 1. 进入 koto 目录
cd /d D:\btq\koto

REM 2. 编译
zcutil\build.sh -j4

REM 3. 设置环境变量
set MINE_GENESIS=1

REM 4. 运行挖矿
kotod.exe > genesis.log 2>&1

REM 等待挖矿完成，Ctrl+C 停止

REM 5. 查看结果
type genesis.log | findstr "Nonce"

REM 6. 正常运行
kotod.exe
```

## 🐛 常见问题

### Q1: 为什么设置了环境变量还是不挖矿？

**检查步骤**:

```powershell
# 1. 确认环境变量已设置
$env:MINE_GENESIS  # 应该输出 1

# 2. 在同一个 shell 会话中运行
$env:MINE_GENESIS=1; .\src\kotod.exe

# 3. 或者一行命令
$env:MINE_GENESIS=1
.\src\kotod.exe
```

### Q2: 启动后看不到挖矿信息

**解决方案**:

```powershell
# 确保在正确的目录
cd src
.\kotod.exe

# 或使用完整路径
$env:MINE_GENESIS=1
D:\btq\koto\src\kotod.exe
```

### Q3: 如何验证是否进入挖矿模式？

启动程序后，应该立即看到：

```
Mining genesis block with yespower algorithm...
Genesis Block Parameters:
  Time: 1512879054
  Bits: 0x1f07ffff
  Target: 0007fff...
```

如果看到这个输出，说明已经进入挖矿模式。

如果看到：

```
Thank you for running a mainnet kotod...
Since starting this node 0 seconds ago:
```

说明进入了正常运行模式，环境变量没有生效。

## 💡 提示

### 永久设置环境变量（不推荐用于挖矿）

```powershell
# 设置用户环境变量（永久）
[System.Environment]::SetEnvironmentVariable("MINE_GENESIS", "1", "User")

# 删除环境变量
[System.Environment]::SetEnvironmentVariable("MINE_GENESIS", $null, "User")
```

**注意**: 挖矿完成后记得删除，否则每次启动都会挖矿！

### 快速命令

创建一个批处理文件 `mine_genesis.bat`:

```batch
@echo off
echo Starting genesis block mining...
set MINE_GENESIS=1
kotod.exe
pause
```

或 PowerShell 脚本 `mine_genesis.ps1`:

```powershell
Write-Host "Starting genesis block mining..."
$env:MINE_GENESIS=1
.\src\kotod.exe
```

## 🎯 推荐工作流

```powershell
# 挖矿阶段
$env:MINE_GENESIS=1; .\src\kotod.exe > genesis.log 2>&1

# 等待完成后，Ctrl+C 停止

# 查看结果
cat genesis.log | Select-String "Nonce","Block Hash","Merkle"

# 更新代码后，正常运行（不要环境变量）
.\src\kotod.exe
```

## ⚡ 性能提示

如果挖矿太慢，修改 `src/primitives/block.h`:

```cpp
// 第 96-97 行
// 从
.N = 131072,
.r = 32,

// 改为（测试用）
.N = 2048,
.r = 8,
```

这会让挖矿从几分钟降到几秒。

## 📚 相关文档

- **快速参考**: `QUICK_MINE_GENESIS.md`
- **完整指南**: `MINE_GENESIS_USAGE.md`
- **新币种**: `QUICK_START_NEW_COIN.md`

---

**总结**: 在 Windows 上，使用环境变量方式是唯一可靠的方法！

```powershell
$env:MINE_GENESIS=1; .\src\kotod.exe
```

