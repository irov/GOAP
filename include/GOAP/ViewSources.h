/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/ArraySources.h"

namespace GOAP
{
    class ViewSources
    {
    public:
        ViewSources( const SourcePtr * _begin, const SourcePtr * _end )
            : m_begin( _begin )
            , m_end( _end )
        {
        }

        template<size_t Count>
        ViewSources( const ArraySources<Count> & _sources )
            : ViewSources( _sources.data(), _sources.data() + _sources.size() )
        {
        }

    public:
        uint32_t size() const
        {
            return (uint32_t)(m_end - m_begin);
        }

        bool empty() const
        {
            return m_end == m_begin;
        }

    public:
        const SourcePtr * begin() const
        {
            return m_begin;
        }

        const SourcePtr * end() const
        {
            return m_end;
        }

    protected:
        const SourcePtr * m_begin;
        const SourcePtr * m_end;
    };
}
