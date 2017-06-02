/*  aboutDialog.cpp
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
 *  Copyright (c) 2013,2017 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

/* Description:
 *
 * Presents a dialog containing information about the QEGui application such as version numbers and credits
 */

#include "aboutDialog.h"
#include "ui_aboutDialog.h"
#include <QString>
#include <QLabel>
#include <QLibraryInfo>
#include <QProcessEnvironment>
#include <QDir>

aboutDialog::aboutDialog( QString QEGuiVersion,                // Version info and the build date/time at compile time of QEGui
                          QString QEFrameworkVersionQEGui,     // Version info and the build date/time at compile time of the copy of QEPlugin library loaded by QEGui
                          QString QEFrameworkVersionUILoader,  // Version info and the build date/time at compile time of the copy of QEPlugin library loaded by QUiLoader while creating QE widgets
                          QString EPICSVersion,                // Version of EPICS base
                          QString QWTVersion,                  // Version of QWT

                          QString macroSubstitutions,          // Macro substitutions (-m parameter)
                          QStringList pathList,                // Path list (-p parameter)
                          QStringList envPathList,             // Path list (environment variable)
                          QString userLevel,                   // Current user level

                          QStringList windowTitles,               // Window titles (windowTitles, windowFiles, windowMacroSubstitutions must be same length)
                          QStringList windowFiles,                // Window file name (windowTitles, windowFiles, windowMacroSubstitutions must be same length)
                          QStringList windowMacroSubstitutions,   // Window macro substitutions (windowTitles, windowFiles, windowMacroSubstitutions must be same length)

                          QString configurationFile,              // Configuration file
                          QString configurationName,              // Configuration name
                          QString autoSaveConfigStatus,           // Current state of Configuration Auto Save

                          QString defaultWindowCustomisationFile,   // Default window customisation file
                          QString defaultWindowCustomisationName,   // Default window customisation name
                          QString startupWindowCustomisationName,   // Startup window customisation name (for windows created at startup)
                          QString internalDefaultCustomisationName, // Internal Default window customisation set name
                          QString windowCustomisationLoadLog,       //Log of window customisations

                          int disconnectedCount,                // Number of disconnected channels
                          int connectedCount,                   // Number of connected channels

                          QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);

    // Versions
    ui->QEGuiVersionLabel->setText( QEGuiVersion );

    ui->QtInstalledPluginsLabel->setText( QLibraryInfo::location ( QLibraryInfo::PluginsPath ) );
    ui->QEFrameworkVersionQEGuiLabel->setText( QEFrameworkVersionQEGui );
    ui->QEFrameworkVersionUILoaderLabel->setText( QEFrameworkVersionUILoader );

    // Note: the EPICS version string is prefixed by the text "EPICS".
    //
    ui->EPICSVersionLabel->setText( EPICSVersion );
    ui->QWTVersionLabel->setText( QWTVersion );

    // Environment
    ui->userLevelLabel->setText( userLevel );
    ui->macroSubstitutionsTextEdit->setPlainText( macroSubstitutions );

    // Paths
    ui->currentPathTextEdit->setPlainText( QDir::currentPath());

    for( int i = 0; i < pathList.count(); i++ )
    {
        ui->pathParameterList->addItem( pathList[i] );
    }

    for( int i = 0; i < envPathList.count(); i++ )
    {
        ui->pathVariableList->addItem( envPathList[i] );
    }

    QString pathVarName;
#ifdef WIN32
        pathVarName = "Path";
#else
        pathVarName = "PATH";
#endif

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    ui->systemPathLabelTextEdit->setPlainText( env.value( pathVarName, QString( "Couldn't find environment variable: " ).append( pathVarName ) ));

    // Windows
    int rowCount = std::min( std::min( windowTitles.count(), windowFiles.count() ), windowMacroSubstitutions.count() );
    ui->windowsTable->setRowCount( rowCount );

    for( int i = 0; i < rowCount; i++ )
    {
        Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;

        QTableWidgetItem* windowTitle = new QTableWidgetItem( windowTitles[i] );
        windowTitle->setFlags( flags );

        QTableWidgetItem* windowFile = new QTableWidgetItem( windowFiles[i] );
        windowFile->setFlags( flags );

        QTableWidgetItem* windowMacroSubs = new QTableWidgetItem( windowMacroSubstitutions[i] );
        windowMacroSubs->setFlags( flags );

        ui->windowsTable->setItem(i, 0, windowTitle );
        ui->windowsTable->setItem(i, 1, windowFile );
        ui->windowsTable->setItem(i, 2, windowMacroSubs );
    }

    if( rowCount )
    {
        ui->windowsTable->resizeColumnsToContents();
    }
    ui->windowsTable->setHorizontalHeaderLabels( QStringList() << "Title" << "File" << "Macro Substitutions" );

    // Configuration
    ui->configurationFileLabel->setText( configurationFile );
    ui->configurationNameLabel->setText( configurationName );
    ui->configurationAutoSaveStatusLabel->setText( autoSaveConfigStatus );

    // Customisation
    ui->defaultWindowCustomisationFileLabel->setText( defaultWindowCustomisationFile );
    ui->defaultWindowCustomisationNameLabel->setText( defaultWindowCustomisationName );
    ui->startupWindowCustomisationNameLabel->setText( startupWindowCustomisationName );
    ui->internalDefaultCustomisationLabel->setText( internalDefaultCustomisationName );
    ui->windowCustomisationLoadLogLabel->setText( windowCustomisationLoadLog );

    // Connections
    ui->disconnectedChannelsLabel->setText( QString( "%1" ).arg( disconnectedCount ));
    ui->connectedChannelsLabel->setText( QString( "%1" ).arg( connectedCount ));
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

// end
