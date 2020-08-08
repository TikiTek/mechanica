@echo off
..\..\buildtools\genie\genie.exe --to=build vs2019
if errorlevel 1 goto error
goto exit

:error
pause

:exit
