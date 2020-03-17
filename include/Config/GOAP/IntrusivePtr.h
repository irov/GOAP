/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

namespace GOAP
{
    template<class T>
    class IntrusivePtr
    {
    public:
        typedef T element_type;

    public:
        static const IntrusivePtr & none()
        {
            static IntrusivePtr ptr_none;

            return ptr_none;
        }

        static IntrusivePtr from( const element_type * _ptr )
        {
            return IntrusivePtr( _ptr );
        }

    public:
        IntrusivePtr()
            : m_ptr( nullptr )
        {
        }

        explicit IntrusivePtr( std::nullptr_t )
            : m_ptr( nullptr )
        {
        }

        explicit IntrusivePtr( const element_type * _ptr )
            : m_ptr( const_cast<element_type *>(_ptr) )
        {
            this->incref();
        }

        template<class U>
        // cppcheck-suppress noExplicitConstructor
        explicit IntrusivePtr( const U * _ptr )
            : m_ptr( static_cast<element_type *>(const_cast<U *>(_ptr)) )
        {
            this->incref();
        }

        IntrusivePtr( const IntrusivePtr & _rhs )
            : m_ptr( _rhs.get() )
        {
            this->incref();
        }

        template<class U>
        IntrusivePtr( const IntrusivePtr<U> & _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            this->incref();
        }

        ~IntrusivePtr()
        {
            this->decref();
        }

        IntrusivePtr & operator = ( const IntrusivePtr & _rhs )
        {
            IntrusivePtr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        IntrusivePtr & operator = ( std::nullptr_t )
        {
            IntrusivePtr swap_ptr;
            swap_ptr.swap( *this );

            return *this;
        }

    public:
        element_type * get() const
        {
            return m_ptr;
        }

        template<class K>
        K getT() const
        {
            K ptr_t = static_cast<K>(m_ptr);

            return ptr_t;
        }

        element_type * operator -> () const
        {
            return m_ptr;
        }

        void swap( IntrusivePtr & _rhs )
        {
            T * tmp = m_ptr;
            m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }

    protected:
        void incref()
        {
            if( m_ptr != nullptr )
            {
                m_ptr->incref();
            }
        }

        void decref()
        {
            if( m_ptr != nullptr )
            {
                m_ptr->decref();
            }
        }

    protected:
        element_type * m_ptr;
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void swap( const IntrusivePtr<T> & _left, const IntrusivePtr<T> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    U intrusive_static_cast( const IntrusivePtr<T> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = static_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    U intrusive_dynamic_cast( const IntrusivePtr<T> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = dynamic_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    U intrusive_get( const IntrusivePtr<T> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U u_ptr = static_cast<U>(t_ptr);

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator < ( const IntrusivePtr<T> & _left, const IntrusivePtr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l < r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator >( const IntrusivePtr<T> & _left, const IntrusivePtr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l > r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator == ( const IntrusivePtr<T> & _left, const IntrusivePtr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator != ( const IntrusivePtr<T> & _left, const IntrusivePtr<T> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator != ( const IntrusivePtr<T> & _left, const U * _right )
    {
        const T * ptr = _left.get();

        return ptr != static_cast<const T *>(_right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator != ( const U * _left, const IntrusivePtr<T> & _right )
    {
        const T * ptr = intrusive_get<T>( _right );

        return ptr != static_cast<const T *>(_left);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator == ( const U * _left, const IntrusivePtr<T> & _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator == ( const IntrusivePtr<T> & _left, const U * _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator != ( const IntrusivePtr<T> & _left, std::nullptr_t _nullptr )
    {
        const T * ptr = _left.get();

        return ptr != _nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator != ( std::nullptr_t _nullptr, const IntrusivePtr<T> & _right )
    {
        const T * ptr = intrusive_get<T *>( _right );

        return ptr != _nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator == ( std::nullptr_t _nullptr, const IntrusivePtr<T> & _right )
    {
        const T * ptr = intrusive_get<T *>( _right );

        return ptr == _nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator == ( const IntrusivePtr<T> & _left, std::nullptr_t _nullptr )
    {
        const T * ptr = _left.get();

        return ptr == _nullptr;
    }
}