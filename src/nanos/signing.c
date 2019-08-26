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

#include "nanos/signing.h"

#include "os_io_seproxyhal.h"

#include "nanos/buttons.h"
#include "nanos/prepro.h"
#include "nanos/screen.h"

#include "ctx.h"
#include "encoding.h"
#include "global.h"
#include "keys.h"
#include "parse.h"

//////////////////////////////////////////////////////////////////////

volatile char fullAddress[VOLATILE_ADDRESS_LENGTH];

//////////////////////////////////////////////////////////////////////

extern ux_state_t ux;
extern unsigned int ux_step;
extern unsigned int ux_step_count;

extern union {
    publicKeyContext_t publicKeyContext;
    transactionContext_t transactionContext;
} tmpCtx;

extern txContent_t txContent;

//////////////////////////////////////////////////////////////////////

uint32_t set_result_get_publicKey(uint8_t *apduBuffer) {
    uint32_t tx = 0;
    uint32_t addressLength = strlen((const char *)tmpCtx.publicKeyContext.address);
    apduBuffer[tx++] = PUBLICKEY_COMPRESSED_LENGTH;

    compress_public_key(&tmpCtx.publicKeyContext.publicKey,
                        apduBuffer + tx,
                        PUBLICKEY_COMPRESSED_LENGTH);

    tx += PUBLICKEY_COMPRESSED_LENGTH;
    apduBuffer[tx++] = addressLength;

    os_memmove(apduBuffer + tx,
               tmpCtx.publicKeyContext.address,
               addressLength);
    tx += addressLength;

    if (tmpCtx.publicKeyContext.getChaincode) {
        os_memmove(apduBuffer + tx,
                   tmpCtx.publicKeyContext.chainCode,
                   HASH_32_LENGTH);
        tx += HASH_32_LENGTH;
    }
    return tx;
}

//////////////////////////////////////////////////////////////////////

void apdu_platform_sign(uint8_t *apduBuffer,
                        volatile unsigned int *flags,
                        volatile unsigned int *tx) {
    uint8_t p1 = apduBuffer[OFFSET_P1];
    uint8_t p2 = apduBuffer[OFFSET_P2];
    uint8_t *workBuffer = apduBuffer + OFFSET_CDATA;
    uint16_t dataLength = apduBuffer[OFFSET_LC];

    UNUSED(tx);
    uint32_t i;
    bool last = (p1 & P1_LAST);
    p1 &= 0x7F;

    if (p1 == P1_FIRST) {
        tmpCtx.transactionContext.pathLength = workBuffer[0];
        if ((tmpCtx.transactionContext.pathLength < 0x01) ||
            (tmpCtx.transactionContext.pathLength > ADDRESS_MAX_BIP32_PATH)) {
            PRINTF("Invalid path\n");
            THROW(0x6a80);
        }

        workBuffer++;
        dataLength--;

        for (i = 0; i < tmpCtx.transactionContext.pathLength; i++) {
            tmpCtx.transactionContext.bip32Path[i] =
                (workBuffer[0] << 24) | (workBuffer[1] << 16) |
                (workBuffer[2] << 8) | (workBuffer[3]);
            workBuffer += 4;
            dataLength -= 4;
        }

        if (((p2 & P2_SECP256K1) == 0) && ((p2 & P2_ED25519) == 0)) {
            THROW(0x6B00);
        }

        if (((p2 & P2_SECP256K1) != 0) && ((p2 & P2_ED25519) != 0)) {
            THROW(0x6B00);
        }

        tmpCtx.transactionContext.curve = (((p2 & P2_ED25519) != 0)
                ? CX_CURVE_Ed25519
                : CX_CURVE_256K1);
    }
    else if (p1 != P1_MORE) {
        THROW(0x6B00);
    }

    if (p1 == P1_FIRST) {
        tmpCtx.transactionContext.rawTxLength = dataLength;
        os_memmove(tmpCtx.transactionContext.rawTx, workBuffer, dataLength);
    }
    else if (p1 == P1_MORE) {
        if ((tmpCtx.transactionContext.rawTxLength + dataLength) > MAX_RAW_TX) {
            THROW(0x6A80);
        }

        os_memmove(tmpCtx.transactionContext.rawTx + tmpCtx.transactionContext.rawTxLength,
                   workBuffer,
                   dataLength);
        tmpCtx.transactionContext.rawTxLength += dataLength;
    }

    if (!last) {
        THROW(0x9000);
    }

    if (parseTx(tmpCtx.transactionContext.rawTx,
                tmpCtx.transactionContext.rawTxLength,
                &txContent)
        != USTREAM_FINISHED) {
        THROW(0x6A80);
    }

    ux_step = 0;
    // "confirm", amount, address, [sourcetag], [destinationtag], fees
    if (txContent.type == OPERATION_TYPE_TRANSFER) {
        ux_step_count = 5U;
    }
    else if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 1U)) {
        ux_step_count = 4U;
    }
    else if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 2U)) {
        ux_step_count = 5U;
    }
    else {
        THROW(0x6888);
    }
    UX_DISPLAY(ui_approval_nanos, (bagl_element_callback_t)ui_approval_prepro);

    *flags |= IO_ASYNCH_REPLY;
}

