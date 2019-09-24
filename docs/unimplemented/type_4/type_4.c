
////////////////////////////////////////////////////////////////////////////////

// The Following are only examples and places where this code could be implemented.
//  It is not final or guaranteed working.
//  This should only serve as a reference for implementing.

////////////////////////////////////////////////////////////////////////////////


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

#include "transactions/assets/type_4.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// MultiSignature Registration (Type 4)
//
// @param MultiSignature *muSig: The MultiSignature (Type 4) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Minimum Participants - 1 Byte:
// - multiSig->min = buffer[0];
//
// Key Count
// - multiSig->count = buffer[1];
//
// PublicKeys - 33N Bytes
// - os_memmove(&multiSig->keys[0], &buffer[2], multiSig->count);
//
// ---
StreamStatus deserializeMultiSignature(MultiSignatureAsset *multiSig,
                                       const uint8_t *buffer,
                                       const uint32_t length) {
    if ((length % 34U) != 0) {
        return USTREAM_FAULT;
    }

    multiSig->min = buffer[0];
    multiSig->count = buffer[1];
    os_memmove(&multiSig->keys[0], &buffer[2], multiSig->count);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static StreamStatus internalDeserializeAsset(Transaction *transaction,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
/////////
    case TRANSACTION_TYPE_MULTI_SIGNATURE:
        status = deserializeMultiSignature(&transaction->asset.multiSignature,
                                            &buffer[assetOffset],
                                            assetLength);
        break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setDisplayMultiSignature(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "MultiSignature", 15U);
    os_memmove((char *)displayCtx.title[0], "Key Count", 10U);
    os_memmove((char *)displayCtx.title[1], "Fees", 5U);

    // Key Count
    printAmount(transaction->asset.multiSignature.count,
                (uint8_t *)displayCtx.var[0], sizeof(displayCtx.var[0]),
                "", 0, 0);

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[1], sizeof(displayCtx.var[1]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

void setDisplay(const Transaction *transaction) {
/////////
    case TRANSACTION_TYPE_MULTI_SIGNATURE:
        setDisplayMultiSignature(transaction);
        setDisplaySteps(2U);
        break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
