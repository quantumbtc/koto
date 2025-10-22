// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2015-2020 The Zcash Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#include "key_io.h"
#include "main.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <optional>
#include <variant>
#include <cstdio>
#include <cstdlib>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, const uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // To create a genesis block for a new chain which is Overwintered:
    //   txNew.nVersion = OVERWINTER_TX_VERSION
    //   txNew.fOverwintered = true
    //   txNew.nVersionGroupId = OVERWINTER_VERSION_GROUP_ID
    //   txNew.nExpiryHeight = <default value>
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 520617983 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database (and is in any case of zero value).
 *
 * >>> from pyblake2 import blake2s
 * >>> 'Zcash' + blake2s(b'The Economist 2016-10-29 Known unknown: Another crypto-currency is born. BTC#436254 0000000000000000044f321997f336d2908cf8c8d6893e88dbf067e2d949487d ETH#2521903 483039a6b6bd8bd05f0584f9a078d075e454925eb71c1f13eaff59b405a721bb DJIA close on 27 Oct 2016: 18,169.68').hexdigest()
 *
 * CBlock(hash=00040fe8, ver=4, hashPrevBlock=00000000000000, hashMerkleRoot=c4eaa5, nTime=1477641360, nBits=1f07ffff, nNonce=4695, vtx=1)
 *   CTransaction(hash=c4eaa5, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff071f0104455a6361736830623963346565663862376363343137656535303031653335303039383462366665613335363833613763616331343161303433633432303634383335643334)
 *     CTxOut(nValue=0.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: c4eaa5
 */
static CBlock CreateGenesisBlock(uint32_t nTime, const uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Koto:Japanese crypto-currency";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

class CMainParams : public CChainParams {
public:
    CMainParams() {
        keyConstants.strNetworkID = "main";
        strCurrencyUnits = "KOTO";
        bip44CoinType = 510; // As registered in https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        consensus.fCoinbaseMustBeShielded = true;
        consensus.nSubsidySlowStartInterval = 43200;
        consensus.nPreBlossomSubsidyHalvingInterval = Consensus::PRE_BLOSSOM_HALVING_INTERVAL;
        consensus.nPostBlossomSubsidyHalvingInterval = POST_BLOSSOM_HALVING_INTERVAL(Consensus::PRE_BLOSSOM_HALVING_INTERVAL);
        consensus.nFoundersRewardPercentage = 3;
        consensus.nFoundersRewardTxPercentage = 10;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        consensus.nPreBlossomPowTargetSpacing = Consensus::PRE_BLOSSOM_POW_TARGET_SPACING;
        consensus.nPostBlossomPowTargetSpacing = Consensus::POST_BLOSSOM_POW_TARGET_SPACING;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = std::nullopt;
        consensus.fPowNoRetargeting = false;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170005;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 335600;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].hashActivationBlock =
            uint256S("019a9db8304e4c97af41388b3406df936340c4b2cf6014b95e34dc7c957e168e");
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170007;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight = 556500;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].hashActivationBlock =
            uint256S("069d7f5b21621d4e9d1072d7e3d417c5603b14be37fcaab5b57e812495635265");
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nProtocolVersion = 170009;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nActivationHeight = 1060000;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].hashActivationBlock =
            uint256S("f4b0a581ed3579131ed969b20db9f165a0b4b6edb4a53c86a53caddb15f9437f");
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].nProtocolVersion = 170011;
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].nActivationHeight = 1480000;
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].hashActivationBlock =
            uint256S("87730e5ef75e52c36abd454e22e068842f5d012ec323f51d601f8562353e4993");
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nProtocolVersion = 170013;
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight = 1530000;
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].hashActivationBlock =
            uint256S("356b8806085d5da6780c7957f11094d884963e8a4cc864b8724d07ada3971975");
        consensus.vUpgrades[Consensus::UPGRADE_NU5].nProtocolVersion = 170017;
        consensus.vUpgrades[Consensus::UPGRADE_NU5].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_ZFUTURE].nProtocolVersion = 0x7FFFFFFF;
        consensus.vUpgrades[Consensus::UPGRADE_ZFUTURE].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.nFundingPeriodLength = consensus.nPostBlossomSubsidyHalvingInterval / 48;

        // guarantees the first 2 characters, when base58 encoded, are "k1", "jz"
        keyConstants.base58Prefixes[PUBKEY_ADDRESS]     = {0x18,0x36};
        // guarantees the first 2 characters, when base58 encoded, are "k3"
        keyConstants.base58Prefixes[SCRIPT_ADDRESS]     = {0x18,0x3B};
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        keyConstants.base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        keyConstants.base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        keyConstants.base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zk"
        keyConstants.base58Prefixes[ZCPAYMENT_ADDRESS] = {0x16,0xA8};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVK"
        keyConstants.base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAB,0xD3};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        keyConstants.base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        keyConstants.bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "koto";
        keyConstants.bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "kviews";
        keyConstants.bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "kivks";
        keyConstants.bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "ksecret-extended-key-main";
        keyConstants.bech32HRPs[SAPLING_EXTENDED_FVK]         = "kxviews";

