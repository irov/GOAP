#	include "LuaScopeProvider.h"

#	include "GOAP/Source.h"

//////////////////////////////////////////////////////////////////////////
LuaScopeProvider::LuaScopeProvider( lua_State * L, int _ref )
	: m_L( L )
	, m_ref( _ref )
{
}
//////////////////////////////////////////////////////////////////////////
LuaScopeProvider::~LuaScopeProvider()
{
	luaL_unref( m_L, LUA_REGISTRYINDEX, m_ref );
}
//////////////////////////////////////////////////////////////////////////
bool LuaScopeProvider::onScope( const GOAP::SourcePtr & _source )
{
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_ref );
		
	if( !lua_isfunction( m_L, -1 ) )
	{
		return false;
	}

	GOAP::Source ** udata = (GOAP::Source **)lua_newuserdata( m_L, sizeof( GOAP::Source * ) );

	GOAP::IntrusivePtrSetup( *udata, _source );

	luaL_setmetatable( m_L, "luaL_GOAP_Source" );

	lua_call( m_L, 1, 1 );

	bool l_result = !!lua_toboolean( m_L, -1 );

	return l_result;
}