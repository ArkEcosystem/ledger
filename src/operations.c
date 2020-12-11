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

#include "operations.h"

#include <stdint.h>

#include <os.h>
#include <cx.h>
#include <ux.h>
#include <os_io_seproxyhal.h>

#include "constants.h"

#include "crypto/keys.h"
#include "crypto/signing.h"

#include "utils/base58.h"

#include "operations/message_op.h"
#include "operations/transaction_op.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
#define OFFSET_P1       2U
#define OFFSET_P2       3U
#define OFFSET_LC       4U
#define OFFSET_CDATA    5U

// Choosing P1_FIRST to be 0x00 has the problem that it is not possible to distinguish
// Case1 from Case2:
//
// Case1 (a single chunk): p1==P1_LAST
// Case2 (a few chunks): p1==P1_FIRST, p1==P1_MORE, p1==P1_MORE, ..., p1==P1_LAST
//
// When we receive a chunk with p1==P1_LAST we can't tell if this is the first chunk
// from Case1 or the last chunk from Case2. Because we append all chunks in a buffer,
// we need to be able to tell if this is the first chunk or not - so that we know
// whether to put the chunk in the beginning of the buffer or append at position
// rawMessageLength.
// To workaround this issue we reset rawMessageLength to 0 in
// ui_idle() and always append chunks at rawMessageLength.
#define P1_CONFIRM      0x01
#define P1_NON_CONFIRM  0x00
#define P2_NO_CHAINCODE 0x00
#define P2_CHAINCODE    0x01

#define P1_FIRST        0x00
#define P1_MORE         0x01
#define P1_LAST         0x80
#define P2_ECDSA        0x40
#define P2_SCHNORR_LEG  0x50

////////////////////////////////////////////////////////////////////////////////
#define INS_GET_PUBLIC_KEY          0x02
#define INS_SIGN                    0x04
#define INS_SIGN_MESSAGE            0x08
#define INS_GET_APP_CONFIGURATION   0x06

////////////////////////////////////////////////////////////////////////////////
extern union {
    PublicKeyContext    publicKey;
    SigningContext      signing;
} tmpCtx;

////////////////////////////////////////////////////////////////////////////////
static void handlePublicKeyContext(volatile unsigned int *tx) {
    uint8_t     p1 = G_io_apdu_buffer[OFFSET_P1];
    uint8_t     p2 = G_io_apdu_buffer[OFFSET_P2];
    uint8_t     *dataBuffer = &G_io_apdu_buffer[OFFSET_CDATA];

    uint8_t     bip32PathLength = *(dataBuffer++);
    uint32_t    bip32Path[ADDRESS_MAX_BIP32_PATH];

    uint8_t                 privateKeyData[HASH_32_LEN];
    cx_ecfp_private_key_t   privateKey;
    cx_ecfp_public_key_t    publicKey;

    uint8_t     p2Chain = p2 & 0x3F;
    cx_curve_t  curve;

    if (bip32PathLength < 1 || bip32PathLength > ADDRESS_MAX_BIP32_PATH) {
        // Invalid path
        THROW(0x6A80);
    }

    if (p1 != P1_CONFIRM && p1 != P1_NON_CONFIRM) {
        // p1 must be set.
        THROW(0x6B00);
    }

    if (p2Chain != P2_CHAINCODE && p2Chain != P2_NO_CHAINCODE) {
        THROW(0x6B01);
    }

    curve = CX_CURVE_256K1;

    // Set the HD path.
    for (uint32_t i = 0; i < bip32PathLength; ++i) {
        bip32Path[i] = U4BE(dataBuffer, 0U);
        dataBuffer += 4U;
    }

    tmpCtx.publicKey.needsChainCode = (p2Chain == P2_CHAINCODE);

    BEGIN_TRY {
        TRY {
            // Derive the privateKey using the HD path.
            os_perso_derive_node_bip32(curve,
                                       bip32Path, bip32PathLength,
                                       privateKeyData,
                                       (tmpCtx.publicKey.needsChainCode
                                                ? tmpCtx.publicKey.chainCode
                                                : NULL));

            // Initialize the privateKey to generate the publicKey,
            cx_ecfp_init_private_key(curve,
                                     privateKeyData, HASH_32_LEN,
                                     &privateKey);
            cx_ecfp_generate_pair(curve, &publicKey, &privateKey, 1U);
        }

        FINALLY {
            MEMSET_BZERO(&privateKeyData, sizeof(privateKeyData));
            MEMSET_TYPE_BZERO(&privateKey, cx_ecfp_private_key_t);
        }
    }
    END_TRY;

    // Compress and write the publicKey to the APDU buffer.
    // (compressedPublicKeyLength(33) + publicKey)
    G_io_apdu_buffer[0] = (uint8_t)PUBLICKEY_COMPRESSED_LEN;
    *tx += sizeof(uint8_t);
    *tx += compressPublicKey(publicKey.W, &G_io_apdu_buffer[sizeof(uint8_t)]);

    // Clear the publicKey object.
    MEMSET_TYPE_BZERO(&publicKey, cx_ecfp_public_key_t);

    // Copy the chaincode if needed.
    if (tmpCtx.publicKey.needsChainCode) {
        MEMCOPY(&G_io_apdu_buffer[*tx], tmpCtx.publicKey.chainCode, HASH_32_LEN);
        *tx += HASH_32_LEN;
    }

    // Displaying PublicKey on-device not supported.
    // Non-confirm, throw finished flag.
    // Else, throw an error.
    p1 == P1_NON_CONFIRM ? THROW(0x9000) : THROW(0x6B00);
}

