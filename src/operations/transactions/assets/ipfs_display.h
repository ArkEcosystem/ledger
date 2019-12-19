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

#ifndef ARK_OPERATIONS_TRANSACTION_ASSETS_IPFS_DISPLAY_H
#define ARK_OPERATIONS_TRANSACTION_ASSETS_IPFS_DISPLAY_H

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "ux/display_context.h"

#include "utils/base58.h"
#include "utils/print.h"
#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////

static const uint8_t STEPS_IPFS = 2U;

////////////////////////////////////////////////////////////////////////////////

void displayIpfs(const Transaction *transaction) {
    const char *const LABEL     = "IPFS";
    const size_t LABEL_SIZE     = 5;

    const char *const LABEL_DAG     = "DAG";
    const size_t LABEL_DAG_SIZE     = 4;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_DAG, LABEL_DAG_SIZE);
    bytecpy((char *)displayCtx.title[1], LABEL_FEE, LABEL_FEE_SIZE);

    // DAG
    encodeBase58((uint8_t *)transaction->asset.ipfs.dag,
                 transaction->asset.ipfs.length,
                 (uint8_t *)displayCtx.var[0], MIN(46, HASH_64_LEN));

    // Let's truncate the DAG if it's longer than 64 bytes.
    if (transaction->asset.ipfs.length > HASH_64_LEN) {
        bytecpy((void *)&displayCtx.var[0][HASH_64_LEN],
                LABEL_ELLIPSES, LABEL_ELLIPSES_SIZE);
    }

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[1], sizeof(displayCtx.var[1]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

#endif
