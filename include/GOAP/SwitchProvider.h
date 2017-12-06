/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class SwitchProvider
        : public Factorable
    {
    public:
        virtual uint32_t onSwitch() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SwitchProvider> SwitchProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class SwitchProviderT
        : public SwitchProvider
    {
    public:
        SwitchProviderT( F _f )
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
        SwitchProviderPtr makeSwitchProvider( F _f )
        {
            SwitchProviderPtr provider = GOAP_NEW SwitchProviderT<F>( _f );

            return provider;
        }
    }
}