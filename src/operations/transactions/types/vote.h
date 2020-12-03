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

#ifndef ARK_OPERATIONS_TRANSACTIONS_TYPES_VOTE_H
#define ARK_OPERATIONS_TRANSACTIONS_TYPES_VOTE_H

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
static const size_t VOTE_LEN = sizeof(uint8_t) + PUBLICKEY_COMPRESSED_LEN;
static const size_t VOTE_MAX_COUNT = 2U;

////////////////////////////////////////////////////////////////////////////////
typedef struct vote_asset_t {
    uint8_t     count;
    uint8_t     data[VOTE_MAX_COUNT] [VOTE_LEN];  // ('0x00/0x01' + pubKey[33])
} Vote;

////////////////////////////////////////////////////////////////////////////////
size_t deserializeVote(Vote *vote, const uint8_t *buffer, size_t size);

#endif  // #define ARK_OPERATIONS_TRANSACTIONS_TYPES_VOTE_H
