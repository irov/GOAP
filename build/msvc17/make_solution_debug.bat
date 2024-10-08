@echo off

set "VERSION=17"
set "YEAR=2022"

set "CONFIGURATION=Debug"
set "SOLUTION_DIR=..\solutions\GOAP_msvc%VERSION%_%CONFIGURATION%"

@pushd ..
@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
CMake -G "Visual Studio %VERSION% %YEAR%" "%CD%\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING=%CONFIGURATION% -DCMAKE_BUILD_TYPE:STRING=%CONFIGURATION% -DLIBGOAP_BUILD_TEST:BOOL=TRUE
@popd
@popd

@echo on
@pause
