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

#include "transactions/ux/transfer_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/base58.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////

static void setVendorField(const Transaction *transaction) {
    const char *const LABEL_VENDORFIELD = "VendorField";
    const size_t LABEL_VENDORFIELD_SIZE = 12;

    bytecpy((char *)displayCtx.title[DISPLAY_CTX_EXTENDED_TITLE_INDEX],
            LABEL_VENDORFIELD,
            LABEL_VENDORFIELD_SIZE);

    bytecpy((char *)displayCtx.extended_text,
            (uint8_t *)transaction->vendorField,
            transaction->vendorFieldLength);
    displayCtx.extended_text[transaction->vendorFieldLength] = '\0';
}

////////////////////////////////////////////////////////////////////////////////

void displayTransfer(const Transaction *transaction) {
    const char *const LABEL     = "Transfer";
    const size_t LABEL_SIZE     = 9;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_TO, LABEL_TO_SIZE);

    bytecpy((char *)displayCtx.title[1], LABEL_EXPIRATION, LABEL_EXPIRATION_SIZE);
    bytecpy((char *)displayCtx.title[2], LABEL_AMOUNT, LABEL_AMOUNT_SIZE);
    bytecpy((char *)displayCtx.title[3], LABEL_FEE, LABEL_FEE_SIZE);

    // RecipientId
    encodeBase58PublicKey((uint8_t *)transaction->asset.transfer.recipientId,
                          ADDRESS_HASH_LEN,
                          (uint8_t *)displayCtx.text[0],
                          sizeof(displayCtx.text[0]),
                          transaction->asset.transfer.recipientId[0],
                          1U);

    // Expiration
    printAmount(transaction->asset.transfer.expiration,
                displayCtx.text[1],
                sizeof(displayCtx.text[1]),
                NULL, 0U, 0U);

    // Amount
    printAmount(transaction->asset.transfer.amount,
                displayCtx.text[2],
                sizeof(displayCtx.text[2]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.text[3],
                sizeof(displayCtx.text[3]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // VendorField
    if (transaction->vendorFieldLength > 0U) {
        setVendorField(transaction);
    }
}

////////////////////////////////////////////////////////////////////////////////