//////////////////////////////////////////////////////////////////////

void apdu_platform_get_public_key(uint8_t *apduBuffer,
                                  volatile unsigned int *flags,
                                  volatile unsigned int *tx) {
    uint8_t p1 = G_io_apdu_buffer[OFFSET_P1];
    uint8_t p2 = G_io_apdu_buffer[OFFSET_P2];
    uint8_t *dataBuffer = G_io_apdu_buffer + OFFSET_CDATA;

    uint8_t privateKeyData[HASH_32_LENGTH + 1];
    uint32_t bip32Path[ADDRESS_MAX_BIP32_PATH];
    uint32_t i;
    uint8_t bip32PathLength = *(dataBuffer++);
    cx_ecfp_private_key_t privateKey;
    uint8_t p2Chain = p2 & 0x3F;
    cx_curve_t curve;
    uint8_t addressLength;
 
    if ((bip32PathLength < 0x01) || (bip32PathLength > ADDRESS_MAX_BIP32_PATH)) {
        PRINTF("Invalid path\n");
        THROW(0x6a80);
    }

    if ((p1 != P1_CONFIRM) && (p1 != P1_NON_CONFIRM)) {
        THROW(0x6B00);
    }

    if ((p2Chain != P2_CHAINCODE) && (p2Chain != P2_NO_CHAINCODE)) {
        THROW(0x6B01);
    }

    if (((p2 & P2_SECP256K1) == 0) && ((p2 & P2_ED25519) == 0)) {
        THROW(0x6B02);
    }

    if (((p2 & P2_SECP256K1) != 0) && ((p2 & P2_ED25519) != 0)) {
        THROW(0x6B03);
    }

    curve = (((p2 & P2_ED25519) != 0) ? CX_CURVE_Ed25519 : CX_CURVE_256K1);

    for (i = 0; i < bip32PathLength; i++) {
        bip32Path[i] = (dataBuffer[0] << 24) |
                       (dataBuffer[1] << 16) |
                       (dataBuffer[2] << 8) |
                       (dataBuffer[3]);
        dataBuffer += 4;
    }

    tmpCtx.publicKeyContext.getChaincode = (p2Chain == P2_CHAINCODE);
 
    if(curve == CX_CURVE_Ed25519){
        os_perso_derive_node_bip32_seed_key(HDW_ED25519_SLIP10,
                                            CX_CURVE_Ed25519,
                                            bip32Path,
                                            bip32PathLength,
                                            privateKeyData,
                                            (tmpCtx.publicKeyContext.getChaincode
                                                ? tmpCtx.publicKeyContext.chainCode
                                                : NULL),
                                            NULL,
                                            0U);
    }
    else {
        os_perso_derive_node_bip32(CX_CURVE_256K1,
                                   bip32Path,
                                   bip32PathLength,
                                   privateKeyData,
                                   (tmpCtx.publicKeyContext.getChaincode
                                        ? tmpCtx.publicKeyContext.chainCode
                                        : NULL));
    }

    cx_ecfp_init_private_key(curve, privateKeyData, HASH_32_LENGTH, &privateKey);
    cx_ecfp_generate_pair(curve,
                          &tmpCtx.publicKeyContext.publicKey,
                          &privateKey,
                          1U);

    os_memset(&privateKey, 0, sizeof(privateKey));
    os_memset(privateKeyData, 0, sizeof(privateKeyData));

    compress_public_key(&tmpCtx.publicKeyContext.publicKey,
                        privateKeyData,
                        PUBLICKEY_COMPRESSED_LENGTH);

    addressLength = base58_encode_public_key(privateKeyData,
                                             PUBLICKEY_COMPRESSED_LENGTH,
                                             tmpCtx.publicKeyContext.address,
                                             sizeof(tmpCtx.publicKeyContext.address),
                                             TOKEN_NETWORK_BYTE,
                                             0U);

    tmpCtx.publicKeyContext.address[addressLength] = '\0';
    if (p1 == P1_NON_CONFIRM) {
        *tx = set_result_get_publicKey(apduBuffer);
        THROW(0x9000);
    }
    else {
        os_memset((void *)fullAddress, 0, sizeof(fullAddress));
        os_memmove((void *)fullAddress, tmpCtx.publicKeyContext.address, 5U);
        os_memmove((void *)(fullAddress + 5), "...", 3);
        os_memmove((void *)(fullAddress + 8),
                   tmpCtx.publicKeyContext.address + addressLength - 4U, 4U);

        // prepare for a UI based reply
        ux_step = 0U;
        ux_step_count = 2U;
        UX_DISPLAY(ui_address_nanos, (bagl_element_callback_t)ui_address_prepro);

        *flags |= IO_ASYNCH_REPLY;
    }
}

#endif  // TARGET_NANOS
