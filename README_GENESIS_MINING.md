# 创世区块挖矿功能 - 总览

## 🎯 快速开始

```bash
# 最简单的使用方式（命令行参数，支持大小写）
./zcutil/build.sh -j$(nproc)
./src/kotod --MINE_GENESIS
```

就这么简单！🎉

```bash
# 支持的命令行格式（大小写均可）
./src/kotod --MINE_GENESIS
./src/kotod --mine_genesis
./src/kotod -minegenesis
```

---

## 📚 文档索引

### 1. 快速参考
- **[QUICK_MINE_GENESIS.md](QUICK_MINE_GENESIS.md)** ⭐ 新手推荐
  - 三步快速启动
  - 常用命令速查
  - 故障排除

### 2. 详细指南
- **[MINE_GENESIS_USAGE.md](MINE_GENESIS_USAGE.md)** 📖 完整教程
  - 四种启用方式详解
  - 完整示例和配置
  - Windows/Linux/Mac 全平台

### 3. 新币种创建
- **[QUICK_START_NEW_COIN.md](QUICK_START_NEW_COIN.md)** 🚀 创建指南
  - 完整的修改清单
  - 所有需要改的文件
  - 参数配置说明

### 4. 技术文档
- **[CHANGELOG_GENESIS_MINING.md](CHANGELOG_GENESIS_MINING.md)** 🔧 修改日志
  - 详细的技术变更
  - 代码对比分析
  - API 变更说明

---

## 🌟 主要特性

### ✨ 多种启用方式

| 方式 | 难度 | 灵活性 | 推荐度 |
|-----|------|--------|--------|
| 命令行参数 | ⭐ 简单 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| 环境变量 | ⭐ 简单 | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| 编译时宏 | ⭐⭐ 中等 | ⭐⭐⭐ | ⭐⭐⭐ |
| 修改源码 | ⭐⭐⭐ 复杂 | ⭐ | ⭐⭐ |

### ✅ 正确的 PoW 验证

使用 `GetPoWHash()` 而不是 `GetHash()`，确保 yespower 算法正确应用。

### 📦 开箱即用

无需修改源代码，设置环境变量即可使用。

---

## 🚀 使用示例

### 示例 1: Linux 快速挖矿（命令行参数）

```bash
./zcutil/build.sh -j$(nproc)
./src/kotod --MINE_GENESIS 2>&1 | tee genesis.log
```

### 示例 2: Windows PowerShell

```powershell
.\zcutil\build.sh -j4
.\src\kotod.exe --MINE_GENESIS
```

### 示例 3: 大小写支持

```bash
./src/kotod --MINE_GENESIS  # 大写
./src/kotod --mine_genesis  # 小写
./src/kotod -minegenesis    # 无下划线
```

### 示例 4: 环境变量方式

```bash
export MINE_GENESIS=1
./src/kotod
```

---

## 📊 工作流程

```
┌─────────────────┐
│  修改币种参数   │
│  (chainparams)  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ 启用挖矿模式    │
│export MINE=1    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  编译并运行     │
│  ./zcutil/build │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  等待创世块     │
│  (几秒到几分钟) │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  复制输出信息   │
│  更新源代码     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  关闭挖矿模式   │
│  unset MINE     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  重新编译测试   │
│  完成！         │
└─────────────────┘
```

---

## ⚙️ 环境要求

### 系统要求
- Linux / macOS / Windows
- 4GB+ RAM
- 多核 CPU（推荐）

### 编译依赖
```bash
# Ubuntu/Debian
sudo apt-get install build-essential pkg-config libc6-dev \
  m4 g++-multilib autoconf libtool ncurses-dev unzip git \
  python3 zlib1g-dev wget bsdmainutils automake curl

# macOS
brew install autoconf automake libtool pkgconfig
```

### 挖矿内存需求

| N 参数 | r 参数 | 内存/线程 | 挖矿速度 |
|--------|--------|----------|---------|
| 2048 | 8 | ~8MB | 几秒 |
| 16384 | 16 | ~64MB | 几十秒 |
| 131072 | 32 | ~512MB | 几分钟 ✅ |

---

## 🔧 配置参数

### 必须修改的参数

