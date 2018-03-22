/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class ChainProvider
        : public Factorable
    {
    public:
        virtual void onChain( bool _skip ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ChainProvider> ChainProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class ChainProviderT
        : public ChainProvider
    {
    public:
        ChainProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onChain( bool _skip ) override
        {
            m_f( _skip );
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
            ChainProviderPtr provider = GOAP_NEW ChainProviderT<F>( _f );

            return provider;
        }
    }
}