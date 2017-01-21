/*  StartupParams.cpp
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
 *  Copyright (c) 2009, 2010 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

// Manage startup parameters. Parse the startup parameters in a command line, serialize
// and unserialize parameters when passing them to another application instance.

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QDir>
#include <ContainerProfile.h>
#include <QEFrameworkVersion.h>

#include <QECommon.h>

#include "StartupParams.h"


#define LIMIT_SCALE(scale)  LIMIT( scale, 40.0, 400.0 )


// Construction
startupParams::startupParams()
{
    adjustScale = 100.0;
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
bool startupParams::getStartupParams( QStringList args )
{
    // Discard application name
    // (At least one argument should always be present - the application name - but checking since only need to check
    if( args.count() )
    {
        args.removeFirst();
    }

    // Get switches and filenames.
    // Switches may be separate or grouped.
    // Any parameters not associated with a switch are considered filenames.
    // Switches that precede a parameter (-p, -m) may be grouped. Associated
    // parameters are then expected in the order the switches were specified.
    // Examples:
    // -e -p /home
    // -epm /home PUMP=02
    while( args.size() )
    {
        // If next is a switch, get the switch and assocaited parameters
        if( args[0].left(1) == QString( "-" ) )
        {
            // Get the next argument
            QString arg = args[0];
            args.removeFirst();

            // Remove the leading '-' and process the argument if there is anything left of it
            while( arg.remove(0,1).size() )
            {
                // Identify the argument by the next letter
                switch( arg[0].toLatin1() )
                {

                   // 'Adjust Scale' flag
                   // Take next non switch parameter as macro substitutions
                   case 'a':
                       // Get the scaling (next parameter, if present, and as long as it isn't a switch)
                       if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                       {
                          QVariant image( QVariant::String );
                          double value;
                          bool okay;

                          image = args [0];
                          args.removeFirst();
                          value = image.toDouble( &okay );
                          if( !okay ){
                             return false;
                          }
                          adjustScale = LIMIT_SCALE( value );

                       } else {
                           return false;
                       }
                       break;

                    // 'Editable' flag
                    case 'e':
                        enableEdit = true;
                        break;

                    // 'Single App' flag
                    case 's':
                        singleApp = true;
                        break;

                    // Help flag
                    //
                    case 'h':
                        printHelp = true;
                        break;

                    // Version flag
                    //
                    case 'v':
                        printVersion = true;
                        break;

                    // 'menu bar disabled' flag
                    case 'b':
                        disableMenu = true;
                        break;

                    // 'status bar disabled' flag
                    case 'u':
                        disableStatus = true;
                        break;

                    // 'auto save configuration disabled' flag
                    case 'o':
                        disableAutoSaveConfiguration = true;
                        break;

                    // 'restore configuration' flag
                    // Take next non switch parameter as the configuration name
                    // If next parameter is a switch, use default configuration
                    case 'r':
                        restore = true;
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            configurationName = args[0];
                            args.removeFirst();
                        }
                        break;

                    // 'configuration file' flag
                    // Take next non switch parameter as the configuration file
                    case 'c':
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            configurationFile = args[0];
                            args.removeFirst();
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    // 'window customisations file' flag
                    // Take next non switch parameter as the window customisations file
                    case 'w':
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            customisationFile = args[0];
                            args.removeFirst();
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    // 'window startup customisations' name
                    // Take next non switch parameter as the window customisation name for all windows startet on startup
                    case 'n':
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            startupCustomisationName = args[0];
                            args.removeFirst();
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    // 'window default customisations' name
                    // Take next non switch parameter as the default window customisation name
                    case 'd':
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            defaultCustomisationName = args[0];
                            args.removeFirst();
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    // 'Title'
                    // Take next non switch parameter as the QEGui default application title
                    case 't':
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            applicationTitle = args[0];
                            args.removeFirst();
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    // 'paths' flag
                    // Take next non switch parameter as path list
                    case 'p':
                        // Get the path list (next parameter, if present, and as long as it isn't a switch)
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            // Split the paths
                            QString pathParam = args[0];
                            pathList = pathParam.split( ContainerProfile::platformSeperator() );
                            args.removeFirst();
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    // 'macros' flag
                    // Take next non switch parameter as macro substitutions
                    case 'm':
                        // Get the macros (next parameter, if present, and as long as it isn't a switch)
                        if( args.count() >= 1 && args[0].left(1) != QString( "-" ) )
                        {
                            substitutions = args[0];
                            args.removeFirst();
                        } else {
                            return false;
                        }
                        break;

                    default:
                        // Unrecognised switch
                        return false;
                }
            }
        }

        // Next is not a switch, get a file name
        else
        {
            filenameList.append( args[0] );
            args.removeFirst();
        }
    }

    return true;
}
