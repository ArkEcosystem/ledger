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
#define P2_SECP256K1    0x40

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
    uint8_t p1 = G_io_apdu_buffer[OFFSET_P1];
    uint8_t p2 = G_io_apdu_buffer[OFFSET_P2];
    uint8_t *dataBuffer = &G_io_apdu_buffer[OFFSET_CDATA];

    uint8_t privateKeyData[HASH_32_LENGTH + 1U];
    uint32_t bip32Path[ADDRESS_MAX_BIP32_PATH];
    uint32_t i;
    uint8_t bip32PathLength = *(dataBuffer++);
    cx_ecfp_private_key_t privateKey;
    uint8_t p2Chain = p2 & 0x3F;
    cx_curve_t curve;
    uint8_t addressLength;
 
    if ((bip32PathLength < 0x01) || (bip32PathLength > ADDRESS_MAX_BIP32_PATH)) {
        // Invalid path
        THROW(0x6A80);
    }

    if ((p1 != P1_CONFIRM) && (p1 != P1_NON_CONFIRM)) {
        THROW(0x6B00);
    }

    if ((p2Chain != P2_CHAINCODE) && (p2Chain != P2_NO_CHAINCODE)) {
        THROW(0x6B01);
    }

    if (p2 != P2_SECP256K1) {
        THROW(0x6B00);
    }

    curve = CX_CURVE_256K1;

    for (i = 0; i < bip32PathLength; ++i) {
        bip32Path[i] = U4BE(dataBuffer, 0U);
        dataBuffer += 4U;
    }

    tmpCtx.publicKey.needsChainCode = (p2Chain == P2_CHAINCODE);

    os_perso_derive_node_bip32(CX_CURVE_256K1,
                               bip32Path, bip32PathLength,
                               privateKeyData,
                               (tmpCtx.publicKey.needsChainCode
                                        ? tmpCtx.publicKey.chainCode
                                        : NULL));

    cx_ecfp_init_private_key(curve, privateKeyData, HASH_32_LENGTH, &privateKey);

    cx_ecfp_generate_pair(curve, &tmpCtx.publicKey.data, &privateKey, 1U);

    os_memset(&privateKey, 0, sizeof(privateKey));
    os_memset(privateKeyData, 0, sizeof(privateKeyData));

    compressPublicKey(&tmpCtx.publicKey.data,
                      privateKeyData,
                      PUBLICKEY_COMPRESSED_LENGTH);

    addressLength = encodeBase58PublicKey(privateKeyData,
                                          HASH_32_LENGTH + 1U,
                                          tmpCtx.publicKey.address,
                                          sizeof(tmpCtx.publicKey.address),
                                          TOKEN_NETWORK_BYTE,
                                          0U);
    tmpCtx.publicKey.address[addressLength] = '\0';

    if (p1 == P1_NON_CONFIRM) {
        *tx = setPublicKeyContext(&tmpCtx.publicKey, G_io_apdu_buffer);
        THROW(0x9000);
    }
}

////////////////////////////////////////////////////////////////////////////////

static void handleSigningContext() {
    uint8_t p1 = G_io_apdu_buffer[OFFSET_P1];
    uint8_t p2 = G_io_apdu_buffer[OFFSET_P2];
    uint8_t *workBuffer = &G_io_apdu_buffer[OFFSET_CDATA];
    uint16_t dataLength = G_io_apdu_buffer[OFFSET_LC];

    uint32_t i;
    bool last = (p1 & P1_LAST);
    p1 &= 0x7F;

    if (p1 == P1_FIRST) {
        tmpCtx.signing.pathLength = workBuffer[0];
        if ((tmpCtx.signing.pathLength < 0x01) ||
            (tmpCtx.signing.pathLength > ADDRESS_MAX_BIP32_PATH)) {
            //Invalid path
            THROW(0x6A80);
        }

        workBuffer++;
        dataLength--;

        for (i = 0U; i < tmpCtx.signing.pathLength; ++i) {
            tmpCtx.signing.bip32Path[i] = U4BE(workBuffer, 0U);
            workBuffer += 4U;
            dataLength -= 4U;
        }

        if (p2 != P2_SECP256K1) {
            THROW(0x6B00);
        }

        tmpCtx.signing.curve = CX_CURVE_256K1;
    }
    else if (p1 != P1_MORE) {
        THROW(0x6B00);
    }

    if (p1 == P1_FIRST) {
        tmpCtx.signing.dataLength = dataLength;
        os_memmove(tmpCtx.signing.data, workBuffer, dataLength);
    }
    else if (p1 == P1_MORE) {
        if ((tmpCtx.signing.dataLength + dataLength) > MAX_RAW_OPERATION) {
            THROW(0x6A80);
        }

        os_memmove(&tmpCtx.signing.data[tmpCtx.signing.dataLength],
                   workBuffer,
                   dataLength);

        tmpCtx.signing.dataLength += dataLength;
    }

    if (!last) {
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
        case INS_GET_PUBLIC_KEY: handlePublicKeyContext(tx); break;

        case INS_SIGN:
            handleSigningContext();
            handleTransaction(tmpCtx.signing.data, tmpCtx.signing.dataLength);
            break;

        case INS_SIGN_MESSAGE:
            handleSigningContext();
            handleMessage(tmpCtx.signing.data, tmpCtx.signing.dataLength);
            break;

        case INS_GET_APP_CONFIGURATION: handleAppConfiguration(tx); break;

        default: THROW(0x6D00);
    }

    *flags |= IO_ASYNCH_REPLY;
}

////////////////////////////////////////////////////////////////////////////////
