PROJECT(GOAP)

OPTION(LIBGOAP_EXTERNAL_BUILD "LIBGOAP_EXTERNAL_BUILD" OFF)
OPTION(LIBGOAP_EXTERNAL_INCLUDE "LIBGOAP_EXTERNAL_INCLUDE" OFF)
OPTION(LIBGOAP_EXTERNAL_INCLUDE_PATH "LIBGOAP_EXTERNAL_INCLUDE_PATH" "")

IF( NOT LIBGOAP_EXTERNAL_BUILD )
    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_temp/${ENGINE_LIB_DIR} )
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/libGOAP_win32 )
    SET( CMAKE_TEMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMakeTemp/libGOAP_win32/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE} )    
    
    SET( CMAKE_CXX_FLAGS "/D_HAS_CXX17 /MP /EHsc /GR")
ENDIF()

SET( SOURCE_DIR src )
SET( INCLUDE_DIR include/GOAP )

# OUR SOURCES
set( sources
${SOURCE_DIR}/Chain.cpp
${SOURCE_DIR}/Source.cpp
${SOURCE_DIR}/Task.cpp
${SOURCE_DIR}/TaskCallback.cpp
${SOURCE_DIR}/TaskDeadLock.cpp
${SOURCE_DIR}/TaskDummy.cpp
${SOURCE_DIR}/TaskFork.cpp
${SOURCE_DIR}/TaskFunction.cpp
${SOURCE_DIR}/TaskGuard.cpp
${SOURCE_DIR}/TaskIf.cpp
${SOURCE_DIR}/TaskParallelNeck.cpp
${SOURCE_DIR}/TaskRaceNeck.cpp
${SOURCE_DIR}/TaskRepeat.cpp
${SOURCE_DIR}/TaskScope.cpp
${SOURCE_DIR}/TaskSwitch.cpp
${SOURCE_DIR}/TranscriptorBase.cpp
${SOURCE_DIR}/TranscriptorParallel.cpp
${SOURCE_DIR}/TranscriptorRace.cpp
${SOURCE_DIR}/Alias.cpp
${SOURCE_DIR}/Exception.cpp
${SOURCE_DIR}/Factorable.cpp
${SOURCE_DIR}/Factory.cpp

${INCLUDE_DIR}/TaskSwitch.h
${INCLUDE_DIR}/TaskRepeat.h
${INCLUDE_DIR}/TaskScope.h
${INCLUDE_DIR}/TaskGuard.h
${INCLUDE_DIR}/TaskIf.h
${INCLUDE_DIR}/TaskDummy.h
${INCLUDE_DIR}/TaskFork.h
${INCLUDE_DIR}/TaskFunction.h
${INCLUDE_DIR}/TaskDeadLock.h
${INCLUDE_DIR}/Task.h
${INCLUDE_DIR}/TaskCallback.h
${INCLUDE_DIR}/Source.h
${INCLUDE_DIR}/SwitchProvider.h
${INCLUDE_DIR}/IntrusiveBase.h
${INCLUDE_DIR}/ScopeProvider.h
${INCLUDE_DIR}/IntrusivePtr.h
${INCLUDE_DIR}/IfProvider.h
${INCLUDE_DIR}/GuardProvider.h
${INCLUDE_DIR}/FunctionProvider.h
${INCLUDE_DIR}/GOAP.h
${INCLUDE_DIR}/ChainProvider.h
${INCLUDE_DIR}/CallbackProvider.h
${INCLUDE_DIR}/Chain.h
${INCLUDE_DIR}/Alias.h
${INCLUDE_DIR}/Exception.h
${INCLUDE_DIR}/Config.h
${INCLUDE_DIR}/Factorable.h
${INCLUDE_DIR}/Factory.h
)

IF( LIBGOAP_EXTERNAL_INCLUDE )
    include_directories(${PROJECT_NAME} ${LIBGOAP_EXTERNAL_INCLUDE_PATH})
ENDIF()

include_directories(${PROJECT_NAME} include)

add_library( ${PROJECT_NAME} STATIC ${sources} )

