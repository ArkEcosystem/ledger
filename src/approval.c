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

#include "approval.h"

#include <os.h>

#include <ux.h>
#include <os_io_seproxyhal.h>

#include "constants.h"

#include "crypto/hashing.h"
#include "crypto/keys.h"
#include "crypto/signing.h"

////////////////////////////////////////////////////////////////////////////////

union {
    PublicKeyContext    publicKey;
    SigningContext      signing;
} tmpCtx;

////////////////////////////////////////////////////////////////////////////////

extern void ui_idle(void);

////////////////////////////////////////////////////////////////////////////////

unsigned int ioApprove(const bagl_element_t *e) {
    uint32_t tx = 0;
    cx_ecfp_private_key_t privateKey;
    uint8_t privateKeyData[HASH_64_LENGTH];

    os_perso_derive_node_bip32(CX_CURVE_256K1,
                               tmpCtx.signing.bip32Path,
                               tmpCtx.signing.pathLength,
                               privateKeyData,
                               NULL);

    cx_ecfp_init_private_key(tmpCtx.signing.curve,
                             privateKeyData,
                             HASH_32_LENGTH,
                             &privateKey);

    os_memset(privateKeyData, 0, sizeof(privateKeyData));


    setPublicKeyContext(&tmpCtx.publicKey, G_io_apdu_buffer);

    if (tmpCtx.signing.curve == CX_CURVE_256K1) {
        cx_sha256_t hashCtx;
        uint8_t hash[CX_SHA256_SIZE];
        hash256(&hashCtx,
                tmpCtx.signing.data,
                tmpCtx.signing.dataLength,
                hash);

        tx = signEcdsa(&privateKey, hash,
                       G_io_apdu_buffer,
                       sizeof(G_io_apdu_buffer));
    }

    os_memset(&privateKey, 0, sizeof(privateKey));

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
    // Go back to the dashboard
    os_sched_exit(0);

    // do not redraw the widget
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
