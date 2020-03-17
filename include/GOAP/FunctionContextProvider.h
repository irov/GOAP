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
    class FunctionContextProvider
        : public Factorable
    {
    public:
        virtual void onFunctionContext( bool _isSkip ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<FunctionContextProvider> FunctionContextProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class FunctionContextProviderT
        : public FunctionContextProvider
    {
    public:
        explicit FunctionContextProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onFunctionContext( bool _isSkip ) override
        {
            m_f( _isSkip );
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        FunctionContextProviderPtr makeFunctionContextProvider( Allocator * _allocator, F _f )
        {
            FunctionContextProvider * provider = _allocator->allocateT<FunctionContextProviderT<F>>( _f );

            return FunctionContextProviderPtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}