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
    class ForProvider
        : public Factorable
    {
    public:
        virtual bool onFor( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ForProvider> ForProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class ForProviderT
        : public ForProvider
    {
    public:
        explicit ForProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        bool onFor( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ) override
        {
            bool successful = m_f( _source, _iterator, _count );

            return successful;
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        ForProviderPtr makeForProvider( F _f )
        {
            return ForProviderPtr::from( new ForProviderT<F>( _f ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}