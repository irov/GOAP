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
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class GuardProvider
        : public Factorable
    {
    public:
        virtual void onGuard() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<GuardProvider> GuardProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class GuardProviderT
        : public GuardProvider
    {
    public:
        GuardProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onGuard() override
        {
            m_f();
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        GuardProviderPtr makeGuardProvider( F _f )
        {
            GuardProviderPtr provider = GOAP_NEW GuardProviderT<F>( _f );

            return provider;
        }
    }
}