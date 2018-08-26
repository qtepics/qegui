/*  instanceManager.h
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2011-2018 Australian Synchrotron
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

#ifndef QEGUI_INSTANCE_MANAGER_H
#define QEGUI_INSTANCE_MANAGER_H

#include <QLocalSocket>
#include <QLocalServer>
#include <StartupParams.h>
#include <QDialog>

class QEGui;

class instanceManager: public QObject
{
    Q_OBJECT

public:
    explicit instanceManager( QEGui* app );
    ~instanceManager();
    bool handball( startupParams* params );
    void newWindow( const startupParams& params );

private:
    QLocalSocket* socket;
    QLocalServer* server;
    QLocalSocket* client;

    QEGui* app;

public slots:
    void connected();
    void readParams();
};

#endif // QEGUI_INSTANCE_MANAGER_H
