#	pragma once

#	include "GOAP/Config.h"

namespace GOAP
{
	class FunctionProvider
		: public IntrusiveBase<FunctionProvider>
	{
	public:
		virtual void onFunction() = 0;
	};

	typedef IntrusivePtr<FunctionProvider> FunctionProviderPtr;

	template<class F>
	class FunctionProviderT
		: public FunctionProvider
	{
	public:
		FunctionProviderT( F _f )
			: m_f( _f )
		{
		}

	public:
		void onFunction() override
		{
			m_f();
		}

	protected:
		F m_f;
	};

	template<class F>
	FunctionProviderPtr makeFunctionProvider( F _f )
	{
		FunctionProviderPtr provider = new FunctionProviderT<F>( _f );

		return provider;
	}
}