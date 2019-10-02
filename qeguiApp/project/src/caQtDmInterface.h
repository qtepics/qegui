/*  caQtDmInterface.h
 * 
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2018-2019 Australian Synchrotron
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
 *    Andrew Starritt
 *  Contact details:
 *    andrews@ansto.gov.au
 */

#ifndef CA_QT_DM_INTERFACE_H
#define CA_QT_DM_INTERFACE_H

#include <QString>
#include <QObject>
#include <QEForm.h>

// PSI classes - differed
class MutexKnobData;
class CaQtDM_Lib;
class loadPlugins;
class ControlsInterface;

// Other classes - differed
class MainWindow;
class ProxyWidget;

/// This class provides the interface to the PSI caQtDM library, which allows
/// caQtDM widgets to be fully functional within qegui.
/// The class module uses the QE_USE_CAQTDM and QE_CAQTDM_VERSION macros to
/// determine which, if any, functionality is invoked.
//
// This code was previously embedded within MainWindow.cpp
//
class CaQtDmInterface : public QObject
{
    Q_OBJECT
public:
   // Construction
   explicit CaQtDmInterface (MainWindow* parent);   // QEGui MainWindow

   // Destruction
   ~CaQtDmInterface();

   // Set close event to the caQtDMLib instance.
   void sendCloseEvent ();

   // create a CaQtDM_Lib instance.
   void createLibrary (const QString& macroSubstitutions, QEForm* gui);

private:
   ProxyWidget* proxyWidget;  // We use ProxyWidget as a typical QE widget for context menu handling.

   void showContextMenu (const QPoint& pos, QWidget *w);
   void setupContextMenu (QEForm* gui);

   // handle medm filename - converts .adl extension to .ui
   static QString adl2caqtdmChecking (const QString& fileName);

   MainWindow* mainWindow;       // parent
   CaQtDM_Lib* caQtDMLib;

   static MutexKnobData* mutexKnobData;

   typedef QMap<QString, ControlsInterface*> InterfacesMap;
   static InterfacesMap interfaces;

private slots:
   void showStandardContextMenu (const QPoint& pos);
   void openNewFile (const QString& inputFile, const QString& macroString,
                     const QString& geometry, const QString& resizeString);
};

#endif  // CA_QT_DM_INTERFACE_H
