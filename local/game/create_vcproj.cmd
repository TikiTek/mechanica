@echo off
cd project
..\..\..\library\buildtools\premake\premake5.exe /outpath=../build vs2012
cd ..
pause