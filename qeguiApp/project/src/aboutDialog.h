/*  aboutDialog.h
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
 * such as version numbers and credits.
 */

#ifndef QEGUI_ABOUT_DIALOG_H
#define QEGUI_ABOUT_DIALOG_H

#include <QEDialog.h>
#include <QAction>
#include <QPoint>
#include <QVariant>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QEDialog
{
   Q_OBJECT

public:
   explicit aboutDialog (
       QString QEFrameworkVersionUILoader,     // Version info and the build date/time at compile time of the copy of QEPlugin library loaded by QUiLoader while creating QE widgets

       QString macroSubstitutions,             // Macro substitutions (-m parameter)
       QStringList pathList,                   // Path list (-p parameter)
       QStringList envPathList,                // Path list (environment variable)
       QString userLevel,                      // Current user level

       QStringList windowTitles,               // Window titles (windowTitles, windowFiles, windowMacroSubstitutions must be same length)
       QStringList windowFiles,                // Window file name (windowTitles, windowFiles, windowMacroSubstitutions must be same length)
       QStringList windowMacroSubstitutions,   // Window macro substitutions (windowTitles, windowFiles, windowMacroSubstitutions must be same length)

       QString configurationFile,              // Configuration file
       QString configurationName,              // Configuration name
       QString autoSaveConfigStatus,           // Current state of Configuration Auto Save

       QString defaultWindowCustomisationFile, // Default window customisation file
       QString defaultWindowCustomisationName, // Default window customisation name
       QString startupWindowCustomisationName, // Window customisation name for windows created at startup
       QString currentCustomisation,           // Internal default window customisation name (the default default!)
       QString windowCustomisationLoadLog,     // Log of window customisations

       int disconnectedCount,                  // Number of disconnected channels
       int connectedCount,                     // Number of connected channels

       QWidget *parent = 0);

   ~aboutDialog();

private:
   enum CopyMode {
      cmPathname,   // full path name to copy/paste buffer.
      cmFilename    // file name only to copy/paste buffer.
   };

   // Encode and decode QAction variant data.
   QVariant encode (const CopyMode mode, const int row);
   bool decode (const QVariant data, CopyMode& mode, int& row);

   Ui::aboutDialog *ui;

private slots:
   void contextMenuRequested (const QPoint& pos);
   void contextMenuTriggered (QAction* action);
};

#endif // QEGUI_ABOUT_DIALOG_H
