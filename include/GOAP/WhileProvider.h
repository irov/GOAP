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
    class WhileProvider
        : public Factorable
    {
    public:
        virtual bool onWhile( const SourceInterfacePtr & _source ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<WhileProvider> WhileProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class WhileProviderT
        : public WhileProvider
    {
    public:
        explicit WhileProviderT( F _f )
            : m_f( _f )
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
        WhileProviderPtr makeWhileProvider( F _f )
        {
            return WhileProviderPtr::from( new WhileProviderT<F>( _f ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}