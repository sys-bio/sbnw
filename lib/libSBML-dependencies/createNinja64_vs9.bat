@echo off
Setlocal EnableDelayedExpansion
SET BASE_DIR=%~dp0
SET NINJA=ninja
SET VS_VERSION=9
SET VS_PREFIX=vs%VS_VERSION%
@SET WindowsSdkDir=C:\Program Files\Microsoft SDKs\Windows\v6.0A\
SET VS=C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC
SET VSDIR=%VS%\bin\amd64
call "c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars64.bat"
SET NINJA_OPTS=-DCMAKE_CXX_COMPILER:FILEPATH="%VSDIR%\cl.exe" -DCMAKE_C_COMPILER:FILEPATH="%VSDIR%\cl.exe" -DCMAKE_LINKER:FILEPATH="%VSDIR%\link.exe" 

if not exist %BASE_DIR%\dist mkdir %BASE_DIR%\dist

:: 
:: Build release static runtime version
:: 

SET WITH_STATIC_RUNTIME=ON
SET CMAKE_BUILD_TYPE=Release
SET VARIANT=%VS_PREFIX%_release_x64_static
SET CMAKE_OPTS=-DCMAKE_BUILD_TYPE:STRING=%CMAKE_BUILD_TYPE% -DBUILD_SHARED_LIBS:BOOL=OFF -DWITH_STATIC_RUNTIME:BOOL=%WITH_STATIC_RUNTIME% -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\install_%VARIANT%

mkdir %BASE_DIR%\%VARIANT%
pushd %BASE_DIR%\%VARIANT%
cmake -G "Ninja" %NINJA_OPTS% %CMAKE_OPTS% %* %BASE_DIR% 
%NINJA%
%NINJA% install
cpack -G ZIP
move "libSBML Dependencies-1.0.0-b1-win64.zip" %BASE_DIR%\dist\libSBML_dependencies_%VARIANT%.zip
popd

:: 
:: Build release dynamic runtime version
:: 

SET WITH_STATIC_RUNTIME=OFF
SET CMAKE_BUILD_TYPE=Release
SET VARIANT=%VS_PREFIX%_release_x64
SET CMAKE_OPTS=-DCMAKE_BUILD_TYPE:STRING=%CMAKE_BUILD_TYPE% -DBUILD_SHARED_LIBS:BOOL=OFF -DWITH_STATIC_RUNTIME:BOOL=%WITH_STATIC_RUNTIME% -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\install_%VARIANT%

mkdir %BASE_DIR%\%VARIANT%
pushd %BASE_DIR%\%VARIANT%
cmake -G "Ninja" %NINJA_OPTS% %CMAKE_OPTS% %* %BASE_DIR% 
%NINJA%
%NINJA% install
cpack -G ZIP
move "libSBML Dependencies-1.0.0-b1-win64.zip" %BASE_DIR%\dist\libSBML_dependencies_%VARIANT%.zip
popd


:: 
:: Build debug static runtime version
:: 

SET WITH_STATIC_RUNTIME=ON
SET CMAKE_BUILD_TYPE=Debug
SET VARIANT=%VS_PREFIX%_debug_x64_static
SET CMAKE_OPTS=-DCMAKE_BUILD_TYPE:STRING=%CMAKE_BUILD_TYPE% -DBUILD_SHARED_LIBS:BOOL=OFF -DWITH_STATIC_RUNTIME:BOOL=%WITH_STATIC_RUNTIME% -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\install_%VARIANT%

mkdir %BASE_DIR%\%VARIANT%
pushd %BASE_DIR%\%VARIANT%
cmake -G "Ninja" %NINJA_OPTS% %CMAKE_OPTS% %* %BASE_DIR% 
%NINJA%
%NINJA% install
cpack -G ZIP
move "libSBML Dependencies-1.0.0-b1-win64.zip" %BASE_DIR%\dist\libSBML_dependencies_%VARIANT%.zip
popd

:: 
:: Build debug dynamic runtime version
:: 

SET WITH_STATIC_RUNTIME=OFF
SET CMAKE_BUILD_TYPE=Debug
SET VARIANT=%VS_PREFIX%_debug_x64
SET CMAKE_OPTS=-DCMAKE_BUILD_TYPE:STRING=%CMAKE_BUILD_TYPE% -DBUILD_SHARED_LIBS:BOOL=OFF -DWITH_STATIC_RUNTIME:BOOL=%WITH_STATIC_RUNTIME% -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\install_%VARIANT%

mkdir %BASE_DIR%\%VARIANT%
pushd %BASE_DIR%\%VARIANT%
cmake -G "Ninja" %NINJA_OPTS% %CMAKE_OPTS% %* %BASE_DIR% 
%NINJA%
%NINJA% install
cpack -G ZIP
move "libSBML Dependencies-1.0.0-b1-win64.zip" %BASE_DIR%\dist\libSBML_dependencies_%VARIANT%.zip
popd

