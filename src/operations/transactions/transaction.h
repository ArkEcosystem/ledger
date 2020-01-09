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

#ifndef ARK_OPERATIONS_TRANSACTION_H
#define ARK_OPERATIONS_TRANSACTION_H

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "transactions/types/assets.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct transaction_t {
    uint8_t     header;
    uint8_t     version;
    uint8_t     network;
    uint16_t    type;
    uint8_t     senderPublicKey[PUBLICKEY_COMPRESSED_LEN];
    uint64_t    fee;
    size_t      vendorFieldLength;
    uint8_t     *vendorField;
    union {
        struct {  // v2
            tx_asset_t  asset;
        };
        struct {  // Legacy
            uint8_t     recipientId[ADDRESS_HASH_LEN];
            uint64_t    amount;
            size_t      assetOffset;
            size_t      assetSize;
            uint8_t     *assetPtr;
        };
    };
} Transaction;

#endif  // #define ARK_OPERATIONS_TRANSACTION_H
