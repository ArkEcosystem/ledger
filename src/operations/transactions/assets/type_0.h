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

#ifndef ARK_OPERATIONS_TRANSACTION_TYPE_0_H
#define ARK_OPERATIONS_TRANSACTION_TYPE_0_H

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct transfer_asset_t {
    uint64_t    amount;
    uint32_t    expiration;
    uint8_t     recipient[ADDRESS_HASH_LENGTH];
} Transfer;

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserializeTransfer(Transfer *transfer,
                                 const uint8_t *buffer,
                                 size_t size);

////////////////////////////////////////////////////////////////////////////////

#endif
