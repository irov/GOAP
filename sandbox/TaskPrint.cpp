#include "TaskPrint.h"

#include "GOAP/Node.h"

#include <stdio.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////////
TaskPrint::TaskPrint( const char * _format, ... )
{
    va_list args;
    va_start( args, _format );

    int size = vsprintf( m_message, _format, args );
    
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

    printf( m_message );

    return true;
}