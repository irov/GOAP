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
        explicit GuardProviderT( F _f )
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
            return GuardProviderPtr::from( new GuardProviderT<F>( _f ) );
        }
    }
}