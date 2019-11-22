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

#include "transactions/assets/type_9.h"

#include <stdint.h>

#include <os.h>
#include <cx.h>

#include "constants.h"

#include "crypto/hashing.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

// Htlc Claim (Type 8) - 64 Bytes
//
// @param HtlcClaim *claim: The Htlc Claim (Type 8) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Lock Transaction Id - 32 Bytes:
// - os_memmove(claim->id, &buffer[0], 32U);
//
// Unlock Secret - 32 Bytes
// - os_memmove(claim->secret, &buffer[32], 64U - 32U);
//
// ---
StreamStatus deserializeHtlcClaim(HtlcClaim *claim,
                                  const uint8_t *buffer,
                                  const uint32_t length) {
    if (length <= HASH_32_LENGTH) {
        return USTREAM_FAULT;
    }

    os_memmove(claim->id, &buffer[0], HASH_32_LENGTH);
    os_memmove(claim->secret, &buffer[HASH_32_LENGTH], length - HASH_32_LENGTH);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
