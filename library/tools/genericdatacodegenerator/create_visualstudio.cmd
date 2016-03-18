@echo off
cd project
..\..\..\buildtools\premake\premake5.exe /outpath=../build vs2015
if errorlevel 1 goto error
goto exit

:error
pause

:exit
cd ..