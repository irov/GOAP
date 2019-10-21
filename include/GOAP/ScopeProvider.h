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
    class ScopeProvider
        : public Factorable
    {
    public:
        virtual void onScope( const SourceInterfacePtr & _source ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ScopeProvider> ScopeProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class ScopeProviderT
        : public ScopeProvider
    {
    public:
        explicit ScopeProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onScope( const SourceInterfacePtr & _source ) override
        {
            m_f( _source );
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        ScopeProviderPtr makeScopeProvider( F _f )
        {
            return ScopeProviderPtr::from( new ScopeProviderT<F>( _f ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}