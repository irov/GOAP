PROJECT(GOAP)

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
${INCLUDE_DIR}/IntrusivePtrBase.h
${INCLUDE_DIR}/ScopeProvider.h
${INCLUDE_DIR}/IntrusivePtr.h
${INCLUDE_DIR}/IfProvider.h
${INCLUDE_DIR}/GuardProvider.h
${INCLUDE_DIR}/FunctionProvider.h
${INCLUDE_DIR}/GOAP.h
${INCLUDE_DIR}/ChainProvider.h
${INCLUDE_DIR}/CallbackProvider.h
${INCLUDE_DIR}/Chain.h
)

include_directories(${PROJECT_NAME} include)

add_library( ${PROJECT_NAME} STATIC ${sources} )

