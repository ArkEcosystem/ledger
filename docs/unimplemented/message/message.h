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

#ifndef ARK_OPERATIONS_MESSAGE_H
#define ARK_OPERATIONS_MESSAGE_H

#include <stdint.h>

#include <os.h>

#include "ui.h"

////////////////////////////////////////////////////////////////////////////////

void handleMessage(const uint8_t *buffer, const uint32_t length) {
    BEGIN_TRY {
        TRY {
            os_memmove((char *)displayCtx.operation, "Message\0", 9U);
            os_memmove((char *)displayCtx.title[0], "message\0", 3U);

            os_memmove((char *)displayCtx.var[0], buffer, length);
            displayCtx.var[0][length] = '\0';

            setDisplaySteps(1U);
        }

        CATCH_OTHER(e) { result = USTREAM_FAULT; }

        FINALLY {}
    }
    END_TRY;
}

////////////////////////////////////////////////////////////////////////////////

#endif


// #define INS_SIGN_MESSAGE            0x08

// void handleInstruction(volatile unsigned int *flags, volatile unsigned int *tx) {
//     if (G_io_apdu_buffer[0] != 0xE0) {
//         THROW(0x6E00);
//     }
//
//     switch (G_io_apdu_buffer[1U]) {
//         case INS_GET_PUBLIC_KEY: getPublicKeyContext(flags, tx); break;
//
//         case INS_SIGN:
//             getSigningContext(flags, tx);
//             handleTransaction(tmpCtx.signing.data, tmpCtx.signing.dataLength);
//             break;
//
//         case INS_SIGN_MESSAGE:
//             getSigningContext(flags, tx);
//             displayMessage(tmpCtx.signing.data, tmpCtx.signing.dataLength);
//             break;
//
//         case INS_GET_APP_CONFIGURATION: getAppConfiguration(flags, tx); break;
//
//         default: THROW(0x6D00);
//     }
// }
