CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(GOAP)

OPTION(LIBGOAP_EXTERNAL_BUILD "LIBGOAP_EXTERNAL_BUILD" OFF)
OPTION(LIBGOAP_EXTERNAL_INCLUDE "LIBGOAP_EXTERNAL_INCLUDE" OFF)
OPTION(LIBGOAP_EXTERNAL_INCLUDE_PATH "LIBGOAP_EXTERNAL_INCLUDE_PATH" "")
OPTION(LIBGOAP_SANDBOX "LIBGOAP_SANDBOX" OFF)

IF( NOT LIBGOAP_EXTERNAL_BUILD )
    set (CMAKE_CXX_STANDARD 11)
    
    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_temp/${ENGINE_LIB_DIR} )
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/libGOAP_win32 )
    SET( CMAKE_TEMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMakeTemp/libGOAP_win32/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE} )       
ENDIF()

SET( SOURCE_DIR src )
SET( INCLUDE_DIR include/GOAP )

# OUR SOURCES
set( sources
${SOURCE_DIR}/Alias.cpp
${SOURCE_DIR}/Chain.cpp
${SOURCE_DIR}/Exception.cpp
${SOURCE_DIR}/Factorable.cpp
${SOURCE_DIR}/Source.cpp
${SOURCE_DIR}/Task.cpp
${SOURCE_DIR}/TaskCallback.cpp
${SOURCE_DIR}/TaskBlock.cpp
${SOURCE_DIR}/TaskDummy.cpp
${SOURCE_DIR}/TaskFork.cpp
${SOURCE_DIR}/TaskFunction.cpp
${SOURCE_DIR}/TaskGuard.cpp
${SOURCE_DIR}/TaskIf.cpp
${SOURCE_DIR}/TaskParallelNeck.cpp
${SOURCE_DIR}/TaskParallelNeck.h
${SOURCE_DIR}/TaskRaceNeck.cpp
${SOURCE_DIR}/TaskRaceNeck.h
${SOURCE_DIR}/TaskRepeat.cpp
${SOURCE_DIR}/TaskScope.cpp
${SOURCE_DIR}/TaskSwitch.cpp
${SOURCE_DIR}/TaskWhile.cpp
${SOURCE_DIR}/TaskFor.cpp
${SOURCE_DIR}/Transcriptor.h
${SOURCE_DIR}/TranscriptorBase.cpp
${SOURCE_DIR}/TranscriptorBase.h
${SOURCE_DIR}/TranscriptorParallel.cpp
${SOURCE_DIR}/TranscriptorParallel.h
${SOURCE_DIR}/TranscriptorRace.cpp
${SOURCE_DIR}/TranscriptorRace.h
${SOURCE_DIR}/Semaphore.cpp
${SOURCE_DIR}/Event.cpp
${SOURCE_DIR}/TaskSemaphore.cpp

${INCLUDE_DIR}/Alias.h
${INCLUDE_DIR}/CallbackProvider.h
${INCLUDE_DIR}/Chain.h
${INCLUDE_DIR}/ChainProvider.h
${INCLUDE_DIR}/Config.h
${INCLUDE_DIR}/Vector.h
${INCLUDE_DIR}/Macros.h
${INCLUDE_DIR}/Exception.h
${INCLUDE_DIR}/Factorable.h
${INCLUDE_DIR}/FunctionProvider.h
${INCLUDE_DIR}/GOAP.h
${INCLUDE_DIR}/GuardProvider.h
${INCLUDE_DIR}/IfProvider.h
${INCLUDE_DIR}/IntrusiveBase.h
${INCLUDE_DIR}/IntrusivePtr.h
${INCLUDE_DIR}/ScopeProvider.h
${INCLUDE_DIR}/ForProvider.h
${INCLUDE_DIR}/Source.h
${INCLUDE_DIR}/SwitchProvider.h
${INCLUDE_DIR}/Task.h
${INCLUDE_DIR}/TaskCallback.h
${INCLUDE_DIR}/TaskBlock.h
${INCLUDE_DIR}/TaskDummy.h
${INCLUDE_DIR}/TaskFork.h
${INCLUDE_DIR}/TaskFunction.h
${INCLUDE_DIR}/TaskGuard.h
${INCLUDE_DIR}/TaskIf.h
${INCLUDE_DIR}/TaskRepeat.h
${INCLUDE_DIR}/TaskScope.h
${INCLUDE_DIR}/TaskSwitch.h
${INCLUDE_DIR}/TaskWhile.h
${INCLUDE_DIR}/TaskFor.h
${INCLUDE_DIR}/Semaphore.h
${INCLUDE_DIR}/Event.h
${INCLUDE_DIR}/EventProvider.h
${INCLUDE_DIR}/TaskSemaphore.h
${INCLUDE_DIR}/SemaphoreFlags.h
${INCLUDE_DIR}/Visitor.h
${INCLUDE_DIR}/Visitable.h
${INCLUDE_DIR}/GeneratorProvider.h
${INCLUDE_DIR}/Zip.h
)

IF( LIBGOAP_EXTERNAL_INCLUDE )
    include_directories(${PROJECT_NAME} ${LIBGOAP_EXTERNAL_INCLUDE_PATH})
ENDIF()

include_directories(${PROJECT_NAME} include)

add_library( ${PROJECT_NAME} STATIC ${sources} )

if( LIBGOAP_SANDBOX )
    set(
    Sandbox_SOURCES
        sandbox/main.cpp
        sandbox/Scheduler.cpp
        sandbox/Scheduler.h
        sandbox/TaskDelay.cpp
        sandbox/TaskDelay.h
        sandbox/TaskPrint.cpp
        sandbox/TaskPrint.h
        sandbox/TaskRoll.cpp
        sandbox/TaskRoll.h
    )
    
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)

    INCLUDE_DIRECTORIES( Sandbox ../include )

    ADD_EXECUTABLE( Sandbox ${Sandbox_SOURCES} )

    TARGET_LINK_LIBRARIES( Sandbox ${PROJECT_NAME} )
endif()