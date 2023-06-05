// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QTest>
#include <qmath.h>
#include <qfloat16.h>

#include <options.h>

class tst_ParseOptions : public QObject
{
Q_OBJECT
private slots:
    void fastSinCos();
};

void tst_ParseOptions::fastSinCos()
{
    // Test evenly spaced angles from 0 to 2pi radians.
    const int LOOP_COUNT = 100000;
    const qreal loopAngle = 2 * M_PI / (LOOP_COUNT - 1);
    for (int i = 0; i < LOOP_COUNT; ++i) {
        qreal angle = i * loopAngle;
        QVERIFY(qAbs(qSin(angle) - qFastSin(angle)) < 1e-5);
        QVERIFY(qAbs(qCos(angle) - qFastCos(angle)) < 1e-5);
    }
}

QTEST_APPLESS_MAIN(tst_ParseOptions)

#include "tst_parseOptions.moc"
