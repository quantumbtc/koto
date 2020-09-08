#if ENABLE_MINING
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "chainparams.h"
#include "key.h"
#include "miner.h"
#include "util.h"


TEST(Miner, GetMinerAddress) {
    SelectParams(CBaseChainParams::MAIN);

    // No miner address set
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }

    mapArgs["-mineraddress"] = "notAnAddress";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }

    // Partial address
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39e";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }

    // Typo in address
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39ee";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }

    // Set up expected scriptPubKey for k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er
    CKeyID keyID;
    keyID.SetHex("13a25b3c55a20de4831a30f973cff8f939be82ee");
    CScript expectedCoinbaseScript = CScript() << OP_DUP << OP_HASH160 << ToByteVector(keyID) << OP_EQUALVERIFY << OP_CHECKSIG;

    // Valid address
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_TRUE(IsValidMinerAddress(minerAddress));
        EXPECT_TRUE(boost::get<boost::shared_ptr<CReserveScript>>(&minerAddress) != nullptr);
        auto coinbaseScript = boost::get<boost::shared_ptr<CReserveScript>>(minerAddress);
        EXPECT_EQ(expectedCoinbaseScript, coinbaseScript->reserveScript);
    }

    // Valid address with leading whitespace
    mapArgs["-mineraddress"] = "  k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_TRUE(IsValidMinerAddress(minerAddress));
        EXPECT_TRUE(boost::get<boost::shared_ptr<CReserveScript>>(&minerAddress) != nullptr);
        auto coinbaseScript = boost::get<boost::shared_ptr<CReserveScript>>(minerAddress);
        EXPECT_EQ(expectedCoinbaseScript, coinbaseScript->reserveScript);
    }

    // Valid address with trailing whitespace
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er  ";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_TRUE(IsValidMinerAddress(minerAddress));
        EXPECT_TRUE(boost::get<boost::shared_ptr<CReserveScript>>(&minerAddress) != nullptr);
        auto coinbaseScript = boost::get<boost::shared_ptr<CReserveScript>>(minerAddress);
        EXPECT_EQ(expectedCoinbaseScript, coinbaseScript->reserveScript);
    }

    // Partial Sapling address
    mapArgs["-mineraddress"] = "koto1sy5jqutlcvuj79u8ndmzc4wn";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }

    // Typo in Sapling address
    mapArgs["-mineraddress"] = "koto1sy5jqut1cvuj79u8ndmzc4wn4qrlje7mwkwnpjq73vqa6epnlkypud5eex28vavqnsl9zwu6q2s";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }

    // Valid Sapling address
    mapArgs["-mineraddress"] = "koto1sy5jqutlcvuj79u8ndmzc4wn4qrlje7mwkwnpjq73vqa6epnlkypud5eex28vavqnsl9zwu6q2s";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_TRUE(IsValidMinerAddress(minerAddress));
        EXPECT_TRUE(boost::get<libzcash::SaplingPaymentAddress>(&minerAddress) != nullptr);
    }

    // Valid Sapling address with leading whitespace
    mapArgs["-mineraddress"] = "  koto1sy5jqutlcvuj79u8ndmzc4wn4qrlje7mwkwnpjq73vqa6epnlkypud5eex28vavqnsl9zwu6q2s";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }

    // Valid Sapling address with trailing whitespace
    mapArgs["-mineraddress"] = "  koto1sy5jqutlcvuj79u8ndmzc4wn4qrlje7mwkwnpjq73vqa6epnlkypud5eex28vavqnsl9zwu6q2s  ";
    {
        MinerAddress minerAddress;
        GetMinerAddress(minerAddress);
        EXPECT_FALSE(IsValidMinerAddress(minerAddress));
    }
}
#endif // ENABLE_MINING
