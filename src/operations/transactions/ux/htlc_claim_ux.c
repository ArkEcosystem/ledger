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

#include "transactions/ux/htlc_claim_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/utils.h"

#include "ux/display_context.h"

////////////////////////////////////////////////////////////////////////////////

void displayHtlcClaim(const Transaction *transaction) {
    const char *const LABEL     = "HTLC Claim";
    const size_t LABEL_SIZE     = 12;

    const char *const LABEL_LOCK_ID     = "Lock Id";
    const size_t LABEL_LOCK_ID_SIZE     = 8;

    const char *const LABEL_SECRET      = "Secret";
    const size_t LABEL_SECRET_SIZE      = 7;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_LOCK_ID, LABEL_LOCK_ID_SIZE);
    bytecpy((char *)displayCtx.title[1], LABEL_SECRET, LABEL_SECRET_SIZE);

    // Id
    bytesToHex((char *)displayCtx.var[0],
               transaction->asset.htlcClaim.id,
               HASH_32_LEN);

    // Secret
    bytecpy((char *)displayCtx.var[1],
            transaction->asset.htlcClaim.secret,
            HASH_32_LEN);
}

////////////////////////////////////////////////////////////////////////////////
