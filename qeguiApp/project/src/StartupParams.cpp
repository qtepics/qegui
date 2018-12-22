/*  StartupParams.cpp
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

// Manage startup parameters. Parse the startup parameters in a command line, serialize
// and unserialize parameters when passing them to another application instance.

#include "StartupParams.h"

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QDir>
#include <ContainerProfile.h>
#include <QEFrameworkVersion.h>
#include <QEAdaptationParameters.h>
#include <QECommon.h>

#define DEBUG qDebug() << "StartupParams" << __LINE__ << __FUNCTION__ << "  "

#define LIMIT_SCALE(scale)  LIMIT( scale, 40.0, 400.0 )

// Construction
startupParams::startupParams()
{
    // Set default values.
    //
    adjustScale = 100.0;
    fontScale = 100.0;
    enableEdit = false;
    disableMenu = false;
    disableStatus = false;
    disableAutoSaveConfiguration = false;
    singleApp = false;
    printHelp = false;    // not serialized
    printVersion = false; // not serialized
    restore = false;
    configurationName = PersistanceManager::defaultName;
    configurationFile = "QEGuiConfig.xml";
}

// Destruction
startupParams::~startupParams() { }  // place holder

// Unserialize application startup parameters
// This must match startupParams::setSharedParams()
// Return true if paramaters are extracted correctly
bool startupParams::getSharedParams( const QByteArray& in )
{
    double scaleValue;
    char * scale;
    int i;

    // Initialise parameters
    filenameList.clear();
    pathList.clear();
    substitutions.clear();
    configurationName.clear();
    configurationFile.clear();

    // Extract parameters from a stream of bytes.
    int len = 0;
    const char* d = in.constData();

    // Check enough buffer to contain at least the version
    if( in.size() < 3 )
    {
        qDebug() << "Start parameters have been received and ignored by this application as they are too short (" << in.size() << "bytes). This is not even long enough to the contain version number";
        return false;
    }

    // Check parameters were packaged by the same version.
    int majorVersion   = d[len]; len += 1;
    int minorVersion   = d[len]; len += 1;
    int releaseVersion = d[len]; len += 1;

    if( majorVersion   != (int)(QEFrameworkVersion::getMajor()) ||
        minorVersion   != (int)(QEFrameworkVersion::getMinor()) ||
        releaseVersion != (int)(QEFrameworkVersion::getRelease()) )
    {
        qDebug() << "Startup parameters have been received (and ignored) from an application with a different version.\n" \
                    "This application version: "          << QEFrameworkVersion::getMajor() << "." \
                                                          << QEFrameworkVersion::getMinor() << "." \
                                                          << QEFrameworkVersion::getRelease() << "\n"
                    "Recieved from application version: " << majorVersion << "." \
                                                          << minorVersion << "." \
                                                          << releaseVersion;
        return false;
    }

    // Unpackage parameters
    scale = (char *) &scaleValue;

    for (i = 0; i < (int) sizeof (adjustScale); i++) {
       scale [i] = d[len];    len += 1;
    }
    adjustScale = LIMIT_SCALE (scaleValue);

    for (i = 0; i < (int) sizeof (fontScale); i++) {
       scale [i] = d[len];    len += 1;
    }
    fontScale = LIMIT_SCALE (scaleValue);

    enableEdit                   = (bool)(d[len]);    len += 1;
    disableMenu                  = (bool)(d[len]);    len += 1;
    disableStatus                = (bool)(d[len]);    len += 1;
    disableAutoSaveConfiguration = (bool)(d[len]);    len += 1;
    singleApp                    = (bool)(d[len]);    len += 1;
    restore                      = (bool)(d[len]);    len += 1;

    int fileCount = d[len];            len += 1;
    for( int i = 0; i < fileCount; i++ )
    {
        filenameList.append( &(d[len]) );  len += filenameList[i].size()+1;
    }

    int pathCount = d[len];            len += 1;
    for( int i = 0; i < pathCount; i++ )
    {
        pathList.append( &(d[len]) );  len += pathList[i].size()+1;
    }
    substitutions.append( &(d[len]) ); len += substitutions.size()+1;

    configurationName = QString( &(d[len]) ); len += configurationName.size()+1;

    return true;
}

// Serialize application startup parameters
// This must match startupParams::getSharedParams()
void startupParams::setSharedParams( QByteArray& out )
{
    // Convert parameters into a stream of bytes.
    int len = 0;
    const char * scale;
    int i;

    out[len++] = QEFrameworkVersion::getMajor();
    out[len++] = QEFrameworkVersion::getMinor();
    out[len++] = QEFrameworkVersion::getRelease();

    scale = (const char *) &adjustScale;
    for (i = 0; i < (int) sizeof (adjustScale); i++) {
       out[len++] = scale [i];
    }

    scale = (const char *) &fontScale;
    for (i = 0; i < (int) sizeof (fontScale); i++) {
       out[len++] = scale [i];
    }

    out[len++] = enableEdit;
    out[len++] = disableMenu;
    out[len++] = disableStatus;
    out[len++] = disableAutoSaveConfiguration;
    out[len++] = singleApp;
    out[len++] = restore;

    out[len++] = filenameList.count();
    for( i = 0; i < filenameList.count(); i++ )
    {
        out.insert( len, filenameList[i].toLatin1() ); len += filenameList[i].size();   out[len++] = '\0';
    }

    out[len++] = pathList.count();
    for( i = 0; i < pathList.count(); i++ )
    {
        out.insert( len, pathList[i].toLatin1() );     len += pathList[i].size();       out[len++] = '\0';
    }

    out.insert( len, substitutions.toLatin1() );       len += substitutions.size();     out[len++] = '\0';

    out.insert( len, configurationName.toLatin1() );   len += configurationName.size(); out[len++] = '\0';
    out.insert( len, configurationFile.toLatin1() );   len += configurationFile.size(); out[len++] = '\0';

}


// Extract required parameters from argv and argc
// which are in QCoreApplication::arguments()
// Also check for any environment variabeles and value form
// adaptation parameter ini file (if available)
//
bool startupParams::getStartupParams()
{
    QEAdaptationParameters ap ("QEGUI_");
    QEOptions opts;

    double td;
    QString ts;

    td = ap.getFloat ("adjust_scale", this->adjustScale);
    this->adjustScale = opts.getFloat ('a', td);

    td = ap.getFloat ("font_scale", this->fontScale);
    this->fontScale = opts.getFloat ('f', td);

    this->singleApp = ap.getBool ("single") || opts.getBool ('s');
    this->enableEdit = ap.getBool ("edit") || opts.getBool ('e');
    this->disableMenu = ap.getBool ("disable_menu") || opts.getBool ('b');
    this->disableStatus = ap.getBool ("disable_status") || opts.getBool ('u');
    this->disableAutoSaveConfiguration = ap.getBool ("disable_autosave") || opts.getBool ('o');

    // syntax is -r [configuration_name]
    //
    ts = ap.getString ("restore", this->configurationName);
    this->configurationName = opts.getString ('r', ts);
    this->restore = ap.getBool ("restore") || opts.getBool ('r');

    ts = ap.getString ("configuration", this->configurationFile);
    this->configurationFile = opts.getString ('c', ts);

    const QChar ps = ContainerProfile::platformSeperator();
    ts = ap.getString ("path", this->pathList.join (ps));
    ts = opts.getString ('p', ts);
    this->pathList = ts.isEmpty() ? QStringList() : ts.split (ps);

    ts = ap.getString ("macros", this->substitutions);
    this->substitutions = opts.getString ('m', ts);

    ts = ap.getString ("customisation_file", this->customisationFile);
    this->customisationFile = opts.getString ('w', ts);

    ts = ap.getString ("customisation_name", this->startupCustomisationName);
    this->startupCustomisationName = opts.getString ('n', ts);

    ts = ap.getString ("default_customisation_name", this->defaultCustomisationName);
    this->defaultCustomisationName = opts.getString ('d', ts);

    ts = ap.getString ("title", this->applicationTitle);
    this->applicationTitle = opts.getString ('t', ts);

    // Option only.
    this->printHelp    = opts.getBool ("help", 'h');
    this->printVersion = opts.getBool ("version", 'v');

    // Extract any parameters
    //
    const int pc = opts.getParameterCount();
    for (int j = 0; j < pc; j++) {
        this->filenameList.append (opts.getParameter (j));
    }

    // Any un-recognised options are ignored
    //
    return true;
}

// end
