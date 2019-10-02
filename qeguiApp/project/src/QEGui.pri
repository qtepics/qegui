# QEGui.pri
#
# This file is part of the EPICS QT Framework, and included into
# and as part of the overall QEGui.pro project file.
#

INCLUDEPATH += src

HEADERS += src/InstanceManager.h
SOURCES += src/InstanceManager.cpp

HEADERS += src/MainWindow.h
SOURCES += src/MainWindow.cpp
FORMS   += src/MainWindow.ui

FORMS   += src/MessageLog.ui

HEADERS += src/QEGui.h
SOURCES += src/QEGui.cpp

HEADERS += src/StartupParams.h
SOURCES += src/StartupParams.cpp

HEADERS += src/aboutDialog.h
SOURCES += src/aboutDialog.cpp
FORMS   += src/aboutDialog.ui

HEADERS += src/caQtDmInterface.h
SOURCES += src/caQtDmInterface.cpp

HEADERS += src/configAutoSave.h
SOURCES += src/configAutoSave.cpp

HEADERS += src/loginDialog.h
SOURCES += src/loginDialog.cpp

SOURCES += src/main.cpp

HEADERS += src/manageConfigDialog.h
SOURCES += src/manageConfigDialog.cpp
FORMS   += src/manageConfigDialog.ui

HEADERS += src/recentFile.h
SOURCES += src/recentFile.cpp

HEADERS += src/restoreDialog.h
SOURCES += src/restoreDialog.cpp
FORMS   += src/restoreDialog.ui

HEADERS += src/saveDialog.h
SOURCES += src/saveDialog.cpp
FORMS   += src/saveDialog.ui

HEADERS += src/saveRestoreManager.h
SOURCES += src/saveRestoreManager.cpp

# These ui files are loaded at runtime as ui files, uic is not invoked.
#
OTHER_FILES += src/ArchiveNameSearch.ui
OTHER_FILES += src/ArchiveStatus.ui
OTHER_FILES += src/General_PV_Edit.ui
OTHER_FILES += src/Plotter.ui
OTHER_FILES += src/PVCorrelation.ui
OTHER_FILES += src/PVDistribution.ui
OTHER_FILES += src/PVProperties.ui
OTHER_FILES += src/ScratchPad.ui
OTHER_FILES += src/StripChart.ui
OTHER_FILES += src/WaveformHistogram.ui

OTHER_FILES += src/QEGuiIcon.png
OTHER_FILES += src/QEGuiIcon.ico
OTHER_FILES += src/QEGuiCustomisationDefault.xml
OTHER_FILES += src/help_usage.txt
OTHER_FILES += src/help_general.txt

RESOURCES += src/QEGui.qrc

# end
