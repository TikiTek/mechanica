@echo off

SET CURDIR=%CD%
SET PATH=%CURDIR%/library/buildtools/cmake/bin;%CURDIR%/library/thirdparty/platform/MinGW/bin

cd ..

if exist binary goto binaryexists
MD binary

:binaryexists

if exist gamebuild goto gamebuildexists
MD gamebuild

:gamebuildexists

if '%1' == '' goto setvsversion
SET vs_version=%1

:start

cd binary

echo Create %vs_version% Project files
cmake.exe ../code -G %vs_version%

cd ../code

goto end

:setvsversion
SET vs_version="Visual Studio 11"
goto start


:end
pause
