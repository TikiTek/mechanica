@echo off
cd project
..\..\..\library\buildtools\premake\premake4.exe /outpath=../build gmake
cd ..
pause