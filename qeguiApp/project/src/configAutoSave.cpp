/*  configAutoSave.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  SPDX-FileCopyrightText: 2016-2025 Australian Synchrotron
 *  SPDX-License-Identifier: LGPL-3.0-only
 *
 *  Author:     Andrew Rhyder
 *  Maintainer: Andrew Starritt
 *  Contact:    andrews@ansto.gov.au
 */

#include "configAutoSave.h"
#include <persistanceManager.h>

#define CONFIG_AUTO_SAVE_NAME "AutoSave"
#define CONFIG_EXIT_SAVE_NAME "ExitSave"

// Called by timer when an auto-save is due
void configAutoSaveSlots::save()
{
    owner->save( CONFIG_AUTO_SAVE_NAME );
}


// Configuration Auto Save construction
configAutoSave::configAutoSave()
{
    running = false;

    mySlots = new configAutoSaveSlots( this );
    QObject::connect( &timer, SIGNAL(timeout()), mySlots, SLOT(save()));
}


// Configuration Auto Save destruction
configAutoSave::~configAutoSave()
{
}

// Start automatic saving of current configuration if required
// Called on start of QEGui when there is something to save
void configAutoSave::startAutoSaveConfig( const QString configFileIn, const bool disableAutoSaveConfiguration )
{
    configFile = configFileIn;

    // Start saving every 30 seconds if enabled
    if( !disableAutoSaveConfiguration )
    {
        timer.start( 30000 );
        running = true;
    }
    else
    {
        running = false;
    }
}

// Stop automatic saving of current configuration
// Called befoer exiting QEGui
void configAutoSave::stopAutoSaveConfig()
{
    // If not running, do nothing
    if( !running )
    {
        return;
    }

    // Ensure no more timer events unless restarted
    timer.stop();

    // Save the current configuration as the configuration at the time the application was neatly shut down
    save( CONFIG_EXIT_SAVE_NAME );

    // Remove any auto saved configuration
    PersistanceManager* pm = profile.getPersistanceManager();
    QStringList names;
    names.append( CONFIG_AUTO_SAVE_NAME );
    pm->deleteConfigs( configFile, QE_CONFIG_NAME, names, false );

    // Flag not running
    running = false;
}

// Provide a status summary string
QString configAutoSave::getAutoSaveConfigStatus()
{
    QString status;

    // Build running message
    if( running )
    {
        status = "Configuration auto-save is running. ";

        // When did it last run (if ever)
        if( lastSave.isValid() )
        {
            status += QString( "Last saved at " ) .append( lastSave.toString( "hh:mm:ss.zzz dd/MM/yyyy" ));
        }
        else
        {
            status += "No configuration has been saved yet.";
        }
    }

    // Build not running message
    else
    {
        status = "Configuration auto-save is not running.";
    }

    return status;
}

// Return the name of the configuration used for Auto Save
QString configAutoSave::getAutoSaveConfigName()
{
    return CONFIG_AUTO_SAVE_NAME;
}

// Called as a timer event when an auto-save is due
void configAutoSave::save( const QString configName )
{
    PersistanceManager* pm = profile.getPersistanceManager();

    // Save the configuration according to the user's requirements
    saveConfiguration( pm, configFile, QE_CONFIG_NAME, configName, false );

    // Note when configuration was last saved
    lastSave = QDateTime::currentDateTime();
    getAutoSaveConfigStatus();
}

