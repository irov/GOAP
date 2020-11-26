/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/Allocator.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class ChainProviderInterface
        : public Factorable
    {
    protected:
        ChainProviderInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void onChain( bool _skip, bool _cancel ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ChainProviderInterface> ChainProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class ChainProviderT
        : public ChainProviderInterface
    {
    public:
        ChainProviderT( Allocator * _allocator, F _f )
            : ChainProviderInterface( _allocator )
            , m_f( _f )
        {
        }

    public:
        void onChain( bool _skip, bool _cancel ) override
        {
            m_f( _skip, _cancel );
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        ChainProviderInterfacePtr makeChainProvider( Allocator * _allocator, F _f )
        {
            ChainProviderInterface * provider = _allocator->allocateT<ChainProviderT<F>>( _f );

            return ChainProviderInterfacePtr::from( provider );
        }
    }
}