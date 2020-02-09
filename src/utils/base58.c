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
 * Parts of this software are based on the Ledger Bitcoin Wallet App
 *
 *   Ledger App - Bitcoin Wallet
 *   (c) 2016-2019 Ledger
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
 ******************************************************************************/

#include "utils/base58.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "constants.h"

#include "crypto/hashing.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
#define MAX_ENC_INPUT_SIZE 120

////////////////////////////////////////////////////////////////////////////////
static const uint8_t BASE58ALPHABET[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

////////////////////////////////////////////////////////////////////////////////
// src: https://github.com/LedgerHQ/ledger-app-btc/blob/master/src/btchip_base58.c#L79
int btchip_encode_base58(const unsigned char *in, size_t length,
                         unsigned char *out, size_t *outlen) {
    unsigned char buffer[MAX_ENC_INPUT_SIZE * 138 / 100 + 1] = {0};
    size_t i = 0, j;
    size_t startAt, stopAt;
    size_t zeroCount = 0;
    size_t outputSize;

    if (length > MAX_ENC_INPUT_SIZE) {
        return -1;
    }

    while ((zeroCount < length) && (in[zeroCount] == 0)) {
        ++zeroCount;
    }

    outputSize = (length - zeroCount) * 138 / 100 + 1;
    stopAt = outputSize - 1;
    for (startAt = zeroCount; startAt < length; startAt++) {
        int carry = in[startAt];
        for (j = outputSize - 1; (int)j >= 0; j--) {
            carry += 256 * buffer[j];
            buffer[j] = carry % 58;
            carry /= 58;

            if (j <= stopAt - 1 && carry == 0) {
                break;
            }
        }
        stopAt = j;
    }

    j = 0;
    while (j < outputSize && buffer[j] == 0) {
        j += 1;
    }

    if (*outlen < zeroCount + outputSize - j) {
        *outlen = zeroCount + outputSize - j;
        return -1;
    }

    os_memset(out, BASE58ALPHABET[0], zeroCount);

    i = zeroCount;
    while (j < outputSize) {
        out[i++] = BASE58ALPHABET[buffer[j++]];
    }
    *outlen = i;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int encodeBase58PublicKey(uint8_t *in, size_t inSize,
                          uint8_t *out, size_t outSize,
                          uint16_t version, uint8_t alreadyHashed) {
    uint8_t temp[inSize + 4];
    uint8_t checksum[HASH_32_LEN];
    size_t versionSize = (version > 255U ? 2 : 1);
    size_t ripeLength = versionSize + HASH_20_LEN;

    if (version > 255U) {
        temp[0] = (uint8_t)(version >> 8);
        temp[1] = (uint8_t)version;
    }
    else {
        temp[0] = (uint8_t)version;
    }

    if (!alreadyHashed) {
        hash160(in, inSize, &temp[versionSize]);
    }
    else {
        bytecpy(&temp[versionSize], &in[versionSize], HASH_20_LEN);
    }

    hash256(temp, ripeLength, checksum);
    hash256(checksum, HASH_32_LEN, checksum);

    bytecpy(&temp[ripeLength], checksum, 4);

    return btchip_encode_base58(temp, ripeLength + 4, out, &outSize);
}
