/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class GeneratorProvider
        : public Factorable
    {
    public:
        virtual void onGenerate( const SourcePtr & _source ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<GeneratorProvider> GeneratorProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class GeneratorProviderT
        : public GeneratorProvider
    {
    public:
        explicit GeneratorProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onGenerate( const SourcePtr & _source ) override
        {
            m_f( _source );
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        GeneratorProviderPtr makeGeneratorProvider( F _f )
        {
            GeneratorProviderPtr provider = new GeneratorProviderT<F>( _f );

            return provider;
        }
    }
}