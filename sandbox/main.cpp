#	include <stdlib.h>

#	include "Scheduler.h"

#	include "GOAP/Task.h"
#	include "GOAP/Chain.h"
#	include "GOAP/Source.h"

#	include "luaGOAP.h"

#	include "TaskDelay.h"
#	include "TaskPrint.h"
#	include "TaskRoll.h"

#	include <Windows.h>
#	include <time.h>

#	include "lua.hpp"

void main()
{
	lua_State * L = luaL_newstate();

	luaopen_base( L );

	luaGOAP( L );

	int erred = luaL_dofile( L, "test.lua" );

	if( erred )
	{
		printf( "Lua error: %s"
			, luaL_checkstring( L, -1 )
			);
	}

	lua_close( L );

	srand( (unsigned int)time( NULL ) );

	printf( "%f %f %f\n", fmod( 0.5, 1.0 ), fmod( 1.3, 1.0 ), fmod( 3.0, 1.0 ) );

	Scheduler * sch = new Scheduler;
		
	GOAP::SourcePtr source = new GOAP::Source();

	source->addTask( new TaskPrint( "begin" ) );
	source->addTask( new TaskDelay( 2000.f, sch ) );

	GOAP::TVectorSources & source_parallel = source->addParallel( 2 );

	source_parallel[0]->addTask( new TaskDelay( 1000.f, sch ) );
	source_parallel[1]->addTask( new TaskPrint( "process" ) );
	
	source->addTask( new TaskPrint( "end" ) );
	source->addTask( new TaskDelay( 1000.f, sch ) );
	source->addTask( new TaskPrint( "****ROLL*****" ) );
	source->addTask( new TaskDelay( 1000.f, sch ) );


	GOAP::TVectorSources & source_race = source->addRace( 3 );

	source_race[0]->addTask( new TaskRoll( 200.f, 1, 6, sch ) );
	source_race[0]->addTask( new TaskPrint( "---1---" ) );

	source_race[1]->addTask( new TaskRoll( 100.f, 2, 12, sch ) );
	source_race[1]->addTask( new TaskPrint( "---2---" ) );

	source_race[2]->addTask( new TaskRoll( 50.f, 4, 24, sch ) );
	source_race[2]->addTask( new TaskPrint( "---3---" ) );

	source->addTask( new TaskDelay( 1000.f, sch ) );
	source->addTask( new TaskPrint( "****WIN*****" ) );
	source->addTask( new TaskDelay( 1000.f, sch ) );

	source->addCallback( [] ( GOAP::CallbackObserver * _observer, bool isSkip ) { printf( "HTTP!!!!!\n" ); Sleep( 1000 ); _observer->onCallback( isSkip ); } );
	
	source->addFunction( [] (){ printf( "WOW!!\n" ); } );

	source->addScope( [] ( const GOAP::SourcePtr & _scope ) -> bool { _scope->addFunction( [] () {printf( "SCOPE????? WOW!!!" ); } ); return true; } );

	source->addFunction( [] (){ printf( "Oh\n" ); } );

	GOAP::ChainPtr tc = new GOAP::Chain( source, nullptr );

	tc->run();

	while( tc->isComplete() == false )
	{
		sch->update(100.f);

		Sleep( 100 );
	}

	printf( "FINALIZE\n" );

	delete sch;
}