#	pragma once

#	include "GOAP/Task.h"

#	include "lua.hpp"

class TaskLua
	: public GOAP::Task
{
public:
	TaskLua( lua_State * L, const std::string & _metaname, int _ref );
	~TaskLua();
	
protected:
	bool onRun() override;

protected:
	lua_State * m_L;
	std::string m_metaname;
	int m_ref;
};

typedef GOAP::IntrusivePtr<TaskLua> TaskLuaPtr;