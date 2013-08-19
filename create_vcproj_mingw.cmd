rem @echo off

mode 180,500

SET CODEDIR=%CD%
SET PATH=%CODEDIR%/library/buildtools/cmake/bin;%CODEDIR%/library/thirdparty/platform/MinGW/bin

cd ../binary

cmake.exe ../code -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=%CODEDIR%/library/buildtools/toolchain/toolchain-mingw32-windows.cmake

cd ../code

pause