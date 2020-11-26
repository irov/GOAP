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
    class FunctionProviderInterface
        : public Factorable
    {
    protected:
        FunctionProviderInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void onFunction() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<FunctionProviderInterface> FunctionProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class FunctionProviderT
        : public FunctionProviderInterface
    {
    public:
        FunctionProviderT( Allocator * _allocator, F _f )
            : FunctionProviderInterface( _allocator )
            , m_f( _f )
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
        FunctionProviderInterfacePtr makeFunctionProvider( Allocator * _allocator, F _f )
        {
            FunctionProviderInterface * provider = _allocator->allocateT<FunctionProviderT<F>>( _f );

            return FunctionProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}