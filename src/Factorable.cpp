/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Factorable.h"
#include "GOAP/Macros.h"

#include <stdlib.h>

namespace GOAP
{
    namespace Detail
    {
        //////////////////////////////////////////////////////////////////////////
        Factorable::Factorable()
#ifdef GOAP_DEBUG
            : m_debugfile(nullptr)
            , m_debugline(0)
#endif
        {
        }
        //////////////////////////////////////////////////////////////////////////
        Factorable::~Factorable()
        {
        }
#ifdef GOAP_DEBUG
        //////////////////////////////////////////////////////////////////////////
        void Factorable::operator delete(void* _ptr)
        {
            free( _ptr );
        }
        //////////////////////////////////////////////////////////////////////////
        void * Factorable::operator new (std::size_t _size, const char * _file, int _line)
        {
            void * ptr = malloc( _size );

            Factorable * t = reinterpret_cast<Factorable*>(ptr);

            t->m_debugfile = _file;
            t->m_debugline = _line;

            return ptr;
        }
        //////////////////////////////////////////////////////////////////////////
        void Factorable::operator delete(void* _ptr, const char * _file, int _line)
        {
            GOAP_UNUSED( _file );
            GOAP_UNUSED( _line );

            free( _ptr );
        }
#endif
        //////////////////////////////////////////////////////////////////////////
        void Factorable::destroy()
        {
            delete this;
        }
    }
}