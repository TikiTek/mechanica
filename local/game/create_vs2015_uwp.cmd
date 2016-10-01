@echo off
cd project
..\..\..\library\buildtools\genie\genie.exe /outpath=../build /vs=uwp vs2015
cd ..

if errorlevel 1 goto error
goto exit

:error
pause

:exit