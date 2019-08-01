/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/IntrusivePtr.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class Source, class T>
    class ZipPair
    {
    public:
        ZipPair( const SourcePtr & _source, const T & _value )
            : source( _source )
            , value( _value )
        {
        }

    public:
        const SourcePtr & source;
        const T & value;
    };
    //////////////////////////////////////////////////////////////////////////
    template<class L, class R>
    class Zip
    {
    public:
        Zip( L _lbegin, L _lend, R _rbegin, R _rend )
            : m_lbegin( _lbegin )
            , m_lend( _lend )
            , m_rbegin( _rbegin )
            , m_rend( _rend )
        {
        }

    public:
        class iterator
        {
        public:
            iterator( L _first, R _second )
                : first( _first )
                , second( _second )
                , index( 0U )
            {
            }

        public:
            iterator( const iterator & _it )
                : first( _it.first )
                , second( _it.second )
                , index( _it.index )
            {
            }

        public:
            iterator & operator = ( const iterator & _it )
            {
                this->first = _it.first;
                this->second = _it.second;
                this->index = _it.index;

                return *this;
            }

        public:
            iterator & operator ++ ()
            {
                ++this->first;
                ++this->second;
                ++this->index;

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
                return this->first == _it.first;
            }

            bool operator != ( const iterator & _it ) const
            {
                return !this->operator == ( _it );
            }

        public:
            ZipPair<typename L::value_type, typename R::value_type> operator * () const
            {
                return ZipPair<typename L::value_type, typename R::value_type>( *this->first, *this->second );
            }

        protected:
            L first;
            R second;
            uint32_t index;
        };

    public:
        iterator begin() const
        {
            return iterator( m_lbegin, m_rbegin );
        }

        iterator end() const
        {
            return iterator( m_lend, m_rend );
        }

    protected:
        L m_lbegin;
        L m_lend;
        R m_rbegin;
        R m_rend;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class L, class R>
        Zip<L, R> makeZip( L _lbegin, L _lend, R _rbegin, R _rend )
        {
            return Zip<L, R>( _lbegin, _lend, _rbegin, _rend );
        }
    }
}