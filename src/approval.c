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
 * 
 * -----
 * 
 * Parts of this software are based on Ledger Nano SDK
 * 
 * (c) 2017 Ledger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include "approval.h"

#include <os.h>
#include <ux.h>

#include "constants.h"

#include "crypto/hashing.h"
#include "crypto/keys.h"
#include "crypto/signing.h"

#include "display/context.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
extern void ui_idle(void);

////////////////////////////////////////////////////////////////////////////////
union {
    PublicKeyContext    publicKey;
    SigningContext      signing;
} tmpCtx;

////////////////////////////////////////////////////////////////////////////////
unsigned int ioApprove(const bagl_element_t *e) {
    MEMSET_TYPE_BZERO(&displayCtx, DisplayContext);

    unsigned short tx = 0;

    uint8_t                 privateKeyData[HASH_32_LEN];
    cx_ecfp_private_key_t   privateKey;

    BEGIN_TRY {
        TRY {
            os_perso_derive_node_bip32(CX_CURVE_256K1,
                                    tmpCtx.signing.bip32Path,
                                    tmpCtx.signing.pathLength,
                                    privateKeyData,
                                    NULL);

            cx_ecfp_init_private_key(tmpCtx.signing.curve,
                                    privateKeyData,
                                    HASH_32_LEN,
                                    &privateKey);

            if (tmpCtx.signing.curve == CX_CURVE_256K1) {
                uint8_t hash[CX_SHA256_SIZE];
                hash256(tmpCtx.signing.data,
                        tmpCtx.signing.dataLength,
                        hash);

                tx = tmpCtx.signing.isSchnorr
                    ? signSchnorr(&privateKey, hash, G_io_apdu_buffer)
                    : signEcdsa(&privateKey, hash,
                                G_io_apdu_buffer, SIG_ECDSA_MAX_LEN);
            }
        }

        FINALLY {
            MEMSET_BZERO(privateKeyData, sizeof(privateKeyData));
            MEMSET_TYPE_BZERO(&privateKey, cx_ecfp_private_key_t);
            MEMSET_BZERO(&tmpCtx, sizeof(tmpCtx));
        }
    }
    END_TRY;

    G_io_apdu_buffer[tx++] = 0x90;
    G_io_apdu_buffer[tx++] = 0x00;

    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);

    // Display back the original UX
    ui_idle();

    // do not redraw the widget
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ioCancel(const bagl_element_t *e) {
    MEMSET_TYPE_BZERO(&displayCtx, DisplayContext);
    MEMSET_BZERO(&tmpCtx, sizeof(tmpCtx));

    G_io_apdu_buffer[0] = 0x69;
    G_io_apdu_buffer[1] = 0x85;

    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2U);

    // Display back the original UX
    ui_idle();

    // do not redraw the widget
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ioExit(const bagl_element_t *e) {
    MEMSET_TYPE_BZERO(&displayCtx, DisplayContext);
    MEMSET_BZERO(&tmpCtx, sizeof(tmpCtx));

    // Go back to the dashboard
    os_sched_exit(0);

    // do not redraw the widget
    return 0;
}
