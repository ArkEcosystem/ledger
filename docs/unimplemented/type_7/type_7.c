
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

#include "transactions/assets/type_7.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "operations/status.h"


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static StreamStatus internalDeserializeAsset(Transaction *transaction,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
/////////
    case TRANSACTION_TYPE_DELEGATE_RESIGNATION:
        // No Payload
        status = USTREAM_FINISHED;
        break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setDisplayDelegateResignation(const Transaction *transaction) {
    os_memmove((char *)displayCtx.operation, "Delegate Resignation", 21U);
    os_memmove((char *)displayCtx.title[0], "PublicKey", 10U);
    os_memmove((char *)displayCtx.title[1], "Fees", 5U);

    // Delegate PublicKey
    bytesToHex((char *)displayCtx.var[0],
               transaction->senderPublicKey,
               PUBLICKEY_COMPRESSED_LENGTH);

    // Fees
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[1], sizeof(displayCtx.var[1]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

void setDisplay(const Transaction *transaction) {
/////////
    case TRANSACTION_TYPE_DELEGATE_RESIGNATION:
        setDisplayDelegateResignation(transaction);
        setDisplaySteps(2U);
        break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
