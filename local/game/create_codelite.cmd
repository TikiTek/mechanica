@echo off
cd project
..\..\..\library\buildtools\premake\premake5.exe /outpath=../build codelite
cd ..
pause