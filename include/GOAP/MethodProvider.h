/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/FunctionProvider.h"
#include "GOAP/Tuple.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    template<class C, class M, class Args>
    class MethodProviderT
        : public FunctionProvider
    {
    public:
        MethodProviderT( C * _self, M _method, Args && _args )
            : m_self( _self )
            , m_method( _method )
            , m_args( std::forward<Args &&>( _args ) )
        {
        }

    public:
        void onFunction() override
        {
            std::apply( m_method, std::tuple_cat( std::make_tuple( m_self ), m_args ) );
        }

    protected:
        C * m_self;
        M m_method;
        Args m_args;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class C, class M, class ... Args>
        FunctionProviderPtr makeMethodProvider( C * _self, M _method, Args ... _args )
        {
            typedef Tuple<Args ...> TupleArgs;

            return FunctionProviderPtr::from( new MethodProviderT<C, M, TupleArgs>( _self, _method, std::make_tuple( std::forward<Args &&>( _args ) ... ) ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}