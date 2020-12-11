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

#include "transactions/types/vote.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Vote (Type 3) - 35 <=> 69 Bytes
//
// @param Vote *vote
// @param const uint8_t *buffer: The serialized buffer at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// @return bool: true if deserialization was successful.
//
// ---
// Internals:
//
// Number of Votes - 1 Byte
// - vote->count = buffer[0];
//
// Vote - 1 + 33(Compressed PublicKey) Bytes:
// - MEMCOPY(vote->data, &buffer[1 + 34N], 34);
//
// ---
bool deserializeVote(Vote *vote, const uint8_t *buffer, size_t size) {
    vote->count = buffer[0];
    const size_t expectedLen = sizeof(uint8_t) + (vote->count * VOTE_LEN);

    if (vote->count == 0U || vote->count > VOTE_MAX_COUNT ||
        size < expectedLen) {
        return false;
    }

    for (uint8_t i = 0U; i < vote->count; ++i) {
        MEMCOPY(vote->data[i],                                  // 34 Bytes
                &buffer[sizeof(uint8_t) + (i * VOTE_LEN)],
                VOTE_LEN);        
    }

    return true;
}
