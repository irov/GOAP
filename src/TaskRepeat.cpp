#	include "GOAP/TaskRepeat.h"
#	include "GOAP/Source.h"
#	include "GOAP/Chain.h"
#	include "GOAP/ChainProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskRepeat::TaskRepeat( const SourcePtr & _repeat, const SourcePtr & _until )
		: m_sourceRepeat( _repeat )
		, m_sourceUntil(_until)
		, m_repeat(true)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskRepeat::~TaskRepeat()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	template<class F>
	static ChainPtr __makeChain( const SourcePtr & _source, F f )
	{
		ChainProviderPtr untilChainProvider = makeChainProvider<F>( f );
		ChainPtr chain = new Chain( _source, untilChainProvider );

		return chain;
	}
	//////////////////////////////////////////////////////////////////////////
	void TaskRepeat::onFinalize()
	{
		m_repeat = false;

		m_sourceRepeat = nullptr;
		m_sourceUntil = nullptr;

		if( m_chainRepeat != nullptr )
		{
			ChainPtr chain = m_chainRepeat;
			m_chainRepeat = nullptr;
			chain->cancel();
		}

		if( m_chainUntil != nullptr )
		{
			ChainPtr chain = m_chainUntil;
			m_chainUntil = nullptr;
			chain->cancel();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskRepeat::onRun()
	{
		ChainPtr chainUntil = __makeChain( m_sourceUntil, [this] ( bool _skip ){this->untilComplete_( _skip ); } );

		m_chainUntil = chainUntil;

		if( m_chainUntil->run() == false )
		{
			return true;
		}

		if( m_repeat == false )
		{
			return true;
		}

		ChainPtr chainRepeat = __makeChain( m_sourceRepeat, [this] ( bool _skip ){this->repeatComplete_( _skip ); } );

		m_chainRepeat = chainRepeat;

		if( m_chainRepeat->run() == false )
		{
			return true;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void TaskRepeat::repeatComplete_( bool _skip )
	{
		if( m_repeat == false )
		{
			return;
		}

		ChainPtr chainRepeat = __makeChain( m_sourceRepeat, [this] ( bool _skip ){this->repeatComplete_( _skip ); } );

		m_chainRepeat = chainRepeat;

		if( m_chainRepeat->run() == false )
		{
			this->complete( true, _skip );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void TaskRepeat::untilComplete_( bool _skip )
	{
		m_repeat = false;

		if( m_chainRepeat != nullptr )
		{
			ChainPtr chain = m_chainRepeat;
			m_chainRepeat = nullptr;
			chain->cancel();
		}

		this->complete( true, _skip );
	}
}
