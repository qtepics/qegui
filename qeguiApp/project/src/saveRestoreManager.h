/*  saveRestoreManager.h
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

#ifndef SAVERESTOREMANAGER_H
#define SAVERESTOREMANAGER_H

#include <QObject>
#include <ContainerProfile.h>
#include <StartupParams.h>

class QEGui;

class saveRestoreManager: public QObject, ContainerProfile
{
    Q_OBJECT

public:
    saveRestoreManager( QEGui* appIn );
    ~saveRestoreManager();

private:
    ContainerProfile profile;

    QEGui* app;

public slots:
    void saveRestore( SaveRestoreSignal::saveRestoreOptions option );
};

#endif // SAVERESTOREMANAGER_H
