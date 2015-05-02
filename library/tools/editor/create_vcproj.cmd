@echo off
cd project
..\..\..\buildtools\premake\premake5.exe /outpath=../build vs2012
cd ..
rem pause