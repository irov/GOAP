/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#   include "GOAP/Task.h"
#   include "GOAP/Source.h"

namespace GOAP
{
    typedef IntrusivePtr<class Source> SourcePtr;

    class Alias
        : public Task
    {
    public:
        Alias();

    protected:
        bool _onRun() override;

    protected:
        virtual void _onGenerate( const SourcePtr & _source ) = 0;
    };

    typedef IntrusivePtr<Alias> AliasPtr;
}