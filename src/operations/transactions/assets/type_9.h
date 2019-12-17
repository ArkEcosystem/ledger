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

#ifndef ARK_OPERATIONS_TRANSACTION_TYPE_9_H
#define ARK_OPERATIONS_TRANSACTION_TYPE_9_H

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct htlc_claim_asset_t {
    uint8_t     id[HASH_32_LENGTH];
    uint8_t     secret[HASH_32_LENGTH];
} HtlcClaim;

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserializeHtlcClaim(HtlcClaim *claim,
                                  const uint8_t *buffer,
                                  size_t size);

////////////////////////////////////////////////////////////////////////////////

#endif
