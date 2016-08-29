#	pragma once

#	include "GOAP/IntrusivePtrBase.h"

#	include "GOAP/FunctionProvider.h"
#	include "GOAP/CallbackProvider.h"

#	include <vector>

namespace GOAP
{	
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Chain> ChainPtr;
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class Transcriptor> TranscriptorPtr;

	typedef std::vector<SourcePtr> TVectorSources;
	typedef std::vector<TranscriptorPtr> TVectorTranscriptor;

	class Source
		: public IntrusivePtrBase<Source>
	{
	public:
		Source();
		~Source();

	public:
		void addTask( const TaskPtr & _task );
		TVectorSources & addParallel( size_t _count );
		TVectorSources & addRace( size_t _count );

		template<class F>
		void addFunction( F _f )
		{
			FunctionProviderPtr provider = new FunctionProviderT<F>( _f );

			this->addFunctionProvider( provider );
		}

		template<class F>
		void addCallback( F _f )
		{
			CallbackProviderPtr provider = new CallbackProviderT<F>( _f );

			this->addCallbackProvider( provider );
		}

	protected:
		void addFunctionProvider( const FunctionProviderPtr & _provider );
		void addCallbackProvider( const CallbackProviderPtr & _provider );

	public:
		TaskPtr parse( const ChainPtr & _chain, const TaskPtr & _task );

	protected:
		TVectorTranscriptor m_descriptions;
	};

	typedef IntrusivePtr<Source> SourcePtr;
}