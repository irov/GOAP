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
    class TriggerProviderInterface
        : public Factorable
    {
    public:
        virtual bool onTrigger( const SourceInterfacePtr & _source ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TriggerProviderInterface> TriggerProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class TriggerProviderT
        : public TriggerProviderInterface
    {
    public:
        explicit TriggerProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        bool onTrigger( const SourceInterfacePtr & _source ) override
        {
            bool result = m_f( _source );

            return result;
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        TriggerProviderInterfacePtr makeTriggerProvider( Allocator * _allocator, F _f )
        {
            TriggerProviderInterface * provider = _allocator->allocateT<TriggerProviderT<F>>( _f );

            return TriggerProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}