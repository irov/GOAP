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
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Chain> ChainPtr;
	
	class Transcriptor
		: public Factorable
	{
	public:
		virtual ~Transcriptor() {};

	public:
		virtual TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) = 0;
	};

	typedef IntrusivePtr<Transcriptor> TranscriptorPtr;
}