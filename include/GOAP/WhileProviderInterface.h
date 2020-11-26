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
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class WhileProviderInterface
        : public Factorable
    {
    protected:
        WhileProviderInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual bool onWhile( const SourceInterfacePtr & _source ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<WhileProviderInterface> WhileProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class WhileProviderT
        : public WhileProviderInterface
    {
    public:
        WhileProviderT( Allocator * _allocator, F _f )
            : WhileProviderInterface( _allocator )
            , m_f( _f )
        {
        }

    public:
        bool onWhile( const SourceInterfacePtr & _source ) override
        {
            bool successful = m_f( _source );

            return successful;
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        WhileProviderInterfacePtr makeWhileProvider( Allocator * _allocator, F _f )
        {
            WhileProviderInterface * provider = _allocator->allocateT<WhileProviderT<F>>( _f );

            return WhileProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}