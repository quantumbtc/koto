# 快速创建新币种指南

## 一键修改清单

按照以下顺序修改文件：

### 1️⃣ 基础配置 (5分钟)

**文件: `configure.ac`**
```bash
# 第3-6行：版本号
define(_CLIENT_VERSION_MAJOR, 1)
define(_CLIENT_VERSION_MINOR, 0)
define(_CLIENT_VERSION_REVISION, 0)
define(_CLIENT_VERSION_BUILD, 50)

# 第12行：币种名称
define(_COPYRIGHT_HOLDERS_SUBSTITUTION,[YourCoin])

# 第13行：项目信息
AC_INIT([YourCoin],[版本号],[你的GitHub地址],[yourcoin])

# 第19-22行：可执行文件名
BITCOIN_DAEMON_NAME=yourcoind
BITCOIN_GUI_NAME=yourcoin-qt
BITCOIN_CLI_NAME=yourcoin-cli
BITCOIN_TX_NAME=yourcoin-tx
```

### 2️⃣ 客户端名称 (1分钟)

**文件: `src/clientversion.cpp`**
```cpp
// 第22行
const std::string CLIENT_NAME("YourCoinName");
```

### 3️⃣ 核心参数 (15分钟) ⭐最重要⭐

**文件: `src/chainparams.cpp`**

```cpp
// 第64行：创世区块消息（必须修改！）
const char* pszTimestamp = "YourCoin: Your unique message here";

// 第86行：币种符号
strCurrencyUnits = "YRC";  // 你的币种符号

// 第87行：BIP44币种类型（选择一个未使用的）
bip44CoinType = 888;  // 查看 https://github.com/satoshilabs/slips/blob/master/slip-0044.md

// 第245-248行：网络魔数（必须修改！避免与其他币种冲突）
pchMessageStart[0] = 0xaa;  // 随机选择
pchMessageStart[1] = 0xbb;
pchMessageStart[2] = 0xcc;
pchMessageStart[3] = 0xdd;

// 第249行：告警公钥（建议生成新的）
vAlertPubKey = ParseHex("你的新公钥");

// 第250行：P2P端口（必须修改！）
nDefaultPort = 9999;  // 选择一个未使用的端口

// 第253-256行：创世区块参数（暂时保持，挖矿后更新）
genesis = CreateGenesisBlock(
    time(NULL),  // 使用当前时间
    0,           // nNonce: 挖矿时会找到
    0x1f07ffff,  // nBits: 难度
    4,           // nVersion
    0            // 奖励
);

// 第260行：启用创世区块挖矿
#define MINE_GENESIS_BLOCK 1  // 改为 1

// 第305-306行：临时注释掉
// assert(consensus.hashGenesisBlock == uint256S("0x..."));
// assert(genesis.hashMerkleRoot == uint256S("0x..."));

// 第263行：DNS种子节点（暂时注释掉）
// vSeeds.push_back(CDNSSeedData("example.com", "dnsseed.example.com"));
```

### 4️⃣ RPC端口 (2分钟)

**文件: `src/chainparamsbase.cpp`**
```cpp
// 第35行：主网RPC端口
nRPCPort = 9998;

// 第48行：测试网RPC端口
nRPCPort = 19998;

// 第62行：回归测试RPC端口
nRPCPort = 19998;
```

### 5️⃣ GUI配置 (3分钟)

**文件: `src/qt/bitcoinunits.cpp`**
```cpp
// 修改所有 KOTO 为你的币种符号
// 第20-23行、30-32行、43-58行
KOTO -> YRC
mKOTO -> mYRC
uKOTO -> uYRC
```

**文件: `src/qt/guiconstants.h`**
```cpp
// 第49-52行
#define QAPP_ORG_NAME "YourCoin"
#define QAPP_ORG_DOMAIN "yourcoin.org"
#define QAPP_APP_NAME_DEFAULT "YourCoin-Qt"
#define QAPP_APP_NAME_TESTNET "YourCoin-Qt-testnet"
```

### 6️⃣ 地址前缀 (5分钟)

**文件: `src/chainparams.cpp`**

使用在线工具生成地址前缀: https://appdevtools.com/base58-encoder-decoder

