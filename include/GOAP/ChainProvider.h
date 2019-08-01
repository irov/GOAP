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
    class ChainProvider
        : public Factorable
    {
    public:
        virtual void onChain( bool _skip, bool _cancel ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ChainProvider> ChainProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class ChainProviderT
        : public ChainProvider
    {
    public:
        explicit ChainProviderT( F _f )
            : m_f( _f )
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
        ChainProviderPtr makeChainProvider( F _f )
        {
            return ChainProviderPtr::from( new ChainProviderT<F>( _f ) );
        }
    }
}