/*  recentFile.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the Australian Synchrotron.
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2013 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

#include "recentFile.h"
#include <QEGui.h>
#include <QDebug>

recentFile::recentFile( QString nameIn, QString pathIn, QStringList pathListIn, QString macroSubstitutionsIn, QString customisationNameIn, QEGui* appIn ) : QAction( nameIn, appIn )
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
