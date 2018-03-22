#include <stdlib.h>

#include "Scheduler.h"

#include "GOAP/Task.h"
#include "GOAP/Chain.h"
#include "GOAP/Source.h"
#include "GOAP/ChainProvider.h"

#include "TaskDelay.h"
#include "TaskPrint.h"
#include "TaskRoll.h"

#ifdef _WIN32
#   include <Windows.h>
#   define GOAP_SLEEP(Time) (::Sleep(Time))
#else
#   include <unistd.h>
#   define GOAP_SLEEP(Time) (::usleep(Time * 1000))
#endif

#include <time.h>

#include <string>
#include <typeinfo>

void main()
{
    Scheduler * sch = new Scheduler;

    srand( (unsigned int)time( NULL ) );

    printf( "%f %f %f\n", fmod( 0.5, 1.0 ), fmod( 1.3, 1.0 ), fmod( 3.0, 1.0 ) );

    GOAP::SourcePtr source = GOAP_NEW GOAP::Source();

    source->addTask( GOAP_NEW TaskPrint( "begin" ) );
    source->addTask( GOAP_NEW TaskDelay( 2000.f, sch ) );

    std::vector<int> v;
    v.push_back( 1 );
    v.push_back( 3 );
    v.push_back( 5 );
    v.push_back( 7 );

    for( auto & zip : source->addParallelZip( v ) )
    {
        zip.source->addTask( GOAP_NEW TaskPrint( std::to_string( *zip.value ).c_str() ) );
    }

    const GOAP::VectorSources & source_parallel = source->addParallel( 2 );

    source_parallel[0]->addTask( GOAP_NEW TaskDelay( 1000.f, sch ) );
    source_parallel[1]->addTask( GOAP_NEW TaskPrint( "process" ) );

    source->addTask( GOAP_NEW TaskPrint( "end" ) );
    source->addTask( GOAP_NEW TaskDelay( 1000.f, sch ) );
    source->addTask( GOAP_NEW TaskPrint( "****ROLL*****" ) );
    source->addTask( GOAP_NEW TaskDelay( 1000.f, sch ) );


    const GOAP::VectorSources & source_race = source->addRace( 3 );

    source_race[0]->addTask( GOAP_NEW TaskRoll( 200.f, 1, 6, sch ) );
    source_race[0]->addTask( GOAP_NEW TaskPrint( "---1---" ) );

    source_race[1]->addTask( GOAP_NEW TaskRoll( 100.f, 2, 12, sch ) );
    source_race[1]->addTask( GOAP_NEW TaskPrint( "---2---" ) );

    source_race[2]->addTask( GOAP_NEW TaskRoll( 50.f, 4, 24, sch ) );
    source_race[2]->addTask( GOAP_NEW TaskPrint( "---3---" ) );

    source->addTask( GOAP_NEW TaskDelay( 1000.f, sch ) );
    source->addTask( GOAP_NEW TaskPrint( "****WIN*****" ) );
    source->addTask( GOAP_NEW TaskDelay( 1000.f, sch ) );

    source->addCallback( []( const GOAP::CallbackObserverPtr & _observer, bool isSkip ) { printf( "HTTP!!!!!\n" ); GOAP_SLEEP( 100 ); _observer->onCallback( isSkip ); } );

    source->addFunction( [](){ printf( "WOW!!\n" ); } );

    source->addScope( []( const GOAP::SourcePtr & _scope ) -> bool { _scope->addFunction( []() {printf( "SCOPE????? WOW!!!" ); } ); return true; } );

    source->addFunction( [](){ printf( "Oh\n" ); } );


    GOAP::IfSource source_if = source->addIf( [](){ return rand() % 2 ? true : false; } );

    source_if.source_true->addTask( GOAP_NEW TaskPrint( "---TRUE---" ) );
    source_if.source_false->addTask( GOAP_NEW TaskPrint( "---FALSE---" ) );

    const GOAP::VectorSources & source_switch = source->addSwitch( 3, [](){ return rand() % 3; } );

    source_switch[0]->addTask( GOAP_NEW TaskPrint( "---Switch 1---" ) );
    source_switch[1]->addTask( GOAP_NEW TaskPrint( "---Switch 2---" ) );
    source_switch[2]->addTask( GOAP_NEW TaskPrint( "---Switch 3---" ) );

    GOAP::SourcePtr source_until = source->addRepeat( [sch]( const GOAP::SourcePtr & _scope ) -> bool
    {
        _scope->addTask( GOAP_NEW TaskDelay( 1000.f, sch ) );
        _scope->addTask( GOAP_NEW TaskPrint( "REPEAT!!!!" ) );

        return true;
    } );

    source_until->addTask( GOAP_NEW TaskDelay( 10000.f, sch ) );

    GOAP::ChainPtr tc = GOAP_NEW GOAP::Chain( source );

    tc->run();

    while( tc->isComplete() == false )
    {
        sch->update( 100.f );

        GOAP_SLEEP( 10 );
    }

    printf( "FINALIZE\n" );

    delete sch;
}