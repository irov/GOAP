#	include "GOAP/Alias.h"

#   include "GOAP/Source.h"
#   include "GOAP/Chain.h"

#   include "GOAP/Exception.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
    bool Alias::_onRun()
    {
        ChainPtr chain = m_chain;

        GOAP::SourcePtr source = new GOAP::Source();

        bool skiped = this->isSkip();
        source->setSkip( skiped );

        this->_onGenerate( source );

        if( this->injectSource( source ) == false )
        {
            Helper::throw_exception( "Alias invalid inject source" );
        }

        return true;
    }
}