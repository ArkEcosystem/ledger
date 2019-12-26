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

#include "ux/display_context.h"

////////////////////////////////////////////////////////////////////////////////

static void setVendorField(const Transaction *transaction) {
    const char *const LABEL_VENDORFIELD = "VendorField";
    const size_t LABEL_VENDORFIELD_SIZE = 12;

    bytecpy((char *)displayCtx.title[1], LABEL_VENDORFIELD,
                                         LABEL_VENDORFIELD_SIZE);

    bytecpy((char *)displayCtx.var[1],
            (uint8_t *)transaction->vendorField,
            MIN(transaction->vendorFieldLength, HASH_64_LEN));

    if (transaction->vendorFieldLength > HASH_64_LEN) {
        bytecpy((char *)&displayCtx.var[1][HASH_64_LEN],
                LABEL_ELLIPSES, LABEL_ELLIPSES_SIZE);
    }
}

////////////////////////////////////////////////////////////////////////////////

void displayTransfer(const Transaction *transaction) {
    const char *const LABEL     = "Transfer";
    const size_t LABEL_SIZE     = 9;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_TO, LABEL_TO_SIZE);

    // Let's offset the screen variables if there's a VendorField.
    // offset == 1 if VendorField; otherwise, 0.
    size_t offset = (transaction->vendorFieldLength != 0U);

    bytecpy((char *)displayCtx.title[1 + offset], LABEL_EXPIRATION,
                                                  LABEL_EXPIRATION_SIZE);
    bytecpy((char *)displayCtx.title[2 + offset], LABEL_AMOUNT,
                                                  LABEL_AMOUNT_SIZE);
    bytecpy((char *)displayCtx.title[3 + offset], LABEL_FEE,
                                                  LABEL_FEE_SIZE);

    // RecipientId
    encodeBase58PublicKey((uint8_t *)transaction->asset.transfer.recipientId,
                          ADDRESS_HASH_LEN,
                          (uint8_t *)displayCtx.var[0],
                          sizeof(displayCtx.var[0]),
                          transaction->asset.transfer.recipientId[0],
                          1U);

    // VendorField
    if (offset) {
        setVendorField(transaction);
    }

    // Expiration
    printAmount(transaction->asset.transfer.expiration,
                displayCtx.var[1 + offset],
                sizeof(displayCtx.var[1 + offset]),
                NULL, 0U, 0U);

    // Amount
    printAmount(transaction->asset.transfer.amount,
                displayCtx.var[2 + offset],
                sizeof(displayCtx.var[2 + offset]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.var[3 + offset],
                sizeof(displayCtx.var[3 + offset]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////
