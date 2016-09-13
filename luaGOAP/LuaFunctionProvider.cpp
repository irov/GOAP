#	include "LuaFunctionProvider.h"

//////////////////////////////////////////////////////////////////////////
LuaFunctionProvider::LuaFunctionProvider( lua_State * L, int _ref, int _args_count, int _args_ref )
	: m_L( L )
	, m_ref( _ref )
	, m_args_count( _args_count )
	, m_args_ref( _args_ref )
{
}
//////////////////////////////////////////////////////////////////////////
LuaFunctionProvider::~LuaFunctionProvider()
{
	luaL_unref( m_L, LUA_REGISTRYINDEX, m_ref );

	if( m_args_count > 0 )
	{
		luaL_unref( m_L, LUA_REGISTRYINDEX, m_args_ref );
	}
}
//////////////////////////////////////////////////////////////////////////
void LuaFunctionProvider::onFunction()
{
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_args_ref );
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_ref );
		
	if( !lua_isfunction( m_L, -1 ) )
	{
		return;
	}

	if( m_args_count > 0 )
	{
		for( int i = 0; i != m_args_count; ++i )
		{
			luaL_checktype( m_L, -2 - i, LUA_TTABLE );
			lua_rawgeti( m_L, -2 - i, i + 1 );
		}
	}
		
	lua_call( m_L, m_args_count, 0 );

	lua_pop( m_L, 1 );
}
