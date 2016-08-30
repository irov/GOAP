#	pragma once

#	include "IntrusivePtrBase.h"

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;

	class ScopeProvider
		: public IntrusivePtrBase<ScopeProvider>
	{
	public:
		virtual bool onScope( const SourcePtr & _source ) = 0;
	};

	typedef IntrusivePtr<ScopeProvider> ScopeProviderPtr;

	template<class F>
	class ScopeProviderT
		: public ScopeProvider
	{
	public:
		ScopeProviderT( F _f )
			: m_f( _f )
		{
		}

	public:
		bool onScope( const SourcePtr & _source ) override
		{
			bool successful = m_f( _source );

			return successful;
		}

	protected:
		F m_f;
	};

	template<class F>
	ScopeProviderPtr makeScopeProvider( F _f )
	{
		ScopeProviderPtr provider = new ScopeProviderT<F>( _f );

		return provider;
	}
}