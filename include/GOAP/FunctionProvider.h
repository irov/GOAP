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
    class FunctionProvider
        : public Factorable
    {
    public:
        virtual void onFunction() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<FunctionProvider> FunctionProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class FunctionProviderT
        : public FunctionProvider
    {
    public:
        explicit FunctionProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onFunction() override
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
        FunctionProviderPtr makeFunctionProvider( Allocator * _allocator, F _f )
        {
            FunctionProvider * provider = _allocator->allocateT<FunctionProviderT<F>>( _f );

            return FunctionProviderPtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}