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

#include "transactions/types/htlc_refund.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////

// Htlc Refund (Type 10) - 32 Bytes
//
// @param HtlcRefund *refund
// @param uint8_t *buffer: The serialized buffer at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// @return bool: true if deserialization was successful.
//
// ---
// Internals:
//
// Lock Transaction Id - 32 Bytes:
// - bytecpy(refund->id, &buffer[0], 32);
//
// ---
bool deserializeHtlcRefund(HtlcRefund *refund,
                           const uint8_t *buffer,
                           size_t size) {
    if (size != HASH_32_LEN) {
        return false;
    }

    bytecpy(refund->id, &buffer[0], HASH_32_LEN);               // 32 Bytes

    return true;
}

////////////////////////////////////////////////////////////////////////////////