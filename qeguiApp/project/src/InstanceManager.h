/*  InstanceManager.h
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  SPDX-FileCopyrightText: 2011-2025 Australian Synchrotron
 *  SPDX-License-Identifier: LGPL-3.0-only
 *
 *  Author:     Andrew Rhyder
 *  Maintainer: Andrew Starritt
 *  Contact:    andrews@ansto.gov.au
 */

#ifndef QEGUI_INSTANCE_MANAGER_H
#define QEGUI_INSTANCE_MANAGER_H

#include <QLocalSocket>
#include <QLocalServer>
#include <StartupParams.h>
#include <QObject>

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
