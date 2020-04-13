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
    class IfProviderInterface
        : public Factorable
    {
    public:
        virtual bool onIf() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<IfProviderInterface> IfProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class IfProviderT
        : public IfProviderInterface
    {
    public:
        explicit IfProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        bool onIf() const override
        {
            bool result = m_f();

            return result;
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        IfProviderInterfacePtr makeIfProvider( Allocator * _allocator, F _f )
        {
            IfProviderInterface * provider = _allocator->allocateT<IfProviderT<F>>( _f );

            return IfProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}