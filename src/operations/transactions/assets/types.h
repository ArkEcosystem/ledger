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

////////////////////////////////////////////////////////////////////////////////

#include "transactions/assets/type_0.h"
#include "transactions/assets/type_1.h"
// #include "transactions/assets/type_2.h"
#include "transactions/assets/type_3.h"
// #include "transactions/assets/type_4.h"
#include "transactions/assets/type_5.h"
// #include "transactions/assets/type_6.h"
// #include "transactions/assets/type_7.h"
#include "transactions/assets/type_8.h"
#include "transactions/assets/type_9.h"
#include "transactions/assets/type_10.h"

////////////////////////////////////////////////////////////////////////////////

#define TRANSACTION_TYPE_TRANSFER               0U
#define TRANSACTION_TYPE_SECOND_SIGNATURE       1U
// Delegate Registration                        2U
#define TRANSACTION_TYPE_VOTE                   3U
// MultiSignature                               4U
#define TRANSACTION_TYPE_IPFS                   5U
// MultiPayment                                 6U
// Delegate Resignation                         7U
#define TRANSACTION_TYPE_HTLC_LOCK              8U
#define TRANSACTION_TYPE_HTLC_CLAIM             9U
#define TRANSACTION_TYPE_HTLC_REFUND            10U

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
