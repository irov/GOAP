/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
    typedef IntrusivePtr<class Factory> FactoryPtr;
    typedef IntrusivePtr<class Source> SourcePtr;

	class Alias
		: public Task
	{
    public:
        Alias( const FactoryPtr & _factory );

    protected:
        bool _onRun() override;

    protected:
        virtual void _onGenerate( const SourcePtr & _source ) = 0;

    protected:
        FactoryPtr m_factory;
	};

	typedef IntrusivePtr<Alias> AliasPtr;
}