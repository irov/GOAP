/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Array.h"
#include "GOAP/Config.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	typedef IntrusivePtr<class Source> SourcePtr;
	//////////////////////////////////////////////////////////////////////////
	template<size_t Count>
	using ArraySources = Array<SourcePtr, Count>;
	//////////////////////////////////////////////////////////////////////////
}
