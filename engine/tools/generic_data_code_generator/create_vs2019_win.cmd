@echo off
cd project
..\..\..\buildtools\genie\genie.exe /outpath=../build vs2019
if errorlevel 1 goto error
goto exit

:error
pause

:exit
cd ..