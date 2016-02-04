@echo off
cd project
..\..\..\library\buildtools\premake\premake5.exe /outpath=../build vs2015
cd ..
pause