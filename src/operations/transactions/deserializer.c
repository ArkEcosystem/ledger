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

#include "transactions/deserializer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "constants.h"
#include "platform.h"

#include "transactions/transaction.h"

#include "transactions/offsets.h"
#include "transactions/types/types.h"

#include "transactions/legacy/deserialize_legacy.h"
#include "transactions/legacy/display_legacy.h"

#include "utils/unpack.h"
#include "utils/utils.h"

#include "transactions/ux/display_ux.h"

////////////////////////////////////////////////////////////////////////////////
Transaction transaction;

////////////////////////////////////////////////////////////////////////////////
// Deserialize Common
//
// @param Transaction *transaction: A Transaction object.
// @param uint8_t *buffer: The serialized transactions buffer.
//
// ---
// Internals:
//
// Header - 1 Byte:
// - 0xFF Marker: buffer[0]
//
// Transaction Version - 1 Byte:
// - transaction->version = buffer[1];
//
// Network Version - 1 Byte:
// - transaction->network = buffer[2];
//
// TypeGroup - 4 Bytes: Not Currently Needed
// - transaction->typeGroup = U4LE(&buffer[3]);
//
// Transaction Type - 2 Bytes:
// - transaction->type = U2LE(&buffer[7]);
//
// Nonce - 8 Bytes: Not Currently Needed
// - transaction->nonce = U8LE(&buffer[9]);
//
// SenderPublicKey - 33 Bytes:
// - MEMCOPY(transaction->senderPublicKey, &buffer[17], 33);
//
// Fee - 8 bytes
// - transaction->fee = U8LE(buffer, 50);
//
// VendorField Length - 1 Byte:
// transaction->vendorFieldLength = buffer[58];
//
// VendorField - 0 - 64 Bytes:
// - transaction->vendorField = (uint8_t *)&buffer[59];
//
// ---
static void deserializeCommon(Transaction *transaction, const uint8_t *buffer) {
    transaction->header             = buffer[HEADER_OFFSET];        // 1 Byte
    transaction->version            = buffer[VERSION_OFFSET];       // 1 Byte
    transaction->network            = buffer[NETWORK_OFFSET];       // 1 Byte
    transaction->type               = U2LE(buffer, TYPE_OFFSET);    // 2 Bytes

    MEMCOPY(transaction->senderPublicKey,                           // 33 Bytes
            &buffer[SENDER_PUBLICKEY_OFFSET],
            PUBLICKEY_COMPRESSED_LEN);

    transaction->fee                = U8LE(buffer, FEE_OFFSET);     // 8 Bytes

    transaction->vendorFieldLength  = buffer[VF_LEN_OFFSET];        // 1 Byte
    // 0 <=> 64 Bytes
    transaction->vendorField = (uint8_t *)&buffer[VF_OFFSET];
}

////////////////////////////////////////////////////////////////////////////////
// Deserialize Common v1
//
// @param TransactionData *data: The Transaction Data destination.
// @param const uint8_t *buffer
//
// ---
// Internals:
//
// Header - 1 Byte:
// - data->header = buffer.at(0);
//
// Transaction Version - 1 Byte:
// - data->version = buffer.at(1);
//
// Network Version - 1 Byte:
// - data->network = buffer.at(2);
//
// Transaction Type - 1 Byte:
// - data->type = buffer.at(3);
//
// Timestamp - 4 Bytes: Not Currently Needed
// - data->timestamp = unpack4LE(buffer, 4);
//
// SenderPublicKey - 33 Bytes:
// - MEMCOPY(&buffer.at(8), data->senderPublicKey, 33);
//
// Fee - 8 bytes
// - data->fee = unpack8LE(buffer, 41);
//
// VendorField Length - 1 Byte:
// - buffer.at(49)
//
// VendorField - 0 - 255 Bytes:
// - data->vendorField.insert(data->vendorField.begin(), &buffer.at(50), &buffer.at(50 + buffer.at(49)));
//
// ---
static void deserializeCommonV1(Transaction *transaction,
                                const uint8_t *buffer) {
    transaction->header             = buffer[HEADER_OFFSET];        // 1 Byte
    transaction->version            = buffer[VERSION_OFFSET];       // 1 Byte
    transaction->network            = buffer[NETWORK_OFFSET];       // 1 Byte
    transaction->type               = buffer[TYPE_OFFSET_V1];       // 1 Byte

    MEMCOPY(transaction->senderPublicKey,                           // 33 Bytes
            &buffer[SENDER_PUBLICKEY_OFFSET_V1],
            PUBLICKEY_COMPRESSED_LEN);

    transaction->fee                = U8LE(buffer, FEE_OFFSET_V1);  // 8 Bytes

    transaction->vendorFieldLength  = buffer[VF_LEN_OFFSET_V1];     // 1 Byte
    // vendorField: 0 <=> 64 Bytes
    transaction->vendorField = (uint8_t *)&buffer[VF_OFFSET_V1];
}

