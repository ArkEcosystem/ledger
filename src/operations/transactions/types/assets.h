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

#ifndef ARK_OPERATIONS_TRANSACTIONS_TYPES_ASSETS_H
#define ARK_OPERATIONS_TRANSACTIONS_TYPES_ASSETS_H

////////////////////////////////////////////////////////////////////////////////

#include "transactions/types/transfer.h"
#include "transactions/types/second_signature.h"
// #include "transactions/types/delegate_registration.h"
#include "transactions/types/vote.h"
// #include "transactions/types/multi_signature.h"
#include "transactions/types/ipfs.h"
// #include "transactions/types/multi_payment.h"
// #include "transactions/types/delegate_resignation.h"
#include "transactions/types/htlc_lock.h"
#include "transactions/types/htlc_claim.h"
#include "transactions/types/htlc_refund.h"

////////////////////////////////////////////////////////////////////////////////

typedef union tx_asset_t {
    Transfer                    transfer;               // Type 0
    SecondSignatureRegistration secondSignature;        // Type 1
    // Delegate Registration                            // Type 2
    Vote                        vote;                   // Type 3
    // MultiSignature                                   // Type 4
    Ipfs                        ipfs;                   // Type 5
    // MultiPayment                                     // Type 6
    // Delegate Resignation                             // Type 7
    HtlcLock                    htlcLock;               // Type 8
    HtlcClaim                   htlcClaim;              // Type 9
    HtlcRefund                  htlcRefund;             // Type 10
} tx_asset_t;

////////////////////////////////////////////////////////////////////////////////

#endif
