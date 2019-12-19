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

#include "transactions/assets/transfer.h"

#include <stdbool.h>
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
// @return bool: true if deserialization was successful.
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
// - bytecpy(transfer->recipientId, &buffer[12], 21);
//
// ---
bool deserializeTransfer(Transfer *transfer,
                         const uint8_t *buffer,
                         size_t size) {
    if (transfer == NULL ||
        buffer == NULL ||
        size != TRANSACTION_TYPE_TRANSFER_SIZE) {
        return false;
    }

    transfer->amount        = U8LE(buffer, 0);                      // 8 Bytes
    transfer->expiration    = U4LE(buffer, sizeof(uint64_t));       // 4 Bytes
    bytecpy(transfer->recipientId,                                  // 21 Bytes
            &buffer[sizeof(uint64_t) + sizeof(uint32_t)],
            ADDRESS_HASH_LEN);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
