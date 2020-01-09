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

#include "transactions/ux/ipfs_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/base58.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////

void displayIpfs(const Transaction *transaction) {
    const char *const LABEL     = "IPFS";
    const size_t LABEL_SIZE     = 5;

    const char *const LABEL_DAG     = "DAG";
    const size_t LABEL_DAG_SIZE     = 4;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_FEE, LABEL_FEE_SIZE);
    bytecpy((char *)displayCtx.title[DISPLAY_CTX_EXTENDED_TITLE_INDEX],
            LABEL_DAG,
            LABEL_DAG_SIZE);

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.text[0], sizeof(displayCtx.text[0]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // DAG
    encodeBase58((uint8_t *)transaction->asset.ipfs.dag,
                 transaction->asset.ipfs.length,
                 (uint8_t *)displayCtx.extended_text,
                 MAX_TEXT_LEN);
}

////////////////////////////////////////////////////////////////////////////////
