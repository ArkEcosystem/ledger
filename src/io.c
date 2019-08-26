/*******************************************************************************
*   Ark Wallet
*   (c) 2017 Ledger
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

#include "io.h"

#include "os.h"
#include "os_io_seproxyhal.h"

#include "ctx.h"
#include "hashing.h"
#include "global.h"

//////////////////////////////////////////////////////////////////////

unsigned char G_io_seproxyhal_spi_buffer[IO_SEPROXYHAL_BUFFER_SIZE_B];

//////////////////////////////////////////////////////////////////////

extern unsigned int ux_step;
extern unsigned int ux_step_count;

extern union {
    publicKeyContext_t publicKeyContext;
    transactionContext_t transactionContext;
} tmpCtx;

extern uint32_t set_result_get_publicKey(unsigned char *apduBuffer);

//////////////////////////////////////////////////////////////////////

// override point, but nothing more to do
void io_seproxyhal_display(const bagl_element_t *element) {
    io_seproxyhal_display_default((bagl_element_t *)element);
}

//////////////////////////////////////////////////////////////////////

unsigned int io_seproxyhal_touch_exit(const bagl_element_t *e) {//
    // Go back to the dashboard
    os_sched_exit(0);

    // do not redraw the widget
    return 0;
}

//////////////////////////////////////////////////////////////////////

unsigned int io_seproxyhal_touch_address_ok(const bagl_element_t *e) {
    uint32_t tx = set_result_get_publicKey(G_io_apdu_buffer);
    G_io_apdu_buffer[tx++] = 0x90;
    G_io_apdu_buffer[tx++] = 0x00;

    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);

    // Display back the original UX
    ui_idle();

    // do not redraw the widget
    return 0;
}

//////////////////////////////////////////////////////////////////////

unsigned int io_seproxyhal_touch_address_cancel(const bagl_element_t *e) {
    G_io_apdu_buffer[0] = 0x69;
    G_io_apdu_buffer[1] = 0x85;

    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);

    // Display back the original UX
    ui_idle();

    // do not redraw the widget
    return 0;
}

//////////////////////////////////////////////////////////////////////

unsigned int io_seproxyhal_touch_tx_ok(const bagl_element_t *e) {
    uint8_t privateKeyData[HASH_64_LENGTH];
    unsigned char finalhash[HASH_32_LENGTH];
    cx_sha256_t localHash;
    
    cx_ecfp_private_key_t privateKey;
    uint32_t tx = 0;

    if(tmpCtx.transactionContext.curve == CX_CURVE_Ed25519){
        os_perso_derive_node_bip32_seed_key(HDW_ED25519_SLIP10,
                                            CX_CURVE_Ed25519,
                                            tmpCtx.transactionContext.bip32Path,
                                            tmpCtx.transactionContext.pathLength,
                                            privateKeyData,
                                            NULL, NULL, 0U);
    }
    else {
        os_perso_derive_node_bip32(CX_CURVE_256K1,
                                   tmpCtx.transactionContext.bip32Path,
                                   tmpCtx.transactionContext.pathLength,
                                   privateKeyData,
                                   NULL);
    }

    cx_ecfp_init_private_key(tmpCtx.transactionContext.curve,
                             privateKeyData,
                             HASH_32_LENGTH,
                             &privateKey);

    os_memset(privateKeyData, 0, sizeof(privateKeyData));

    if (tmpCtx.transactionContext.curve == CX_CURVE_256K1) {
        hash_sha256(&localHash,
                    tmpCtx.transactionContext.rawTx,
                    tmpCtx.transactionContext.rawTxLength,
                    finalhash);

        #if CX_APILEVEL >= 8U
            tx = cx_ecdsa_sign(&privateKey,
                               CX_RND_RFC6979 | CX_LAST,
                               CX_SHA256,
                               finalhash,
                               sizeof(finalhash),
                               G_io_apdu_buffer,
                               sizeof(G_io_apdu_buffer),
                               NULL);
        #else
            tx = cx_ecdsa_sign(&privateKey,
                               CX_RND_RFC6979 | CX_LAST,
                               CX_SHA256,
                               finalhash,
                               sizeof(finalhash),
                               G_io_apdu_buffer);

            G_io_apdu_buffer[0] = 0x30;
        #endif
    }
    else {
        #if CX_APILEVEL >= 8
            tx = cx_eddsa_sign(&privateKey,
                                CX_LAST,
                                CX_SHA512,
                                tmpCtx.transactionContext.rawTx,
                                tmpCtx.transactionContext.rawTxLength,
                                NULL,
                                0,
                                G_io_apdu_buffer,
                                sizeof(G_io_apdu_buffer),
                                NULL);
        #else
            tx = cx_eddsa_sign(&privateKey,
                                NULL,
                                CX_LAST,
                                CX_SHA512,
                                tmpCtx.transactionContext.rawTx,
                                tmpCtx.transactionContext.rawTxLength,
                                G_io_apdu_buffer);
        #endif
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

//////////////////////////////////////////////////////////////////////

unsigned int io_seproxyhal_touch_tx_cancel(const bagl_element_t *e) {
    G_io_apdu_buffer[0] = 0x69;
    G_io_apdu_buffer[1] = 0x85;

    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);

    // Display back the original UX
    ui_idle();

    // do not redraw the widget
    return 0;
}

//////////////////////////////////////////////////////////////////////

unsigned char io_event(unsigned char channel) {
    // nothing done with the event
    // throw an error on the transport layer if needed.

    // can't have more than one tag in the reply, not supported yet.
    switch (G_io_seproxyhal_spi_buffer[0]) {
        case SEPROXYHAL_TAG_FINGER_EVENT:
            UX_FINGER_EVENT(G_io_seproxyhal_spi_buffer);
            break;
        case SEPROXYHAL_TAG_BUTTON_PUSH_EVENT:
            UX_BUTTON_PUSH_EVENT(G_io_seproxyhal_spi_buffer);
            break;
        case SEPROXYHAL_TAG_STATUS_EVENT:
            if (G_io_apdu_media == IO_APDU_MEDIA_USB_HID &&
                    !(U4BE(G_io_seproxyhal_spi_buffer, 3) &
                        SEPROXYHAL_TAG_STATUS_EVENT_FLAG_USB_POWERED)) {
                THROW(EXCEPTION_IO_RESET);
            }
        // no break is intentional
        default:
            UX_DEFAULT_EVENT();
            break;
        case SEPROXYHAL_TAG_DISPLAY_PROCESSED_EVENT:
            UX_DISPLAYED_EVENT({});
            break;
        case SEPROXYHAL_TAG_TICKER_EVENT:
            UX_TICKER_EVENT(G_io_seproxyhal_spi_buffer, {
                if (UX_ALLOWED) {
                    #if 0
                        if (skipWarning && (ux_step == 0U)) {
                            ux_step++;
                        }
                    #endif
                    if (ux_step_count) {
                        // prepare next screen
                        ux_step = (ux_step + 1U) % ux_step_count;

                        // redisplay screen
                        UX_REDISPLAY();
                    }
                }
            });
            break;
    }

    // close the event if not done previously (such as by a display)
    if (!io_seproxyhal_spi_is_status_sent()) {
        io_seproxyhal_general_status();
    }

    // command has been processed, DO NOT reset the current APDU transport
    return 1U;
}

//////////////////////////////////////////////////////////////////////

unsigned short io_exchange_al(unsigned char channel, unsigned short tx_len) {
    switch (channel & ~(IO_FLAGS)) {
        case CHANNEL_KEYBOARD:
            break;
        
        case CHANNEL_SPI:
            // Multiplexed I/O exchange.
            // done over SPI channel and TLV encapsulated protocol.
            if (tx_len) {
                io_seproxyhal_spi_send(G_io_apdu_buffer, tx_len);
                if (channel & IO_RESET_AFTER_REPLIED) {
                    reset();
                }

                // Nothing received from the master so far.
                // (it's a tx transaction)
                return 0; 
            }
            else {
                return io_seproxyhal_spi_recv(G_io_apdu_buffer,
                                              sizeof(G_io_apdu_buffer),
                                              0);
            }
        default:
        THROW(INVALID_PARAMETER);
    }
    return 0;
}