////////////////////////////////////////////////////////////////////////////////
// Deserialize Asset
//
// @param Transaction *transaction
// @param uint8_t *buffer: The serialized buffer at the asset offset.
// @param size_t size: The Asset Buffer Size.
//
// @return bool: true if successful
//
// ---
// Internals:
//
// - case TRANSFER
// - case VOTE
// - case IPFS
// - case HTLC_LOCK
// - case HTLC_CLAIM
// - case HTLC_REFUND
//
// ---
static bool deserializeAsset(Transaction *transaction,
                                     const uint8_t *buffer,
                                     size_t size) {
    switch (transaction->type) {
        // Transfer
        case TRANSFER_TYPE:
            return deserializeTransfer(
                    &transaction->asset.transfer, buffer, size);

        // Vote
        case VOTE_TYPE:
            return deserializeVote(
                    &transaction->asset.vote, buffer, size);

        // case MULTI_SIGNATURE_TYPE:

        // Ipfs
        case IPFS_TYPE:
            return deserializeIpfs(
                    &transaction->asset.ipfs, buffer, size);

        // Htlc Lock
        case HTLC_LOCK_TYPE:
            return deserializeHtlcLock(
                    &transaction->asset.htlcLock, buffer, size);

        // Htlc Claim
        case HTLC_CLAIM_TYPE:
            return deserializeHtlcClaim(
                    &transaction->asset.htlcClaim, buffer, size);

        // Htlc Refund
        case HTLC_REFUND_TYPE:
            return deserializeHtlcRefund(
                    &transaction->asset.htlcRefund, buffer, size);

        // Unknown Transaction Type
        default: return false;
    };
}

////////////////////////////////////////////////////////////////////////////////
// v2 and v1 Transactions
static bool internalDeserialize(Transaction *transaction,
                                        const uint8_t *buffer,
                                        size_t size) {
    size_t assetOffset = 0;
    switch (buffer[VERSION_OFFSET]) {
        // v2
        case TRANSACTION_VERSION_TYPE_2:
            deserializeCommon(transaction, buffer);
            assetOffset = VF_OFFSET + transaction->vendorFieldLength;
            break;

        // v1
        case TRANSACTION_VERSION_TYPE_1:
            deserializeCommonV1(transaction, buffer);
            assetOffset = VF_OFFSET_V1 + transaction->vendorFieldLength;
            break;

        default: return false;
    }

    if (deserializeAsset(transaction,
                         &buffer[assetOffset],
                         size - assetOffset)) {
        SetUx(transaction);
        return true;
    }

    // Unknown Transaction Version
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// Legacy Transactions
static bool internalDeserializeLegacy(Transaction *transaction,
                                              const uint8_t *buffer,
                                              size_t size) {
    if (buffer[HEADER_OFFSET] == TRANSFER_TYPE ||
        buffer[HEADER_OFFSET] == VOTE_TYPE) {
        deserializeCommonLegacy(transaction, buffer, size);

        transaction->assetOffset = ASSET_OFFSET_LEGACY;
        transaction->assetPtr = (uint8_t*)&buffer[transaction->assetOffset];

        SetUxLegacy(transaction);

        return true;
    }

    // Unknown Transaction Version and Type
    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool deserialize(const uint8_t *buffer, size_t size) {
    bool successful = buffer[HEADER_OFFSET] == TRANSACTION_HEADER
            ? internalDeserialize(&transaction, buffer, size)
            : internalDeserializeLegacy(&transaction, buffer, size);

    if (!successful) {
        MEMSET_TYPE_BZERO(&transaction, Transaction);
    }

    return successful;
}
