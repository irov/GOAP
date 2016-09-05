#	pragma once

#	include "GOAP/FunctionProvider.h"

#	include "lua.hpp"

class LuaFunctionProvider
	: public GOAP::FunctionProvider
{
public:
	LuaFunctionProvider( lua_State * L, int _ref, int _args_count, int _args_ref );
	~LuaFunctionProvider();

protected:
	void onFunction() override;

protected:
	lua_State * m_L;
	int m_ref;
	int m_args_count;
	int m_args_ref;
};
