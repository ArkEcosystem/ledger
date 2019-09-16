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

#ifndef ARK_TRANSACTIONS_ASSETS_TYPES_H
#define ARK_TRANSACTIONS_ASSETS_TYPES_H

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

#define TRANSACTION_TYPE_TRANSFER               0x00
#define TRANSACTION_TYPE_SECOND_SIGNATURE       0x01
///////// Delegate Registration              // 0x02
#define TRANSACTION_TYPE_VOTE                   0x03
///////// MultiSignature                     // 0x04
#define TRANSACTION_TYPE_IPFS                   0x05
///////// MultiPayment                       // 0x06
///////// Delegate Resignation               // 0x07
#define TRANSACTION_TYPE_HTLC_LOCK              0x08
#define TRANSACTION_TYPE_HTLC_CLAIM             0x09
#define TRANSACTION_TYPE_HTLC_REFUND            0x0A

////////////////////////////////////////////////////////////////////////////////

enum TransactionTypes { TRANSFER = 0U,              // Type 0
                        SECOND_SIGNATURE,           // Type 1
                        ///////// Delegate Reg.     // Type 2
                        VOTE = 3U,                  // Type 3
                        ///////// MultiSignature    // Type 4
                        IPFS = 5U,                  // Type 5
                        ///////// MultiPayment      // Type 6
                        ///////// Delegate Res.     // Type 7
                        HTLC_LOCK = 9U,             // Type 8
                        HTLC_CLAIM,                 // Type 9
                        HTLC_REFUND                 // Type 10
};

////////////////////////////////////////////////////////////////////////////////

// static 
typedef union tx_asset_t {
    Transfer                    transfer;               // Type 0
    SecondSignatureRegistration secondSignature;        // Type 1
    ///////// Delegate Registration                     // Type 2
    Vote                        vote;                   // Type 3
    ///////// MultiSignature                            // Type 4
    Ipfs                        ipfs;                   // Type 5
    ///////// MultiPayment                              // Type 6
    ///////// Delegate Resignation                      // Type 7
    HtlcLock                    htlcLock;               // Type 8
    HtlcClaim                   htlcClaim;              // Type 9
    HtlcRefund                  htlcRefund;             // Type 10
} tx_asset_t;

////////////////////////////////////////////////////////////////////////////////

#endif
