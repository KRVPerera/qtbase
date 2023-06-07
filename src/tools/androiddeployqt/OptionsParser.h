// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QTBASE_TOOLS_ANDROIDDEPLOYQT_OPTIONSPARSER_H
#define QTBASE_TOOLS_ANDROIDDEPLOYQT_OPTIONSPARSER_H

#include <QStringList>
#include "options.h"

namespace android {
namespace deploy {

class OptionsParser {
public:
    OptionsParser() : options(), m_syntaxError(false) {}

    Options getOptions() const {
        return options;
    }

    bool parseOptions(QStringList arguments);
    void printHelp() const;

    bool isSyntaxErrorOrHelpRequested() const {
        return m_syntaxError || options.helpRequested || options.outputDirectory.isEmpty();
    }

private:
    Options options;
    bool m_syntaxError;
};

} // namespace deploy
} // namespace android

#endif // QTBASE_TOOLS_ANDROIDDEPLOYQT_OPTIONSPARSER_H
