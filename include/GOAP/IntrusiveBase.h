/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#   include <stdint.h>

#	include "GOAP/IntrusivePtr.h"

namespace GOAP
{
    namespace Detail
    {
        template<class T>
        class IntrusiveBase
        {
        public:
            IntrusiveBase()
                : m_reference( 0 )
            {
            }

        public:
            inline static void intrusive_ptr_add_ref( T * _ptr );
            inline static void intrusive_ptr_dec_ref( T * _ptr );

        public:
            inline static void intrusive_ptr_destroy( T * _ptr );

        protected:
            uint32_t m_reference;
        };
        //////////////////////////////////////////////////////////////////////////
        template<class T, class U>
        inline void IntrusivePtrSetup( T *& _ptr, U * _other )
        {
            _ptr = static_cast<T *>(_other);

            if( _other != nullptr )
            {
                T * ptr = static_cast<T *>(_other);

                T::intrusive_ptr_add_ref( ptr );
            }
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusivePtrSetup( T *& _ptr, const IntrusivePtr<T> & _other )
        {
            T * other_ptr = _other.get();

            GOAP::IntrusivePtrSetup( _ptr, other_ptr );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusivePtrRelease( T *& _ptr )
        {
            if( _ptr != nullptr )
            {
                T::intrusive_ptr_dec_ref( _ptr );
                _ptr = nullptr;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusiveBase<T>::intrusive_ptr_add_ref( T * _ptr )
        {
            ++_ptr->m_reference;
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusiveBase<T>::intrusive_ptr_dec_ref( T * _ptr )
        {
            if( --_ptr->m_reference == 0 )
            {
                T::intrusive_ptr_destroy( _ptr );
            }
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusiveBase<T>::intrusive_ptr_destroy( T * _ptr )
        {
            delete _ptr;
            //Empty
        }
    }
}