#if 0
        {
            std::vector<std::string> ecc_addresses = {
                "t3LmX1cxWPPPqL4TZHx42HU3U5ghbFjRiif",
                "t3Toxk1vJQ6UjWQ42tUJz2rV2feUWkpbTDs",
                "t3ZBdBe4iokmsjdhMuwkxEdqMCFN16YxKe6",
                "t3ZuaJziLM8xZ32rjDUzVjVtyYdDSz8GLWB",
                "t3bAtYWa4bi8VrtvqySxnbr5uqcG9czQGTZ",
                "t3dktADfb5Rmxncpe1HS5BRS5Gcj7MZWYBi",
                "t3hgskquvKKoCtvxw86yN7q8bzwRxNgUZmc",
                "t3R1VrLzwcxAZzkX4mX3KGbWpNsgtYtMntj",
                "t3ff6fhemqPMVujD3AQurxRxTdvS1pPSaa2",
                "t3cEUQFG3KYnFG6qYhPxSNgGi3HDjUPwC3J",
                "t3WR9F5U4QvUFqqx9zFmwT6xFqduqRRXnaa",
                "t3PYc1LWngrdUrJJbHkYPCKvJuvJjcm85Ch",
                "t3bgkjiUeatWNkhxY3cWyLbTxKksAfk561R",
                "t3Z5rrR8zahxUpZ8itmCKhMSfxiKjUp5Dk5",
                "t3PU1j7YW3fJ67jUbkGhSRto8qK2qXCUiW3",
                "t3S3yaT7EwNLaFZCamfsxxKwamQW2aRGEkh",
                "t3eutXKJ9tEaPSxZpmowhzKhPfJvmtwTEZK",
                "t3gbTb7brxLdVVghSPSd3ycGxzHbUpukeDm",
                "t3UCKW2LrHFqPMQFEbZn6FpjqnhAAbfpMYR",
                "t3NyHsrnYbqaySoQqEQRyTWkjvM2PLkU7Uu",
                "t3QEFL6acxuZwiXtW3YvV6njDVGjJ1qeaRo",
                "t3PdBRr2S1XTDzrV8bnZkXF3SJcrzHWe1wj",
                "t3ZWyRPpWRo23pKxTLtWsnfEKeq9T4XPxKM",
                "t3he6QytKCTydhpztykFsSsb9PmBT5JBZLi",
                "t3VWxWDsLb2TURNEP6tA1ZSeQzUmPKFNxRY",
                "t3NmWLvZkbciNAipauzsFRMxoZGqmtJksbz",
                "t3cKr4YxVPvPBG1mCvzaoTTdBNokohsRJ8n",
                "t3T3smGZn6BoSFXWWXa1RaoQdcyaFjMfuYK",
                "t3gkDUe9Gm4GGpjMk86TiJZqhztBVMiUSSA",
                "t3eretuBeBXFHe5jAqeSpUS1cpxVh51fAeb",
                "t3dN8g9zi2UGJdixGe9txeSxeofLS9t3yFQ",
                "t3S799pq9sYBFwccRecoTJ3SvQXRHPrHqvx",
                "t3fhYnv1S5dXwau7GED3c1XErzt4n4vDxmf",
                "t3cmE3vsBc5xfDJKXXZdpydCPSdZqt6AcNi",
                "t3h5fPdjJVHaH4HwynYDM5BB3J7uQaoUwKi",
                "t3Ma35c68BgRX8sdLDJ6WR1PCrKiWHG4Da9",
                "t3LokMKPL1J8rkJZvVpfuH7dLu6oUWqZKQK",
                "t3WFFGbEbhJWnASZxVLw2iTJBZfJGGX73mM",
                "t3L8GLEsUn4QHNaRYcX3EGyXmQ8kjpT1zTa",
                "t3PgfByBhaBSkH8uq4nYJ9ZBX4NhGCJBVYm",
                "t3WecsqKDhWXD4JAgBVcnaCC2itzyNZhJrv",
                "t3ZG9cSfopnsMQupKW5v9sTotjcP5P6RTbn",
                "t3hC1Ywb5zDwUYYV8LwhvF5rZ6m49jxXSG5",
                "t3VgMqDL15ZcyQDeqBsBW3W6rzfftrWP2yB",
                "t3LC94Y6BwLoDtBoK2NuewaEbnko1zvR9rm",
                "t3cWCUZJR3GtALaTcatrrpNJ3MGbMFVLRwQ",
                "t3YYF4rPLVxDcF9hHFsXyc5Yq1TFfbojCY6",
                "t3XHAGxRP2FNfhAjxGjxbrQPYtQQjc3RCQD",
            };

            // ZF and MG each use a single address repeated 48 times,
            // once for each funding period.
            std::vector<std::string> zf_addresses(48, "t3dvVE3SQEi7kqNzwrfNePxZ1d4hUyztBA1");
            std::vector<std::string> mg_addresses(48, "t3XyYW8yBFRuMnfvm5KLGFbEVz25kckZXym");

            consensus.AddZIP207FundingStream(
                keyConstants,
                Consensus::FS_ZIP214_BP,
                consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight, 2726400,
                ecc_addresses);
            consensus.AddZIP207FundingStream(
                keyConstants,
                Consensus::FS_ZIP214_ZF,
                consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight, 2726400,
                zf_addresses);
            consensus.AddZIP207FundingStream(
                keyConstants,
                Consensus::FS_ZIP214_MG,
                consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight, 2726400,
                mg_addresses);
        }
