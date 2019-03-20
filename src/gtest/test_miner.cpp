#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "chainparams.h"
#include "key.h"
#include "miner.h"
#include "util.h"


TEST(Miner, GetScriptForMinerAddress) {
    SelectParams(CBaseChainParams::MAIN);

    // No miner address set
    {
        boost::shared_ptr<CReserveScript> coinbaseScript;
        GetScriptForMinerAddress(coinbaseScript);
        EXPECT_FALSE((bool) coinbaseScript);
    }

    mapArgs["-mineraddress"] = "notAnAddress";
    {
        boost::shared_ptr<CReserveScript> coinbaseScript;
        GetScriptForMinerAddress(coinbaseScript);
        EXPECT_FALSE((bool) coinbaseScript);
    }

    // Partial address
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39e";
    {
        boost::shared_ptr<CReserveScript> coinbaseScript;
        GetScriptForMinerAddress(coinbaseScript);
        EXPECT_FALSE((bool) coinbaseScript);
    }

    // Typo in address
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39ee";
    {
        boost::shared_ptr<CReserveScript> coinbaseScript;
        GetScriptForMinerAddress(coinbaseScript);
        EXPECT_FALSE((bool) coinbaseScript);
    }

    // Set up expected scriptPubKey for k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er
    CKeyID keyID;
    keyID.SetHex("13a25b3c55a20de4831a30f973cff8f939be82ee");
    CScript expectedCoinbaseScript = CScript() << OP_DUP << OP_HASH160 << ToByteVector(keyID) << OP_EQUALVERIFY << OP_CHECKSIG;

    // Valid address
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er";
    {
        boost::shared_ptr<CReserveScript> coinbaseScript;
        GetScriptForMinerAddress(coinbaseScript);
        EXPECT_TRUE((bool) coinbaseScript);
        EXPECT_EQ(expectedCoinbaseScript, coinbaseScript->reserveScript);
    }

    // Valid address with leading whitespace
    mapArgs["-mineraddress"] = "  k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er";
    {
        boost::shared_ptr<CReserveScript> coinbaseScript;
        GetScriptForMinerAddress(coinbaseScript);
        EXPECT_TRUE((bool) coinbaseScript);
        EXPECT_EQ(expectedCoinbaseScript, coinbaseScript->reserveScript);
    }

    // Valid address with trailing whitespace
    mapArgs["-mineraddress"] = "k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er  ";
    {
        boost::shared_ptr<CReserveScript> coinbaseScript;
        GetScriptForMinerAddress(coinbaseScript);
        EXPECT_TRUE((bool) coinbaseScript);
        EXPECT_EQ(expectedCoinbaseScript, coinbaseScript->reserveScript);
    }
}
