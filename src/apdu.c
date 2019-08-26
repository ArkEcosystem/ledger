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

#include "apdu.h"

#include "os.h"
#include "cx.h"
#include "os_io_seproxyhal.h"

#include "apdu_handlers.h"

#include "ctx.h"

//////////////////////////////////////////////////////////////////////

#define CLA_BEGIN   0U
#define OFFSET_INS  1U

#define CLA                         0xE0
#define INS_GET_PUBLIC_KEY          0x02
#define INS_SIGN                    0x04
#define INS_GET_APP_CONFIGURATION   0x06

//////////////////////////////////////////////////////////////////////

// display stepped screens
ux_state_t ux;
unsigned int ux_step;
unsigned int ux_step_count;

union {
    publicKeyContext_t publicKeyContext;
    transactionContext_t transactionContext;
} tmpCtx;

txContent_t txContent;

//////////////////////////////////////////////////////////////////////

void handle_apdu(volatile unsigned int *flags,
                 volatile unsigned int *tx,
                 txContent_t *txContent) {
    unsigned short sw = 0U;

    BEGIN_TRY {
        TRY {
            if (G_io_apdu_buffer[0] != 0xE0) {
                THROW(0x6E00);
            }

            switch (G_io_apdu_buffer[OFFSET_INS]) {
                case INS_GET_PUBLIC_KEY:
                    apdu_handle_get_public_key(G_io_apdu_buffer, flags, tx);
                    break;
                case INS_SIGN:
                    apdu_handle_sign(G_io_apdu_buffer, flags, tx);
                    break;
                case INS_GET_APP_CONFIGURATION:
                    apdu_handle_get_app_configuration(G_io_apdu_buffer, flags, tx);
                    break;
                default:
                    THROW(0x6D00);
                    break;
            }
        }
        CATCH(EXCEPTION_IO_RESET) {
            THROW(EXCEPTION_IO_RESET);
        }
        CATCH_OTHER(e) {
            switch (e & 0xF000) {
                case 0x6000:
                    // Wipe the transaction content and report the exception.
                    sw = e;
                    os_memset(&txContent, 0, sizeof(txContent));
                    break;
                case 0x9000:
                    // All is well.
                    sw = e;
                    break;
                default:
                    // Internal error.
                    sw = 0x6800 | (e & 0x7FF);
                    break;
            }

            // Unexpected exception => report
            G_io_apdu_buffer[*tx] = sw >> 8;
            G_io_apdu_buffer[*tx + 1] = sw;

            *tx += 2U;
        }
        FINALLY {}
    }
    END_TRY;
}
