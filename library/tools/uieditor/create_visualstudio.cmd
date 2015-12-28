@echo off
cd project
..\..\..\buildtools\premake\premake5.exe /outpath=../build vs2015
cd ..
pause