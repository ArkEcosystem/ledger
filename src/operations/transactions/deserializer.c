/*******************************************************************************
*   Ark Wallet
*   (c) 2017 Ledger
*   (c) ARK Ecosystem
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include "transactions/deserializer.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"
#include "utils/unpack.h"

#include "transactions/status.h"

#include "transactions/transaction.h"

#include "transactions/assets/types.h"
#include "transactions/display.h"

#include "transactions/legacy/deserialize_legacy.h"
#include "transactions/legacy/display_legacy.h"

////////////////////////////////////////////////////////////////////////////////

Transaction transaction;

////////////////////////////////////////////////////////////////////////////////

// Deserialize Common
//
// @param uint8_t *buffer: The serialized transactions buffer.
// @param Transaction *transaction: A Transaction object.
//
// ---
// Internals:
//
// Header - 1 Byte:
// - Skip 0xFF Marker: buffer[0]
//
// Transaction Version - 1 Byte:
// - transaction->version = buffer[1];
//
// Network Version - 1 Byte:
// - transaction->network = buffer[2];
//
// TypeGroup - 4 Bytes:
// - transaction->typeGroup = U4LE(&buffer[3]);
//
// Transaction Type - 2 Bytes:
// - transaction->type = U2LE(&buffer[7]);
//
// Nonce - 8 Bytes:
// - transaction->nonce = U8LE(&buffer[9]);
//
// SenderPublicKey - 33 Bytes:
// - os_memmove(transaction->senderPublicKey, &buffer[17], 33);
//
// Fee - 8 bytes
// - transaction->fee = U8LE(buffer, 50);
//
// VendorField Length - 1 Byte:
// transaction->vendorFieldLength = buffer[58];
//
// VendorField - 0 - 255 Bytes:
// - os_memmove(transaction->vendorField, buffer + 58 + 1, MIN(VendorFieldLen, 64));
//
// ---
static void internalDeserializeCommon(Transaction *transaction,
                                      const uint8_t *buffer) {
    transaction->header             = buffer[0];
    transaction->version            = buffer[1];
    transaction->type               = U2LE(buffer, 7U);
    os_memmove(transaction->senderPublicKey, &buffer[17], 33U);
    transaction->fee                = U8LE(buffer, 50U);
    transaction->vendorFieldLength  = buffer[58];
}

////////////////////////////////////////////////////////////////////////////////

// Deserialize Type
//
// @param uint8_t *buffer: The serialized transactions buffer.
// @param Transaction *transaction: A Transaction object.
//
// ---
// Internals:
//
// - case TRANSFER
// - case SECOND_SIGNATURE
// - case DELEGATE_REGISTRATION
// - case VOTE
// - case MULTI_SIGNATURE
// - case IPFS
// - case MULTIPAYMENT
// - case DELEGATE_RESIGNATION
// - case HTLC_LOCK
// - case HTLC_CLAIM
// - case HTLC_REFUND
//
// ---
static StreamStatus internalDeserializeAsset(Transaction *transaction,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
    StreamStatus status = USTREAM_FAULT;

    uint32_t assetOffset = 58U + transaction->vendorFieldLength + 1U;
    uint32_t assetLength = length - assetOffset;

    switch (transaction->type) {
        case TRANSACTION_TYPE_TRANSFER:
            status = deserializeTransfer(&transaction->asset.transfer,
                                         &buffer[assetOffset],
                                         assetLength);
            break;

        case TRANSACTION_TYPE_SECOND_SIGNATURE:
            status = deserializeSecondSignature(
                    &transaction->asset.secondSignature,
                    &buffer[assetOffset],
                    assetLength);
            break;

        case TRANSACTION_TYPE_VOTE:
            status = deserializeVote(&transaction->asset.vote,
                                     &buffer[assetOffset],
                                     assetLength);
            break;

        case TRANSACTION_TYPE_IPFS:
            status = deserializeIpfs(&transaction->asset.ipfs,
                                     &buffer[assetOffset],
                                     assetLength);
            break;

        case TRANSACTION_TYPE_HTLC_LOCK:
            status = deserializeHtlcLock(&transaction->asset.htlcLock,
                                         &buffer[assetOffset],
                                         assetLength);
            break;

        case TRANSACTION_TYPE_HTLC_CLAIM:
            status = deserializeHtlcClaim(&transaction->asset.htlcClaim,
                                          &buffer[assetOffset],
                                          assetLength);
            break;

        case TRANSACTION_TYPE_HTLC_REFUND:
            status = deserializeHtlcRefund(&transaction->asset.htlcRefund,
                                           &buffer[assetOffset],
                                           assetLength);
            break;

        default: break;
    };

    return status;
}

////////////////////////////////////////////////////////////////////////////////

static StreamStatus internalDeserialize(Transaction *transaction,
                                        const uint8_t *buffer,
                                        const uint32_t length) {
    StreamStatus status = USTREAM_FAULT;

    // V2 Transactions
    if (buffer[0] == 0xFF && buffer[1] == TRANSACTION_VERSION_TYPE_2) {
        internalDeserializeCommon(transaction, buffer);
        status = internalDeserializeAsset(transaction, buffer, length);
        if (status == USTREAM_FINISHED) {
            setDisplay(transaction);
        }
    }

    // V1 or LegacyTransactions
    else if (buffer[0] != 0xFF ||
            (buffer[0] == 0xFF && buffer[1] == TRANSACTION_VERSION_TYPE_1)) {

        status = deserializeLegacy(transaction, buffer, length);

        // Transfer
        if (transaction->type == TRANSACTION_TYPE_TRANSFER) {
            setTransferLegacy(transaction);
        }
        // Vote
        else if (transaction->type == TRANSACTION_TYPE_VOTE) {
            setVoteLegacy(transaction);
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserialize(const uint8_t *buffer, const uint32_t length) {
    StreamStatus result;
    BEGIN_TRY {
        TRY {
            os_memset(&transaction, 0U, sizeof(Transaction));
            result = internalDeserialize(&transaction, buffer, length);
        }

        CATCH_OTHER(e) {
            os_memset(&transaction, 0U, sizeof(Transaction));
            result = USTREAM_FAULT;
        }

        FINALLY {}
    }
    END_TRY;

    return result;
}

////////////////////////////////////////////////////////////////////////////////
