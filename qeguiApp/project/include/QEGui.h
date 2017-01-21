/*  QEGui.cpp
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

#ifndef QEGUI_H
#define QEGUI_H

#include <QTimer>
#include <StartupParams.h>
#include <ContainerProfile.h>
#include <MainWindow.h>
#include <loginDialog.h>
#include <recentFile.h>
#include <windowCustomisation.h>
#include <configAutoSave.h>

// Class representing the QEGui application
class QEGui : public QApplication, ContainerProfile, public configAutoSave
{
public:
    QEGui( int& argc, char **argv );            // Construction

    int run();                                  // Main application code including call to exec()

    startupParams* getParams();                 // Get the parsed application startup parameters

    int         getMainWindowCount();                       // Get the number of main windows
    MainWindow* getMainWindow( int i );                     // Get a main window from the application's list of main windows
    int         getMainWindowPosition( MainWindow* mw );    // Locate a main window in the application's list of main windows
    void        addMainWindow( MainWindow* window );        // Add a main window to the application's list of main windows
    void        removeMainWindow( MainWindow* window );     // Remove a main window from the application's list of main windows given a reference to the main window
    void        removeMainWindow( int i );                  // Remove a main window from the application's list of main windows given an index into the application's list of main windows

    void        addGui( QEForm* gui, QString customisationName );  // Add a GUI to the application's list of GUIs

    void        login( QWidget* fromForm );                 // Change user level

    const QList<recentFile*>&  getRecentFiles();            // Return list of recently added files

    void        launchRecentGui( QString path, QStringList pathList, QString macroSubstitutions, QString customisationName );

    windowCustomisationList* getMainWindowCustomisations()  { return &winCustomisations; }
    windowCustomisation*     getCustomisation(QString name) { return winCustomisations.getCustomisation(name); }

    MainWindow*   raiseGui(  QString guiFileName, QString macroSubstitutions, QString title );
    const QString getCustomisationLog() { return winCustomisations.log.getLog(); }

    void saveConfiguration( PersistanceManager* pm, const QString configFile, const QString rootName, const QString configName, const bool warnUser);   // Save the current configuration

private:
    void printFile (const QString&  filename,
                    std::ostream & stream);         // Print file to stream
    void printVersion ();                           // Print the version info
    void printUsage (std::ostream & stream);        // Print brief usage statement
    void printHelp ();                              // Print help info

    startupParams params;                           // Parsed startup prarameters
    QList<MainWindow*> mainWindowList;              // List of all main windows
    void addGuiToWindowsMenu( QEForm* gui );

    QList<recentFile*> recentFiles;                 // List of recently opened files

    loginDialog* loginForm;                         // Dialog to use when changing user level. Keep one instance to maintain logout history

    windowCustomisationList winCustomisations;      // List of window customisations
};

#endif // QEGUI_H
