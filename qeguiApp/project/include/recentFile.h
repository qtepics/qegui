/*  recentFile.h
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

/*
 * Description:
 *
 * The application keeps a list of recent files.
 * An insctance of the recentFile class is used to represent each recent file.
 * Since the recentFile class is based on QAction, the list of recent files can be directly
 * added to each main window 'Recent...' menu.
 */

#ifndef RECENTFILE_H
#define RECENTFILE_H

#include <QObject>
#include <QAction>
#include <QString>

#include <QLabel>
#include <QEWidget.h>
#include <QEString.h>
#include <QEStringFormatting.h>
#include <QEPluginLibrary_global.h>
#include <managePixmaps.h>
#include <QEStringFormattingMethods.h>
#include <QCaVariableNamePropertyManager.h>


class QEGui;

// Class used to hold recent file information
class recentFile : public QAction
{
    Q_OBJECT
public:
    recentFile( QString nameIn, QString pathIn, QStringList pathListIn, QString macroSubstitutionsIn, QString customisationNameIn, QEGui* appIn );
    QString name;               // GUI title
    QString path;               // Full GUI file name
    QStringList pathList;       // Paths for locating other files
    QString macroSubstitutions; // Macro Substitutions
    QString customisationName;  // Window customisations

    QEGui* app;                 // Reference to main application

public slots:
    void recentSelected( bool );// Slot to act on selectino of this action in a 'Recent...' menu

};


#endif // RECENTFILE_H
