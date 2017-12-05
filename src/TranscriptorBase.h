/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "Transcriptor.h"

namespace GOAP
{
	class TranscriptorBase
		: public Transcriptor
	{
	public:
		explicit TranscriptorBase( const TaskPtr & _task );
        ~TranscriptorBase() override;

	public:
		TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override;

	protected:
		TaskPtr m_task;
	};
}