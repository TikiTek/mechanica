@echo off
..\..\buildtools\tiki_build\premake5.exe --to=build vs2017
if errorlevel 1 goto error
goto exit

:error
pause

:exit

