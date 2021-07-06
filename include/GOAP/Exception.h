/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

#include <exception>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Exception
        : public std::exception
    {
    public:
        explicit Exception( const char * _message );
        ~Exception() throw();

    public:
        const char * what() const throw() override;

    protected:
        const char * m_message;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        void throw_exception( const char * _format, ... );
    }
    //////////////////////////////////////////////////////////////////////////
}

