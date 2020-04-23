@ECHO OFF
REM Generate qegui msi file.
REM

echo Running candle
candle.exe EPICS_Qt_Installer.wxs

echo Running light, generating version %1%
light.exe -ext WixUIExtension -out EPICS_Qt_Installer_%1% EPICS_Qt_Installer.wixobj

REM end
