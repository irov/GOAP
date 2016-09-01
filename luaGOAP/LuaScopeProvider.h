#	pragma once

#	include "GOAP/ScopeProvider.h"

#	include "lua.hpp"

class LuaScopeProvider
	: public GOAP::ScopeProvider
{
public:
	LuaScopeProvider( lua_State * L, int _ref );
	~LuaScopeProvider();

protected:
	bool onScope( const GOAP::SourcePtr & _source ) override;

protected:
	lua_State * m_L;
	int m_ref;
};
