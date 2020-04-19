@echo off
cd project
..\..\..\library\buildtools\genie\genie.exe --quiet --targets_action=vs2019 targets
if errorlevel 1 goto error
goto exit

:error
pause

:exit
cd ..
