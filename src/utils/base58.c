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
 * (btchip_encode_base58)
 * - src: https://github.com/LedgerHQ/ledger-app-btc/blob/master/src/btchip_base58.c#L79
 * 
 * changes:
 * - rename 'btchip_encode_base58' -> 'Base58Encode'.
 * - moved global 'MAX' variable to local scope.
 * - added more checks.
 * - added U suffix to integer literals.
 * - use stdint-types.
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
// Base58 Alphabet Table
static const uint8_t BASE58ALPHABET[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

////////////////////////////////////////////////////////////////////////////////
// Base58 Encode an input stream. Outputs a formatted string.
//
// @param const unsigned char *in:  input byte-array to be encoded.
// @param size_t length:            length of the input.
// @param unsigned char *out:       destination where output will be written.
// @param size_t *outlen:           ptr to the max length of writable space.
//
// @return int: '0' if successful, otherwise '-1'.
//
// ---
int Base58Encode(const uint8_t *in, size_t length, char *out, size_t *outlen) {
    const size_t MAX_ENC_INPUT_SIZE = 120U;

    if (in == NULL || length == 0U ||
        out == NULL || *out < 0U || outlen == 0U ||
        length > MAX_ENC_INPUT_SIZE) {
        return -1;
    }

    size_t i = 0U, j;
    size_t startAt, stopAt;
    size_t zeroCount = 0U;
    size_t outputSize;

    while ((zeroCount < length) && (in[zeroCount] == 0U)) {
        ++zeroCount;
    }

    unsigned char buffer[MAX_ENC_INPUT_SIZE * 138U / 100U + 1U] = { 0U };

    outputSize = (length - zeroCount) * 138U / 100U + 1U;
    stopAt = outputSize - 1U;
    for (startAt = zeroCount; startAt < length; startAt++) {
        int carry = in[startAt];
        for (j = outputSize - 1U; (int)j >= 0U; j--) {
            carry += 256U * buffer[j];
            buffer[j] = carry % 58U;
            carry /= 58U;

            if (j <= stopAt - 1U && carry == 0U) {
                break;
            }
        }
        stopAt = j;
    }

    j = 0U;
    while (j < outputSize && buffer[j] == 0U) {
        j += 1U;
    }

    if (*outlen < zeroCount + outputSize - j) {
        *outlen = zeroCount + outputSize - j;
        return -1;
    }

    MEMSET(out, BASE58ALPHABET[0], zeroCount);

    i = zeroCount;
    while (j < outputSize) {
        out[i++] = BASE58ALPHABET[buffer[j++]];
    }
    *outlen = i;

    return 0U;
}

////////////////////////////////////////////////////////////////////////////////
// Base58Check Encode an input stream, Outputs a formatted string.
//
// for more information on Base58Check Encoding:
// - https://en.bitcoin.it/wiki/Base58Check_encoding
//
// @param const uint8_t *in:    input byte-array to be encoded.
// @param size_t length:        length of the input.
// @param char *out:            destination where output will be written.
// @param size_t outLen:        max length of writable space.
//
// @return int: '0' if successful, otherwise '-1'.
//
// --
int Base58CheckEncode(const uint8_t *in, size_t length,
                      char *out, size_t outLen) {
    const size_t BASE58_CHECKSUM_LEN = 4U;

    if (in == NULL || length == 0U || out == NULL ||
        length + BASE58_CHECKSUM_LEN > outLen) {
        return -1;
    }

    uint8_t temp[length + BASE58_CHECKSUM_LEN];
    uint8_t checksum[HASH_32_LEN];

    MEMCOPY(temp, in, length);

    // Calculate the checksum.
    hash256(temp, length, checksum);
    hash256(checksum, HASH_32_LEN, checksum);

    // append the first 4 bytes of the checksum to the output.
    MEMCOPY(&temp[length], checksum, BASE58_CHECKSUM_LEN);
    MEMSET_BZERO(checksum, HASH_32_LEN);

    if (Base58Encode(temp, length + BASE58_CHECKSUM_LEN, out, &outLen) < 0) {
        MEMSET_BZERO(temp, length + BASE58_CHECKSUM_LEN);
        MEMSET_BZERO(out, outLen);

        return -1;
    }

    MEMSET_BZERO(temp, length + BASE58_CHECKSUM_LEN);

    return 0U;
}
