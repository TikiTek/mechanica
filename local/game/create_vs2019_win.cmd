@echo off
..\..\engine\buildtools\tiki_build\premake5.exe --to=build vs2019
if errorlevel 1 goto error
goto exit

:error
pause

:exit
