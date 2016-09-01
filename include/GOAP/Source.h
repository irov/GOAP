#	pragma once

#	include "GOAP/IntrusivePtrBase.h"

#	include "GOAP/FunctionProvider.h"
#	include "GOAP/CallbackProvider.h"
#	include "GOAP/ScopeProvider.h"
#	include "GOAP/IfProvider.h"

#	include <vector>

namespace GOAP
{	
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Chain> ChainPtr;
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class Transcriptor> TranscriptorPtr;

	typedef std::vector<SourcePtr> TVectorSources;
	typedef std::vector<TranscriptorPtr> TVectorTranscriptor;

	struct IfSource
	{
		SourcePtr source_true;
		SourcePtr source_false;
	};
	
	class Source
		: public IntrusivePtrBase<Source>
	{
	public:
		Source();
		~Source();

	public:
		void setSkip( bool _skip );
		bool isSkip() const;

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

		template<class F>
		void addScope( F _f )
		{
			ScopeProviderPtr provider = new ScopeProviderT<F>( _f );

			this->addScopeProvider( provider );
		}

		template<class F>
		IfSource addIf( F _f )
		{
			IfProviderPtr provider = new IfProviderT<F>( _f );

			IfSource desc = this->addIfProvider( provider );

			return desc;
		}

		template<class F>
		SourcePtr addRepeat( F _f )
		{
			ScopeProviderPtr provider = new ScopeProviderT<F>( _f );

			SourcePtr source = this->addRepeatProvider( provider );

			return source;
		}

	protected:
		void addFunctionProvider( const FunctionProviderPtr & _provider );
		void addCallbackProvider( const CallbackProviderPtr & _provider );
		void addScopeProvider( const ScopeProviderPtr & _provider );
		IfSource addIfProvider( const IfProviderPtr & _provider );
		SourcePtr addRepeatProvider( const ScopeProviderPtr & _provider );

	public:
		TaskPtr parse( const ChainPtr & _chain, const TaskPtr & _task );

	protected:
		TVectorTranscriptor m_descriptions;

		bool m_skip;
	};

	typedef IntrusivePtr<Source> SourcePtr;
}