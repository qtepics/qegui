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
class CaQtDmInterface;

/// These classes provides the interface to the PSI caQtDM library, which allows
/// caQtDM widgets to be fully functional within qegui.
/// The class module uses the QE_USE_CAQTDM and QE_CAQTDM_VERSION macros to
/// determine which, if any, functionality is invoked.
//
// CaQtDmFormInterface is essentially a private class, but is declared in the
// header file to allow moc to work as expected.
//
// This code was previously embedded within MainWindow.cpp
//
class CaQtDmFormInterface : public QObject
{
   Q_OBJECT
private:
   // Construction
   explicit CaQtDmFormInterface (MainWindow* mainWindow,    // QEGui MainWindow
                                 CaQtDmInterface* parent);

   // Destruction
   ~CaQtDmFormInterface();

   // Set close event to the caQtDMLib instance.
   void sendCloseEvent ();

   // create a CaQtDM_Lib instance.
   void createLibrary (const QString& macroSubstitutions, QEForm* gui);

private:
   // We use ProxyWidget as a typical QE widget for context menu handling.
   ProxyWidget* proxyWidget;

   void showContextMenu (const QPoint& pos, QWidget *w);
   void setupContextMenu (QEForm* gui);

   // handle medm filename - converts .adl extension to .ui
   static QString adl2caqtdmChecking (const QString& fileName);

   MainWindow* mainWindow;
   CaQtDM_Lib* caQtDMLib;

   typedef QMap<QString, ControlsInterface*> InterfacesMap;
   static MutexKnobData* mutexKnobData;
   static InterfacesMap interfaces;

private slots:
   void showStandardContextMenu (const QPoint& pos);
   void openNewFile (const QString& inputFile, const QString& macroString,
                     const QString& geometry, const QString& resizeString);

   friend class CaQtDmInterface;
};


//------------------------------------------------------------------------------
//
class CaQtDmInterface : public QObject
{
public:
   // Modifies the QE framework attributes to include CaQtDm if is integrated.
   //
   static void updateAttributes (QString& attributes);

   // Construction
   explicit CaQtDmInterface (MainWindow* parent);  // QEGui MainWindow

   // Destruction
   ~CaQtDmInterface();

   // Set close event to the caQtDMLib instance.
   void sendCloseEvent (QEForm* gui = NULL);

   // create a CaQtDM_Lib instance.
   void createLibrary (const QString& macroSubstitutions, QEForm* gui);

private:
   MainWindow* mainWindow;       // parent

   // Each QEForm (there may be more than one, i.e. tabs and docs) gets
   // its own CaQtDmFormInterface object.
   //
   typedef QMap<QEForm*, CaQtDmFormInterface*> CaQtDmFormInterfacesMap;
   CaQtDmFormInterfacesMap formInterfaceMap;
};

#endif  // CA_QT_DM_INTERFACE_H
