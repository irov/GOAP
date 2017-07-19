#	include "GOAP/Alias.h"

#   include "GOAP/Source.h"
#   include "GOAP/Chain.h"

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

        }

        return true;
    }
}