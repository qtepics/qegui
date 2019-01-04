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

#define DEBUG qDebug () << "caQtDmInterface" << __LINE__ << __FUNCTION__ << "  "

// Only include PSI caQtDM integration if required.
// To include PSI caQtDM stuff, don't define QE_USE_CAQTDM directly, define the
// environment variable QE_CAQTDM to be processed by QEGuiApp.pro
//
// TODO: - change to QE_CAQTDM_SUPPORT??
//
#ifdef QE_USE_CAQTDM

#include <mutexKnobData.h>
#include <mutexKnobDataWrapper.h>
#include <caqtdm_lib.h>

#else

// dummy differed classes
//
class MutexKnobData : public QObject { };
class CaQtDM_Lib    : public QObject { };

#endif // QE_USE_CAQTDM

//------------------------------------------------------------------------------
//
CaQtDmInterface::CaQtDmInterface (QObject* parent) : QObject (parent)
{
   this->mutexKnobData = NULL;
   this->caQtDMLib = NULL;

#ifdef QE_USE_CAQTDM
   // PSI data acquisition
   //
   this->mutexKnobData = new MutexKnobData();
   MutexKnobDataWrapperInit (this->mutexKnobData);
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
CaQtDmInterface::~CaQtDmInterface ()
{
   if (this->mutexKnobData) this->mutexKnobData->deleteLater();
   if (this->caQtDMLib) this->caQtDMLib->deleteLater();
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::sendCloseEvent ()
{
#ifdef QE_USE_CAQTDM
   QApplication::sendEvent (this->caQtDMLib, new QCloseEvent());
#endif // QE_USE_CAQTDM
}

//------------------------------------------------------------------------------
//
void CaQtDmInterface::createLibrary (const QString& macroSubstitutions, QEForm* gui)
{
#ifdef QE_USE_CAQTDM
   // Destroy previous library if need be.
   //
   if (this->caQtDMLib) this->caQtDMLib->deleteLater ();
   this->caQtDMLib = NULL;

   // Which 'this' is this, does it matter?
   //
   this->caQtDMLib = new CaQtDM_Lib (this, "", macroSubstitutions,
                                     this->mutexKnobData, 0, false, gui);
#else
   // Avoid unused parameter warnings
   //
   Q_UNUSED (macroSubstitutions)
   Q_UNUSED (gui)
#endif // QE_USE_CAQTDM
}

// end

