/*******************************************************************************
 * This file is part of the ARK Ledger App.
 *
 * Copyright (c) ARK Ecosystem <info@ark.io>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "transactions/ux/entity_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/str.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
// Passing the type/action vars to LABELS[] doesn't currently work,
// so we need to be verbose here.
void setEntityOperationLabel(const Transaction *transaction) {
    char type[9] = { '\0' };
    char action[8] = { '\0' };

    switch (transaction->asset.entity.type) {
        case BUSINESS:  SPRINTF(type, "%s", UX_ENTITY_TYPE_LABELS[0]); break;
        case PRODUCT:   SPRINTF(type, "%s", UX_ENTITY_TYPE_LABELS[1]); break;
        case PLUGIN:    SPRINTF(type, "%s", UX_ENTITY_TYPE_LABELS[2]); break;
        case MODULE:    SPRINTF(type, "%s", UX_ENTITY_TYPE_LABELS[3]); break;
        case DELEGATE:  SPRINTF(type, "%s", UX_ENTITY_TYPE_LABELS[4]); break;
        default: break;
    }

    switch(transaction->asset.entity.action) {
        case ENTITY_REGISTER:
            SPRINTF(action, "%s", UX_ENTITY_ACTION_LABELS[0]);
            break;
        
        case ENTITY_UPDATE:
            SPRINTF(action, "%s", UX_ENTITY_ACTION_LABELS[1]);
            break;
        
        case ENTITY_RESIGN:
            SPRINTF(action, "%s", UX_ENTITY_ACTION_LABELS[2]);
            break;
        
        default: break;
    }

    SPRINTF(displayCtx.operation, "%s %s", type, action);
}

////////////////////////////////////////////////////////////////////////////////
void SetUxEntity(const Transaction *transaction) {
    // Operation Label
    setEntityOperationLabel(transaction);

    size_t page = 0U;

    // SubType
    SPRINTF(displayCtx.title[0], "%s:", UX_ENTITY_PAGE_LABELS[0]);
    UintToString(transaction->asset.entity.subType,
                 displayCtx.text[0], sizeof(displayCtx.text[0]));
    page++;


    // Fee
    SPRINTF(displayCtx.title[1], "%s:", UX_LABEL_FEE);
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_LEN, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[1], sizeof(displayCtx.text[1]));
    page++;

    // Name
    if (transaction->asset.entity.name.length > 0U) {
        SPRINTF(displayCtx.title[page], "%s:", UX_ENTITY_PAGE_LABELS[1]);
        MEMCOPY(displayCtx.text[page],
                transaction->asset.entity.name.data,
                transaction->asset.entity.name.length);
        page++;
    }

    // Ipfs
    if (transaction->asset.entity.ipfs.length > 0U) {
        SPRINTF(displayCtx.title[page], "%s:", UX_ENTITY_PAGE_LABELS[2]);
        MEMCOPY(displayCtx.text[page],
                transaction->asset.entity.ipfs.data,
                transaction->asset.entity.ipfs.length);
        page++;
    }

    // Registration Id
    if (transaction->asset.entity.registrationId.length > 0U) {
        SPRINTF(displayCtx.title[page], "%s:", UX_ENTITY_PAGE_LABELS[3]);
        BytesToHex(transaction->asset.entity.registrationId.data,
                   transaction->asset.entity.registrationId.length,
                   displayCtx.text[page], sizeof(displayCtx.text[page]));
    }
}

////////////////////////////////////////////////////////////////////////////////
size_t getEntitySteps(const Transaction *transaction) {
    return UX_ENTITY_BASE_STEPS +
           (transaction->asset.entity.name.length > 0U) +
           (transaction->asset.entity.ipfs.length > 0U) +
           (transaction->asset.entity.registrationId.length > 0U);
}