#endif

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000001714e5c11bccf");

        /**
         * The message start string should be awesome! ⓩ❤
         */
        pchMessageStart[0] = 0x4b;
        pchMessageStart[1] = 0x6f;
        pchMessageStart[2] = 0x74;
        pchMessageStart[3] = 0x6f;
        vAlertPubKey = ParseHex("045d763849214a55543e2a88442fdd5c666ac04cf66dcfb6b423b5e45200569de7e9456820cc30dfd7e66e174196a53ac7c64b57ebc6ab033ec1306a9bcbd64aed");
        nDefaultPort = 8433;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(
	    1512879054,
	    2764,
	    0x1f07ffff, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        // Mine genesis block
        // Can be enabled via:
        //   1. Environment variable: MINE_GENESIS=1 (推荐)
        //   2. Compile time: make CPPFLAGS="-DMINE_GENESIS_BLOCK=1"
        // Note: Command line args not available yet during chainparams construction
        bool fMineGenesis = false;
        
        // Check environment variable (primary method)
        const char* envMineGenesis = getenv("MINE_GENESIS");
        if (envMineGenesis != nullptr && atoi(envMineGenesis) != 0) {
            fMineGenesis = true;
        }
        
        // Check compile-time define
        #ifdef MINE_GENESIS_BLOCK
        #if MINE_GENESIS_BLOCK
        fMineGenesis = true;
        #endif
        #endif
        
        if (fMineGenesis)
        {
            printf("Mining genesis block with yespower algorithm...\n");
            printf("Genesis Block Parameters:\n");
            printf("  Time: %u\n", genesis.nTime);
            printf("  Bits: 0x%08x\n", genesis.nBits);
            printf("  Target: %s\n", consensus.powLimit.ToString().c_str());
            printf("\nYespower Parameters:\n");
            printf("  N = 131072 (memory parameter)\n");
            printf("  r = 32 (block size parameter)\n");
            printf("  Memory per thread: ~512 MB\n");
            printf("\nStarting mining...\n\n");
            
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 hashBlock;
            uint256 hashPoW;
            genesis.nNonce = 0;
            
            while (true) {
                // Use GetPoWHash() for yespower algorithm verification
                hashPoW = genesis.GetPoWHash();
                
                if (UintToArith256(hashPoW) <= hashTarget) {
                    hashBlock = genesis.GetHash();
                    printf("\n");
                    printf("========================================\n");
                    printf("   Genesis Block Found!\n");
                    printf("========================================\n");
                    printf("Nonce:       %u\n", genesis.nNonce);
                    printf("Block Hash:  %s\n", hashBlock.ToString().c_str());
                    printf("PoW Hash:    %s\n", hashPoW.ToString().c_str());
                    printf("Merkle Root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
                    printf("Target:      %s\n", ArithToUint256(hashTarget).ToString().c_str());
                    printf("========================================\n");
                    printf("\nUpdate chainparams.cpp with:\n\n");
                    printf("genesis = CreateGenesisBlock(%u, %u, 0x%08x, %d, 0);\n", 
                           genesis.nTime, genesis.nNonce, genesis.nBits, genesis.nVersion);
                    printf("assert(consensus.hashGenesisBlock == uint256S(\"0x%s\"));\n", hashBlock.ToString().c_str());
                    printf("assert(genesis.hashMerkleRoot == uint256S(\"0x%s\"));\n", genesis.hashMerkleRoot.ToString().c_str());
                    printf("\n========================================\n");
                    break;
                }
                
                // Display progress every 100 nonces
                if (genesis.nNonce % 100 == 0) {
                    // Show current nonce and partial hashes for progress tracking
                    printf("Nonce: %8u | PoW: %s...\r", 
                           genesis.nNonce, 
                           hashPoW.ToString().substr(0, 20).c_str());
                    fflush(stdout);
                }
                
                genesis.nNonce++;
                
                if (genesis.nNonce == 0) {
                    printf("\nNonce wrapped around (4294967296), incrementing time...\n");
                    genesis.nTime++;
                    printf("New time: %u\n", genesis.nTime);
                }
            }
        }
        
        assert(consensus.hashGenesisBlock == uint256S("0x6d424c350729ae633275d51dc3496e16cd1b1d195c164da00f39c499a2e9959e"));
        assert(genesis.hashMerkleRoot == uint256S("0xe18deb20a8da8ae6a9e965a10f52873adb65f4f568a5ac4b24ab074c7c81bb72"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("ko-to.org", "dnsseed.ko-to.org"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock)
            (77500, uint256S("0x8ed5cf1de772152f15cb392fd5e686796f77c08386eba922cbe109d4df96381a"))
            (150000, uint256S("0x31d1e2c5aa0a4ece690cb68912ddce8db49f837919bf141e163135dc95ae2bc4"))
            (200000, uint256S("0x134c68f3769274f2757fa32795c0cc9aa2fbf18b47ac3670e57652721011d3b4"))
            (300000, uint256S("0xf1fddcac288c30e451a498bd049c8db9d45663f4e7d7c37c37353d2f317b3634"))
            (400000, uint256S("0x852027615028ba35a7c7173e69b6e59eff0d60585c30658eb0d2c67ba453e029"))
            (500000, uint256S("0xf2fdfd332036ec8674ba2d1c1a10f2cf2291f41a068ef12c5cb21bd45f7116ed"))
            (600000, uint256S("0x322d57bb27260cd770a258f8e43b1e08d548f2c6014fbaa38ef65f7ebd521547"))
            (650000, uint256S("0x825a9cfc0eea84e7c34e524a7cef5f56b5532fa164f3980390419358b530a2ec"))
            (700000, uint256S("0xc1774d610daa8dd37e09803c4adea7d621cb05934b46561568ca43e172e72bcb"))
            (750000, uint256S("0xd9eb625eff0714058efb07be471d5ba4018c995b67e8abc67275bee50c2fe7fe")),
            1558339235,     // * UNIX timestamp of last checkpoint block
            2639821,        // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            5068            // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 60))
        };

        // Hardcoded fallback value for the Sprout shielded value pool balance
        // for nodes that have not reindexed since the introduction of monitoring
        // in #2795.
        nSproutValuePoolCheckpointHeight = 722400;
        nSproutValuePoolCheckpointBalance = 603936718320696;
        fZIP209Enabled = true;
        hashSproutValuePoolCheckpointBlock = uint256S("aefbc36781564d1a3519b39e211d85b0dd9a53efff9009c9f2a99fbd9268bc5c");

	// Founders reward script expects a vector of 2-of-3 multisig addresses
	vFoundersRewardAddress = {
	    "k3EEgqMM8jpaGW6XXUHpv3kDprMfm1x1VEf"
	};
	assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight(0));
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        keyConstants.strNetworkID = "test";
        strCurrencyUnits = "TOKO";
        bip44CoinType = 1;
        consensus.fCoinbaseMustBeShielded = true;
        consensus.nSubsidySlowStartInterval = 43200;
        consensus.nPreBlossomSubsidyHalvingInterval = Consensus::PRE_BLOSSOM_HALVING_INTERVAL;
        consensus.nPostBlossomSubsidyHalvingInterval = POST_BLOSSOM_HALVING_INTERVAL(Consensus::PRE_BLOSSOM_HALVING_INTERVAL);
        consensus.nFoundersRewardPercentage = 3;
        consensus.nFoundersRewardTxPercentage = 10;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        consensus.nPreBlossomPowTargetSpacing = Consensus::PRE_BLOSSOM_POW_TARGET_SPACING;
        consensus.nPostBlossomPowTargetSpacing = Consensus::POST_BLOSSOM_POW_TARGET_SPACING;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = std::nullopt;
        consensus.fPowNoRetargeting = false;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170003;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 93500;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].hashActivationBlock =
            uint256S("8e6684d30b08e6e3474ec2abb66d7b3508d16dbf49e66ce697340995fc455506");
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170007;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight = 275500;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].hashActivationBlock =
            uint256S("64cd1b50b132e95c1981dfe7eca151ffd99356ac970d975d5cd06ea7090aa7bb");
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nProtocolVersion = 170008;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nActivationHeight = 710000;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].hashActivationBlock =
            uint256S("8bcf13409aff9a8b484d92b8e0ff26d559c4145ab52afc4283dee6f915ea0869");
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].nProtocolVersion = 170010;
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].nActivationHeight = 1050800;
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].hashActivationBlock =
            uint256S("ea626d2eea61873a08a9546e3e8a5d248c079b8114895888c6e2f211d624d5eb");
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nProtocolVersion = 170012;
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight = 1249000;
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].hashActivationBlock =
            uint256S("9a671dd602556450bfa3f467d1bacee25bd2618a52f102d288dc39fd9601feb3");
        consensus.vUpgrades[Consensus::UPGRADE_NU5].nProtocolVersion = 170015;
        consensus.vUpgrades[Consensus::UPGRADE_NU5].nActivationHeight = 1770000;
        consensus.vUpgrades[Consensus::UPGRADE_ZFUTURE].nProtocolVersion = 0x7FFFFFFF;
        consensus.vUpgrades[Consensus::UPGRADE_ZFUTURE].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.nFundingPeriodLength = consensus.nPostBlossomSubsidyHalvingInterval / 48;

        // guarantees the first 2 characters, when base58 encoded, are "km"
        keyConstants.base58Prefixes[PUBKEY_ADDRESS]     = {0x18,0xA4};
        // guarantees the first 2 characters, when base58 encoded, are "k2"
        keyConstants.base58Prefixes[SCRIPT_ADDRESS]     = {0x18,0x39};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        keyConstants.base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        keyConstants.base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        keyConstants.base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        keyConstants.base58Prefixes[ZCPAYMENT_ADDRESS]  = {0x16,0xB6};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        keyConstants.base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        keyConstants.base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        keyConstants.bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ktestsapling";
        keyConstants.bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "kviewtestsapling";
        keyConstants.bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "kivktestsapling";
        keyConstants.bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "ksecret-extended-key-test";
        keyConstants.bech32HRPs[SAPLING_EXTENDED_FVK]         = "kxviewtestsapling";

