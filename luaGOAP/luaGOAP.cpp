#	include "luaGOAP.h"

#	include "GOAP/Task.h"
#	include "GOAP/Chain.h"
#	include "GOAP/Source.h"
#	include "GOAP/TaskFunction.h"
#	include "GOAP/TaskScope.h"
#	include "GOAP/TaskIf.h"
#	include "GOAP/TaskRepeat.h"
#	include "GOAP/ChainProvider.h"

#	include "LuaTask.h"
#	include "LuaScopeProvider.h"
#	include "LuaFunctionProvider.h"
#	include "LuaIfProvider.h"

static const char * LUA_GOAP_Source_Metaname = "luaL_GOAP_Source";
static const char * LUA_GOAP_Chain_Metaname = "luaL_GOAP_Chain";
static const char * LUA_GOAP_Task_Metaname = "luaL_GOAP_Task";

static int l_Task_destructor( lua_State * L )
{
	lua_getfield( L, -1, "__self" );
	GOAP::Task ** task = (GOAP::Task **)lua_touserdata( L, -1 );
	lua_pop( L, 1 );

	GOAP::IntrusivePtrRelease( *task );

	return 0;
}

static int l_Task_complete( lua_State * L )
{
	lua_getfield( L, -1, "__self" );
	GOAP::Task ** task = (GOAP::Task **)lua_touserdata( L, -1 );
	lua_pop( L, 1 );

	(*task)->complete( true, false );

	return 0;
}

static int l_Task_constructor( lua_State * L )
{
	lua_getfield( L, 1, "__name" );
	const char * metaname = lua_tostring( L, -1 );
	lua_pop( L, 1 );

	lua_createtable( L, 0, 0 );
	
	GOAP::Task ** udata = (GOAP::Task **)lua_newuserdata( L, sizeof( GOAP::Task * ) );
	lua_setfield( L, -2, "__self");

	lua_pushcfunction( L, l_Task_destructor );
	lua_setfield( L, -2, "__gc" );

	lua_pushcfunction( L, l_Task_complete );
	lua_setfield( L, -2, "complete" );
	
	lua_pushvalue( L, -2 );
	lua_setfield( L, -2, "params" );
	
	lua_pushvalue( L, 1 );
	lua_setmetatable( L, -2 );

	int ref = luaL_ref( L, LUA_REGISTRYINDEX );

	GOAP::TaskPtr task = new LuaTask( L, ref, metaname );
		
	GOAP::IntrusivePtrSetup( *udata, task );
	
	lua_rawgeti( L, LUA_REGISTRYINDEX, ref );

	return 1;
}

static int l_Task_tostring( lua_State * L )
{
	const char * name = lua_tostring( L, lua_upvalueindex( 1 ) );
	const void * ptr = lua_topointer( L, lua_upvalueindex( 1 ) );
	lua_pushfstring( L, "<goap: %s as %p>", name, ptr );
		
	return 1;
}

static int bind_goap_task( lua_State * L )
{
	const char * metaname = luaL_checkstring( L, 1 );

	luaL_newmetatable( L, metaname );

	lua_pushcclosure( L, l_Task_constructor, 0 );
	lua_setfield( L, -2, "__call" );

	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );

	lua_pushstring( L, metaname );
	lua_pushcclosure( L, l_Task_tostring, 1 );
	lua_setfield( L, -2, "__tostring" );
	
	lua_createtable( L, 0, 0 );

	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );

	lua_pushvalue( L, -2 );
	lua_setmetatable( L, -2 );

	return 1;
}

static int l_Source_constructor( lua_State * L )
{
	GOAP::Source ** udata = (GOAP::Source **)lua_newuserdata( L, sizeof( GOAP::Source * ) );

	GOAP::IntrusivePtrSetup( *udata, new GOAP::Source );

	luaL_setmetatable( L, LUA_GOAP_Source_Metaname );

	return 1;
}

static int l_Source_destructor( lua_State * L )
{
	GOAP::Source ** source = (GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );
	
	GOAP::IntrusivePtrRelease( *source );

	return 0;
}

static int l_Source_addTask( lua_State * L )
{
	GOAP::Source ** source = (GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );
		
	lua_getfield( L, -1, "__self" );
	GOAP::Task ** task = (GOAP::Task **)lua_touserdata( L, -1 );
	lua_pop( L, 1 );

	(*source)->addTask( *task );

	return 0;
}

static int l_Source_addParallel( lua_State * L )
{
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	lua_Integer pnum = lua_tointeger( L, 2 );

	GOAP::TVectorSources & sources = source->addParallel( (size_t)pnum );

	for( GOAP::TVectorSources::iterator
		it = sources.begin(),
		it_end = sources.end();
	it != it_end;
	++it )
	{
		GOAP::SourcePtr & source = *it;

		GOAP::Source ** udata = (GOAP::Source **)lua_newuserdata( L, sizeof( GOAP::Source * ) );
		GOAP::IntrusivePtrSetup( *udata, source );

		luaL_setmetatable( L, LUA_GOAP_Source_Metaname );
	}

	return (int)pnum;
}

