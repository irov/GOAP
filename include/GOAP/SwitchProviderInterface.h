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
    class SwitchProviderInterface
        : public Factorable
    {
    public:
        virtual uint32_t onSwitch() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SwitchProviderInterface> SwitchProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class SwitchProviderT
        : public SwitchProviderInterface
    {
    public:
        explicit SwitchProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        uint32_t onSwitch() override
        {
            uint32_t result = m_f();

            return result;
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        SwitchProviderInterfacePtr makeSwitchProvider( Allocator * _allocator, F _f )
        {
            SwitchProviderInterface * provider = _allocator->allocateT<SwitchProviderT<F>>( _f );

            return SwitchProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}