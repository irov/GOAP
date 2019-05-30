CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(GOAP)

OPTION(LIBGOAP_EXTERNAL_BUILD "LIBGOAP_EXTERNAL_BUILD" OFF)
OPTION(LIBGOAP_EXTERNAL_INCLUDE "LIBGOAP_EXTERNAL_INCLUDE" OFF)
OPTION(LIBGOAP_EXTERNAL_INCLUDE_PATH "LIBGOAP_EXTERNAL_INCLUDE_PATH" "")
OPTION(LIBGOAP_BUILD_TEST "LIBGOAP_BUILD_TEST" ON)

IF( NOT LIBGOAP_EXTERNAL_BUILD )
    set (CMAKE_CXX_STANDARD 17)
    
    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_GENERATOR} )
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_GENERATOR} )      
ENDIF()

SET( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
SET( INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include )

# OUR SOURCES
set( GOAP_SOURCES
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
    ${SOURCE_DIR}/TaskRaceNeck.cpp
    ${SOURCE_DIR}/TaskRepeat.cpp
    ${SOURCE_DIR}/TaskScope.cpp
    ${SOURCE_DIR}/TaskSwitch.cpp
    ${SOURCE_DIR}/TaskWhile.cpp
    ${SOURCE_DIR}/TaskFor.cpp    
    ${SOURCE_DIR}/TranscriptorBase.cpp
    ${SOURCE_DIR}/TranscriptorBase.h
    ${SOURCE_DIR}/TranscriptorParallel.cpp
    ${SOURCE_DIR}/TranscriptorParallel.h
    ${SOURCE_DIR}/TranscriptorRace.cpp
    ${SOURCE_DIR}/TranscriptorRace.h
    ${SOURCE_DIR}/Semaphore.cpp
    ${SOURCE_DIR}/Event.cpp
    ${SOURCE_DIR}/TaskSemaphore.cpp
)

set( GOAP_HEADERS
    ${INCLUDE_DIR}/GOAP/Alias.h
    ${INCLUDE_DIR}/GOAP/Array.h
    ${INCLUDE_DIR}/GOAP/ArraySources.h
    ${INCLUDE_DIR}/GOAP/CallbackProvider.h
    ${INCLUDE_DIR}/GOAP/Chain.h
    ${INCLUDE_DIR}/GOAP/ChainProvider.h
    ${INCLUDE_DIR}/GOAP/Config.h
    ${INCLUDE_DIR}/GOAP/Vector.h
    ${INCLUDE_DIR}/GOAP/Macros.h
    ${INCLUDE_DIR}/GOAP/Exception.h
    ${INCLUDE_DIR}/GOAP/Factorable.h
    ${INCLUDE_DIR}/GOAP/FunctionProvider.h
    ${INCLUDE_DIR}/GOAP/GOAP.h
    ${INCLUDE_DIR}/GOAP/GuardProvider.h
    ${INCLUDE_DIR}/GOAP/IfProvider.h
    ${INCLUDE_DIR}/GOAP/IntrusiveBase.h
    ${INCLUDE_DIR}/GOAP/IntrusivePtr.h
    ${INCLUDE_DIR}/GOAP/ScopeProvider.h
    ${INCLUDE_DIR}/GOAP/ForProvider.h
    ${INCLUDE_DIR}/GOAP/Source.h
    ${INCLUDE_DIR}/GOAP/SwitchProvider.h
    ${INCLUDE_DIR}/GOAP/Task.h
    ${INCLUDE_DIR}/GOAP/TaskCallback.h
    ${INCLUDE_DIR}/GOAP/TaskBlock.h
    ${INCLUDE_DIR}/GOAP/TaskDummy.h
    ${INCLUDE_DIR}/GOAP/TaskFork.h
    ${INCLUDE_DIR}/GOAP/TaskFunction.h
    ${INCLUDE_DIR}/GOAP/TaskGuard.h
    ${INCLUDE_DIR}/GOAP/TaskIf.h
    ${INCLUDE_DIR}/GOAP/TaskParallelNeck.h
    ${INCLUDE_DIR}/GOAP/TaskRepeat.h
    ${INCLUDE_DIR}/GOAP/TaskRaceNeck.h
    ${INCLUDE_DIR}/GOAP/TaskScope.h
    ${INCLUDE_DIR}/GOAP/TaskSwitch.h
    ${INCLUDE_DIR}/GOAP/TaskWhile.h
    ${INCLUDE_DIR}/GOAP/TaskFor.h
    ${INCLUDE_DIR}/GOAP/Transcriptor.h
    ${INCLUDE_DIR}/GOAP/TranscriptorParallelArray.h
    ${INCLUDE_DIR}/GOAP/TranscriptorRaceArray.h
    ${INCLUDE_DIR}/GOAP/Semaphore.h
    ${INCLUDE_DIR}/GOAP/Event.h
    ${INCLUDE_DIR}/GOAP/EventProvider.h
    ${INCLUDE_DIR}/GOAP/TaskSemaphore.h
    ${INCLUDE_DIR}/GOAP/SemaphoreFlags.h
    ${INCLUDE_DIR}/GOAP/Visitor.h
    ${INCLUDE_DIR}/GOAP/Visitable.h
    ${INCLUDE_DIR}/GOAP/GeneratorProvider.h
    ${INCLUDE_DIR}/GOAP/Zip.h
)

IF( LIBGOAP_EXTERNAL_INCLUDE )
    include_directories(${PROJECT_NAME} ${LIBGOAP_EXTERNAL_INCLUDE_PATH})
ENDIF()

include_directories(${PROJECT_NAME} ${INCLUDE_DIR})

add_library( ${PROJECT_NAME} STATIC ${GOAP_SOURCES} ${GOAP_HEADERS} )

if( LIBGOAP_BUILD_TEST )
    SET( SANDBOX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sandbox )
    
    set( Sandbox_SOURCES
        ${SANDBOX_DIR}/main.cpp
        ${SANDBOX_DIR}/Scheduler.cpp
        ${SANDBOX_DIR}/Scheduler.h
        ${SANDBOX_DIR}/TaskDelay.cpp
        ${SANDBOX_DIR}/TaskDelay.h
        ${SANDBOX_DIR}/TaskPrint.cpp
        ${SANDBOX_DIR}/TaskPrint.h
        ${SANDBOX_DIR}/TaskRoll.cpp
        ${SANDBOX_DIR}/TaskRoll.h
    )
    
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)

    INCLUDE_DIRECTORIES( Sandbox ${INCLUDE_DIR} )

    ADD_EXECUTABLE( Sandbox ${Sandbox_SOURCES} )

    TARGET_LINK_LIBRARIES( Sandbox ${PROJECT_NAME} )
endif()