static int l_Source_addRace( lua_State * L )
{
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	lua_Integer pnum = lua_tointeger( L, 2 );

	GOAP::TVectorSources & sources = source->addRace( (size_t)pnum );

	for( GOAP::TVectorSources::iterator
		it = sources.begin(),
		it_end = sources.end();
	it != it_end;
	++it )
	{
		GOAP::SourcePtr & source = *it;

		GOAP::Source ** udata = (GOAP::Source **)lua_newuserdata( L, sizeof( GOAP::Source * ) );
		GOAP::IntrusivePtrSetup( *udata, source );

		luaL_setmetatable( L, LUA_GOAP_Source_Metaname );
	}

	return (int)pnum;
}

static int l_Source_addFunction( lua_State * L )
{
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	luaL_checktype( L, 2, LUA_TFUNCTION );

	int top = lua_gettop( L );

	int args_count = top - 2;

	int args_ref = -1;

	if( args_count > 0 )
	{		
		lua_createtable( L, args_count, 0 );
		for( int i = 0; i < args_count; i++ )
		{
			lua_pushvalue( L, 3 + i );
			int t[] = {LUA_TNUMBER, LUA_TSTRING};
			luaL_checktype( L, -1, t[i] );
			lua_rawseti( L, -2, i + 1 );
		}

		args_ref = luaL_ref( L, LUA_REGISTRYINDEX );
	}

	lua_pushvalue( L, 2 );
	lua_isfunction( L, -1 );

	int ref = luaL_ref( L, LUA_REGISTRYINDEX );

	GOAP::FunctionProviderPtr provider = new LuaFunctionProvider( L, ref, args_count, args_ref );

	GOAP::TaskPtr task = new GOAP::TaskFunction( provider );

	source->addTask( task );

	return 0;
}

static int l_Source_addScope( lua_State * L )
{
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	luaL_checktype( L, 2, LUA_TFUNCTION );

	lua_pushvalue( L, 2 );
	lua_isfunction( L, -1 );

	int ref = luaL_ref( L, LUA_REGISTRYINDEX );
	
	GOAP::ScopeProviderPtr provider = new LuaScopeProvider( L, ref );

	GOAP::TaskPtr task = new GOAP::TaskScope( provider );

	source->addTask( task );

	return 0;
}

static int l_Source_addIf( lua_State * L )
{
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	luaL_checktype( L, 2, LUA_TFUNCTION );

	lua_pushvalue( L, 2 );
	lua_isfunction( L, -1 );

	int ref = luaL_ref( L, LUA_REGISTRYINDEX );
	
	GOAP::IfProviderPtr provider = new LuaIfProvider( L, ref );

	GOAP::SourcePtr sourceTrue = new GOAP::Source();
	GOAP::SourcePtr sourceFalse = new GOAP::Source();

	GOAP::TaskPtr task = new GOAP::TaskIf( provider, sourceTrue, sourceFalse );

	source->addTask( task );

	GOAP::Source ** lua_sourceTrue = (GOAP::Source **)lua_newuserdata( L, sizeof( GOAP::Source * ) );
	GOAP::IntrusivePtrSetup( *lua_sourceTrue, sourceTrue );

	luaL_setmetatable( L, LUA_GOAP_Source_Metaname );

	GOAP::Source ** lua_sourceFalse = (GOAP::Source **)lua_newuserdata( L, sizeof( GOAP::Source * ) );
	GOAP::IntrusivePtrSetup( *lua_sourceFalse, sourceFalse );

	luaL_setmetatable( L, LUA_GOAP_Source_Metaname );

	return 2;
}

static int l_Source_addRepeat( lua_State * L )
{
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	luaL_checktype( L, 2, LUA_TFUNCTION );

	lua_pushvalue( L, 2 );
	lua_isfunction( L, -1 );

	int ref = luaL_ref( L, LUA_REGISTRYINDEX );

	GOAP::ScopeProviderPtr provider = new LuaScopeProvider( L, ref );
	
	GOAP::SourcePtr sourceUntil = new GOAP::Source();

	GOAP::TaskPtr task = new GOAP::TaskRepeat( provider, sourceUntil );

	source->addTask( task );

	GOAP::Source ** lua_sourceUntil = (GOAP::Source **)lua_newuserdata( L, sizeof( GOAP::Source * ) );
	GOAP::IntrusivePtrSetup( *lua_sourceUntil, sourceUntil );

	luaL_setmetatable( L, LUA_GOAP_Source_Metaname );

	return 1;
}

static int luaGOAP_Source_tostring( lua_State * L )
{
	const void * ptr = lua_topointer( L, lua_upvalueindex( 1 ) );
	lua_pushfstring( L, "<goap: Source as %p>", ptr );

	return 1;
}

