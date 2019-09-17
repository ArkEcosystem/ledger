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

#include "transactions/assets/type_10.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "transactions/status.h"

////////////////////////////////////////////////////////////////////////////////

// Htlc Refund (Type 10) - 32 Bytes
//
// @param HtlcRefund *refund: The Htlc Refund (Type 10) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Lock Transaction Id - 32 Bytes:
// - os_memmove(refund->id, &buffer[0], 32U);
//
// ---
StreamStatus deserializeHtlcRefund(HtlcRefund *refund,
                                   const uint8_t *buffer,
                                   const uint32_t length) {
    if (length != HASH_32_LENGTH) {
        return USTREAM_FAULT;
    }

    os_memmove(refund->id, &buffer[0], HASH_32_LENGTH);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
