# $File: //ASP/tec/gui/qegui/trunk/qeguiApp/project/QEGuiApp.pro $
# $Revision: #20 $
# $DateTime: 2021/09/29 16:41:26 $
# Last checked in by: $Author: starritt $
#
# Copyright (c) 2009-2021 Australian Synchrotron
#
# This file is part of the EPICS QT Framework, initially developed at the Australian Synchrotron.
#
# The EPICS QT Framework is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# The EPICS QT Framework is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with the EPICS QT Framework. If not, see <http://www.gnu.org/licenses/>.
#
# Author: Andrew Rhyder
# Contact details: andrew.rhyder@synchrotron.org.au
#
# To analyse code performance using the GNU gprof profiling tool:
# - Include the following two lines
# - Clean the project
# - Run qmake
# - Build the code
# - Run the program from a terminal
# - analyse the results with the command: gprof <your-program-name>
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

# Points to the target directoy in which bin/EPICS_HOST_ARCH/qegui
# will be created. This follows the regular EPICS Makefile paradigm.
#
TOP=../..

message ("QT_VERSION = "$$QT_MAJOR_VERSION"."$$QT_MINOR_VERSION"."$$QT_PATCH_VERSION )

# Qt 4 configuration
equals( QT_MAJOR_VERSION, 4 ) {
    CONFIG += uitools designer
    QT += core gui network
    warning( "**** QT4 is getting old. Active QT4 support will has ceased. ****" )
}

# Qt 5 configuration
equals( QT_MAJOR_VERSION, 5 ) {
    CONFIG += qwt
    QT += core gui network uitools designer
    QT += printsupport  # required by caQtDM
}

## Added by Anton Mezger (check why and include if required)
#unix {
#    QMAKE_CXXFLAGS += "-g"
#    QMAKE_CFLAGS_RELEASE += "-g"
#}

TARGET = qegui
TEMPLATE = app

# Determine EPICS_BASE
_EPICS_BASE = $$(EPICS_BASE)

# Check EPICS dependancies
isEmpty( _EPICS_BASE ) {
    error( "EPICS_BASE must be defined. Ensure EPICS is installed and EPICS_BASE environment variable is defined." )
}

_EPICS_HOST_ARCH = $$(EPICS_HOST_ARCH)
isEmpty( _EPICS_HOST_ARCH ) {
    error( "EPICS_HOST_ARCH must be defined. Ensure EPICS is installed and EPICS_HOST_ARCH environment variable is defined." )
}

# Determine QE framework library
#
_QE_FRAMEWORK = $$(QE_FRAMEWORK)
isEmpty( _QE_FRAMEWORK ) {
    error( "QE_FRAMEWORK must be defined. Ensure EPICS is installed and EPICS_HOST_ARCH environment variable is defined." )
}

# Install the generated plugin library and include files in QE_TARGET_DIR if defined.
_QE_TARGET_DIR = $$(QE_TARGET_DIR)
isEmpty( _QE_TARGET_DIR ) {
    INSTALL_DIR = $$TOP
    message( "QE_TARGET_DIR is not defined. The QE GUI application will be installed into the <top> directory." )
} else {
    INSTALL_DIR = $$(QE_TARGET_DIR)
    message( "QE_TARGET_DIR is defined. The QE GUI application will be installed directly into" $$INSTALL_DIR )
}

# The APPLICATION ends up here.
#
DESTDIR = $$INSTALL_DIR/bin/$$(EPICS_HOST_ARCH)

# Place all intermediate generated files in architecture specific locations
#
MOC_DIR        = O.$$(EPICS_HOST_ARCH)/moc
OBJECTS_DIR    = O.$$(EPICS_HOST_ARCH)/obj
UI_DIR         = O.$$(EPICS_HOST_ARCH)/ui
RCC_DIR        = O.$$(EPICS_HOST_ARCH)/rcc
MAKEFILE       = Makefile.$$(EPICS_HOST_ARCH)

# We don't get this include path for free - need to be explicit.
#
INCLUDEPATH += O.$$(EPICS_HOST_ARCH)/ui


