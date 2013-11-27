@echo off
cd project
..\..\..\library\buildtools\premake\premake4.exe /outpath=../build vs2012
cd ..
pause