# $File: //ASP/tec/gui/qegui/trunk/qeguiApp/Makefile $
# $Revision: #5 $
# $DateTime: 2017/05/01 17:24:01 $
# Last checked in by: $Author: starritt $
#
# This make file is the bridge beween the EPICS build system and the Qt project qmake/make paradigm.
#

TOP=..

# Pick up configure/RELEASE definitions.
#
include $(TOP)/configure/CONFIG
#
# Note: we do not include the rule $(TOP)/configure/RULES

# Note: this makefile calls up qmake. Depending on which version of Qt you are using, you may
# have to setup qmake in your development PATH to call up the required version of qmake.
# E.g. on Linux with Qt4, create the following symbolic link:
#
#    /usr/local/bin/qmake -> /usr/bin/qmake-qt4
#

# Create environment variables from Make variable as defined in $(TOP)/configure/RELEASE
# Note use of := which avoids "*** Recursive variable `EPICS_BASE' references itself (eventually)" error
#
export EPICS_BASE := $(EPICS_BASE)
export QE_FRAMEWORK := $(QE_FRAMEWORK)

# Commands
#
NOOP = :

# Directories of interest.
#
SOURCE_DIR = project
TARGET_DIR = $(TOP)/bin/$(EPICS_HOST_ARCH)

# Files
#
MAKEFILE = Makefile.$(EPICS_HOST_ARCH)
PROJECT  = QEGuiApp.pro

ifeq ($(OS),Windows_NT)
   BINFILE = qegui.exe
else
   BINFILE = qegui
endif

TARGET=$(TARGET_DIR)/$(BINFILE)

.PHONY: all install clean uninstall always

all: $(TARGET)

install: $(TARGET)

# The project file places the executable in bin/architecture directory, no additonal install required.
# Note: we always run this step
#
$(TARGET) : $(SOURCE_DIR)/$(MAKEFILE)  always
	@echo "=== Building $(BINFILE) application"                 && \
	cd  $(SOURCE_DIR)                                           && \
	$(MAKE) -j3  -f $(MAKEFILE)                                 && \
	echo "=== Complete"


$(SOURCE_DIR)/$(MAKEFILE) : $(SOURCE_DIR)/$(PROJECT)
	@echo "=== Running qmake - generating $(MAKEFILE)"          && \
	cd  $(SOURCE_DIR)                                           && \
	qmake -o $(MAKEFILE) $(PROJECT) -r


# Do a qt clean, then delete all qmake generated Makefiles.
# Note the : "command" is a null command in bash.
#
clean:
	cd  $(SOURCE_DIR) && test -f $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) clean && $(RM) $(MAKEFILE) || $(NOOP)


uninstall:
	rm -f $(TARGET)

always:

# end
