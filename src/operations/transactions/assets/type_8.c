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

#include "transactions/assets/type_8.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "utils/unpack.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

// Htlc Lock (Type 8) - 66 Bytes
//
// @param HtlcLock *lock: The Htlc Lock (Type 8) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Amount - 8 Bytes:
// - lock->amount = U4LE(buffer, 0U);
//
// Secret Hash - 32 Bytes
// - os_memmove(lock->secretHash, &buffer[8], 32U);
//
// Expiration Type- 1 Byte
// - lock->expirationType = buffer[40];
//
// Expiration Value - 4 Bytes
// - lock->expirationValue = U4LE(buffer, 41U);
//
// Recipient - 21 Bytes
// - os_memmove(lock->recipient, &buffer[45], 21U);
//
// ---
StreamStatus deserializeHtlcLock(HtlcLock *lock,
                                 const uint8_t *buffer,
                                 uint32_t length) {
    if (length != 66U) {
        return USTREAM_FAULT;
    }

    lock->amount            = U8LE(buffer, 0U);
    os_memmove(lock->secretHash, &buffer[8], HASH_32_LENGTH);
    lock->expirationType    = buffer[40];
    lock->expiration        = U4LE(buffer, 41U);
    os_memmove(lock->recipient, &buffer[45], ADDRESS_HASH_LENGTH);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
