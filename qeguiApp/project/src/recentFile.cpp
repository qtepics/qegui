/*  recentFile.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  SPDX-FileCopyrightText: 2013-2025 Australian Synchrotron
 *  SPDX-License-Identifier: LGPL-3.0-only
 *
 *  Author:     Andrew Rhyder
 *  Maintainer: Andrew Starritt
 *  Contact:    andrews@ansto.gov.au
 */

#include "recentFile.h"
#include <QEGui.h>
#include <QDebug>

recentFile::recentFile( const QString& nameIn, const QString& pathIn, const QStringList& pathListIn,
                        const QString& macroSubstitutionsIn, const QString& customisationNameIn,
                        QEGui* appIn ) : QAction( nameIn, appIn )
{
    name = nameIn;
    path = pathIn;
    pathList = pathListIn;
    macroSubstitutions = macroSubstitutionsIn;
    customisationName = customisationNameIn;
    app = appIn;
    QObject::connect( this, SIGNAL( triggered( bool ) ), this, SLOT( recentSelected( bool ) ) );
}

void recentFile::recentSelected( bool )
{
    // Open the file
    app->launchRecentGui( path, pathList, macroSubstitutions, customisationName );
}

// end
