/*  StartupParams.h 
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
 *  Copyright (c) 2009, 2010 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

// Manage startup parameters. Parse the startup parameters in a command line, serialize and unserialize parameters when passing them to another application instance.


#ifndef STARTUPPARAMS_H
#define STARTUPPARAMS_H

#include <QByteArray>
#include <QSharedMemory>
#include <QStringList>

// Class to manage startup parameters, and in particular sharing them with an other instance of this application
class startupParams
{

  public:
    startupParams();                                // Construction

    void setSharedParams( QByteArray& out );        // Serialise parameters as a serries of bytes
    bool getSharedParams( const QByteArray& in );   // Extract parameters from a series of bytes (return true if all OK)

    bool getStartupParams( QStringList args );      // Extract startup parameters from command line arguments

    // Startup parameters
    double adjustScale;                             // GUI scaling parameter (-a)
    bool enableEdit;                                // Flag true if 'Edit' menu should be available
    bool disableMenu;                               // Flag true if menu bar should be disabled
    bool disableStatus;                             // Flag true if status bar should be disabled
    bool disableAutoSaveConfiguration;              // Flag true if autosave configuration should be disabled
    bool singleApp;                                 // True if only a single instance of this application should be started
    bool printHelp;                                 // True if and only if user requests help (-h).
    bool printVersion;                              // True if and only if user requests version (-v).
    bool restore;                                   // Flag true if restoring from config file
    QString configurationName;                      // Configuation name (Multiple named configurations can be saved in the configuration file)
    QString configurationFile;                      // Configuration file
    QStringList filenameList;                       // Default gui file names
    QStringList pathList;                           // Default gui file path
    QString substitutions;                          // Substitutions. For example, "SECTOR=01,PUMP=03"
    QString customisationFile;                      // Window customisations file (containing named customisations of menu items and buttons)
    QString defaultCustomisationName;               // Default window customisation name (name of customisation in windowCustomisationFile)
    QString startupCustomisationName;               // Window customisation name for windows created at startup (name of customisation in windowCustomisationFile)
    QString applicationTitle;                       // Default application title
};


#endif // STARTUPPARAMS_H
