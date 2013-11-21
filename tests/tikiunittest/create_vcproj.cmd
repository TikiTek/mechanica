@echo off
cd project
..\..\..\buildtools\premake\premake4.exe /outpath=../build vs2010
cd ..
pause