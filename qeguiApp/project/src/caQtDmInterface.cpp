/*  caQtDmInterface.cpp
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

#include "caQtDmInterface.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QVariant>
#include <MainWindow.h>
#include <QELabel.h>
#include <QEAdaptationParameters.h>

#define DEBUG qDebug () << "caQtDmInterface" << __LINE__ << __FUNCTION__ << "  "

// Only include PSI caQtDM integration if required.
// To include PSI caQtDM stuff, don't define QE_USE_CAQTDM directly, define the
// environment variable QE_CAQTDM to be processed by QEGuiApp.pro
// Also define environment variable QE_CAQTDM_MAJOR_VERSION to appropriate version
//
#if QE_CAQTDM_MAJOR_VERSION != 4
#undef QE_USE_CAQTDM
#warning caQtDm integration disabled -  only caQtDm version 4 supported.
#endif

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
// CaQtDmFormInterface methods
//==============================================================================
//
// Static class members
//
MutexKnobData* CaQtDmFormInterface::mutexKnobData = NULL;
CaQtDmFormInterface::InterfacesMap CaQtDmFormInterface::interfaces = CaQtDmFormInterface::InterfacesMap ();


//------------------------------------------------------------------------------
//
CaQtDmFormInterface::CaQtDmFormInterface (MainWindow* mainWindowIn,
                                          CaQtDmInterface* parent) : QObject (parent)
{
   this->mainWindow = mainWindowIn;
   this->caQtDMLib = NULL;
   this->proxyWidget = NULL;

#ifdef QE_USE_CAQTDM
   // PSI data acquisition
   if (!CaQtDmFormInterface::mutexKnobData) {
      CaQtDmFormInterface::mutexKnobData = new MutexKnobData();
      loadPlugins loadplugins;
      loadplugins.loadAll (CaQtDmFormInterface::interfaces, CaQtDmFormInterface::mutexKnobData);
   }
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
CaQtDmFormInterface::~CaQtDmFormInterface ()
{
   if (this->proxyWidget) delete this->proxyWidget;
   if (this->caQtDMLib) {
      this->sendCloseEvent ();
      this->caQtDMLib->deleteLater();
      this->caQtDMLib = NULL;
   }
}

//------------------------------------------------------------------------------
//
void CaQtDmFormInterface::sendCloseEvent ()
{
#ifdef QE_USE_CAQTDM
   if (this->caQtDMLib) {
      QApplication::sendEvent (this->caQtDMLib, new QCloseEvent());
   }
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
void CaQtDmFormInterface::createLibrary (const QString& macroSubstitutions, QEForm* gui)
{
#ifdef QE_USE_CAQTDM
   // Destroy previous library if need be.
   //
   if (this->caQtDMLib){
      this->caQtDMLib->deleteLater ();
   }

   MessageWindow* msgWindow = NULL;
   this->caQtDMLib = new CaQtDM_Lib (this->mainWindow, "", macroSubstitutions,
                                     CaQtDmFormInterface::mutexKnobData, interfaces,
                                     msgWindow, false, gui);

   this->caQtDMLib->allowResizing (true);  // This avoid resize event seg fault.

   // PSI event->handler connection
   //
   QObject::connect (
      this->caQtDMLib, SIGNAL(Signal_OpenNewWFile (const QString&, const QString&, const QString&, const QString&)),
      this,            SLOT  (openNewFile         (const QString&, const QString&, const QString&, const QString&)));

   // Has user specified using standard context menu for PSI widgets?
   // Note: this is a run time descision.
   //
   QEAdaptationParameters ap ("QEGUI_");
   if (ap.getBool ("caqtdm_context_menu")) {
      this->setupContextMenu (gui);
   }

#else
   // Avoid unused parameter warnings
   //
   Q_UNUSED (macroSubstitutions)
   Q_UNUSED (gui)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
QString CaQtDmFormInterface::adl2caqtdmChecking (const QString& fileName)
{
   QString result = fileName;

#ifdef QE_USE_CAQTDM
   const bool isMedmFile = fileName.endsWith (".adl");
   if (isMedmFile) {
      result.chop (3);
      result.append ("ui");
   }
#endif // QE_USE_CAQTDM

   return result;
}

//------------------------------------------------------------------------------
//
void CaQtDmFormInterface::setupContextMenu (QEForm* gui)
{
#ifdef QE_USE_CAQTDM
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

      // We assume all and only PSI classess start with ca (for now).
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
#else
   // Avoid unused parameter warnings
   Q_UNUSED (gui)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
void CaQtDmFormInterface::showContextMenu (const QPoint& pos, QWidget* widget)
{
#ifdef QE_USE_CAQTDM
   if (widget && this->proxyWidget) {
      // Extract the PV name from the PSI widget.
      // Use channel property??
      //
      QVariant channel = widget->property ("channel");
      if (channel.type() == QVariant::String) {
         QString pvName = channel.toString ();
         this->proxyWidget->setVariableName (pvName, 0);

         // Convert pos (which is relative to the widget) to a position
         // which is relative to the proxy widget.
         //
         QPoint globalPos = widget->mapToGlobal (pos);
         QPoint proxyPos = this->proxyWidget->mapFromGlobal (globalPos);

         this->proxyWidget->showContextMenu (proxyPos);
      }
   }
#else
   // Avoid unused parameter warnings
   Q_UNUSED (pos)
   Q_UNUSED (widget)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
// slot
void CaQtDmFormInterface::showStandardContextMenu (const QPoint& pos)
{
#ifdef QE_USE_CAQTDM
   QWidget* widget = qobject_cast<QWidget *>(sender());
   if (widget && this->proxyWidget) {
      this->showContextMenu (pos, widget);
   }
#else
   // Avoid unused parameter warnings
   Q_UNUSED (pos)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
// slot - call back form a PSI caQtDm widget
//
void CaQtDmFormInterface::openNewFile (const QString& inputFile,
                                       const QString& macroString,
                                       const QString&,
                                       const QString&)
{
#ifdef QE_USE_CAQTDM
   ProfilePublisher publisher (new QEWidget (this->mainWindow), macroString);

   // Convert .adl extension to .ui
   //
   QString uiName = CaQtDmFormInterface::adl2caqtdmChecking (inputFile);

   this->mainWindow->launchGui (uiName, "", "", QEActionRequests::OptionNewWindow,
                                false, QEFormMapper::nullHandle () );
#else
   // Avoid unused parameter warnings
   Q_UNUSED (inputFile);
   Q_UNUSED (macroString);
#endif // QE_USE_CAQTDM
}

//==============================================================================
// CaQtDmInterface methods
//==============================================================================
//
CaQtDmInterface::CaQtDmInterface (MainWindow* parent) : QObject (parent)
{
   this->mainWindow = parent;
}

//------------------------------------------------------------------------------
//
CaQtDmInterface::~CaQtDmInterface ()
{
#ifdef QE_USE_CAQTDM
   // Note: the actual caQtDmFormInterface objects are own by this and therefore
   // will be automatically deleted which in turn will invoke the sendCloseEvent.
   //
   this->formInterfaceMap.clear();
#endif
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::sendCloseEvent (QEForm* gui)
{
   if (!gui) return;  //  sanity check

#ifdef QE_USE_CAQTDM
   CaQtDmFormInterface* caQtDmFormInterface = this->formInterfaceMap[gui];
   if (caQtDmFormInterface) {
       this->formInterfaceMap.remove (gui);
       delete caQtDmFormInterface;
   }
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::createLibrary (const QString& macroSubstitutions, QEForm* gui)
{
   if (!gui) return;  //  sanity check

#ifdef QE_USE_CAQTDM
   // Create form interface object and create the associated CaQtDM_Lib instance
   // and insert into the interface map.
   //
   CaQtDmFormInterface* caQtDmFormInterface = new CaQtDmFormInterface (this->mainWindow, this);
   caQtDmFormInterface->createLibrary (macroSubstitutions, gui);
   this->formInterfaceMap.insert (gui, caQtDmFormInterface);
#else
   // Avoid unused parameter warnings
   //
   Q_UNUSED (macroSubstitutions)
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
// static
void CaQtDmInterface::updateAttributes (QString& attributes)
{
#ifdef QE_USE_CAQTDM
   if (!attributes.isEmpty() && (attributes != "None")) {
      attributes.append (", caQtDm integration");
   } else {
      attributes = "caQtDm integration";
   }
#else
   // Avoid unused parameter warnings
   //
   Q_UNUSED (attributes)
#endif // QE_USE_CAQTDM
}

// end
