# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_parseOptions Test:
#####################################################################

include_directories(../../)

qt_internal_add_test(tst_parseOptions
        SOURCES
            tst_parseOptions.cpp
            ../../OptionsParser.cpp
            ../../options.h
            ../../OptionsParser.h
        DEFINES
            QT_NO_CAST_FROM_ASCII
            QT_NO_CAST_TO_ASCII
            QT_NO_FOREACH
        LIBRARIES
            Qt::Core
        )
