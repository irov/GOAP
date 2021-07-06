/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/ArraySources.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class Type>
    class ViewSources
    {
    public:
        ViewSources( const SourceInterfacePtr * _begin, const SourceInterfacePtr * _end )
            : m_begin( _begin )
            , m_end( _end )
        {
        }

        template<size_t Count>
        ViewSources( const ArraySources<Count> & _sources )
            : ViewSources( _sources.data(), _sources.data() + _sources.size() )
        {
        }

        ViewSources( const Vector<SourceInterfacePtr> & _sources )
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
        class iterator
        {
        public:
            explicit iterator( const SourceInterfacePtr * _source )
                : m_source( _source )
            {
            }

        public:
            iterator( const iterator & _it )
                : m_source( _it.m_source )
            {
            }

        public:
            iterator & operator = ( const iterator & _it )
            {
                this->m_source = _it.m_source;

                return *this;
            }

        public:
            iterator & operator ++ ()
            {
                ++this->m_source;

                return *this;
            }

            iterator operator++ ( int )
            {
                iterator result( *this );
                ++(*this);
                return result;
            }

            bool operator == ( const iterator & _it ) const
            {
                return this->m_source == _it.m_source;
            }

            bool operator != ( const iterator & _it ) const
            {
                return !this->operator == ( _it );
            }

        public:
            IntrusivePtr<Type> operator * () const
            {
                const SourceInterface * source = m_source->get();

                return IntrusivePtr<Type>::from( source );
            }

            IntrusivePtr<Type> operator -> () const
            {
                const SourceInterface * source = m_source->get();

                return IntrusivePtr<Type>::from( source );
            }

        protected:
            const SourceInterfacePtr * m_source;
        };

    public:
        iterator begin() const
        {
            return iterator( m_begin );
        }

        iterator end() const
        {
            return iterator( m_end );
        }

    public:
        IntrusivePtr<Type> operator [] ( size_t _index ) const
        {
            return *(m_begin + _index);
        }

    public:
        const SourceInterfacePtr * m_begin;
        const SourceInterfacePtr * m_end;
    };
    //////////////////////////////////////////////////////////////////////////
}
