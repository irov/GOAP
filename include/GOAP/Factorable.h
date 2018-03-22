/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/IntrusiveBase.h"

#ifdef GOAP_DEBUG
#   include <stdint.h>
#endif

#ifdef _DEBUG
#   ifndef GOAP_NDEBUG
#       define GOAP_DEBUG
#   endif
#endif

#ifdef GOAP_DEBUG
#   include <new>
#   define GOAP_NEW new(__FILE__, __LINE__)
#   define GOAP_DELETE delete(__FILE__, __LINE__)
#else
#   define GOAP_NEW new
#   define GOAP_DELETE delete
#endif

namespace GOAP
{
    namespace Detail
    {
        class Factorable
            : public IntrusiveBase
        {
        public:
            Factorable();
            virtual ~Factorable();

#ifdef GOAP_DEBUG
        public:
            void operator delete(void* _ptr);

            void * operator new (std::size_t _size, const char * _file, int _line);
            void operator delete(void* _ptr, const char * _file, int _line);
#endif

        protected:
            void destroy() override;

#ifdef GOAP_DEBUG
        protected:
            const char * m_debugfile;
            uint32_t m_debugline;
#endif
        };
    }
}