#	pragma once

#	include "GOAP/IntrusivePtrBase.h"
#	include "GOAP/Task.h"

#	include "lua.hpp"

void luaGOAP( lua_State * L );
void luaGOAP_createTask( lua_State * L, const GOAP::TaskPtr & _task, const char * _type );
