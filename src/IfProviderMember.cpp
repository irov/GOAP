/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/IfProviderMember.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    IfProviderMember::IfProviderMember( bool & _member )
        : m_member( _member )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool IfProviderMember::onIf()
    {
        bool result = m_member;

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        IfProviderPtr makeIfProviderMember( bool & _member )
        {
            return IfProviderPtr::from( new IfProviderMember( _member ) );
        }
    }
}