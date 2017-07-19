#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
    typedef IntrusivePtr<class Source> SourcePtr;

	class Alias
		: public Task
	{
    protected:
        bool _onRun() override;

    protected:
        virtual void _onGenerate( const SourcePtr & _source ) = 0;
	};

	typedef IntrusivePtr<Alias> AliasPtr;
}