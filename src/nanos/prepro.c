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

#include "os.h"

#if defined(TARGET_NANOS)

#include "nanos/prepro.h"

#include <string.h>

#include "os_io_seproxyhal.h"

#include "nanos/menu.h"

#include "ctx.h"
#include "encoding.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

volatile char fullAmount[50];
bagl_element_t tmp_element;

//////////////////////////////////////////////////////////////////////

extern volatile char fullAddress[VOLATILE_ADDRESS_LENGTH];
extern unsigned int ux_step;

//////////////////////////////////////////////////////////////////////

unsigned int ui_address_prepro(const bagl_element_t *element) {
    if (element->component.userid > 0U) {
        unsigned int display = (ux_step == element->component.userid - 1U);
        if (display) {
            switch (element->component.userid) {
            case 1:
                UX_CALLBACK_SET_INTERVAL(2000);
                break;
            case 2:
                UX_CALLBACK_SET_INTERVAL(MAX(
                        3000,
                        1000 + bagl_label_roundtrip_duration_ms(element, 7)));
                break;
            }
        }
        return display;
    }
    return 1U;
}

//////////////////////////////////////////////////////////////////////

extern union {
    publicKeyContext_t publicKeyContext;
    transactionContext_t transactionContext;
} tmpCtx;

extern txContent_t txContent;

//////////////////////////////////////////////////////////////////////

unsigned int ui_approval_prepro(const bagl_element_t *element) {
    unsigned int display = 1U;
    uint32_t addressLength;

    if (element->component.userid > 0U) {
        // display the meta element when at least bigger
        display = (ux_step == element->component.userid - 1U) ||
                  (element->component.userid >= 0x02 && ux_step >= 1U);

        if (display) {
            switch (element->component.userid) {
                case 0x01:
                    UX_CALLBACK_SET_INTERVAL(2000);
                    break;
                case 0x02:
                case 0x12:
                    os_memmove(&tmp_element, element, sizeof(bagl_element_t));
                    display = ux_step - 1U;

                    if (txContent.type == OPERATION_TYPE_TRANSFER) {
                        switch(display) {
                            case 0:  // Operation
                                strcpy((char *)fullAmount, "Transfer");

                                goto display_transfer;
                            case 1:  // Destination
                                addressLength = base58_encode_public_key(
                                        txContent.recipientId,
                                        ADDRESS_HASH_LENGTH,
                                        (unsigned char *)fullAddress,
                                        sizeof(fullAddress),
                                        txContent.recipientId[0],
                                        1U);

                                fullAddress[addressLength] = '\0';

                                goto display_transfer;
                            case 2:  // Amount
                                print_amount(txContent.amount,
                                            (unsigned char *)fullAmount,
                                            sizeof(fullAmount),
                                            TOKEN_NAME,
                                            TOKEN_NAME_LENGTH,
                                            TOKEN_DECIMALS);

                                goto display_transfer;
                            case 3:  // fees
                                print_amount(txContent.fee,
                                            (uint8_t *)fullAmount,
                                            sizeof(fullAmount),
                                            TOKEN_NAME,
                                            TOKEN_NAME_LENGTH,
                                            TOKEN_DECIMALS);

                                display_transfer:
                                tmp_element.text = ui_approval_transfer[display][(element->component.userid) >> 4];

                                break;
                        }
                    } else if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 1U)) {
                        switch(display) {
                            case 0:  // Operation
                                strcpy((char *)fullAmount, "1 vote");

                                goto display_vote1;
                            case 1:  // Vote
                                os_memmove((void *)fullAddress,
                                            tmpCtx.transactionContext.rawTx + txContent.assetOffset,
                                            67U);

                                fullAddress[67] = '\0';

                                goto display_vote1;
                            case 2:  // fees
                                print_amount(txContent.fee,
                                             (uint8_t *)fullAmount,
                                             sizeof(fullAmount),
                                             TOKEN_NAME,
                                             TOKEN_NAME_LENGTH,
                                             TOKEN_DECIMALS);

                                display_vote1:
                                tmp_element.text = ui_approval_vote1[display][(element->component.userid) >> 4];

                                break;
                        }
                    } else if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 2U)) {
                        switch(display) {
                            case 0: // Operation
                                strcpy((char *)fullAmount, "2 votes");

                                goto display_vote2;
                            case 1: // Vote 1
                                os_memmove((uint8_t *)fullAddress,
                                            tmpCtx.transactionContext.rawTx + txContent.assetOffset,
                                            67U);

                                fullAddress[67] = '\0';

                                goto display_vote2;
                            case 2: // Vote 2
                                os_memmove((uint8_t *)fullAddress,
                                            tmpCtx.transactionContext.rawTx + txContent.assetOffset + 67,
                                            67);

                                fullAddress[67] = '\0';

                                goto display_vote2;
                            case 3: // fees
                                print_amount(txContent.fee,
                                            (uint8_t *)fullAmount,
                                            sizeof(fullAmount),
                                            TOKEN_NAME,
                                            TOKEN_NAME_LENGTH,
                                            TOKEN_DECIMALS);

                                display_vote2:
                                tmp_element.text = ui_approval_vote2[display][(element->component.userid) >> 4];

                                break;
                        }
                    }
                UX_CALLBACK_SET_INTERVAL(MAX(
                    3000,
                    1000 + bagl_label_roundtrip_duration_ms(&tmp_element, 7)));

                return (unsigned int)&tmp_element;
            }
        }
    }
    return display;
}

#endif // TARGET_NANOS
