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

#include "transactions/display.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "utils/base58.h"
#include "utils/hex.h"
#include "utils/print.h"

#include "transactions/transaction.h"

#include "ux.h"
#include "ux/display_context.h"

////////////////////////////////////////////////////////////////////////////////

static void setDisplayTransfer(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "Transfer", 9U);
    os_memmove((char *)displayCtx.title[0], "To", 3U);
    os_memmove((char *)displayCtx.title[1], "Expiration", 11U);
    os_memmove((char *)displayCtx.title[2], "Amount", 7U);
    os_memmove((char *)displayCtx.title[3], "Fees", 5U);

    // Recipient
    encodeBase58PublicKey((uint8_t *)transaction->asset.transfer.recipient,
                          ADDRESS_HASH_LENGTH,
                          (uint8_t *)displayCtx.var[0],
                          sizeof(displayCtx.var[0]),
                          transaction->asset.transfer.recipient[0],
                          1U);
    displayCtx.var[0][ADDRESS_LENGTH] = '\0';

    // Expiration
    printAmount(transaction->asset.transfer.expiration,
                displayCtx.var[1], sizeof(displayCtx.var[1]),
                NULL, 0U, 0U);

    // Amount
    printAmount(transaction->asset.transfer.amount,
                displayCtx.var[2], sizeof(displayCtx.var[2]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.var[3], sizeof(displayCtx.var[3]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

static void setDisplaySecondSignature(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "Second Signature Registration", 30U);
    os_memmove((char *)displayCtx.title[0], "PublicKey", 10U);
    os_memmove((char *)displayCtx.title[1], "Fees", 5U);

    // PublicKey of Second Signature
    bytesToHex((char *)displayCtx.var[0],
                transaction->asset.secondSignature.publicKey,
                PUBLICKEY_COMPRESSED_LENGTH);

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[1], sizeof(displayCtx.var[1]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

static void setDisplayVote(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "Vote", 5U);
    os_memmove((char *)displayCtx.title[0], "Vote", 7U);
    os_memmove((char *)displayCtx.title[1], "Fees", 5U);

    // Vote
    displayCtx.var[0][0] = (transaction->asset.vote.data[0] == 1U) ? '+' : '-';
    bytesToHex((char *)&displayCtx.var[0][1],
                &transaction->asset.vote.data[1],
                PUBLICKEY_COMPRESSED_LENGTH);

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[1], sizeof(displayCtx.var[1]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

static void setDisplayIpfs(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "IPFS", 5U);
    os_memmove((char *)displayCtx.title[0], "DAG", 4U);
    os_memmove((char *)displayCtx.title[1], "Fees", 5U);

    // DAG
    encodeBase58((uint8_t *)transaction->asset.ipfs.dag,
                 transaction->asset.ipfs.length,
                 (uint8_t *)displayCtx.var[0], MIN(46U, HASH_64_LENGTH));
    displayCtx.var[0][transaction->asset.ipfs.length] = '\0';

    // Let's truncate the DAG if it's longer than 64 bytes.
    if (transaction->asset.ipfs.length > HASH_64_LENGTH) {
        os_memmove((void *)&displayCtx.var[0][HASH_64_LENGTH], (char *)"...", 4U);
    }

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[1], sizeof(displayCtx.var[1]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

static void setDisplayHtlcLock(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "HTLC Lock", 10U);
    os_memmove((char *)displayCtx.title[0], "To", 3U);
    os_memmove((char *)displayCtx.title[1], "Secret Hash", 12U);
    os_memmove((char *)displayCtx.title[2], "Expiration", 11U);
    os_memmove((char *)displayCtx.title[3], "Amount", 7U);
    os_memmove((char *)displayCtx.title[4], "Fees", 5U);

    // Recipient
    encodeBase58PublicKey((uint8_t *)transaction->asset.htlcLock.recipient,
                          ADDRESS_HASH_LENGTH,
                          (uint8_t *)displayCtx.var[0],
                          ADDRESS_LENGTH,
                          transaction->asset.htlcLock.recipient[0],
                          1U);
    displayCtx.var[0][ADDRESS_LENGTH] = '\0';

    // Secret Hash
    bytesToHex((char *)displayCtx.var[1],
               (uint8_t *)transaction->asset.htlcLock.secretHash,
               HASH_32_LENGTH);

    // Expiration
    if (transaction->asset.htlcLock.expirationType == 1U) {
        printAmount(transaction->asset.htlcLock.expiration,
                    displayCtx.var[2], sizeof(displayCtx.var[2]),
                    "time: ", 6U, 0U);
    } else {
        printAmount(transaction->asset.htlcLock.expiration,
                    displayCtx.var[2], sizeof(displayCtx.var[2]),
                    "height: ", 8U, 0U);
    }

    // Amount
    printAmount(transaction->asset.htlcLock.amount,
                (uint8_t *)displayCtx.var[3], sizeof(displayCtx.var[3]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
    // Fees
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[4], sizeof(displayCtx.var[4]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

static void setDisplayHtlcClaim(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "HTLC Claim", 11U);
    os_memmove((char *)displayCtx.title[0], "Lock Id", 8U);
    os_memmove((char *)displayCtx.title[1], "Secret", 7U);

    // Id
    bytesToHex((char *)displayCtx.var[0],
               transaction->asset.htlcClaim.id,
               HASH_32_LENGTH);

    // Secret
    os_memmove((char *)displayCtx.var[1],
               transaction->asset.htlcClaim.secret,
               HASH_32_LENGTH);
    displayCtx.var[1][HASH_32_LENGTH] = '\0';
}

////////////////////////////////////////////////////////////////////////////////

void setDisplayHtlcRefund(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "HTLC Refund", 12U);
    os_memmove((char *)displayCtx.title[0], "Lock Id", 8U);

    // Lock Id
    bytesToHex((char *)displayCtx.var[0],
               transaction->asset.htlcRefund.id,
               HASH_32_LENGTH);
}

////////////////////////////////////////////////////////////////////////////////

void setDisplay(const Transaction *transaction) {
    switch (transaction->type) {
        case TRANSACTION_TYPE_TRANSFER:
            setDisplayTransfer(transaction);
            setDisplaySteps(4U);
            break;

        case TRANSACTION_TYPE_SECOND_SIGNATURE:
            setDisplaySecondSignature(transaction);
            setDisplaySteps(2U);
            break;

        case TRANSACTION_TYPE_VOTE:
            setDisplayVote(transaction);
            setDisplaySteps(2U);
            break;

        case TRANSACTION_TYPE_IPFS:
            setDisplayIpfs(transaction);
            setDisplaySteps(2U);
            break;

        case TRANSACTION_TYPE_HTLC_LOCK:
            setDisplayHtlcLock(transaction);
            setDisplaySteps(5U);
            break;

        case TRANSACTION_TYPE_HTLC_CLAIM:
            setDisplayHtlcClaim(transaction);
            setDisplaySteps(2U);
            break;

        case TRANSACTION_TYPE_HTLC_REFUND:
            setDisplayHtlcRefund(transaction);
            setDisplaySteps(1U);
            break;

        default: break;
    };
}
