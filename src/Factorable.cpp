/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Factorable.h"
#include "GOAP/Allocator.h"
#include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Factorable::Factorable( Allocator * _allocator )
        : m_allocator( _allocator )
        , m_reference( 0 )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Factorable::~Factorable()
    {
    };
    //////////////////////////////////////////////////////////////////////////
    Allocator * Factorable::getAllocator() const
    {
        return m_allocator;
    }
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
            m_allocator->deallocateT( this );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t Factorable::getrefcount() const
    {
        return m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
}