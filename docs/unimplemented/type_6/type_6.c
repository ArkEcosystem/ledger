
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

#include "transactions/assets/type_6.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "utils/unpack.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

// MultiPayment (Type 6) - 0 <=> 255 Bytes
//
// @param MultiPaymentAsset *multipayment: The MultiPayment (Type 6) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Number of Payments - 2 Bytes:
// - payments->n_payments = U2BE(buffer, 0);
//
// Amounts[] - 8 Bytes * n_payments
// - payments->amounts[i] = U8BE(&buffer[sizeof(uint16_t) + (i * sizeof(uint64_t))], sizeof(uint64_t))
//
// Addresses[] - 21 Bytes * n_payments
// - os_memmove(&payments->addresses[i * ADDRESS_HASH_LENGTH], &buffer[(sizeof(uint16_t) + (i * (sizeof(uint64_t) + ADDRESS_HASH_LENGTH))], ADDRESS_HASH_LENGTH);
//
// ---
ParserStatus deserializeMultiPayment(MultiPaymentAsset *payments,
                                     const uint8_t *buffer,
                                     const uint32_t length) {
    payments->n_payments = U2BE(buffer, 0);

    for (uint8_t i = 0U; i < payments->n_payments; ++i) {
        payments->amounts[i] = U8BE(&buffer[sizeof(uint16_t) +
                                           (i * sizeof(uint64_t))],
                                    sizeof(uint64_t));

        os_memmove(&payments->addresses[i * ADDRESS_HASH_LENGTH],
                   &buffer[(sizeof(uint16_t) + (i * (sizeof(uint64_t) + ADDRESS_HASH_LENGTH)))],
                   ADDRESS_HASH_LENGTH);
    }

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const char * const ui_menu_multi_payment[][2] = {
    { "Operation",      (const char *const)amountBuffer },
    { "Payment Count",  (const char *const)screenBuffer },
    { "Total Amount",   (const char *const)screenBuffer },
    { "Fees",           (const char *const)amountBuffer },
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static ParserStatus internalDeserializeAsset(Transaction *transaction,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
/////////
    case TRANSACTION_TYPE_MULTIPAYMENT:
        return deserializeMultiPayment(&transaction->asset.multiPayment,
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
    // MultiPayment / Type 6
    else if (transaction.type == TRANSACTION_TYPE_MULTIPAYMENT) {
        displayMultiPayment(&transaction, step);
        element->text =
        ui_menu_multi_payment[step][(element->component.userid) >> 4U];
    }
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setDisplaySteps(const Transaction *transaction) {
/////////
    // MultiPayment / Type 6
    else if (transaction->type == TRANSACTION_TYPE_MULTIPAYMENT) {
        operation_set_steps(5U);
    }
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void displayMultiPayment(const Transaction *transaction, uint8_t step);

void displayMultiPayment(const Transaction *transaction, uint8_t step) {
    switch(step) {
        // Operation type
        case 0:
            os_memmove((void *)amountBuffer, "MultiPayment\0", 13U);
            break;

        // Count
        case 1:
            printAmount(transaction->asset.multiPayment.n_payments,
                        (uint8_t *)screenBuffer,
                        sizeof(transaction->asset.multiPayment.n_payments),
                        "", 0, 0);
            break;

        // Total Amount
        case 2:
            printAmount(transaction->asset.transfer.amount,
                        (uint8_t *)amountBuffer, sizeof(amountBuffer),
                        TOKEN_NAME, TOKEN_NAME_LENGTH,
                        TOKEN_DECIMALS);
            break;

        // Fees
        case 3:
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
