/*
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
 *  Copyright (c) 2011 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include <QLocalSocket>
#include <QLocalServer>
#include <StartupParams.h>
#include <QDialog>

class QEGui;

class instanceManager: public QObject
{
    Q_OBJECT

public:
    instanceManager( QEGui* app );
    ~instanceManager();
    bool handball( startupParams* params );
    void newWindow( const startupParams& params );

private:
    QLocalSocket* socket;
    QLocalServer* server;
    QLocalSocket *client;

    QEGui* app;

public slots:
    void connected();
    void readParams();
};

#endif // INSTANCEMANAGER_H
