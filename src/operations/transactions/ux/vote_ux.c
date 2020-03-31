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

#include "transactions/ux/vote_ux.h"

#include <stdbool.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/str.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
void SetUxVote(const Transaction *transaction) {
    bool isVote = transaction->asset.vote.data[0] == 1U;

    SPRINTF(displayCtx.operation, "%s",
            isVote ? UX_VOTE_LABELS[1] : UX_VOTE_LABELS[0]);
    SPRINTF(displayCtx.title[0], "%s:", UX_VOTE_LABELS[2]);
    SPRINTF(displayCtx.title[1], "%s:", UX_LABEL_FEE);

    // Vote
    displayCtx.text[0][0] = isVote ? UX_VOTE_SYMBOLS[1] : UX_VOTE_SYMBOLS[0];
    BytesToHex(&transaction->asset.vote.data[1], PUBLICKEY_COMPRESSED_LEN,
               &displayCtx.text[0][1], sizeof(displayCtx.text[0]) - 1);

    // Fee
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_LEN, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[1], sizeof(displayCtx.text[1]));
}

