/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class ForProvider
        : public Factorable
    {
    public:
        virtual bool onFor( const SourcePtr & _source, uint32_t _iterator, uint32_t _count ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ForProvider> ForProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class ForProviderT
        : public ForProvider
    {
    public:
        ForProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        bool onFor( const SourcePtr & _source, uint32_t _iterator, uint32_t _count ) override
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
            ForProviderPtr provider = GOAP_NEW ForProviderT<F>( _f );

            return provider;
        }
    }
}