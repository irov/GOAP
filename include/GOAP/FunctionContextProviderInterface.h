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
    class FunctionContextProviderInterface
        : public Factorable
    {
    public:
        virtual void onFunctionContext( bool _isSkip ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<FunctionContextProviderInterface> FunctionContextProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class FunctionContextProviderT
        : public FunctionContextProviderInterface
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
        FunctionContextProviderInterfacePtr makeFunctionContextProvider( Allocator * _allocator, F _f )
        {
            FunctionContextProviderInterface * provider = _allocator->allocateT<FunctionContextProviderT<F>>( _f );

            return FunctionContextProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}