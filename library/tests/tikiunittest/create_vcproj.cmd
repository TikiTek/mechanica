@echo off
cd project
..\..\..\buildtools\premake\premake4.exe /outpath=../build vs2012
cd ..
pause