/*virtual 
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#   include "GOAP/Task.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class Alias
        : public Task
    {
    public:
        Alias();
        ~Alias() override;

    protected:
        bool _onRun() override final;
        void _onFinally() override final;

    protected:
        virtual void _onGenerate( const SourcePtr & _source ) = 0;
        virtual void _onAliasFinally();

    protected:
        class BeginGuardProvider;
        class EndGuardProvider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Alias> AliasPtr;
    //////////////////////////////////////////////////////////////////////////
}