#if 0
        // Testnet funding streams
        {
            std::vector<std::string> ecc_addresses = {
            };

            // ZF and MG use the same address for each funding period
            std::vector<std::string> zf_addresses = {
            };
            std::vector<std::string> mg_addresses = {
            };

            consensus.AddZIP207FundingStream(
                keyConstants,
                Consensus::FS_ZIP214_BP,
                consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight, 2796000,
                ecc_addresses);
            consensus.AddZIP207FundingStream(
                keyConstants,
                Consensus::FS_ZIP214_ZF,
                consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight, 2796000,
                zf_addresses);
            consensus.AddZIP207FundingStream(
                keyConstants,
                Consensus::FS_ZIP214_MG,
                consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight, 2796000,
                mg_addresses);
        }
#endif

        // On testnet we activate this rule 6 blocks after Blossom activation. From block 299188 and
        // prior to Blossom activation, the testnet minimum-difficulty threshold was 15 minutes (i.e.
        // a minimum difficulty block can be mined if no block is mined normally within 15 minutes):
        // <https://zips.z.cash/zip-0205#change-to-difficulty-adjustment-on-testnet>
        // However the median-time-past is 6 blocks behind, and the worst-case time for 7 blocks at a
        // 15-minute spacing is ~105 minutes, which exceeds the limit imposed by the soft fork of
        // 90 minutes.
        //
        // After Blossom, the minimum difficulty threshold time is changed to 6 times the block target
        // spacing, which is 7.5 minutes:
        // <https://zips.z.cash/zip-0208#minimum-difficulty-blocks-on-the-test-network>
        // 7 times that is 52.5 minutes which is well within the limit imposed by the soft fork.

        static_assert(6 * Consensus::POST_BLOSSOM_POW_TARGET_SPACING * 7 < MAX_FUTURE_BLOCK_TIME_MTP - 60,
                      "MAX_FUTURE_BLOCK_TIME_MTP is too low given block target spacing");
        consensus.nFutureTimestampSoftForkHeight = 851830;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("00000000000000000000000000000000000000000000000000000006dabaf6d0");

        pchMessageStart[0] = 0x54;
        pchMessageStart[1] = 0x6f;
        pchMessageStart[2] = 0x6b;
        pchMessageStart[3] = 0x6f;
        vAlertPubKey = ParseHex("043a6bd19f557ad74d57a5e991fb3d018595e551b1fcfd8ac94479b1642206fded363eb990465afe826a8879d4d9b08942d1ea7c0d52772130b4b91cad28720dc7");
        nDefaultPort = 18433;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(
	    1512879055,
	    28,
	    0x2007ffff, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xbf84afbde20c2d213b68b231ddb585ab616ef7567226820f00d9b397d774d2f0"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("ko-to.org", "testnet.ko-to.org"));
        vSeeds.push_back(CDNSSeedData("kotocoin.info", "dnsseed.testnet.kotocoin.info"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock)
            (85000, uint256S("0xf1b07e23116246700d5ad5e12a1105f02faa6b9d952fac8da5e2bb299b86372e")),
            1526264849,  // * UNIX timestamp of last checkpoint block
            86482,       // * total number of transactions between genesis and last checkpoint
            1465          //   total number of tx / (checkpoint block height / (24 * 24))
        };

        // Hardcoded fallback value for the Sprout shielded value pool balance
        // for nodes that have not reindexed since the introduction of monitoring
        // in #2795.
        nSproutValuePoolCheckpointHeight = 478416;
        nSproutValuePoolCheckpointBalance = 254952316712485;
        fZIP209Enabled = true;
        hashSproutValuePoolCheckpointBlock = uint256S("564e2778bed6de32e887109f2aa1d2407db3073cd4f5cbb44398e00b4e458237");

	// Founders reward script expects a vector of multisig addresses
	vFoundersRewardAddress = {
            "k2A4ArX2YAQJ6Qiut7tNNWUhq3Np7Et525K"
	};
	assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight(0));
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        keyConstants.strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        bip44CoinType = 1;
        consensus.fCoinbaseMustBeShielded = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nPreBlossomSubsidyHalvingInterval = Consensus::PRE_BLOSSOM_REGTEST_HALVING_INTERVAL;
        consensus.nPostBlossomSubsidyHalvingInterval = POST_BLOSSOM_HALVING_INTERVAL(Consensus::PRE_BLOSSOM_REGTEST_HALVING_INTERVAL);
        consensus.nFoundersRewardPercentage = 3;
        consensus.nFoundersRewardTxPercentage = 10;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        consensus.nPreBlossomPowTargetSpacing = Consensus::PRE_BLOSSOM_POW_TARGET_SPACING;
        consensus.nPostBlossomPowTargetSpacing = Consensus::POST_BLOSSOM_POW_TARGET_SPACING;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = 0;
        consensus.fPowNoRetargeting = true;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170003;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170006;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nProtocolVersion = 170008;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].nProtocolVersion = 170010;
        consensus.vUpgrades[Consensus::UPGRADE_HEARTWOOD].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nProtocolVersion = 170012;
        consensus.vUpgrades[Consensus::UPGRADE_CANOPY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_NU5].nProtocolVersion = 170015;
        consensus.vUpgrades[Consensus::UPGRADE_NU5].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_ZFUTURE].nProtocolVersion = 0x7FFFFFFF;
        consensus.vUpgrades[Consensus::UPGRADE_ZFUTURE].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.nFundingPeriodLength = consensus.nPostBlossomSubsidyHalvingInterval / 48;
        // Defined funding streams can be enabled with node config flags.

        // These prefixes are the same as the testnet prefixes
        keyConstants.base58Prefixes[PUBKEY_ADDRESS]     = {0x18,0xA4};
        keyConstants.base58Prefixes[SCRIPT_ADDRESS]     = {0x18,0x39};
        keyConstants.base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        keyConstants.base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        keyConstants.base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        keyConstants.base58Prefixes[ZCPAYMENT_ADDRESS]  = {0x16,0xB6};
        keyConstants.base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        keyConstants.base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        keyConstants.bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "kregtestsapling";
        keyConstants.bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "kviewregtestsapling";
        keyConstants.bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "kivkregtestsapling";
        keyConstants.bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "ksecret-extended-key-regtest";
        keyConstants.bech32HRPs[SAPLING_EXTENDED_FVK]         = "kxviewregtestsapling";

	// Founders reward script expects a vector of multisig addresses
	vFoundersRewardAddress = { "k2A4ArX2YAQJ6Qiut7tNNWUhq3Np7Et525K" };
	assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight(0));

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0x52;
        pchMessageStart[1] = 0x65;
        pchMessageStart[2] = 0x6b;
        pchMessageStart[3] = 0x6f;
        nDefaultPort = 18344;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(
	    1512879056,
	    7,
	    0x200f0f0f, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xdd905d5cda469020ddc364fdb530a4fb4559b9a117f78fdfbcc89d29d4909289"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_SPROUT && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }

    void UpdateFundingStreamParameters(Consensus::FundingStreamIndex idx, Consensus::FundingStream fs)
    {
        assert(idx >= Consensus::FIRST_FUNDING_STREAM && idx < Consensus::MAX_FUNDING_STREAMS);
        consensus.vFundingStreams[idx] = fs;
    }

    void UpdateRegtestPow(
        int64_t nPowMaxAdjustDown,
        int64_t nPowMaxAdjustUp,
        uint256 powLimit,
        bool noRetargeting)
    {
        consensus.nPowMaxAdjustDown = nPowMaxAdjustDown;
        consensus.nPowMaxAdjustUp = nPowMaxAdjustUp;
        consensus.powLimit = powLimit;
        consensus.fPowNoRetargeting = noRetargeting;
    }

    void SetRegTestZIP209Enabled() {
        fZIP209Enabled = true;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestshieldcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeShielded();
    }

    // When a developer is debugging turnstile violations in regtest mode, enable ZIP209
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-developersetpoolsizezero")) {
        regTestParams.SetRegTestZIP209Enabled();
    }
}


