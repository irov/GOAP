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
bool TaskLua::onInitialize()
{
	return this->askMethod_( "onInitialize", true );
}
//////////////////////////////////////////////////////////////////////////
void TaskLua::onFinalize()
{
	this->callMethod_( "onFinalize" );
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onValidate() const
{
	return this->askMethod_( "onValidate", true );
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onCheck() const
{
	return this->askMethod_( "onCheck", true );
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onRun()
{
	return this->askMethod_( "onRun", true );
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onSkipable() const
{
	return this->askMethod_( "onSkipable", false );
}
//////////////////////////////////////////////////////////////////////////
void TaskLua::onSkipNoSkiped()
{
	this->callMethod_( "onSkipNoSkiped" );
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onSkipBlock()
{
	return this->askMethod_( "onSkipBlock", false );
}
//////////////////////////////////////////////////////////////////////////
void TaskLua::onComplete()
{
	this->callMethod_( "onComplete" );
}
//////////////////////////////////////////////////////////////////////////
void TaskLua::onSkip()
{
	this->callMethod_( "onSkip" );
}
//////////////////////////////////////////////////////////////////////////
void TaskLua::onCancel()
{
	this->callMethod_( "onCancel" );
}
//////////////////////////////////////////////////////////////////////////
void TaskLua::onFinally()
{
	this->callMethod_( "onFinally" );
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onCheckRun() const
{
	if( this->callMethod_( "onCheckRun" ) == true )
	{
		bool l_result = !!lua_toboolean( m_L, -1 );
		return l_result;

		return l_result;
	}

	return Task::onCheckRun();
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::onCheckSkip() const
{
	if( this->callMethod_( "onCheckSkip" ) == true )
	{
		bool l_result = !!lua_toboolean( m_L, -1 );
		return l_result;

		return l_result;
	}

	return Task::onCheckRun();
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::callMethod_( const char * _method, int _return ) const
{
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_ref );

	lua_getfield( m_L, -1, _method );

	if( !lua_isfunction( m_L, -1 ) )
	{
		return false;
	}

	lua_pushvalue( m_L, -2 );
	lua_call( m_L, 1, _return );

	return true;
}
//////////////////////////////////////////////////////////////////////////
bool TaskLua::askMethod_( const char * _method, bool _default ) const
{
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_ref );

	lua_getfield( m_L, -1, _method );

	if( !lua_isfunction( m_L, -1 ) )
	{
		return _default;
	}

	lua_pushvalue( m_L, -2 );
	lua_call( m_L, 1, 1 );

	bool l_result = !!lua_toboolean( m_L, -1 );

	return l_result;
}