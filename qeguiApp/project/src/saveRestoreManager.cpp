/*  saveRestoreManager.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2009-2018 Australian Synchrotron
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
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

/* This class is used to manage saving and restoring for QEGui.
   The main item saved is the number of main windows saved so the restore will know how many main windows to restore.
   Each main window will save it's own info.
*/

#include <saveRestoreManager.h>
#include <MainWindow.h>
#include <QEGui.h>
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
                userLevelTypes meta;
                appElement.addValue ("UserLevel", QEUtilities::enumToString( meta, "userLevels", getUserLevel() ));
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
                userLevelTypes meta;
                userLevelTypes::userLevels levelInt;
                bool ok;
                levelInt = (userLevelTypes::userLevels)QEUtilities::stringToEnum( meta, "userLevels", levelString, &ok );
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