static void luaGOAP_Source( lua_State * L )
{
	luaL_newmetatable( L, LUA_GOAP_Source_Metaname );

	static luaL_Reg regs[] =
	{
		{"__call", l_Source_constructor},
		{"__gc", l_Source_destructor},
		{"addTask", l_Source_addTask},
		{"addParallel", l_Source_addParallel},
		{"addRace", l_Source_addRace},
		{"addFunction", l_Source_addFunction},
		{"addScope", l_Source_addScope},
		{"addIf", l_Source_addIf},
		{"addRepeat", l_Source_addRepeat},
		{NULL, NULL}
	};

	luaL_setfuncs( L, regs, 0 );

	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );

	lua_pushcfunction( L, luaGOAP_Source_tostring );
	lua_setfield( L, -2, "__tostring" );

	lua_createtable( L, 0, 0 );
	luaL_setmetatable( L, LUA_GOAP_Source_Metaname );

	lua_setglobal( L, "Source" );
}

static int l_Chain_constructor( lua_State * L )
{
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, -1, LUA_GOAP_Source_Metaname );

	GOAP::Chain ** udata = (GOAP::Chain **)lua_newuserdata( L, sizeof( GOAP::Chain * ) );

	GOAP::IntrusivePtrSetup( *udata, new GOAP::Chain( source, nullptr ) );

	luaL_setmetatable( L, LUA_GOAP_Chain_Metaname );

	return 1;
}

static int l_Chain_destructor( lua_State * L )
{
	GOAP::Chain * chain = *(GOAP::Chain **)luaL_checkudata( L, 1, LUA_GOAP_Chain_Metaname );

	GOAP::IntrusivePtrRelease( chain );

	return 0;
}

static int l_Chain_run( lua_State * L )
{
	GOAP::Chain * chain = *(GOAP::Chain **)luaL_checkudata( L, 1, LUA_GOAP_Chain_Metaname );

	chain->run();

	return 0;
}

static int l_Chain_skip( lua_State * L )
{
	GOAP::Chain * chain = *(GOAP::Chain **)luaL_checkudata( L, 1, LUA_GOAP_Chain_Metaname );

	chain->skip();

	return 0;
}

static int l_Chain_cancel( lua_State * L )
{
	GOAP::Chain * chain = *(GOAP::Chain **)luaL_checkudata( L, 1, LUA_GOAP_Chain_Metaname );

	chain->cancel();

	return 0;
}

static int luaGOAP_Chain_tostring( lua_State * L )
{
	const void * ptr = lua_topointer( L, lua_upvalueindex( 1 ) );
	lua_pushfstring( L, "<goap: Chain as %p>", ptr );

	return 1;
}

static void luaGOAP_Chain( lua_State * L )
{
	luaL_newmetatable( L, LUA_GOAP_Chain_Metaname );

	static luaL_Reg regs[] =
	{
		{"__call", l_Chain_constructor},
		{"__gc", l_Chain_destructor},
		{"run", l_Chain_run},
		{"skip", l_Chain_skip},
		{"cancel", l_Chain_cancel},
		{"__tostring", luaGOAP_Chain_tostring},
		{NULL, NULL}
	};

	luaL_setfuncs( L, regs, 0 );

	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );

	lua_createtable( L, 0, 0 );
	luaL_setmetatable( L, LUA_GOAP_Chain_Metaname );

	lua_setglobal( L, "Chain" );
}

int luaGOAP_TaskExternal_tostring( lua_State * L )
{
	const void * ptr = lua_topointer( L, lua_upvalueindex( 1 ) );
	lua_pushfstring( L, "<goap: external Task as %s>", ptr );
	
	return 1;
}

static void luaGOAP_Task( lua_State * L )
{
	luaL_newmetatable( L, LUA_GOAP_Task_Metaname );

	static luaL_Reg regs[] =
	{
		{"__gc", l_Task_destructor},
		{"complete", l_Task_complete},
		{NULL, NULL}
	};

	luaL_setfuncs( L, regs, 0 );

	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );

	lua_pushcfunction( L, luaGOAP_TaskExternal_tostring );
	lua_setfield( L, -2, "__tostring" );
}

void luaGOAP( lua_State * L )
{
	luaGOAP_Source( L );
	luaGOAP_Chain( L );
	luaGOAP_Task( L );

	lua_register( L, "Task", &bind_goap_task );
}

void luaGOAP_createTask( lua_State * L, const GOAP::TaskPtr & _task, const char * _type )
{
	lua_createtable( L, 0, 0 );
	luaL_setmetatable( L, LUA_GOAP_Task_Metaname );

	GOAP::Task ** udata = (GOAP::Task **)lua_newuserdata( L, sizeof( GOAP::Task * ) );
	GOAP::IntrusivePtrSetup( *udata, _task );

	lua_setfield( L, -2, "__self" );

	lua_pushstring( L, _type );
	lua_setfield( L, -2, "type" );
}
