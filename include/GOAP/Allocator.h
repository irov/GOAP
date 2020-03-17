/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

#include <utility>
#include <new>

namespace GOAP
{ 
    class Allocator
    {
    public:
        Allocator() = default;
        virtual ~Allocator() = default;

    public:
        template<class T, class ... Args>
        T * allocateT( Args && ... _args )
        {
            const size_t s = sizeof( T );
            void * p = this->allocate( s );

            new (p) T( std::forward<Args>( _args ) ... );

            Factorable * f = static_cast<Factorable *>(p);
            f->setAllocator( this );

            return static_cast<T *>(p);
        }

        template<class T>
        void deallocateT( T * _t )
        {
            _t->~T();

            this->deallocate( _t );
        }

    protected:
        virtual void * allocate( size_t _size ) = 0;
        virtual void deallocate( void * _ptr ) = 0;
    };
}