在 `src/chainparams.cpp` 中：

```cpp
// 第 64 行：创世区块消息（必改）
const char* pszTimestamp = "Your unique message here";

// 第 86 行：币种符号
strCurrencyUnits = "YourCoin";

// 第 245-248 行：网络魔数（必改）
pchMessageStart[0] = 0xaa;
pchMessageStart[1] = 0xbb;
pchMessageStart[2] = 0xcc;
pchMessageStart[3] = 0xdd;

// 第 250 行：P2P 端口
nDefaultPort = 9999;
```

### 可选修改的参数

```cpp
// src/primitives/block.h：挖矿难度
.N = 2048,   // 降低以加快挖矿
.r = 8,

// src/chainparams.cpp：地址前缀
keyConstants.base58Prefixes[PUBKEY_ADDRESS] = {0x??};
```

---

## 🎯 输出格式

成功挖矿后会显示：

```
========================================
   Genesis Block Found!
========================================
Nonce:       2764
Block Hash:  6d424c350729ae...
PoW Hash:    0000123456789a...
Merkle Root: e18deb20a8da8a...
========================================

Update chainparams.cpp with:

genesis = CreateGenesisBlock(1512879054, 2764, 0x1f07ffff, 4, 0);
assert(consensus.hashGenesisBlock == uint256S("0x6d424c35..."));
assert(genesis.hashMerkleRoot == uint256S("0xe18deb20..."));
```

直接复制这些内容到您的代码中即可！

---

## 🐛 故障排除

### 问题：环境变量不生效

```bash
# 确认设置
echo $MINE_GENESIS  # 应输出 1

# 在同一会话运行
export MINE_GENESIS=1 && ./src/kotod
```

### 问题：挖矿太慢

```cpp
// 修改 src/primitives/block.h
.N = 2048,   // 降低难度
.r = 8,
```

### 问题：编译错误

```bash
# 清理后重新编译
make clean
./zcutil/build.sh -j$(nproc)
```

更多问题请查看 [MINE_GENESIS_USAGE.md](MINE_GENESIS_USAGE.md)

---

## 📖 学习路径

### 初学者 🌱
1. 阅读 [QUICK_MINE_GENESIS.md](QUICK_MINE_GENESIS.md)
2. 跟随示例操作
3. 查看输出格式

### 中级用户 🌿
1. 阅读 [MINE_GENESIS_USAGE.md](MINE_GENESIS_USAGE.md)
2. 了解四种启用方式
3. 尝试不同配置

### 高级用户 🌳
1. 阅读 [QUICK_START_NEW_COIN.md](QUICK_START_NEW_COIN.md)
2. 创建完整的新币种
3. 查看 [CHANGELOG_GENESIS_MINING.md](CHANGELOG_GENESIS_MINING.md) 了解技术细节

---

## 💡 最佳实践

1. **使用环境变量**: 最灵活的方式
   ```bash
   export MINE_GENESIS=1
   ```

2. **保存日志**: 便于后续查看
   ```bash
   ./src/kotod 2>&1 | tee genesis.log
   ```

3. **验证结果**: 挖矿后测试节点启动
   ```bash
   unset MINE_GENESIS
   ./src/kotod
   ```

4. **备份代码**: 修改前使用 Git
   ```bash
   git commit -am "Before mining genesis"
   ```

5. **文档记录**: 记录所有自定义参数

---

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

### 报告问题
- 描述问题和环境
- 提供复现步骤
- 附加日志和截图

### 改进文档
- 修正错误
- 添加示例
- 翻译文档

---

## 📜 许可证

与 Koto 项目相同的许可证。

---

## 🎉 总结

创世区块挖矿现在变得超级简单：

```bash
# 推荐方式：命令行参数（支持大小写）
./src/kotod --MINE_GENESIS
```

- ✅ 命令行参数支持（大小写均可）
- ✅ 无需修改源代码
- ✅ 支持多平台
- ✅ 灵活配置
- ✅ 正确的 PoW 验证
- ✅ 详细的文档

开始创建您的新币种吧！🚀

---

**文档更新**: 2025年10月
**维护者**: Koto Development Team
**反馈**: 欢迎在 Issues 中讨论

