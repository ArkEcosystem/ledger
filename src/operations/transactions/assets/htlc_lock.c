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

#include "transactions/assets/htlc_lock.h"

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
// - bytecpy(lock->secretHash, &buffer[8], 32);
//
// Expiration Type- 1 Byte
// - lock->expirationType = buffer[40];
//
// Expiration Value - 4 Bytes
// - lock->expirationValue = U4LE(buffer, 41);
//
// RecipientId - 21 Bytes
// - bytecpy(lock->recipientId, &buffer[45], 21);
//
// ---
bool deserializeHtlcLock(HtlcLock *lock, const uint8_t *buffer, size_t size) {
    if (size != TRANSACTION_TYPE_HTLC_LOCK_SIZE) {
        return false;
    }

    size_t offset = 0;

    lock->amount            = U8LE(buffer, offset);             // 8 Bytes

    offset += sizeof(uint64_t);     // += sizeof(amount)

    bytecpy(lock->secretHash,                                   // 32 Bytes
            &buffer[offset],
            HASH_32_LEN);

    offset += HASH_32_LEN;          // += sizeof(secretHash)

    lock->expirationType    = buffer[offset];                   // 1 Byte

    offset += sizeof(uint8_t);      // += sizeof(expirationType)

    lock->expiration        = U4LE(buffer, offset);             // 4 Bytes

    offset += sizeof(uint32_t);      // += sizeof(expiration)

    bytecpy(lock->recipientId,                                  // 21 Bytes
            &buffer[offset],
            ADDRESS_HASH_LEN);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
