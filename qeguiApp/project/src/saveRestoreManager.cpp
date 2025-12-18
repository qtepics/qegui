/*  saveRestoreManager.cpp
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

/* This class is used to manage saving and restoring for QEGui.
   The main item saved is the number of main windows saved so the restore will know how many main windows to restore.
   Each main window will save it's own info.
*/

#include <saveRestoreManager.h>
#include <MainWindow.h>
#include <QEGui.h>
#include <QEEnums.h>
#include <QECommon.h>

#define SAVERESTORE_NAME "QEGui"

// Construction
saveRestoreManager::saveRestoreManager( QEGui* appIn )
{
    // Save reference to the QEGui application
    app = appIn;

    // Setup to respond to requests to save or restore persistant data
    PersistanceManager* persistanceManager = profile.getPersistanceManager();
    QObject::connect( persistanceManager->getSaveRestoreObject(), SIGNAL( saveRestore( SaveRestoreSignal::saveRestoreOptions ) ), this, SLOT( saveRestore( SaveRestoreSignal::saveRestoreOptions ) ), Qt::DirectConnection );
}

// Destruction
saveRestoreManager::~saveRestoreManager()
{
}

// A save or restore has been requested (Probably by QEGui itself)
void saveRestoreManager::saveRestore( SaveRestoreSignal::saveRestoreOptions option )
{
    PersistanceManager* pm = profile.getPersistanceManager();

    switch( option )
    {
        // Save the application data
        case SaveRestoreSignal::SAVE:
            {
                // Start with the top level element - the QEGui application
                PMElement appElement =  pm->addNamedConfiguration( SAVERESTORE_NAME );

                // Note the number of main windows. This will determine how many main windows are expected on restore
                appElement.addValue( "MainWindows", app->getMainWindowCount() );

                // Note the current user level
                const QE::UserLevels userLevel = getUserLevel();
                appElement.addValue( "UserLevel", QE::UserLevelsImage (userLevel));
            }
            break;

        // First restore phase.
        // This application will create the main windows and the GUIs they contain
        case SaveRestoreSignal::RESTORE_APPLICATION:
            {
                // Get the data for this application
                PMElement QEGuiData = pm->getNamedConfiguration( SAVERESTORE_NAME );

                // If none, do nothing
                if( QEGuiData.isNull() )
                {
                    return;
                }

                // Note the current user level
                QString levelString;
                QEGuiData.getValue( "UserLevel", levelString );
                bool ok;
                QE::UserLevels levelInt = QE::UserLevelsValue( levelString, ok );
                if( ok )
                {
                    setUserLevel( levelInt );
                }

                // Get the number of expected main windows
                int numMainWindows = 0;
                QEGuiData.getValue( "MainWindows", numMainWindows );

                // Create the main windows. They will restore themselves
                setupProfile( NULL, app->getParams()->pathList, "", app->getParams()->substitutions );
                for( int i = 0; i < numMainWindows; i++ )
                {
                    MainWindow* mw = new MainWindow( app, "", "", "", QEFormMapper::nullHandle(), false, NULL, NULL );
                    mw->show();
                }

                releaseProfile();
            }
            break;

        // Second resore phase.
        // This application has done its work. The widgets that have been created will be able to act on the second phase
        case SaveRestoreSignal::RESTORE_QEFRAMEWORK:
            break;

    }

}

// end
