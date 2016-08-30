#	include "GOAP/Source.h"

#	include "GOAP/Task.h"
#	include "GOAP/Chain.h"

#	include "GOAP/FunctionProvider.h"
#	include "GOAP/TaskFunction.h"
#	include "GOAP/TaskCallback.h"
#	include "GOAP/TaskScope.h"

#	include "TranscriptorBase.h"
#	include "TranscriptorParallel.h"
#	include "TranscriptorRace.h"


namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	Source::Source()
		: m_skip(false)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Source::~Source()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void Source::setSkip( bool _skip )
	{
		m_skip = _skip;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Source::isSkip() const
	{
		return m_skip;
	}
	//////////////////////////////////////////////////////////////////////////
	void Source::addTask( const TaskPtr & _task )
	{
		TranscriptorBase * description = new TranscriptorBase( _task );

		m_descriptions.push_back( description );
	}
	//////////////////////////////////////////////////////////////////////////
	TVectorSources & Source::addParallel( size_t _count )
	{
		TranscriptorParallel * description = new TranscriptorParallel( _count );

		m_descriptions.push_back( description );

		TVectorSources & sources = description->getSources();

		return sources;
	}
	//////////////////////////////////////////////////////////////////////////
	TVectorSources & Source::addRace( size_t _count )
	{
		TranscriptorRace * description = new TranscriptorRace( _count );

		m_descriptions.push_back( description );

		TVectorSources & sources = description->getSources();

		return sources;
	}
	//////////////////////////////////////////////////////////////////////////
	void Source::addFunctionProvider( const FunctionProviderPtr & _provider )
	{
		TaskPtr task = new TaskFunction( _provider );

		this->addTask( task );
	}
	//////////////////////////////////////////////////////////////////////////
	void Source::addCallbackProvider( const CallbackProviderPtr & _provider )
	{
		TaskPtr task = new TaskCallback( _provider );

		this->addTask( task );
	}
	//////////////////////////////////////////////////////////////////////////
	void Source::addScopeProvider( const ScopeProviderPtr & _provider )
	{
		TaskPtr task = new TaskScope( _provider );

		this->addTask( task );
	}
	//////////////////////////////////////////////////////////////////////////
	TaskPtr Source::parse( const ChainPtr & _chain, const TaskPtr & _task )
	{
		TaskPtr current_task = _task;

		for( TVectorTranscriptor::iterator
			it = m_descriptions.begin(),
			it_end = m_descriptions.end();
		it != it_end;
		++it )
		{
			const TranscriptorPtr & description = *it;

			TaskPtr last_task = description->generate( _chain, current_task );

			current_task = last_task;
		}

		return current_task;
	}
}