#===========================================================
# Integration of PSI's caQtDM in QEGui.
# If integration is required, define environment variable QE_CAQTDM

_QE_CAQTDM = $$(QE_CAQTDM)
isEmpty( _QE_CAQTDM ) {
    message( "Integration with PSI's caQtDM will NOT be included in QEGui. If you want caQtDM integrated, download and" )
    message( "... build it and define the environment variable QE_CAQTDM to point to the caQtDM_Project directory." )
} else {
    message( "QE_CAQTDM is defined: $$(QE_CAQTDM)")
    message( "Integration with PSI's caQtDM will be included in QEGui. caQtDM libraries and include files will be expected" )
    message( "... and be located using the QE_CAQTDM environment variable (which will should point to the to point to the" )
    message( "... caQtDM_Project directory). Undefine environment variable QE_CAQTDM if you do not want caQtDM integration." )

#   check Version. 
    _QE_CAQTDM_MAJOR_VERSION = $$(QE_CAQTDM_MAJOR_VERSION)

    equals( _QE_CAQTDM_MAJOR_VERSION, 4 ){
        message( "!!! The downloaded caQtDM major version must be 4, e.g. V4.2.4, otherwise it might have an incompatable build error." )
    } else {
        error( "When using caQtDM, QE_CAQTDM_MAJOR_VERSION must be defined. Allowed value is 4. It is currently defined as '"$$(QE_CAQTDM_MAJOR_VERSION)"'.")
    }

    DEFINES += QE_USE_CAQTDM
    DEFINES += QE_CAQTDM_MAJOR_VERSION="$$(QE_CAQTDM_MAJOR_VERSION)"
}

#===========================================================

include (src/QEGui.pri)

# Include the following gdbmacros line for debugging only
#SOURCES += <YOUR-QTSDK-DIRECTORY>/share/qtcreator/gdbmacros/gdbmacros.cpp

# Include header files from the QE framework
#
INCLUDEPATH += $$(QE_FRAMEWORK)/include

# Explicity add framework, and hence QEFrameworkVersion.h, to the dependacy path
# so that changes to the version/release numbers force relavent recompilations.
#
DEPENDPATH += $$(QE_FRAMEWORK)/include

LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom

# Set run time path for shared libraries
#
unix: QMAKE_LFLAGS += -Wl,-rpath,$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)

LIBS += -L$$(QE_FRAMEWORK)/lib/$$(EPICS_HOST_ARCH) -lQEFramework
unix: QMAKE_LFLAGS += -Wl,-rpath,$$(QE_FRAMEWORK)/lib/$$(EPICS_HOST_ARCH)

# Qt 5+ configuration
greaterThan( QT_MAJOR_VERSION, 4 ) {
    win32:RC_ICONS += src/QEGuiIcon.ico
}

