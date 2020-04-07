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

#include "transactions/types/htlc_lock.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/unpack.h"
#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Htlc Lock (Type 8) - 66 Bytes
//
// @param HtlcLock *lock
// @param uint8_t *buffer: The serialized buffer at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// @return bool: true if deserialization was successful.
//
// ---
// Internals:
//
// Amount - 8 Bytes:
// - lock->amount = U4LE(buffer, 0);
//
// Secret Hash - 32 Bytes
// - MEMCOPY(lock->secretHash, &buffer[8], 32);
//
// Expiration Type- 1 Byte
// - lock->expirationType = buffer[40];
//
// Expiration Value - 4 Bytes
// - lock->expirationValue = U4LE(buffer, 41);
//
// RecipientId - 21 Bytes
// - MEMCOPY(lock->recipientId, &buffer[45], 21);
//
// ---
bool deserializeHtlcLock(HtlcLock *lock, const uint8_t *buffer, size_t size) {
    if (size != TRANSACTION_TYPE_HTLC_LOCK_SIZE) {
        return false;
    }

    size_t offset = 0;

    lock->amount = U8LE(buffer, offset);                            // 8 Bytes
    offset += sizeof(uint64_t);

    MEMCOPY(lock->secretHash, &buffer[offset], HASH_32_LEN);        // 32 Bytes
    offset += HASH_32_LEN;

    lock->expirationType = buffer[offset];                          // 1 Byte
    offset += sizeof(uint8_t);

    lock->expiration = U4LE(buffer, offset);                        // 4 Bytes
    offset += sizeof(uint32_t);

    MEMCOPY(lock->recipientId, &buffer[offset], ADDRESS_HASH_LEN);  // 21 Bytes

    return true;
}

