
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

#include "transactions/assets/type_0.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "utils/unpack.h"

#include "transactions/status.h"

////////////////////////////////////////////////////////////////////////////////

// Deserialize Transfer (Type 0) - 33 bytes
//
// @param Transfer *transfer: The Transfer (Type 0) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Amount - 8 Bytes:
// - transfer->amount = U8LE(buffer);
//
// Expiration - 4 Bytes:
// - transfer->expiration = U4LE(&buffer[sizeof(uint64_t)]);
//
// Recipient - 21 Bytes:
// - os_memmove(transfer->recipient, &buffer[sizeof(uint64_t) + sizeof(uint32_t)], ADDRESS_HASH_LENGTH);
//
// ---
StreamStatus deserializeTransfer(Transfer *transfer,
                                 const uint8_t *buffer,
                                 const uint32_t length) {
    if (length != PUBLICKEY_COMPRESSED_LENGTH) {
        return USTREAM_FAULT;
    }

    transfer->amount = U8LE(buffer, 0U);
    os_memmove(transfer->recipient,
               &buffer[sizeof(uint64_t) + sizeof(uint32_t)],
               ADDRESS_HASH_LENGTH);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
