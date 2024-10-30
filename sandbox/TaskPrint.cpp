#include "TaskPrint.h"

#include "GOAP/NodeInterface.h"

#include <cstdio>
#include <cstdarg>

//////////////////////////////////////////////////////////////////////////
TaskPrint::TaskPrint( GOAP::Allocator * _allocator, const char * _format, ... )
    : GOAP::TaskInterface( _allocator )
{
    va_list args;
    va_start( args, _format );

    int size = vsnprintf( m_message, 1024, _format, args );
    
    va_end( args );

    if( size > 0 )
    {
        m_message[size + 0] = '\n';
        m_message[size + 1] = '\0';
    }
    else
    {
        m_message[0] = '\0';
    }
}
//////////////////////////////////////////////////////////////////////////
TaskPrint::~TaskPrint()
{
}
//////////////////////////////////////////////////////////////////////////
bool TaskPrint::_onRun( GOAP::NodeInterface * _node )
{
    GOAP_UNUSED( _node );

    printf( "%s", m_message );

    return true;
}