#===========================================================
# PSI's caQtDM integration
#
isEmpty( _QE_CAQTDM ) {
} else {

    # Note,the following checks for QWT are repeated in framework.pro - keep in sync
    # Check QWT is accessable. Check there is a chance QMAKEFEATURES includes a path to
    # the qwt features directory, or that QWT_INCLUDE_PATH is defined.
    # Note, qwt install may set up QMAKEFEATURES to point to the product features file,
    # rather than the directory. Not sure if this is wrong, but changing it to the
    # directory works (C:\Qwt-6.0.1\features\qwt.prf to  C:\Qwt-6.0.1\features)
    _QWT_INCLUDE_PATH = $$(QWT_INCLUDE_PATH)
    isEmpty( _QWT_INCLUDE_PATH ) {
        _QMAKEFEATURES = $$(QMAKEFEATURES)
        _QWT_FEATURE = $$find( _QMAKEFEATURES, [Q|q][W|w][T|t] )
        isEmpty( _QWT_FEATURE ) {
            error( "Qwt does not appear to be available. It is required when building QEGui with caQtDM integration. I've checked if 'qwt' is in QMAKEFEATURES or if QWT_INCLUDE_PATH is defined" )
        }
    }

    #===========================================================
    # Cribbed from framework.pro
    # I expect we only need include file path here, as the qe framework
    # library will oull in the qwt libraries for us.
    # Keep consistant with framework.pro (approx lines 326 - 365)
    #
    # The following QWT include path and library path are only required if
    # qwt was not installed fully, with qwt available as a Qt 'feature'.
    # When installed as a Qt 'feature' all that is needed is CONFIG += qwt (above)
    #
    INCLUDEPATH += $$(QWT_INCLUDE_PATH)

    # Depending on build, the qwt library below may need to be -lqwt or -lqwt6
    # The 'scope' labels Debug and Release need to have first letter capitalised for it to work in win32.
    #
    win32 {
        _QWT_ROOT = $$(QWT_ROOT)
        isEmpty( _QWT_ROOT ) {
            error( "QWT_ROOT is not defined. It is required when building the QE framework on windows, e.g. C:/qwt-6.1.3/" )
        }

        message( "Using QWT_ROOT environment variable to locate QWT library: $$(QWT_ROOT)" )
        LIBS += -L$$(QWT_ROOT)/lib

        Debug {
            message( "Using qwtd (not qwt) for this debug build" )
            LIBS += -lqwtd
        }
        Release {
            message( "Using qwt (not qwtd) for this release build" )
            LIBS += -lqwt
        }
    }

    unix {
        _QWT_ROOT = $$(QWT_ROOT)
        isEmpty( _QWT_ROOT ) {
            message( "QWT_ROOT is not defined, so using default location of QWT library" )
            LIBS += -lqwt
        } else {
            message( "Using QWT_ROOT environment variable to locate QWT library: $$(QWT_ROOT)" )
            LIBS += -L$$(QWT_ROOT)/lib -lqwt
            QMAKE_LFLAGS += -Wl,-rpath,$$(QWT_ROOT)/lib
        }
    }


    #===========================================================
    # Add caQtDM source locations
    #
    equals( _QE_CAQTDM_MAJOR_VERSION, 3 ){
        INCLUDEPATH += $(QE_CAQTDM)/caQtDM_Lib/src \
                       $(QE_CAQTDM)/caQtDM_QtControls/src \
                       $(QWT_INCLUDE_PATH)
    } else {
        INCLUDEPATH += $(QE_CAQTDM)/caQtDM_Lib/src \
                       $(QE_CAQTDM)/caQtDM_Lib/caQtDM_Plugins \
                       $(QE_CAQTDM)/caQtDM_Viewer/parser \
                       $(QE_CAQTDM)/caQtDM_QtControls/src \
                       $(QWT_INCLUDE_PATH)
    }

    # Include QtPrintSupport for Qt version 5
    #
    equals( QT_MAJOR_VERSION, 5 ) {
       INCLUDEPATH += $$(QTINC)/QtPrintSupport
    }

    # Reference caQtDM library. Look in installed location if supplied, otherwise, caQtDM project area.
    #
    _QE_CAQTDM_LIB = $$(QE_CAQTDM_LIB)
    isEmpty( _QE_CAQTDM_LIB ) {
        equals( _QE_CAQTDM_MAJOR_VERSION, 3 ){
            message( "QE_CAQTDM_LIB is not defined so looking for caQtDM library in $QE_CAQTDM/caQtDM_Lib" )
            _QE_CAQTDM_LIB = $(QE_CAQTDM)/caQtDM_Lib
        } else {
            message( "QE_CAQTDM_LIB is not defined so looking for caQtDM library in $QE_CAQTDM/caQtDM_Binaries" )
            _QE_CAQTDM_LIB = $(QE_CAQTDM)/caQtDM_Binaries
        }
    }

    equals( _QE_CAQTDM_MAJOR_VERSION, 3 ){
        LIBS += -L$${_QE_CAQTDM_LIB} -lcaQtDM_Lib
    } else {
        LIBS += -L$${_QE_CAQTDM_LIB} -lcaQtDM_Lib -lqtcontrols
    }
    unix: QMAKE_LFLAGS += -Wl,-rpath,$${_QE_CAQTDM_LIB}

}

# end
