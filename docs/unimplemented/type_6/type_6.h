
////////////////////////////////////////////////////////////////////////////////

// The Following are only examples and places where this code could be implemented.
//  It is not final or guaranteed working.
//  This should only serve as a reference for implementing.

////////////////////////////////////////////////////////////////////////////////


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

#ifndef ARK_TRANSACTION_TYPE_6_H
#define ARK_TRANSACTION_TYPE_6_H

#include <stdint.h>

#include "constants.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct multipayment_asset_t {
    uint16_t    n_payments;         // 2,259 network max
    uint8_t    amounts[2259];       // payment(uint64_t) * n_payments
    uint8_t    addresses[47439];    // address[21] * n_payments
} MultiPayment;

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserializeMultiPayment(MultiPayment *payments,
                                     const uint8_t *buffer,
                                     const uint32_t length);

////////////////////////////////////////////////////////////////////////////////

#endif
