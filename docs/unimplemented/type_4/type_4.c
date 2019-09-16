
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

// MultiSignature Registration (Type 4)
//
// @param MultiSignatureAsset *muSig: The MultiSignature (Type 4) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Minimum Participants - 1 Byte:
// - muSig->min = buffer[0];
//
// Key Count
// - (Asset Length - 1) / PublicKey Length
//
// PublicKeys - 33N Bytes
// - os_memmove(muSig->keys, &buffer[1U], keysLength);
//
// ---
ParserStatus deserializeMultiSignature(MultiSignatureAsset *muSig,
                                       const uint8_t *buffer,
                                       const uint32_t length) {
    const uint8_t keysLength = length - 1U;

    PRINTF("\nkeysLength: %d\n", keysLength);

    if ((keysLength % PUBLICKEY_COMPRESSED_LENGTH) != 0) {
        return USTREAM_FAULT;
    }

    muSig->min = buffer[0];
    muSig->count = keysLength / PUBLICKEY_COMPRESSED_LENGTH;
    os_memmove(muSig->keys, &buffer[1U], keysLength);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const char * const ui_menu_multi_signature[][2] = {
    { "Operation",  (const char *const)amountBuffer },
    { "Key Count",  (const char *const)screenBuffer },
    { "Fees",       (const char *const)amountBuffer },
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static ParserStatus internalDeserializeAsset(Transaction *transaction,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
/////////
      case TRANSACTION_TYPE_MULTI_SIGNATURE:
          return deserializeMultiSignature(&transaction->asset.multiSignature,
                                           &buffer[assetOffset],
                                           assetLength);
          break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void displayTransaction(uint8_t step, bagl_element_t *element) {
/////////
    // MultiSignature Registration / Type 4
    else if (transaction.type == TRANSACTION_TYPE_MULTI_SIGNATURE) {
        displayMultiSignature(&transaction, step);
        element->text =
        ui_menu_multi_signature[step][(element->component.userid) >> 4U];
    }
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setDisplaySteps(const Transaction *transaction) {
/////////
    // MultiSignature Registration / Type 4
        else if (transaction->type == TRANSACTION_TYPE_MULTI_SIGNATURE) {
        operation_set_steps(4U);
    }
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void displayMultiSignature(const Transaction *transaction, uint8_t step);

void displayMultiSignature(const Transaction *transaction, uint8_t step) {
    switch(step) {
        // Operation type
        case 0:
            os_memmove((void *)amountBuffer, "MultiSignature\0", 16U);
            break;

        // Key Count
        case 1:
            printAmount(transaction->asset.multiSignature.count,
                        (uint8_t *)screenBuffer,
                        transaction->asset.multiSignature.count,
                        "", 0, 0);
            break;

        // Fees
        case 2:
            printAmount(transaction->fee,
                        (uint8_t *)amountBuffer, sizeof(amountBuffer),
                        TOKEN_NAME, TOKEN_NAME_LENGTH,
                        TOKEN_DECIMALS);
            break;

        default: break;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////