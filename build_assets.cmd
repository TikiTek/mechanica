@echo off

mode 120, 50

SET CURDIR=%CD%
SET PATH=%WINDIR%/System32;%CURDIR%/library/buildtools/cmake/bin;%CURDIR%/library/thirdparty/platform/MinGW/bin

cd ..\binary

IF "%1" == "-r" GOTO rebuild

GOTO start

:rebuild
del *.sqlite

:start

rem cmake -DCMAKE_BUILD_TYPE=Release .
cmake --build . --target tikiassets
rem cmake -DCMAKE_BUILD_TYPE=Debug .

debug\tikiassets.exe

IF ERRORLEVEL 1 GOTO error

GOTO end

:error
pause

:end