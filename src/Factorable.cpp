/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Factorable.h"
#include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Factorable::Factorable()
        : m_reference( 0 )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Factorable::~Factorable()
    {
    };
    //////////////////////////////////////////////////////////////////////////
    uint32_t Factorable::incref()
    {
        ++m_reference;

        return m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    void Factorable::decref()
    {
        if( --m_reference == 0 )
        {
            delete this;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t Factorable::getrefcount() const
    {
        return m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    void * Factorable::operator new (size_t _size)
    {
        return GOAP_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Factorable::operator delete (void * _ptr, size_t _size)
    {
        GOAP_UNUSED( _size );

        GOAP_FREE( _ptr, _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void * Factorable::operator new []( size_t _size )
    {
        return GOAP_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void Factorable::operator delete []( void * _ptr, size_t _size )
    {
        GOAP_UNUSED( _size );

        GOAP_FREE( _ptr, _size );
    }
}