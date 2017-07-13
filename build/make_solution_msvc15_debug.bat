@echo off

set "build_dir=build"
set "configuration=Debug"
set "CMAKE_EXE=%CD%\..\..\cmake\bin\cmake.exe"
set "SOLUTION_DIR=solutions\GOAP_msvc15_debug"

@pushd ..
@call vcvarsall_msvc15
@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
%CMAKE_EXE% -G "Visual Studio 15 2017" "%CD%\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING='Debug' -DCMAKE_BUILD_TYPE:STRING='Debug' -DLIBMATH_SANDBOX:BOOL=TRUE
@popd

@echo on
@pause
