/*  caQtDmInterface.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2018 Australian Synchrotron
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

#include "caQtDmInterface.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <MainWindow.h>
#include <QELabel.h>
#include <QEAdaptationParameters.h>

#define DEBUG qDebug () << "caQtDmInterface" << __LINE__ << __FUNCTION__ << "  "

// Only include PSI caQtDM integration if required.
// To include PSI caQtDM stuff, don't define QE_USE_CAQTDM directly, define the
// environment variable QE_CAQTDM to be processed by QEGuiApp.pro
//
#ifdef QE_USE_CAQTDM

#include <mutexKnobData.h>
#include <mutexKnobDataWrapper.h>
#include <caqtdm_lib.h>
#include <loadPlugins.h>

#else

// dummy differed classes
//
class MutexKnobData     : public QObject { };
class CaQtDM_Lib        : public QObject { };
class loadPlugins       : public QObject { };
class ControlsInterface : public QObject { };

#endif // QE_USE_CAQTDM


//==============================================================================
// ProxyWidget class and methods
//==============================================================================
// We use QELabel as a typical QEWidget rather than roll our own.
// This is uce to create the EPICS Qt standard context menu.
//
class ProxyWidget : public QELabel {
public:
   explicit ProxyWidget (MainWindow* parent);
   ~ProxyWidget () { }
};

//------------------------------------------------------------------------------
//
ProxyWidget::ProxyWidget (MainWindow* parent) : QELabel (parent)
{
   this->setVisible (false);
   this->setConsumer (parent);
}


//==============================================================================
// CaQtDmInterface methods
//==============================================================================
//
// Static class memebers
//
MutexKnobData* CaQtDmInterface::mutexKnobData = NULL;
CaQtDmInterface::InterfacesMap CaQtDmInterface::interfaces = CaQtDmInterface::InterfacesMap ();


//------------------------------------------------------------------------------
//
CaQtDmInterface::CaQtDmInterface (MainWindow* parent) : QObject (parent)
{
   this->mainWindow = parent;
   this->caQtDMLib = NULL;
   this->proxyWidget = NULL;

#ifdef QE_USE_CAQTDM
   // PSI data acquisition
#ifdef QE_CAQTDM_VERSION_3
   this->mutexKnobData = new MutexKnobData();
   MutexKnobDataWrapperInit (this->mutexKnobData);
#else
   // CaQtDm Version 4 or later
   if (!this->mutexKnobData) {
      CaQtDmInterface::mutexKnobData = new MutexKnobData();
      loadPlugins loadplugins;
      loadplugins.loadAll (CaQtDmInterface::interfaces, CaQtDmInterface::mutexKnobData);
   }
#endif

#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
CaQtDmInterface::~CaQtDmInterface ()
{
   if (this->proxyWidget) delete this->proxyWidget;
   if (this->caQtDMLib) this->caQtDMLib->deleteLater();
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::sendCloseEvent ()
{
#ifdef QE_USE_CAQTDM
   if (this->caQtDMLib) {
      QApplication::sendEvent (this->caQtDMLib, new QCloseEvent());
   }
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::createLibrary (const QString& macroSubstitutions, QEForm* gui)
{
#ifdef QE_USE_CAQTDM
   // Destroy previous library if need be.
   //
   if (this->caQtDMLib){
      this->caQtDMLib->deleteLater ();
   }
   this->caQtDMLib = NULL;

#ifdef QE_CAQTDM_VERSION_3
   this->caQtDMLib = new CaQtDM_Lib (caQtDMLib, "", macroSubstitutions,
                                     this->mutexKnobData, 0, false, gui);
#else
   // CaQtDm Version 4 or later
   this->caQtDMLib = new CaQtDM_Lib (this->mainWindow, "", macroSubstitutions,
                                     this->mutexKnobData, interfaces, 0, false, gui);

   // PSI event->handller connection
   //
   QObject::connect (
      this->caQtDMLib, SIGNAL(Signal_OpenNewWFile (const QString&, const QString&, const QString&, const QString&)),
      this,            SLOT  (openNewFile         (const QString&, const QString&, const QString&, const QString&)));

   // Has user specified standard context menu?
   //
   QEAdaptationParameters ap ("QEGUI_");
   if (ap.getBool ("caqtdm_context_menu")) {
      this->setupContextMenu (gui);
   }

#endif

#else
   // Avoid unused parameter warnings
   //
   Q_UNUSED (macroSubstitutions)
   Q_UNUSED (gui)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
QString CaQtDmInterface::adl2caqtdmChecking (const QString& fileName)
{
   QString result = fileName;

#ifdef QE_USE_CAQTDM
#ifndef QE_CAQTDM_VERSION_3
   const bool isMedmFile = fileName.endsWith (".adl");
   if (isMedmFile) {
      result.chop (3);
      result.append ("ui");
   }
#endif
#endif

   return result;
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::setupContextMenu (QEForm* gui)
{
#ifdef QE_USE_CAQTDM
#ifndef QE_CAQTDM_VERSION_3
   // CaQtDm Version 4 or later
   if (!gui) return;   // sanity check

   if (this->proxyWidget) {
      delete this->proxyWidget;
   }
   this->proxyWidget = new ProxyWidget (this->mainWindow);

   const QList<QWidget*> widgetList = gui->findChildren<QWidget*>();
   foreach (QWidget* widget, widgetList) {
      if (!widget) continue;                // sanity check
      if (!widget->metaObject()) continue;  // sanity check

      QString className (widget->metaObject()->className());

      // We assume all and only PSI classess start with ca (for now)
      //
      if (className.startsWith ("ca") && !widget->toolTip().isEmpty()) {
         widget->setContextMenuPolicy (Qt::CustomContextMenu);
         // Disconnect any existing connection and reconnect to the proxy object.
         //
         disconnect (widget, SIGNAL(customContextMenuRequested(const QPoint&)), 0, 0);
         connect (widget, SIGNAL(customContextMenuRequested (const QPoint&)),
                  this,   SLOT  (showStandardContextMenu    (const QPoint&)));
      }
   }

#endif
#else
   // Avoid unused parameter warnings
   Q_UNUSED (gui)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::showContextMenu( const QPoint& pos, QWidget* widget)
{
#ifdef QE_USE_CAQTDM
#ifndef QE_CAQTDM_VERSION_3

   if (!widget) return;
   // Extract the PV name from the PSI widget.
   QString pvName = widget->toolTip();
   pvName = pvName.remove(0, 57);
   pvName.chop(18);
   if (this->proxyWidget) {
      this->proxyWidget->setVariableName (pvName, 0);
      this->proxyWidget->showContextMenu (pos);
   }
#endif
#else
   // Avoid unused parameter warnings
   Q_UNUSED (pos)
   Q_UNUSED (widget)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
// slot
void CaQtDmInterface::showStandardContextMenu (const QPoint& pos)
{
#ifdef QE_USE_CAQTDM
#ifndef QE_CAQTDM_VERSION_3
   QWidget* widget = qobject_cast<QWidget *>(sender());
   if (widget && this->proxyWidget){
      this->showContextMenu (pos, widget);
   }
#endif
#else
   // Avoid unused parameter warnings
   Q_UNUSED (pos)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
// slot
void CaQtDmInterface::openNewFile (const QString& inputFile,
                                   const QString& macroString,
                                   const QString& geometry,
                                   const QString& resizeString)
{
#ifdef QE_USE_CAQTDM
#ifndef QE_CAQTDM_VERSION_3

   ProfilePublisher publisher (new QEWidget (this->mainWindow), macroString);

   // Convert  .adl extension to .ui
   //
   QString uiName = CaQtDmInterface::adl2caqtdmChecking (inputFile);

   this->mainWindow->launchGui (uiName, "", "", QEActionRequests::OptionNewWindow,
                                false, QEFormMapper::nullHandle () );

#endif
#else
   // Avoid unused parameter warnings
   Q_UNUSED (inputFile)
   Q_UNUSED (macroString)
#endif // QE_USE_CAQTDM

   // Avoid unused parameter warnings
   Q_UNUSED (geometry);
   Q_UNUSED (resizeString);
}

// end
