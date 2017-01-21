/*  MainContext.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the Australian Synchrotron.
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2009, 2010 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

#include "MainContext.h"

// Construction
//
mainContext::mainContext( int argc, char *argv[], QWidget *parent ) : QObject( parent )
{
    instance = new QtSingleApplication( argc, argv );
    QStringList args = QCoreApplication::arguments();
    params.getStartupParams( args );

    share.setKey( "ASgui_instance_communication" );

    // Set up a connection so that new instances of this application can request that the current instance do the work
    QObject::connect( instance, SIGNAL(messageReceived(const QString&)), this, SLOT(newAppRequest(const QString&)));

}

// If there is another instance of the application running, pass on any parameters to it.
// Returns true if successfully hand-balled.
// Return false if not hand-balled. This includes errors as well an no other instance of this application
bool mainContext::handball()
{
    // Get the startup parameters from the command line arguments
    QStringList args = QCoreApplication::arguments();
    startupParams newParams;
    newParams.getStartupParams( args );

    // Don't pass the requests on if specifically asked not to
    if( newParams.singleApp )
    {
        return false;
    }

    // Don't pass the requests on if no other app to pass them on to!
    if( !instance->isRunning())
    {
        qDebug() << "no instance running";
        return false;
    }

    qDebug() << "another instance running";

    // Get the shared memory
    if( share.attach() )
    {
        qDebug() << "Already shared memory available (there shouldn't be - we create it here";
        return false;
    }

    // Build a serial copy of the parameters
    QByteArray ba;
    newParams.setSharedParams( ba );

    // Create the shared memory
    share.create( ba.size(), QSharedMemory::ReadWrite );

    // Lock the shared memory
    if( !share.lock() )
    {
        qDebug() << "Could not lock shared menory";
        return false;
    }

    // Copy the serialised startup parameters to the shared memory
    memcpy( share.data(), ba.data(), ba.size() );

    // Release the shared memory
    if( !share.unlock() )
        return false;

    // Notify the existing application
    qDebug() << "Waking up other instance";
    instance->sendMessage( "Wake up!" );

    // Release the shared memory
    share.detach();

    return true;
}

// Another instance of this application has started, noticed this application is already running,
// placed a set of startup parameters in shared memory and asked this instance of the application
// to deal with them
void mainContext::newAppRequest( const QString& message )
{
    qDebug() << message;

    // Get the shared memory

    if( !share.attach() )
    {
        qDebug() << "Could not attach";
        return;
    }

    // Lock the shared memory
    if( !share.lock() )
    {
        qDebug() << "Could not lock shared menory";
        return;
    }

    // Extract parameters from a serial copy of the parameters
    params.getSharedParams( share.data() );

    // Release the shared memory
    if( !share.unlock() )
        return;

    // Release the shared memory
    share.detach();


    // Create the main window.
    qDebug() << "Creating an new window";
    newWindow();
}

// Create a new main window
void mainContext::newWindow()
{
    qDebug() << "New window: filename: " << params.filename << "path: " << params.path << "substitutions: " << params.substitutions << "enableEdit: " << params.enableEdit;
    MainWindow* mw = new MainWindow( params.filename, params.path, params.substitutions, params.enableEdit );
    mw->show();
    instance->setActivationWindow( mw );
}

// Start event processing in this application instance
// Used if this instance of the application is the one managing all startup requests
int mainContext::exec()
{
    return instance->exec();
}
