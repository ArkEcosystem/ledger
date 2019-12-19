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

#ifndef ARK_OPERATIONS_TRANSACTIONS_ASSETS_TYPES_H
#define ARK_OPERATIONS_TRANSACTIONS_ASSETS_TYPES_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////

#include "transactions/assets/transfer.h"
#include "transactions/assets/second_signature.h"
// #include "transactions/assets/delegate_registration.h"
#include "transactions/assets/vote.h"
// #include "transactions/assets/multi_signature.h"
#include "transactions/assets/ipfs.h"
// #include "transactions/assets/multi_payment.h"
// #include "transactions/assets/delegate_resignation.h"
#include "transactions/assets/htlc_lock.h"
#include "transactions/assets/htlc_claim.h"
#include "transactions/assets/htlc_refund.h"

////////////////////////////////////////////////////////////////////////////////

// ARK Transaction Types
enum TransactionTypes : uint8_t {
    TRANSFER_TYPE                   = 0U,
    SECOND_SIGNATURE_TYPE           = 1U,
    // DELEGATE_REGISTRATION_TYPE   = 2U,
    VOTE_TYPE                       = 3U,
    // MULTI_SIGNATURE_TYPE         = 4U,
    IPFS_TYPE                       = 5U,
    // MULTI_PAYMENT_TYPE           = 6U,
    // DELEGATE_RESIGNATION_TYPE    = 7U,
    HTLC_LOCK_TYPE                  = 8U,
    HTLC_CLAIM_TYPE                 = 9U,
    HTLC_REFUND_TYPE                = 10U
};

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
