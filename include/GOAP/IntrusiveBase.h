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
        class IntrusiveBase
        {
        public:
            IntrusiveBase()
                : m_reference( 0 )
            {
            }

        public:
            inline static void IntrusivePtrAddRef( IntrusiveBase * _ptr );
            inline static void IntrusivePtrDecRef( IntrusiveBase * _ptr );

        public:
            virtual void destroy() = 0;

        protected:
            uint32_t m_reference;
        };
        //////////////////////////////////////////////////////////////////////////
        inline void IntrusiveBase::IntrusivePtrAddRef( IntrusiveBase * _ptr )
        {
            ++_ptr->m_reference;
        }
        //////////////////////////////////////////////////////////////////////////
        inline void IntrusiveBase::IntrusivePtrDecRef( IntrusiveBase * _ptr )
        {
            if( --_ptr->m_reference == 0 )
            {
                _ptr->destroy();
            }
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusivePtrSetup( T *& _ptr, T * _other )
        {
            _ptr = _other;

            if( _other != nullptr )
            {
                T::IntrusivePtrAddRef( _other );
            }
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusivePtrSetup( T *& _ptr, const IntrusivePtr<T> & _other )
        {
            T * other_ptr = _other.get();

            IntrusivePtrSetup( _ptr, other_ptr );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusivePtrRelease( T *& _ptr )
        {
            if( _ptr != nullptr )
            {
                T::IntrusivePtrDecRef( _ptr );
                _ptr = nullptr;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusiveThisAcquire( T * _ptr )
        {
            T::IntrusivePtrAddRef( _ptr );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        inline void IntrusiveThisRelease( T * _ptr )
        {
            T::IntrusivePtrDecRef( _ptr );
        }
    }
}