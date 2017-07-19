#	pragma once

#	include "GOAP/Config.h"

namespace GOAP
{
	class ChainProvider
		: public IntrusiveBase<ChainProvider>
	{
	public:
		virtual void onChain( bool _skip ) = 0;
	};

	typedef IntrusivePtr<ChainProvider> ChainProviderPtr;

	template<class F>
	class ChainProviderT
		: public ChainProvider
	{
	public:
		ChainProviderT( F _f )
			: m_f( _f )
		{
		}

	public:
		void onChain( bool _skip ) override
		{
			m_f( _skip );
		}

	protected:
		F m_f;
	};

    namespace Helper
    {
        template<class F>
        ChainProviderPtr makeChainProvider( F _f )
        {
            ChainProviderPtr provider = new ChainProviderT<F>( _f );

            return provider;
        }
    }
}