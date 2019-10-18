/*virtual
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Node.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class Alias
        : public TaskInterface
    {
    public:
        Alias();
        ~Alias() override;

    protected:
        bool _onRun( NodeInterface * _task ) override final;
        void _onFinally() override final;

    protected:
        virtual void _onAliasGenerate( const SourcePtr & _source ) = 0;
        virtual void _onAliasFinally();
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Alias> AliasPtr;
    //////////////////////////////////////////////////////////////////////////
}