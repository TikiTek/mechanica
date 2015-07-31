@echo off

call:delete_folter library/tools/commandlineconverter/build
call:delete_folter library/tools/editor/build
call:delete_folter library/tools/genericdatacodegenerator/build

call:delete_folter library/tests/libraryunittests/build

call:delete_folter local/game/build

:delete_folter
if exist %~1 (
  rmdir /s /q %~1
)
goto:eof