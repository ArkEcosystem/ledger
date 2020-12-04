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

#include "platform.h"

#if defined(SUPPORTS_MULTISIGNATURE)

#include "transactions/types/signatures.h"

#include <stddef.h>
#include <stdint.h>

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Deserialize Signatures - 65N bytes
//
// @param Signatures *signatures
// @param uint8_t *buffer: The serialized buffer beginning at Signatures.
// @param size_t size: The Asset Size.
//
// @return   0: error
// @return > 0: asset size
//
// ---
// Internals:
//
// Counts:
// - signatures->count = size / TRANSACTION_SIGNATURES_SIZE;
//
// Signatures - 64N Bytes:
// - MEMCOPY(transfer->signatures.data, &buffer[N], 64);
//
// ---
size_t deserializeSignatures(Signatures *signatures,
                             const uint8_t *buffer,
                             size_t size) {
    if (signatures == NULL ||
        buffer == NULL ||
        size % TRANSACTION_SIGNATURES_SIZE != 0) {
        return 0U;
    }

    signatures->count = size / TRANSACTION_SIGNATURES_SIZE;
    if (signatures->count > MULTI_SIG_MAX) {
        return 0U;
    }

    for (size_t i = 0; i < signatures->count; ++i) {
        MEMCOPY(signatures->data[i],
                &buffer[sizeof(uint8_t) + (i * TRANSACTION_SIGNATURES_SIZE)],
                SIG_SCHNORR_LEN);
    }

    return signatures->count * TRANSACTION_SIGNATURES_SIZE;
}

#endif  // SUPPORTS_MULTISIGNATURE
