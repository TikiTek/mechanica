@echo off
cd project
..\..\..\library\buildtools\premake\premake5.exe /outpath=../build codeblocks
cd ..
pause