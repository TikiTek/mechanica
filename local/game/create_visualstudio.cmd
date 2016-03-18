@echo off
cd project
..\..\..\library\buildtools\premake\premake5.exe /outpath=../build vs2015
cd ..

if errorlevel 1 goto error
goto exit

:error
pause

:exit