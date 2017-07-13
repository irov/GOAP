#	pragma once

#	include "IntrusivePtrBase.h"

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;

	class GuardProvider
		: public IntrusivePtrBase<GuardProvider>
	{
	public:
		virtual void onGuard() = 0;
	};

	typedef IntrusivePtr<GuardProvider> GuardProviderPtr;

	template<class F>
	class GuardProviderT
		: public GuardProvider
	{
	public:
		GuardProviderT(F _f)
			: m_f( _f )
		{
		}

	public:
		void onGuard() override
		{
			m_f();
		}

	protected:
		F m_f;
	};

	template<class F>
	GuardProviderPtr makeGuardProvider( F _f )
	{
		GuardProviderPtr provider = new GuardProviderT<F>(_f);

		return provider;
	}
}