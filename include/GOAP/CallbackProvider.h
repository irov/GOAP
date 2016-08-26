#	pragma once

#	include "IntrusivePtrBase.h"

namespace GOAP
{
	class CallbackObserver
	{
	public:
		virtual void onCallback( bool _skip ) = 0;
	};

	class CallbackProvider
		: public IntrusivePtrBase<CallbackProvider>
	{
	public:
		virtual void call( CallbackObserver * _callback, bool _skip ) = 0;
	};

	typedef IntrusivePtr<CallbackProvider> CallbackProviderPtr;

	template<class F>
	class CallbackProviderT
		: public CallbackProvider
	{
	public:
		CallbackProviderT( F _f )
			: m_f( _f )
		{
		}

	public:
		void call( CallbackObserver * _callback, bool _skip ) override
		{
			m_f( _callback, _skip );
		}

	protected:
		F m_f;
	};

	template<class F>
	CallbackProviderPtr makeCallbackProvider( F _f )
	{
		CallbackProviderPtr provider = new CallbackProviderT<F>( _f );

		return provider;
	}
}