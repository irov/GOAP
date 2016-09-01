#	pragma once

#	include "GOAP/IfProvider.h"

#	include "lua.hpp"

class LuaIfProvider
	: public GOAP::IfProvider
{
public:
	LuaIfProvider( lua_State * L, int _ref );
	~LuaIfProvider();

protected:
	bool onIf() override;

protected:
	lua_State * m_L;
	int m_ref;
};
