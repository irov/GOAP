/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Factorable.h"

namespace GOAP
{
	class CallbackObserver
	{
	public:
		virtual void onCallback( bool _skip ) = 0;
	};

	class CallbackProvider
		: public Factorable
	{
	public:
        virtual void onCallback( CallbackObserver * _callback, bool _skip ) = 0;
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
		void onCallback( CallbackObserver * _callback, bool _skip ) override
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