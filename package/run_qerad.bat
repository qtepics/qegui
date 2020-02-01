@ECHO OFF
REM run_qerad.bat
REM
REM Wrapper script for qerad. We do not define QT_PLUGIN_PATH as a global
REM envirionment variable as this can break other applications. So we
REM define locally here.

REM the plugin libraries are in the designer sub directory
set QT_PLUGIN_PATH=C:\Program Files(x86)\Australian Synchrotron\EPICS Qt

REM run QeGui. Pass on any/all parameters.

ECHO Starting QeRad.
START qerad.exe %*

REM end
