/*  InstanceManager.cpp
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
/* This class is used to manage maintaining only a single instance of QEGui when required.

    On creation it attempts to connect to a server on an already running QEGui.
    If it can't connect, it assumes it is the only version of QEGui and starts the server itself.

    When QEGui starts with a -s parameter, indicating that only a single instance of the application is required,
    It passes all the startup parameters to the handball() method of this class.

    If this class has connected to a server, it passes the startup parameters to the server and then stops. The QEGui
    application running the server then starts a new main window based on the handballed parameters.
    If this class is not connected to a server, the handball() method returns indicating that it was unable to handball
    the parameters and this instance of QEGui should start a new window regardless of the -s parameter.
*/

#include <stdlib.h>
#include <InstanceManager.h>
#include <MainWindow.h>
#include <ContainerProfile.h>
#include <QEGui.h>
#include <QMessageBox>

#define QEGUISERVERNAME "QEGuiInstance"

// Construction
// Look for an instance server, and if can't find one, then start one
instanceManager::instanceManager( QEGui* appIn ) : QObject( appIn )
{
    app = appIn;

    // Build the server name to be <user>_<QEGUISERVERNAME>
    // The username is included since (on Linux at least) a temporary file is
    // created (in /tmp on Linux) using the name of the server.
    // For multiple users this avoids conflict. Also, if a temporary
    // file has been left following a crash by a different user, the
    // temporary file can't be deleted if permissions don't allow. This results
    // in the server unable to start.
    QByteArray userEnv;
#ifdef WIN32 //for windows
    userEnv = qgetenv( "USERNAME" );
#else //for Mac or Linux
    userEnv = qgetenv( "USER" );
#endif
    QString serverName = QString( QEGUISERVERNAME ).append( "_" ).append( userEnv );

    // Create a socket
    socket = new QLocalSocket(this);
    socket->connectToServer( serverName, QIODevice::WriteOnly );

    // Assume no server
    server = NULL;

    // If no other instance is found, discard the socket  and start a server for future instances
    // (no socket will be used to indicate no other instance)
    if( !socket->waitForConnected( 1000 ) )
    {
        delete socket;
        socket = 0;

        // Kill any other server.
        // This is required if an eariler instance has crashed
        QLocalServer::removeServer( serverName );

        // Start a server to listen for other instances of QEGui starting
        server = new QLocalServer( this );
        connect( server, SIGNAL(newConnection()), this, SLOT(connected()));
        if( !server->listen( serverName ))
        {
            qDebug() << QString( "Couldn't start server. On Linux, check if there is a temporary file /tmp/" ).append( serverName ).append( " and delete it" );
            delete server;
            server = NULL;
        }
    }
}

// Destruction
instanceManager::~instanceManager()
{
    delete socket;
    if( server )
        delete server;
}

// Pass on the startup parameters to an already existing instance of the application
bool instanceManager::handball( startupParams* params )
{
    // If no other instance, do nothing
    if( !socket )
        return false;

    // Build a serial copy of the parameters
    QByteArray ba;
    params->setSharedParams( ba );

    // Send it to the other instance
    socket->write( ba );

    // Wait until it is gone.
    // The application will be closing down almost immedietly and if we don't
    // wait here it doesn't get written
    socket->waitForBytesWritten ( 10000 );

    // Return indicating startup request has been hand-balled to another instance of QEGui
    return true;
}

// Slot called when the server starts
void instanceManager::connected()
{
    client = server->nextPendingConnection();
    connect( client, SIGNAL(readyRead ()), this, SLOT(readParams()));
}

// Read the startup parameters from a new instance of the application.
// The new instance wants this old instance to do the work.
// It has passed on the startup parameters and will now exit
void instanceManager::readParams()
{
    QByteArray ba(client->readAll());
    startupParams params;
    if( params.getSharedParams( ba ) )
    {
        newWindow( params );
    }
}

// Create new main windows
void instanceManager::newWindow( const startupParams& params )
{
    // Set up the profile for the new windows
    ContainerProfile profile;

    // Get the persistance manager
    PersistanceManager* persistanceManager = profile.getPersistanceManager();

    QString configName; // configuration to restore. If empty, not restoring.

    // If auto save configuration is enabled, and there is an auto saved configuration,
    // then we did not shut down cleanly. Offer to restart with the auto saved configuration.
    if( !params.disableAutoSaveConfiguration && persistanceManager->isConfigurationPresent(  params.configurationFile, QE_CONFIG_NAME, app->getAutoSaveConfigName() ) )
    {
        QMessageBox msgBox;
        msgBox.setText( "An automatically saved configuration has been found which indicates this application was not shut down properly (or another QEGui is running using the same configuration file).\n\n Would you like to restart with the auto-saved configuration?" );
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        switch ( msgBox.exec() )
        {
           case QMessageBox::Yes:
                // Yes, prepare to restore autosaved config
                configName = app->getAutoSaveConfigName();
                break;

            case QMessageBox::No:
            case QMessageBox::Cancel:
            default:
                // No, do nothing
                break;
        }

    }

    // If not restoring an autosaved configuration and we are restoring according to command line parameters, prepare to restore to the requested saved configuration
    if( configName.isEmpty() && params.restore )
    {
        configName = params.configurationName;
    }

    // If restoring, restore saved configuration
    if( !configName.isEmpty() )
    {
        // Ask the persistance manager to restore a configuration.
        // The persistance manager will signal all interested objects (including this application) that
        // they should collect and apply restore data.
        persistanceManager->restore( params.configurationFile, QE_CONFIG_NAME, configName );

        // If the restoration did not create any windows, warn the user.
        // This is especially important as an .ui file specified on the command line will now be opened,
        // or failing that, an empty window, neither of which will look like the expected configuration
        if( app->getMainWindowCount() == 0 )
        {
            QMessageBox::warning( 0, "Configuration Restore",
                                  QString( "Configuration restoration did not create any windows.\n"
                                           "Looked for configuration named '%1'.").arg( params.configurationName ) );
        }
    }

    // Not restoring, or if restoring didn't create any main windows, open the required guis
    if( configName.isEmpty() || app->getMainWindowCount() == 0 )
    {
        profile.setupProfile( NULL, params.pathList, "", params.substitutions );

        // If no files specified, open a single window without a file name
        if( !params.filenameList.count() )
        {
            MainWindow* mw = new MainWindow( app, "", "", params.defaultCustomisationName,
                                             QEFormMapper::nullHandle(),  true, NULL, NULL );
            mw->show();
        }

        // Files have been specified. Open a window for each of them
        else
        {
            for( int i = 0; i < params.filenameList.count(); i++ )
            {
                // If there is at least one window, ask it to open the new window using the normal action request method.
                // This is better than just creating a new window as the normal action request method will do
                // usefull stuff like finding and presenting an existing window if it exists.
                if( app->getMainWindowCount() )
                {
                   MainWindow* mw = app->getMainWindow( 0 );
                   mw->requestAction( QEActionRequests( params.filenameList[i], params.startupCustomisationName, QEActionRequests::OptionNewWindow, QEFormMapper::nullHandle() ) );
                }
                // If there are currently no windows, create one
                else
                {
                    MainWindow* mw = new MainWindow( app, params.filenameList[i],
                                                     "", params.startupCustomisationName,
                                                     QEFormMapper::nullHandle(), true, NULL, NULL );
                    mw->show();
                }
            }
        }

        profile.releaseProfile();
    }

    // Release the profile
}

// end
