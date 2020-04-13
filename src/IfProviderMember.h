/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/IfProviderInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class IfProviderMember
        : public IfProviderInterface
    {
    public:
        GOAP_INLINE explicit IfProviderMember( const bool * _member )
            : m_member( _member )
        {
        }

    public:
        GOAP_INLINE bool onIf() const override
        {
            bool result = *m_member;

            return result;
        }

    protected:
        const bool * m_member;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<IfProviderMember> IfProviderMemberPtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        GOAP_INLINE IfProviderMemberPtr makeIfProviderMember( Allocator * _allocator, const bool * _member )
        {
            IfProviderMember * provider = _allocator->allocateT<IfProviderMember>( _member );

            return IfProviderMemberPtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}