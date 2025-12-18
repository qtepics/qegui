/*  main.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  SPDX-FileCopyrightText: 2009-2025 Australian Synchrotron
 *  SPDX-License-Identifier: LGPL-3.0-only
 *
 *  Author:     Andrew Rhyder
 *  Maintainer: Andrew Starritt
 *  Contact:    andrews@ansto.gov.au
 */

#include <QtGlobal>
#include <QApplication>
#include <QEGui.h>

//------------------------------------------------------------------------------
//
int main(int argc, char *argv[])
{
    // Intercept help/verson arguments before QApplication created.
    //
    if (argc >= 2) {
        const QString firstArg (argv[1]);

        if ((firstArg == "-h") || (firstArg == "--help")) {
            QEGui::printHelp();
            return 0;
        }

        if ((firstArg == "-v") || (firstArg == "--version")) {
            QEGui::printVersion();
            return 0;
        }
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    // Avoid Qt WebEngine seems to be initialized from a plugin warning.
    QCoreApplication::setAttribute( Qt::AA_ShareOpenGLContexts );
#endif

    QEGui* app = new QEGui( argc, argv );
    int ret = app->run();
    delete app;
    return ret;
}

// end
