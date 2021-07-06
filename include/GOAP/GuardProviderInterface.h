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
    class GuardProviderInterface
        : public Factorable
    {
    public:
        GuardProviderInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void onGuard() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<GuardProviderInterface> GuardProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class GuardProviderT
        : public GuardProviderInterface
    {
    public:
        GuardProviderT( Allocator * _allocator, F _f )
            : GuardProviderInterface( _allocator )
            , m_f( _f )
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
        GuardProviderInterfacePtr makeGuardProvider( Allocator * _allocator, F _f )
        {
            GuardProviderInterface * provider = _allocator->allocateT<GuardProviderT<F>>( _f );

            return GuardProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}