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

#include "platform.h"

#if defined(SUPPORTS_MULTISIGNATURE)

#include "transactions/ux/multi_signature_ux.h"

#include <stddef.h>

#include "constants.h"

#include "operations/transactions/transaction.h"
#include "transactions/types/types.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/str.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
static size_t setMultiSignatureLabels(const Transaction *transaction) {
    const char* const OP_FMT = "%s: %s";
    SPRINTF(displayCtx.operation, OP_FMT,
            MULTI_SIG_LABELS[0], MULTI_SIG_LABELS[1]);

    // Fee
    const char* const FEE_FMT = "%s:";
    SPRINTF(displayCtx.title[0], FEE_FMT, UX_LABEL_FEE);
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_LEN, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[0], sizeof(displayCtx.text[0]));

    const size_t count = transaction->asset.multiSignature.count;
    const char* const KEY_FMT = "%s: %d/%d";

    // PublicKeys
    for (uint8_t i = 0U; i < count; ++i) {
        const size_t offset = 1U + i;
        SPRINTF(displayCtx.title[offset], KEY_FMT,
                MULTI_SIG_LABELS[2], offset, count);
        BytesToHex(transaction->asset.multiSignature.keys[i],
                   PUBLICKEY_COMPRESSED_LEN,
                   displayCtx.text[offset],
                   sizeof(displayCtx.text[offset]));
    }

    return 1U + transaction->asset.multiSignature.count;
}

////////////////////////////////////////////////////////////////////////////////
static size_t setMultiSignedLabels(const Transaction *transaction) {
    char temp[DISPLAY_CTX_OP_SIZE] = { '\0' };
    const char* const FMT = "%s %s";
    MEMCOPY(temp, displayCtx.operation, DISPLAY_CTX_OP_SIZE);
    SPRINTF(displayCtx.operation, FMT, MULTI_SIG_LABELS[0], temp);
}


////////////////////////////////////////////////////////////////////////////////
size_t SetUxMultiSignature(const Transaction *transaction) {
    return transaction->type == MULTI_SIG_REGISTRATION_TYPE
        ? setMultiSignatureLabels(transaction)
        : setMultiSignedLabels(transaction);
}

#endif  // SUPPORTS_MULTISIGNATURE
