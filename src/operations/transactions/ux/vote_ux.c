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
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
void displayVote(const Transaction *transaction) {
    const char *const LABEL_VOTE    = "Vote";
    const size_t LABEL_VOTE_SIZE    = 5;

    const char *const LABEL_UNVOTE  = "Unvote";
    const size_t LABEL_UNVOTE_SIZE  = 7;

    const char SYMBOL_PLUS      = '+';
    const char SYMBOL_MINUS     = '-';

    bool isVote = (transaction->asset.vote.data[0] == 1U);
    const char *voteLabel       = isVote ? LABEL_VOTE : LABEL_UNVOTE;
    const size_t voteLabelSize  = isVote ? LABEL_VOTE_SIZE : LABEL_UNVOTE_SIZE;
    const char voteSymbol       = isVote ? SYMBOL_PLUS : SYMBOL_MINUS;

    bytecpy((char *)displayCtx.operation, LABEL_VOTE, LABEL_VOTE_SIZE);
    bytecpy((char *)displayCtx.title[0], voteLabel, voteLabelSize);
    bytecpy((char *)displayCtx.title[1], LABEL_FEE, LABEL_FEE_SIZE);

    // Vote
    displayCtx.text[0][0] = voteSymbol;
    bytesToHex((char *)&displayCtx.text[0][1],
                &transaction->asset.vote.data[1],
                PUBLICKEY_COMPRESSED_LEN);

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.text[1], sizeof(displayCtx.text[1]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);
}
