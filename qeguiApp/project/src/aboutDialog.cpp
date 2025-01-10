/*  aboutDialog.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2013-2025 Australian Synchrotron
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
 *  Original author: Andrew Rhyder
 *  Maintained by:   Andrew Starritt
 *
 *  Contact details:
 *  as-open-source@ansto.gov.au
 *  800 Blackburn Road, Clayton, Victoria 3168, Australia.
 */

/* Description:
 *
 * Presents a dialog containing information about the QEGui application
 * such as version numbers and credits
 */

#include "aboutDialog.h"
#include "ui_aboutDialog.h"
#include <QtGlobal>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLibraryInfo>
#include <QMenu>
#include <QMetaType>
#include <QProcessEnvironment>
#include <QString>
#include <QVariantList>
#include <QEPlatform.h>
#include <QEFrameworkVersion.h>
#include <caQtDmInterface.h>

#define DEBUG qDebug() << "aboutDialog" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
aboutDialog::aboutDialog (
    QString QEFrameworkVersionUILoader,       // Version info and the build date/time at compile time of the copy of QEPlugin library loaded by QUiLoader while creating QE widgets

    QString macroSubstitutions,               // Macro substitutions (-m parameter)
    QStringList pathList,                     // Path list (-p parameter)
    QStringList envPathList,                  // Path list (environment variable)
    QString userLevel,                        // Current user level

    QStringList windowTitles,                 // Window titles (windowTitles, windowFiles, windowMacroSubstitutions must be same length)
    QStringList windowFiles,                  // Window file name (windowTitles, windowFiles, windowMacroSubstitutions must be same length)
    QStringList windowMacroSubstitutions,     // Window macro substitutions (windowTitles, windowFiles, windowMacroSubstitutions must be same length)

    QString configurationFile,                // Configuration file
    QString configurationName,                // Configuration name
    QString autoSaveConfigStatus,             // Current state of Configuration Auto Save

    QString defaultWindowCustomisationFile,   // Default window customisation file
    QString defaultWindowCustomisationName,   // Default window customisation name
    QString startupWindowCustomisationName,   // Startup window customisation name (for windows created at startup)
    QString internalDefaultCustomisationName, // Internal Default window customisation set name
    QString windowCustomisationLoadLog,       // Log of window customisations

    int disconnectedCount,                    // Number of disconnected channels
    int connectedCount,                       // Number of connected channels

    QWidget* parent) :
    QEDialog (parent),
    ui (new Ui::aboutDialog)
{
   ui->setupUi (this);

   // Versions and build times of QEGui itself and the QE framework library.
   //
   const QString QEGuiVersion = QString (QE_VERSION_STRING " " QE_VERSION_DATE_TIME);
   const QString QEFrameworkVersionQEGui = QString ("%1 %2")
                                               .arg (QEFrameworkVersion::getString())
                                               .arg (QEFrameworkVersion::getDateTime());

   // Versions
   ui->QEGuiVersionLabel->setText (QEGuiVersion);

#if QT_VERSION < 0x060000
   ui->QtInstalledPluginsLabel->setText (QLibraryInfo::location (QLibraryInfo::PluginsPath));
#else
   ui->QtInstalledPluginsLabel->setText (QLibraryInfo::path (QLibraryInfo::PluginsPath));
#endif
   ui->QEFrameworkVersionQEGuiLabel->setText (QEFrameworkVersionQEGui);
   ui->QEFrameworkVersionUILoaderLabel->setText (QEFrameworkVersionUILoader);

   // Get and basic framework attributes and modify if needs be.
   //
   QString attributes = QEFrameworkVersion::getAttributes();
   CaQtDmInterface::updateAttributes (attributes);

   ui->QEFrameworkAttributes->setText (attributes);

   // Note: all but the QT version strings are prefixed by the text "EPICS", "ACAI" and "QWT".
   //
   ui->QTVersionLabel->setText ("QT " + QEFrameworkVersion::getQtVersionStr());
   ui->EPICSVersionLabel->setText (QEFrameworkVersion::getEpicsVersionStr());
   ui->ACAIVersionLabel->setText (QEFrameworkVersion::getAcaiVersionStr());
   ui->QWTVersionLabel->setText (QEFrameworkVersion::getQwtVersionStr());

   // Environment
   //
   ui->userLevelLabel->setText (userLevel);
   ui->macroSubstitutionsTextEdit->setPlainText (macroSubstitutions);

   // Paths
   ui->currentPathTextEdit->setPlainText (QDir::currentPath ());

   for (int i = 0; i < pathList.count (); i++) {
      ui->pathParameterList->addItem (pathList[i]);
   }

   for (int i = 0; i < envPathList.count (); i++) {
      ui->pathVariableList->addItem (envPathList[i]);
   }

   QString pathVarName;

#ifdef WIN32
   pathVarName = "Path";
#else
   pathVarName = "PATH";
#endif

   QProcessEnvironment env = QProcessEnvironment::systemEnvironment ();

   QString message = QString ("Couldn't find environment variable: ").append (pathVarName);
   ui->systemPathLabelTextEdit->setPlainText (env.value (pathVarName, message));

   // Windows
   int rowCount = std::min (std::min (windowTitles.count (), windowFiles.count ()),
                           windowMacroSubstitutions.count ());
   ui->windowsTable->setRowCount (rowCount);

   for (int i = 0; i < rowCount; i++) {
      Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;

      QTableWidgetItem* windowTitle = new QTableWidgetItem (windowTitles[i]);
      windowTitle->setFlags (flags);

      QTableWidgetItem* windowFile = new QTableWidgetItem (windowFiles[i]);
      windowFile->setFlags (flags);

      QTableWidgetItem* windowMacroSubs = new QTableWidgetItem (windowMacroSubstitutions[i]);
      windowMacroSubs->setFlags (flags);

      ui->windowsTable->setItem (i, 0, windowTitle);
      ui->windowsTable->setItem (i, 1, windowFile);
      ui->windowsTable->setItem (i, 2, windowMacroSubs);
   }

   if (rowCount) {
      ui->windowsTable->resizeColumnsToContents ();
   }
   ui->windowsTable->
       setHorizontalHeaderLabels (QStringList () << "Title" << "File" << "Macro Substitutions");

   // Configuration
   ui->configurationFileLabel->setText (configurationFile);
   ui->configurationNameLabel->setText (configurationName);
   ui->configurationAutoSaveStatusLabel->setText (autoSaveConfigStatus);

   // Customisation
   ui->defaultWindowCustomisationFileLabel->setText (defaultWindowCustomisationFile);
   ui->defaultWindowCustomisationNameLabel->setText (defaultWindowCustomisationName);
   ui->startupWindowCustomisationNameLabel->setText (startupWindowCustomisationName);
   ui->internalDefaultCustomisationLabel->setText (internalDefaultCustomisationName);
   ui->windowCustomisationLoadLogLabel->setText (windowCustomisationLoadLog);

   // Connections
   ui->disconnectedChannelsLabel->setText (QString ("%1").arg (disconnectedCount));
   ui->connectedChannelsLabel->setText (QString ("%1").arg (connectedCount));

   // Allow window ui file names to be copied.
   QTableWidget* table = ui->windowsTable;      // alias

   table->setContextMenuPolicy (Qt::CustomContextMenu);
   QObject::connect (table, SIGNAL (customContextMenuRequested (const QPoint &)),
                    this,  SLOT   (contextMenuRequested       (const QPoint &)));
}

