# 创世区块挖矿 - 快速参考

## 🚀 三步启动

```bash
# 1. 设置环境变量
export MINE_GENESIS=1

# 2. 编译运行
./zcutil/build.sh -j$(nproc)
./src/kotod

# 3. 等待输出
# Genesis Block Found! 
```

## 📋 四种方式对比

```bash
# 方式 1: 环境变量 ⭐推荐
export MINE_GENESIS=1
./src/kotod

# 方式 2: 一行命令
MINE_GENESIS=1 ./src/kotod

# 方式 3: 编译时宏
./zcutil/build.sh CPPFLAGS="-DMINE_GENESIS_BLOCK=1"

# 方式 4: 修改代码（不推荐）
# 在 chainparams.cpp 顶部添加: #define MINE_GENESIS_BLOCK 1
```

## 🔧 Windows 环境

### PowerShell
```powershell
$env:MINE_GENESIS=1
.\src\kotod.exe
```

### CMD
```cmd
set MINE_GENESIS=1
src\kotod.exe
```

## ✅ 验证是否启用

```bash
# 应该看到此输出：
Mining genesis block with yespower algorithm...
Genesis Block Parameters:
  Time: 1512879054
  Bits: 0x1f07ffff
  Target: 0007fff...

Yespower Parameters:
  N = 131072 (memory parameter)
  r = 32 (block size parameter)
  Memory per thread: ~512 MB

Starting mining...
```

## 📝 完整流程

```bash
# 1. 修改参数（创世区块消息、网络魔数、端口等）
vim src/chainparams.cpp

# 2. 启用挖矿
export MINE_GENESIS=1

# 3. 编译运行
./zcutil/build.sh -j$(nproc)
./src/kotod 2>&1 | tee genesis.log

# 4. 等待输出（Ctrl+C 停止）

# 5. 复制输出到 chainparams.cpp
# genesis = CreateGenesisBlock(时间, nonce, 难度, 版本, 0);
# assert(consensus.hashGenesisBlock == uint256S("0x哈希"));
# assert(genesis.hashMerkleRoot == uint256S("0x哈希"));

# 6. 关闭挖矿
unset MINE_GENESIS

# 7. 重新编译
./zcutil/build.sh -j$(nproc)

# 8. 正常运行
./src/kotod
```

## 🎯 输出示例

```
========================================
   Genesis Block Found!
========================================
Nonce:       2764
Block Hash:  6d424c350729ae633275d51dc3496e16cd1b1d195c164da00f39c499a2e9959e
PoW Hash:    0000123456789abcdef0123456789abcdef0123456789abcdef0123456789ab
Merkle Root: e18deb20a8da8ae6a9e965a10f52873adb65f4f568a5ac4b24ab074c7c81bb72
Target:      0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
========================================

Update chainparams.cpp with:

genesis = CreateGenesisBlock(1512879054, 2764, 0x1f07ffff, 4, 0);
assert(consensus.hashGenesisBlock == uint256S("0x6d424c35..."));
assert(genesis.hashMerkleRoot == uint256S("0xe18deb20..."));

========================================
```

## ⚡ 重要参数位置

| 参数 | 文件 | 行号 | 说明 |
|-----|------|-----|-----|
| 创世区块消息 | `src/chainparams.cpp` | 64 | 必须修改 |
| 币种符号 | `src/chainparams.cpp` | 86 | KOTO → YourCoin |
| 网络魔数 | `src/chainparams.cpp` | 245-248 | 必须修改 |
| P2P端口 | `src/chainparams.cpp` | 250 | 8433 → 您的端口 |
| RPC端口 | `src/chainparamsbase.cpp` | 35 | 8432 → 您的端口 |
| 地址前缀 | `src/chainparams.cpp` | 142-161 | 地址开头字符 |
| 客户端名称 | `src/clientversion.cpp` | 22 | MagicBean → YourName |
| 挖矿难度 | `src/primitives/block.h` | 96-97 | N, r 参数 |

## 🐛 常见问题

### Q: 设置了环境变量但不挖矿？
```bash
# 确认设置
echo $MINE_GENESIS  # 应输出 1

# 同一会话运行
export MINE_GENESIS=1 && ./src/kotod
```

### Q: 挖矿太慢？
```cpp
// 修改 src/primitives/block.h 降低难度
.N = 2048,   // 从 131072 降低
.r = 8,      // 从 32 降低
```

### Q: 如何停止挖矿？
```bash
# Ctrl+C 停止程序
# 然后取消环境变量
unset MINE_GENESIS
```

## 📚 详细文档

- **完整指南**: `MINE_GENESIS_USAGE.md`
- **参数说明**: `QUICK_START_NEW_COIN.md`
- **原理解析**: 查看源码注释

## 🎉 记住

**推荐方式**: 环境变量
```bash
export MINE_GENESIS=1
./src/kotod
```

简单、灵活、无需改代码！✨

