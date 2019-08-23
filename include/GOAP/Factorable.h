/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/IntrusivePtrBase.h"

namespace GOAP
{
    class Factorable
        : public IntrusivePtrBase
    {
    public:
        Factorable();
        virtual ~Factorable();

    public:
        uint32_t incref() override;
        void decref() override;
        uint32_t getrefcount() const override;

    public:
        void * operator new (size_t _size);
        void operator delete (void * _ptr, size_t _size);
        void * operator new[]( size_t _size );
        void operator delete[]( void * _ptr, size_t _size );

    protected:
        uint32_t m_reference;
    };
}