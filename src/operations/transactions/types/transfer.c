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

#include "transactions/types/transfer.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/unpack.h"
#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Deserialize Transfer (Type 0) - 33 bytes
//
// @param Transfer *transfer
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param size_t size: The Asset Size.
//
// @return   0: error
// @return > 0: asset size
//
// ---
// Internals:
//
// Amount - 8 Bytes:
// - transfer->amount = U8LE(buffer, 0U);
//
// Expiration - 4 Bytes:
// - transfer->expiration = U4LE(buffer, 8);
//
// RecipientId - 21 Bytes:
// - MEMCOPY(transfer->recipientId, &buffer[12], 21);
//
// ---
size_t deserializeTransfer(Transfer *transfer,
                           const uint8_t *buffer,
                           size_t size) {
    if (transfer == NULL ||
        buffer == NULL ||
        size < TRANSACTION_TYPE_TRANSFER_SIZE) {
        return 0U;
    }

    transfer->amount        = U8LE(buffer, 0);                      // 8 Bytes
    transfer->expiration    = U4LE(buffer, sizeof(uint64_t));       // 4 Bytes
    MEMCOPY(transfer->recipientId,                                  // 21 Bytes
            &buffer[sizeof(uint64_t) + sizeof(uint32_t)],
            ADDRESS_HASH_LEN);

    return TRANSACTION_TYPE_TRANSFER_SIZE;
}