```cpp
// 第142-161行：主网地址前缀
// 示例：让地址以 "Y" 开头
keyConstants.base58Prefixes[PUBKEY_ADDRESS] = {0x4C};  // Y
keyConstants.base58Prefixes[SCRIPT_ADDRESS] = {0x0A};  // 自定义

// Bech32前缀
keyConstants.bech32HRPs[SAPLING_PAYMENT_ADDRESS] = "yrc";
keyConstants.bech32HRPs[SAPLING_FULL_VIEWING_KEY] = "yrcviews";
keyConstants.bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "yrcivks";
keyConstants.bech32HRPs[SAPLING_EXTENDED_SPEND_KEY] = "yrc-secret-extended-key-main";
keyConstants.bech32HRPs[SAPLING_EXTENDED_FVK] = "yrcxviews";
```

### 7️⃣ 挖矿算法参数 (可选)

**文件: `src/primitives/block.h`**
```cpp
// 第94-99行：调整 yespower 参数
yespower_params_t params = {
    .version = YESPOWER_0_5,
    .N = 2048,    // 内存参数 (原值: 131072)
    .r = 8,       // 块大小参数 (原值: 32)
    // 降低参数使挖矿更快
    .pers = (const uint8_t *)&ss[0],
    .perslen = ss.size()
};
```

## 🚀 执行流程

### 步骤 1: 修改所有上述参数
```bash
# 使用文本编辑器修改所有标记的文件
```

### 步骤 2: 挖掘创世区块
```bash
# 编译
./zcutil/build.sh -j$(nproc)

# 运行（会自动挖矿）
./src/kotod

# 等待输出创世区块信息...
```

### 步骤 3: 更新创世区块信息
```bash
# 将输出的信息更新到 src/chainparams.cpp
# 1. 更新 CreateGenesisBlock() 调用
# 2. 更新 assert() 语句
# 3. 设置 MINE_GENESIS_BLOCK = 0
```

### 步骤 4: 最终编译
```bash
# 清理
make clean

# 重新编译
./zcutil/build.sh -j$(nproc)

# 测试运行
./src/kotod --help
```

## 📋 检查清单

- [ ] `configure.ac` - 币种名称和版本
- [ ] `src/clientversion.cpp` - 客户端名称
- [ ] `src/chainparams.cpp` - 创世区块消息
- [ ] `src/chainparams.cpp` - 币种符号
- [ ] `src/chainparams.cpp` - 网络魔数
- [ ] `src/chainparams.cpp` - P2P端口
- [ ] `src/chainparams.cpp` - 地址前缀
- [ ] `src/chainparams.cpp` - BIP44类型
- [ ] `src/chainparamsbase.cpp` - RPC端口
- [ ] `src/qt/bitcoinunits.cpp` - GUI币种单位
- [ ] `src/qt/guiconstants.h` - GUI应用名称
- [ ] 挖掘创世区块
- [ ] 更新创世区块哈希
- [ ] 关闭挖矿模式
- [ ] 最终编译测试

## ⚠️ 常见错误

1. **忘记修改网络魔数** → 会与原链冲突
2. **忘记修改端口** → 无法启动节点
3. **忘记注释断言** → 挖矿时会崩溃
4. **忘记关闭挖矿模式** → 每次启动都会重新挖矿
5. **地址前缀冲突** → 地址可能被误认为其他币种

## 🎯 推荐配置

### 快速挖矿配置（测试用）
```cpp
// src/primitives/block.h
.N = 2048,
.r = 8,
// 挖矿时间: 几秒钟
```

### 中等安全配置
```cpp
.N = 16384,
.r = 16,
// 挖矿时间: 几十秒
```

### 高安全配置（生产环境）
```cpp
.N = 131072,
.r = 32,
// 挖矿时间: 几分钟
// 内存需求: 512MB/线程
```

## 📚 参考资源

- **BIP44币种类型**: https://github.com/satoshilabs/slips/blob/master/slip-0044.md
- **Base58编码器**: https://appdevtools.com/base58-encoder-decoder
- **端口列表**: https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
- **Yespower算法**: https://www.openwall.com/yespower/

## 💡 提示

1. **备份原始代码**: 在修改前先备份
2. **使用Git**: 每个步骤都提交一次
3. **记录参数**: 将所有自定义参数记录在文档中
4. **测试网优先**: 先在测试网测试所有功能
5. **社区反馈**: 发布前请社区成员测试

---

完成所有步骤后，您就成功创建了一个新的山寨币！🎉

