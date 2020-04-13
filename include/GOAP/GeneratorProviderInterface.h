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
    class GeneratorProviderInterface
        : public Factorable
    {
    public:
        virtual float onDelay( uint32_t _index ) = 0;
        virtual void onEvent( const SourceInterfacePtr & _source, uint32_t _index, float _time ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<GeneratorProviderInterface> GeneratorProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class FD, class FE>
    class GeneratorProviderT
        : public GeneratorProviderInterface
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

        void onEvent( const SourceInterfacePtr & _source, uint32_t _index, float _time ) override
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
        GeneratorProviderInterfacePtr makeGeneratorProvider( Allocator * _allocator, FD _fdelay, FE _fevent )
        {
            GeneratorProviderInterface * provider = _allocator->allocateT<GeneratorProviderT<FD, FE>>( _fdelay, _fevent );

            return GeneratorProviderInterfacePtr::from( provider );
        }
    }
}