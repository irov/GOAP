#	include "luaGOAP.h"

#	include "GOAP/Task.h"
#	include "GOAP/Chain.h"
#	include "GOAP/Source.h"
#	include "GOAP/TaskScope.h"

#	include "TaskLua.h"
#	include "LuaScopeProvider.h"

static const char * LUA_GOAP_Source_Metaname = "luaL_GOAP_Source";
static const char * LUA_GOAP_Chain_Metaname = "luaL_GOAP_Chain";

static int l_Task_constructor( lua_State * L )
{
	lua_getfield( L, 1, "__metaname" );
	const char * metaname = lua_tostring( L, -1 );
	lua_pop( L, 1 );

	lua_createtable( L, 0, 0 );
	luaL_setmetatable( L, metaname );
	
	TaskLua ** udata = (TaskLua **)lua_newuserdata( L, sizeof( TaskLua * ) );
	luaL_setmetatable( L, metaname );
	lua_setfield( L, -2, "__self");

	lua_pushstring( L, metaname );
	lua_setfield( L, -2, "__metaname" );
		
	lua_pushvalue( L, -2 );
	lua_setfield( L, -2, "params" );

	int ref = luaL_ref( L, LUA_REGISTRYINDEX );
		
	GOAP::IntrusivePtrSetup( *udata, new TaskLua( L, ref ) );
	
	lua_rawgeti( L, LUA_REGISTRYINDEX, ref );

	return 1;
}

static int l_Task_destructor( lua_State * L )
{
	lua_getfield( L, -1, "__metaname" );
	const char * metaname = lua_tostring( L, -1 );
	lua_pop( L, 1 );

	TaskLua * task = *(TaskLua **)luaL_checkudata( L, 1, metaname );

	IntrusivePtrRelease( task );

	return 0;
}

static int bind_goap_task( lua_State * L )
{
	const char * metaname = luaL_checkstring( L, 1 );

	luaL_newmetatable( L, metaname );

	static luaL_Reg regs[] =
	{
		{"new", l_Task_constructor},
		{"__gc", l_Task_destructor},
		{NULL, NULL}
	};

	lua_pushstring( L, metaname );
	lua_setfield( L, -2, "__metaname" );

	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );

	luaL_setfuncs( L, regs, 0 );

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
	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	IntrusivePtrRelease( source );

	return 0;
}

static int l_Source_addTask( lua_State * L )
{
	lua_getfield( L, -1, "__metaname" );
	const char * metaname = lua_tostring( L, -1 );
	lua_pop( L, 1 );

	GOAP::Source * source = *(GOAP::Source **)luaL_checkudata( L, 1, LUA_GOAP_Source_Metaname );

	luaL_checktype( L, -1, LUA_TTABLE );
	lua_getfield( L, -1, "__self" );
	TaskLua * task = *(TaskLua **)luaL_checkudata( L, -1, metaname );

	source->addTask( task );

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

static void luaGOAP_Source( lua_State * L )
{
	luaL_newmetatable( L, LUA_GOAP_Source_Metaname );

	static luaL_Reg regs[] =
	{
		{"new", l_Source_constructor},
		{"__gc", l_Source_destructor},
		{"addTask", l_Source_addTask},
		{"addParallel", l_Source_addParallel},
		{"addRace", l_Source_addRace},
		{"addScope", l_Source_addScope},
		{NULL, NULL}
	};

	luaL_setfuncs( L, regs, 0 );

	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index" );

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

	IntrusivePtrRelease( chain );

	return 0;
}

static int l_Chain_run( lua_State * L )
{
	GOAP::Chain * chain = *(GOAP::Chain **)luaL_checkudata( L, 1, LUA_GOAP_Chain_Metaname );

	chain->run();

	return 0;
}

static void luaGOAP_Chain( lua_State * L )
{
	luaL_newmetatable( L, LUA_GOAP_Chain_Metaname );

	static luaL_Reg regs[] =
	{
		{"new", l_Chain_constructor},
		{"__gc", l_Chain_destructor},
		{"run", l_Chain_run},
		{NULL, NULL}
	};

	luaL_setfuncs( L, regs, 0 );

	lua_pushvalue( L, -1 );

	lua_setfield( L, -1, "__index" );

	lua_setglobal( L, "Chain" );
}

void luaGOAP( lua_State * L )
{
	luaGOAP_Source( L );
	luaGOAP_Chain( L );

	lua_register( L, "Task", &bind_goap_task );
}