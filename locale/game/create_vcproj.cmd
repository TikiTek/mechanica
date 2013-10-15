@echo off
cd project
..\..\..\library\buildtools\premake\premake4.exe /outpath=../build vs2010
cd ..
pause