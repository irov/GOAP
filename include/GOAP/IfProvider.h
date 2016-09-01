#	pragma once

#	include "IntrusivePtrBase.h"

namespace GOAP
{
	class IfProvider
		: public IntrusivePtrBase<IfProvider>
	{
	public:
		virtual bool onIf() = 0;
	};

	typedef IntrusivePtr<IfProvider> IfProviderPtr;

	template<class F>
	class IfProviderT
		: public IfProvider
	{
	public:
		IfProviderT( F _f )
			: m_f( _f )
		{
		}

	public:
		bool onIf() override
		{
			bool result = m_f();

			return result;
		}

	protected:
		F m_f;
	};

	template<class F>
	IfProviderPtr makeIfProvider( F _f )
	{
		IfProviderPtr provider = new IfProviderT<F>( _f );

		return provider;
	}
}