////////////////////////////////////////////////////////////////////////////////
static void handleSigningContext() {
    uint8_t     p1 = G_io_apdu_buffer[OFFSET_P1];
    uint8_t     p2 = G_io_apdu_buffer[OFFSET_P2];

    uint16_t    dataLength = G_io_apdu_buffer[OFFSET_LC];
    uint8_t     *workBuffer = &G_io_apdu_buffer[OFFSET_CDATA];

    bool isLastPayload = (p1 & P1_LAST);
    p1 &= 0x7F;

    // First payload,
    // - Unpack the path from bytes.
    // - Check and set the Signing Algo.
    // - Set the curve-type.
    if (p1 == P1_FIRST) {
        // Set and check the path length.
        if (dataLength < 1) {
            // Not enough data
            THROW(0x6A80);
        }
        tmpCtx.signing.pathLength = workBuffer[0];
        if (tmpCtx.signing.pathLength < 1 ||
            tmpCtx.signing.pathLength > ADDRESS_MAX_BIP32_PATH) {
            //Invalid path
            THROW(0x6A80);
        }

        workBuffer++;
        dataLength--;

        // Unpack the path.
        for (uint32_t i = 0U; i < tmpCtx.signing.pathLength; ++i) {
            if (dataLength < 4) {
                // Not enough data
                THROW(0x6A80);
            }
            tmpCtx.signing.bip32Path[i] = U4BE(workBuffer, 0U);
            workBuffer += 4U;
            dataLength -= 4U;
        }

        if (p2 != P2_ECDSA && p2 != P2_SCHNORR_LEG) {
            // Ecdsa or Schnorr must be selected.
            THROW(0x6B00);
        }

        tmpCtx.signing.isSchnorr = p2 == P2_SCHNORR_LEG;

        tmpCtx.signing.curve = CX_CURVE_256K1;
    }
    else if (p1 != P1_MORE) {
        THROW(0x6B00);
    }

    // Iff first payload, copy to the signing context data.
    if (p1 == P1_FIRST) {
        if (dataLength > MAX_RAW_OPERATION) {
            THROW(0x6A80);
        }
        tmpCtx.signing.dataLength = dataLength;
        MEMCOPY(tmpCtx.signing.data, workBuffer, dataLength);
    }
    // Iff n'th payload, and append to signing context data and add to length.
    else if (p1 == P1_MORE) {
        if (tmpCtx.signing.dataLength + dataLength > MAX_RAW_OPERATION) {
            THROW(0x6A80);
        }

        MEMCOPY(&tmpCtx.signing.data[tmpCtx.signing.dataLength],
                workBuffer,
                dataLength);

        tmpCtx.signing.dataLength += dataLength;
    }

    // Throw "Finished", more data is expected.
    if (!isLastPayload) {
        THROW(0x9000);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void handleAppConfiguration(volatile unsigned int *tx) {
    G_io_apdu_buffer[0] = 0x00;
    G_io_apdu_buffer[1] = LEDGER_MAJOR_VERSION;
    G_io_apdu_buffer[2] = LEDGER_MINOR_VERSION;
    G_io_apdu_buffer[3] = LEDGER_PATCH_VERSION;

    *tx = 4U;
    THROW(0x9000);
}

////////////////////////////////////////////////////////////////////////////////
void handleOperation(volatile unsigned int *flags, volatile unsigned int *tx) {
    if (G_io_apdu_buffer[0] != 0xE0) {
        THROW(0x6E00);
    }

    switch (G_io_apdu_buffer[1]) {
        case INS_GET_PUBLIC_KEY:
            handlePublicKeyContext(tx);
            break;

        case INS_SIGN:
            handleSigningContext();
            if (handleTransaction(tmpCtx.signing.data,
                                  tmpCtx.signing.dataLength) == false) {
                // Deserialization failed
                MEMSET_BZERO(&tmpCtx, sizeof(tmpCtx));
                THROW(0x6A80);
            }
            break;

        case INS_SIGN_MESSAGE:
            handleSigningContext();
            if (handleMessage(tmpCtx.signing.data,
                              tmpCtx.signing.dataLength) == false) {
                // Parsing failed
                MEMSET_BZERO(&tmpCtx, sizeof(tmpCtx));
                THROW(0x6A80);
            }
            break;

        case INS_GET_APP_CONFIGURATION:
            handleAppConfiguration(tx);
            break;

        default: THROW(0x6D00);
    }

    *flags |= IO_ASYNCH_REPLY;
}
