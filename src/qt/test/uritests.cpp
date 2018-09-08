// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uritests.h"

#include "guiutil.h"
#include "walletmodel.h"

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?label=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?amount=0.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?amount=1.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?amount=100&label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseBitcoinURI("koto://k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?message=Wikipedia Example Address", &rv));
    QVERIFY(rv.address == QString("k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?req-message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?amount=1,000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("koto:k1KafBsNNEYWuPgruiDx7c4Xw4bfrfF39er?amount=1,000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));
}
