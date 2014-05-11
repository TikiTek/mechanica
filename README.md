TikiEngine 3
============


TikiEngine is a 3D game framework.


Requirements
------------

At current state you need to build at least Visual Studio 2012 or greater.


How to compile
--------------

1. Goto '/local/game'
2. Execute 'create_vcproj.cmd' batch-file
3. Goto 'build' folder and open Solution file
4. Build

How to start
------------

1. Open 'resourcemanager.cpp'
2. Comment the following line in: 'm_pAssetConverter->convertAll();'
3. Build and run
4. Wait for crash/assert
5. Close and start again. Now should it work
Optional: comment the line in 'resoucemanager.cpp' out to increase start up time