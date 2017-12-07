/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

namespace GOAP
{
    static const uint32_t FLAG_SEMAPHORE_TEST_EQUAL = 0x00000001;
    static const uint32_t FLAG_SEMAPHORE_TEST_GREATER = 0x00000002;
    static const uint32_t FLAG_SEMAPHORE_TEST_LESS = 0x00000004;
    static const uint32_t FLAG_SEMAPHORE_APPLY_ASSIGN = 0x00000010;
    static const uint32_t FLAG_SEMAPHORE_APPLY_ADD = 0x00000020;
    static const uint32_t FLAG_SEMAPHORE_APPLY_SUBTRACT = 0x00000040;
}