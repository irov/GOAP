#	include "TaskLua.h"

//////////////////////////////////////////////////////////////////////////
TaskLua::TaskLua( lua_State * L, const std::string & _metaname, int _ref )
	: m_L( L )
	, m_metaname( _metaname )
	, m_ref( _ref )
{ 
}
//////////////////////////////////////////////////////////////////////////
TaskLua::~TaskLua()
{
	luaL_unref( m_L, LUA_REGISTRYINDEX, m_ref );
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onRun()
{
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_ref );
		
	lua_getfield( m_L, -1, "onRun" );

	bool task_continue = true;

	if( lua_isfunction( m_L, -1 ) )
	{
		lua_pushvalue( m_L, -2 );
		lua_call( m_L, 1, 1 );

		task_continue = !!lua_toboolean( m_L, -1 );
	}

	return task_continue;
}