//------------------------------------------------------------------------------
//
aboutDialog::~aboutDialog ()
{
   delete ui;
}

//------------------------------------------------------------------------------
//
QVariant aboutDialog::encode (const CopyMode mode, const int row)
{
   QVariantList result;

   result.append (QVariant (int (mode)));
   result.append (QVariant (row));

   return result;
}

//------------------------------------------------------------------------------
//
bool aboutDialog::decode (const QVariant data, CopyMode & mode, int &row)
{
   QTableWidget* table = ui->windowsTable;      // alias

   const QMetaType::Type vtype = QEPlatform::metaType( data );

   if (vtype != QMetaType::QVariantList) return false;

   const QVariantList dataArray = data.toList ();

   if (dataArray.size () != 2) return false;

   bool okay;
   int i;

   i = dataArray.value (0).toInt (&okay);
   if (!okay) return false;
   mode = CopyMode (i);

   row = dataArray.value (1).toInt (&okay);
   if (!okay) return false;

   if (row < 0 || row >= table->rowCount ()) {
      DEBUG "row" << row << "not in range 0 to" << table->rowCount ();
      return false;
   }

   return true;
}

//------------------------------------------------------------------------------
//
void aboutDialog::contextMenuRequested (const QPoint & pos)
{
   static const QPoint posOffset = QPoint (0, 16);  // allows for the header line.

   QTableWidget* table = qobject_cast <QTableWidget*>(this->sender ());
   if (!table) return;   // sanity check, just in case

   QTableWidgetItem* item = table->itemAt (pos);
   if (!item) return;    // sanity check, just in case

   const QPoint globalPos = table->mapToGlobal (pos + posOffset);
   if (item->column () != 1) return;   // column 1 is the ui file name column.

   QMenu* menu = new QMenu ();

   QAction* action = new QAction ("Copy ui pathname", menu);

   action->setCheckable (false);
   action->setData (encode (cmPathname, item->row ()));
   menu->addAction (action);

   action = new QAction ("Copy ui filename", menu);
   action->setCheckable (false);
   action->setData (encode (cmFilename, item->row ()));
   menu->addAction (action);

   QObject::connect (menu, SIGNAL (triggered            (QAction*)),
                    this, SLOT   (contextMenuTriggered (QAction*)));

   menu->exec (globalPos);

   delete menu;
}

//------------------------------------------------------------------------------
//
void aboutDialog::contextMenuTriggered (QAction* action)
{
   QTableWidget* table = ui->windowsTable;      // alias

   if (!action) return;   // sanity check

   const QVariant data = action->data ();
   bool okay;
   CopyMode mode;
   int row;

   okay = decode (data, mode, row);
   if (!okay) return;

   QTableWidgetItem* item = table->item (row, 1);

   if (!item) return;   // sanity check

   QString uiName = item->text ();

   if (mode == cmFilename) {
      // Extract the file name from the full path name.
      QFileInfo fi (uiName);
      uiName = fi.fileName ();
   }

   QClipboard* cb = QApplication::clipboard ();
   if (!cb) return;   // sanity check

   cb->setText (uiName);
}

// end
