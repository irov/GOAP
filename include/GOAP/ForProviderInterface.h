/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class ForProviderInterface
        : public Factorable
    {
    public:
        virtual bool onFor( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ForProviderInterface> ForProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class ForProviderT
        : public ForProviderInterface
    {
    public:
        explicit ForProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        bool onFor( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ) override
        {
            bool successful = m_f( _source, _iterator, _count );

            return successful;
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        ForProviderInterfacePtr makeForProvider( Allocator * _allocator, F _f )
        {
            ForProviderInterface * provider = _allocator->allocateT<ForProviderT<F>>( _f );

            return ForProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}