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

#include "transactions/deserializer_v1.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "constants.h"

#include "transactions/transaction.h"

#include "transactions/offsets.h"

#include "utils/str.h"
#include "utils/unpack.h"
#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// !DEPRECATED
//
// Deserialize v1 Transaction common header values.
//
// @param Transaction *transaction: transaction object ptr.
// @param const uint8_t *buffer:    of the serialized transaction.
// @param size_t *size:             of the buffer.
//
// @return bool: true if deserialization was successful.
//
// ---
static bool deserializeCommonV1(Transaction *transaction,
                                const uint8_t *buffer,
                                size_t size) {
    if (size < FEE_OFFSET_V1 + sizeof(uint64_t)) { return false; }

    transaction->header     = buffer[HEADER_OFFSET];        // 1 Byte
    transaction->version    = buffer[VERSION_OFFSET];       // 1 Byte
    transaction->network    = buffer[NETWORK_OFFSET];       // 1 Byte
    transaction->type       = buffer[TYPE_OFFSET_V1];       // 1 Byte

    MEMCOPY(transaction->senderPublicKey,                   // 33 Bytes
            &buffer[SENDER_PUBLICKEY_OFFSET_V1],
            PUBLICKEY_COMPRESSED_LEN);

    transaction->fee        = U8LE(buffer, FEE_OFFSET_V1);  // 8 Bytes

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// !DEPRECATED
//
// Deserialize a v1 VendorField.
//
// @param Transaction *transaction: transaction object ptr.
// @param const uint8_t *buffer:    of the serialized transaction.
// @param size_t *size:             of the buffer.
//
// @return bool: true if deserialization was successful.
//
// ---
static bool deserializeVendorFieldV1(Transaction *transaction,
                                     const uint8_t *buffer,
                                     size_t size) {
    if (size < buffer[0] || buffer[0] > VENDORFIELD_V1_MAX_LEN) {
        return false;
    }

    transaction->vendorFieldLength = buffer[0];     // 1 Byte

    if (transaction->vendorFieldLength > 0U &&
        IsPrintableAscii((const char*)&buffer[1],
                         transaction->vendorFieldLength,
                         false) == false) {
        return false;
    }

    // 0 <=> 64 Bytes
    transaction->vendorField = (uint8_t*)&buffer[1];

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// !DEPRECATED
//
// Deserialize v1 Transaction Headers.
//
// @param Transaction *transaction: transaction object ptr.
// @param const uint8_t *buffer:    of the serialized transaction.
// @param size_t size:              of the buffer.
//
// @return bool: true if deserialization was successful.
//
// ---
size_t deserializeHeaderV1(Transaction *transaction,
                           const uint8_t *buffer,
                           size_t size) {
    if (deserializeCommonV1(transaction, buffer, size) == false ||
        deserializeVendorFieldV1(transaction,
                                    &buffer[VF_LEN_OFFSET_V1],
                                    size - VF_LEN_OFFSET_V1) == false) {
        return 0U;
    }

    return VF_OFFSET_V1 + transaction->vendorFieldLength;
}