#	include "LuaTask.h"

//////////////////////////////////////////////////////////////////////////
LuaTask::LuaTask( lua_State * L, int _ref )
	: m_L( L )
	, m_ref( _ref )
{ 
}
//////////////////////////////////////////////////////////////////////////
LuaTask::~LuaTask()
{
	luaL_unref( m_L, LUA_REGISTRYINDEX, m_ref );
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onInitialize()
{
	return this->askMethod_( "onInitialize", true );
}
//////////////////////////////////////////////////////////////////////////
void LuaTask::onFinalize()
{
	this->callMethod_( "onFinalize" );
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onValidate() const
{
	return this->askMethod_( "onValidate", true );
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onCheck() const
{
	return this->askMethod_( "onCheck", true );
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onRun()
{
	return this->askMethod_( "onRun", true );
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onSkipable() const
{
	return this->askMethod_( "onSkipable", false );
}
//////////////////////////////////////////////////////////////////////////
void LuaTask::onSkipNoSkiped()
{
	this->callMethod_( "onSkipNoSkiped" );
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onSkipBlock()
{
	return this->askMethod_( "onSkipBlock", false );
}
//////////////////////////////////////////////////////////////////////////
void LuaTask::onComplete()
{
	this->callMethod_( "onComplete" );
}
//////////////////////////////////////////////////////////////////////////
void LuaTask::onSkip()
{
	this->callMethod_( "onSkip" );
}
//////////////////////////////////////////////////////////////////////////
void LuaTask::onCancel()
{
	this->callMethod_( "onCancel" );
}
//////////////////////////////////////////////////////////////////////////
void LuaTask::onFinally()
{
	this->callMethod_( "onFinally" );
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onCheckRun() const
{
	if( this->callMethod_( "onCheckRun" ) == true )
	{
		bool l_result = !!lua_toboolean( m_L, -1 );

		return l_result;
	}

	return Task::onCheckRun();
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::onCheckSkip() const
{
	if( this->callMethod_( "onCheckSkip" ) == true )
	{
		bool l_result = !!lua_toboolean( m_L, -1 );

		return l_result;
	}

	return Task::onCheckRun();
}
//////////////////////////////////////////////////////////////////////////
bool LuaTask::callMethod_( const char * _method, int _return ) const
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
bool LuaTask::askMethod_( const char * _method, bool _default ) const
{
	lua_rawgeti( m_L, LUA_REGISTRYINDEX, m_ref );

	lua_getfield( m_L, -1, _method );

	if( !lua_isfunction( m_L, -1 ) )
	{
		lua_pop( m_L, 2 );

		return _default;
	}

	lua_pushvalue( m_L, -2 );
	lua_call( m_L, 1, 1 );

	bool l_result = !!lua_toboolean( m_L, -1 );

	return l_result;
}