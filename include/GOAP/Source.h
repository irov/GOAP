#	pragma once

#	include "IntrusivePtrBase.h"

#	include "FunctionProvider.h"
#	include "CallbackProvider.h"

#	include <vector>

namespace GOAP
{	
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Chain> TaskChainPtr;
	typedef IntrusivePtr<class Source> TaskSourcePtr;
	typedef IntrusivePtr<class Transcriptor> TaskDescriptionPtr;

	typedef std::vector<TaskSourcePtr> TVectorSources;
	typedef std::vector<TaskDescriptionPtr> TVectorDescriptions;

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
		TaskPtr parse( const TaskChainPtr & _chain, const TaskPtr & _task );

	protected:
		TVectorDescriptions m_descriptions;
	};

	typedef IntrusivePtr<Source> TaskSourcePtr;
}