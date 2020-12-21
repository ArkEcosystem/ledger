/*******************************************************************************
 * This file is part of the ARK Ledger App.
 *
 * Copyright (c) ARK Ecosystem <info@ark.io>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#ifndef ARK_TRANSACTIONS_DEFAULTS_OFFSETS_HPP
#define ARK_TRANSACTIONS_DEFAULTS_OFFSETS_HPP

#include <stddef.h>

////////////////////////////////////////////////////////////////////////////////
// v2 Transaction Offsets
static const size_t HEADER_OFFSET               = 0;
static const size_t VERSION_OFFSET              = 1;
static const size_t NETWORK_OFFSET              = 2;
static const size_t TYPEGROUP_OFFSET            = 3;
static const size_t TYPE_OFFSET                 = 7;
static const size_t SENDER_PUBLICKEY_OFFSET     = 17;
static const size_t FEE_OFFSET                  = 50;
static const size_t VF_LEN_OFFSET               = 58;
static const size_t VF_OFFSET                   = 59;

////////////////////////////////////////////////////////////////////////////////
// !DEPRECATED
// v1 Transaction Offsets
static const size_t TYPE_OFFSET_V1                  = 3;
static const size_t SENDER_PUBLICKEY_OFFSET_V1      = 8;
static const size_t FEE_OFFSET_V1                   = 41;
static const size_t VF_LEN_OFFSET_V1                = 49;
static const size_t VF_OFFSET_V1                    = 50;

#endif
