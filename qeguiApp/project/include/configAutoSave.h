/*  configAutoSave.h
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
 *  Copyright (c) 2016 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 *
 * DESCRIPTION:
 *
 * This class saves the current configuration at regular intervals.
 * This configuration is deleted on a clean exit.
 *
 * When starting the application can check for a saved configuration.
 * If present, this implies the application did not exit cleanly.
 * The user can be given the opportunity to restore to the saved configuraiton.
 * This may also be useful when debugging a crash.
 *
 * The standard configuration save/restore mechanism available to the user is used to implement this auto save function.
 *
 */

#ifndef CONFIGAUTOSAVE_H
#define CONFIGAUTOSAVE_H

#include <QTimer>
#include <QDateTime>
#include <ContainerProfile.h>

class configAutoSave;

class configAutoSaveSlots: public QObject
{
    Q_OBJECT

public:
    explicit configAutoSaveSlots( configAutoSave* ownerIn) { owner = ownerIn; }
    configAutoSave* owner;

public slots:
    void save();  // Called when an auto-save is due

};

class configAutoSave
{
public:
    explicit configAutoSave();
    ~configAutoSave();

    void startAutoSaveConfig( const QString configFileIn, const bool disableAutoSaveConfiguration );
    void stopAutoSaveConfig();
    QString getAutoSaveConfigStatus();

    virtual void saveConfiguration( PersistanceManager* pm, const QString configFile, const QString rootName, const QString configName, const bool warnUser  ) = 0; // Overridden in QEGui.h
    void save( const QString configName );                        // Called when an auto-save is due (including on exit)

    QString getAutoSaveConfigName();

private:
    QTimer timer;
    configAutoSaveSlots* mySlots;
    QString configFile;
    bool running;
    QDateTime lastSave;
    ContainerProfile profile;           // Environment profile for QE applications and QE widgets

};

#endif // CONFIGAUTOSAVE_H