// Block height must be >0 and <=last founders reward block height
// Index variable i ranges from 0 - (vFoundersRewardAddress.size()-1)
std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {
/*
    int maxHeight = consensus.GetLastFoundersRewardBlockHeight(0);
    assert(nHeight > 0 && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vFoundersRewardAddress.size()) / vFoundersRewardAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vFoundersRewardAddress[i];
*/
    return vFoundersRewardAddress[0];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= consensus.GetLastFoundersRewardBlockHeight(nHeight));

    KeyIO keyIO(*this);
    CTxDestination address = keyIO.DecodeDestination(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    assert(IsValidDestination(address));
    assert(IsScriptDestination(address));
    CScriptID scriptID = std::get<CScriptID>(address); // address is a variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetFoundersRewardAddressAtIndex(int i) const {
    assert(i >= 0 && i < vFoundersRewardAddress.size());
    return vFoundersRewardAddress[i];
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}

void UpdateFundingStreamParameters(Consensus::FundingStreamIndex idx, Consensus::FundingStream fs)
{
    regTestParams.UpdateFundingStreamParameters(idx, fs);
}

void UpdateRegtestPow(
    int64_t nPowMaxAdjustDown,
    int64_t nPowMaxAdjustUp,
    uint256 powLimit,
    bool noRetargeting)
{
    regTestParams.UpdateRegtestPow(nPowMaxAdjustDown, nPowMaxAdjustUp, powLimit, noRetargeting);
}
