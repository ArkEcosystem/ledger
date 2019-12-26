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

// ARK Transaction Types
enum TransactionTypes {
    TRANSFER_TYPE                   = 0,
    SECOND_SIGNATURE_TYPE           = 1,
    // DELEGATE_REGISTRATION_TYPE   = 2,
    VOTE_TYPE                       = 3,
    // MULTI_SIGNATURE_TYPE         = 4,
    IPFS_TYPE                       = 5,
    // MULTI_PAYMENT_TYPE           = 6,
    // DELEGATE_RESIGNATION_TYPE    = 7,
    HTLC_LOCK_TYPE                  = 8,
    HTLC_CLAIM_TYPE                 = 9,
    HTLC_REFUND_TYPE                = 10
};

////////////////////////////////////////////////////////////////////////////////

#endif
