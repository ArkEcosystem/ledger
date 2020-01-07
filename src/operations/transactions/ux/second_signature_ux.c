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

#include "transactions/ux/second_signature_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////

void displaySecondSignature(const Transaction *transaction) {
    const char *const LABEL     = "2nd Signature";
    const size_t LABEL_SIZE     = 14;

    const char *const LABEL_PUBLICKEY   = "PublicKey";
    const size_t LABEL_PUBLICKEY_SIZE   = 10;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_PUBLICKEY, LABEL_PUBLICKEY_SIZE);
    bytecpy((char *)displayCtx.title[1], LABEL_FEE, LABEL_FEE_SIZE);

    // PublicKey of Second Signature
    bytesToHex((char *)displayCtx.text[0],
                transaction->asset.secondSignature.publicKey,
                PUBLICKEY_COMPRESSED_LEN);

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.text[1], sizeof(displayCtx.text[1]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////
