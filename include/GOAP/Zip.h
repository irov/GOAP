/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/IntrusivePtr.h"

#include <tuple>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    template<class S, class L, class R>
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
            std::tuple<S, typename R::value_type> operator * () const
            {
                return std::forward_as_tuple( *this->first, *this->second );
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
        template<class S, class L, class R>
        Zip<S, L, R> makeZip( L _lbegin, L _lend, R _rbegin, R _rend )
        {
            return Zip<S, L, R>( _lbegin, _lend, _rbegin, _rend );
        }
    }
}