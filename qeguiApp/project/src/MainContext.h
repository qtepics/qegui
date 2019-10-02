/*  MainContext.h
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

#ifndef MAINCONTEXT_H
#define MAINCONTEXT_H

#include <QObject>
#include <StartupParams.h>
#include <MainWindow.h>
#include <qtsingleapplication.h>

// Class to manage multiple instances of this application
// This class holds application startup parameters, recieves signals from other instances of this application
class mainContext : public QObject
{
    Q_OBJECT

  public:
    mainContext( int argc, char *argv[], QWidget* parent = 0 );
    bool handball();                // Pass start up details to an earlier instance of this application if present
    void newWindow();               // Create a new window
    int exec();                     // Start event processing in this application instance

  public slots:
    void newAppRequest( const QString& message);    // Slot to recieve notifications from aonther application instance

  private:
    startupParams params;           // Start up parameters parsed from command line arguments or from serialised arguments passed in shared memory
    QSharedMemory share;            // Shared memory used to pass start up arguments from another application instance
    QtSingleApplication* instance;  // Application instance manger

};

#endif // MAINCONTEXT_H
