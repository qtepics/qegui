/*  StartupParams.h
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

// Manage startup parameters. Parse the startup parameters in a command line, serialize and unserialize parameters when passing them to another application instance.


#ifndef QEGUI_STARTUP_PARAMS_H
#define QEGUI_STARTUP_PARAMS_H

#include <QByteArray>
#include <QSharedMemory>
#include <QStringList>

// Class to manage startup parameters, and in particular sharing them with an other instance of this application
class startupParams
{
public:
    explicit startupParams();                       // Construction
    ~startupParams();                               // Destruction

    bool getStartupParams();                        // Extract startup parameters from command line arguments

    void setSharedParams( QByteArray& out );        // Serialise parameters as a serries of bytes
    bool getSharedParams( const QByteArray& in );   // Extract parameters from a series of bytes (return true if all OK)

    // Converniance fuction to read a PV name list file.
    static QStringList readNameList (const QString& filename);

    // Startup parameters
    double adjustScale;                             // GUI scaling parameter (-a)
    double fontScale;                               // Additional font scaling (-f) above and beyond adjustScale.
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
    QString knownPVListFile;                        // File holding the list of know PVs - for selection dialog
    QString oosPVListFile;                          // File holding the list of OOS PVs - for alarm colour manager
    QStringList filenameList;                       // Default gui file names
    QStringList pathList;                           // Default gui file path
    QString substitutions;                          // Substitutions. For example, "SECTOR=01,PUMP=03"
    QString customisationFile;                      // Window customisations file (containing named customisations of menu items and buttons)
    QString defaultCustomisationName;               // Default window customisation name (name of customisation in windowCustomisationFile)
    QString startupCustomisationName;               // Window customisation name for windows created at startup (name of customisation in windowCustomisationFile)
    QString applicationTitle;                       // Default application title
};


#endif // QEGUI_STARTUP_PARAMS_H
