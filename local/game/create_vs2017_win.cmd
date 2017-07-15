@echo off
cd project
..\..\..\library\buildtools\genie\genie.exe /outpath=../build vs2017
if errorlevel 1 goto error
goto exit

:error
pause

:exit
cd ..
