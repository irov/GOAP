#	include "LuaIfProvider.h"

#	include "GOAP/Source.h"

//////////////////////////////////////////////////////////////////////////
LuaIfProvider::LuaIfProvider( lua_State * L, int _ref )
	: m_L( L )
	, m_ref( _ref )
{
}
//////////////////////////////////////////////////////////////////////////
LuaIfProvider::~LuaIfProvider()
{
	luaL_unref( m_L, LUA_REGISTRYINDEX, m_ref );
}
//////////////////////////////////////////////////////////////////////////
bool LuaIfProvider::onIf()
{
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_ref );
		
	if( !lua_isfunction( m_L, -1 ) )
	{
		return false;
	}

	lua_call( m_L, 0, 1 );

	bool l_result = !!lua_toboolean( m_L, -1 );

	return l_result;
}