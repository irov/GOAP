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
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class GeneratorProvider
        : public Factorable
    {
    public:
        virtual float onDelay( uint32_t _index ) = 0;
        virtual void onEvent( const SourcePtr & _source, uint32_t _index, float _time ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<GeneratorProvider> GeneratorProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class FD, class FE>
    class GeneratorProviderT
        : public GeneratorProvider
    {
    public:
        explicit GeneratorProviderT( FD _fdelay, FE _fevent )
            : m_fdelay( _fdelay )
            , m_fevent( _fevent )
        {
        }

    public:
        float onDelay( uint32_t _index ) override
        {
            float t = m_fdelay( _index );

            return t;
        }

        void onEvent( const SourcePtr & _source, uint32_t _index, float _time ) override
        {
            m_fevent( _source, _index, _time );
        }

    protected:
        FD m_fdelay;
        FE m_fevent;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class FD, class FE>
        GeneratorProviderPtr makeGeneratorProvider( FD _fdelay, FE _fevent )
        {
            return GeneratorProviderPtr::from( new GeneratorProviderT<FD, FE>( _fdelay, _fevent ) );;
        }
    }
}