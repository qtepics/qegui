@ECHO OFF
REM run_qegui.bat
REM
REM Wrapper script for qegui. We do not define QT_PLUGIN_PATH as a global
REM envirionment variable as this can break other applications. So we
REM define locally here.

REM the plugin libraries are in the designer sub directory
set QT_PLUGIN_PATH=%QE_FRAMEWORK%

REM Note the initial and final space. This is required on windows
set QE_ARCHIVE_LIST=" CR01ARC04:17665/mgmt/bpl/ BSXARC01:17665/mgmt/bpl/ SR02IR01ARC01:17665/mgmt/bpl/ SR03ID01ARC01:17665/mgmt/bpl/ SR03BM01ARC01:17665/mgmt/bpl/ MX3ARC01:17665/mgmt/bpl/ SR05ID01ARC01:17665/mgmt/bpl/ SR08ID01ARC01:17665/mgmt/bpl/ NANOARC01:17665/mgmt/bpl/ MCTARC01:17665/mgmt/bpl/ ADS1ARC01:17665/mgmt/bpl/ ADS2ARC01:17665/mgmt/bpl/ SR10BM01ARC01:17665/mgmt/bpl/ SR12ID01ARC01:17665/mgmt/bpl/ MEXARC01:17665/mgmt/bpl/ SR13ID01ARC01:17665/mgmt/bpl/ SR14ID01ARC01:17665/mgmt/bpl/ "


REM run QeGui. Pass on any/all parameters.
START qegui.exe %*
ECHO Starting QeGui. This console will close itself in 5 seconds.
timeout 5

REM end
