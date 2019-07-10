/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Exception.h"

#include <stdio.h>
#include <stdarg.h>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Exception::Exception( const char * _message )
        : m_message( _message )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Exception::~Exception() throw()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const char * Exception::what() const throw()
    {
        return m_message;
    }
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        //////////////////////////////////////////////////////////////////////////
        void throw_exception( const char * _format, ... )
        {
            va_list argList;

            va_start( argList, _format );

            char message[4096] = { 0 };
            vsprintf( message, _format, argList );

            va_end( argList );

            throw Exception( message );
        }
    }
}