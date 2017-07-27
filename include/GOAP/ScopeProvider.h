/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Config.h"

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;

	class ScopeProvider
		: public Factorable
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