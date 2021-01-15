// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include "util.h"
#include "uritests.h"
#include "compattests.h"

#include <QCoreApplication>
#include <QObject>
#include <QTest>

#if defined(QT_STATICPLUGIN) && QT_VERSION < 0x050000
#include <QtPlugin>
Q_IMPORT_PLUGIN(qcncodecs)
Q_IMPORT_PLUGIN(qjpcodecs)
Q_IMPORT_PLUGIN(qtwcodecs)
Q_IMPORT_PLUGIN(qkrcodecs)
#endif

const std::function<std::string(const char*)> G_TRANSLATION_FUN = nullptr;

// This is all you need to run all the tests
int main(int argc, char *argv[])
{
    SetupEnvironment();
    bool fInvalid = false;

    // Don't remove this, it's needed to access
    // QCoreApplication:: in the tests
    QCoreApplication app(argc, argv);
    app.setApplicationName("Koto-Qt-test");

    URITests test1;
    if (QTest::qExec(&test1) != 0)
        fInvalid = true;
    CompatTests test4;
    if (QTest::qExec(&test4) != 0)
        fInvalid = true;

    return fInvalid;
}
