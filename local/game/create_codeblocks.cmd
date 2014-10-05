@echo off
cd project
..\..\..\library\buildtools\premake\premake4.exe /outpath=../build codeblocks
cd ..
pause