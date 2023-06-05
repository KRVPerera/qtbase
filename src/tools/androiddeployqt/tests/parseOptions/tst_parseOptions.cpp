// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QTest>
#include <QString>
#include <QStringList>

#include <options.h>
#include <OptionsParser.h>

class tst_ParseOptions : public QObject
{
Q_OBJECT
private slots:
    void testSimpleHelpRequested();
};

void tst_ParseOptions::testSimpleHelpRequested()
{
    QStringList programArguments;
    programArguments << QString("--help"_L1);
    android::deploy::OptionsParser optionsParser;
    optionsParser.parseOptions(programArguments);
    const auto options = optionsParser.getOptions();
    QVERIFY(optionsParser.isSyntaxErrorOrHelpRequested());
    QVERIFY(options.helpRequested);
}

QTEST_APPLESS_MAIN(tst_ParseOptions)

#include "tst_parseOptions.moc"
