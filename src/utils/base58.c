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

#include "utils/base58.h"

#include <stddef.h>
#include <stdint.h>

#include <os.h>

#include "hashing.h"

////////////////////////////////////////////////////////////////////////////////

const uint8_t BASE58TABLE[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff,  0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff,  0x9,  0xa,  0xb,  0xc,  0xd,  0xe,  0xf,
    0x10, 0xff, 0x11, 0x12, 0x13, 0x14, 0x15, 0xff, 0x16, 0x17, 0x18, 0x19,
    0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
    0xff, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff
};

const uint8_t BASE58ALPHABET[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

////////////////////////////////////////////////////////////////////////////////

uint8_t encodeBase58(uint8_t WIDE *in,
                     size_t inSize,
                     uint8_t *out,
                     size_t maxOutSize) {
    uint8_t tmp[164];
    uint8_t buffer[164];
    size_t j;
    size_t startAt;
    size_t zeroCount = 0;

    if (inSize > sizeof(tmp)) {
        THROW(INVALID_PARAMETER);
    }

    os_memmove(tmp, in, inSize);
    while ((zeroCount < inSize) && (tmp[zeroCount] == 0U)) {
        ++zeroCount;
    }

    j = 2 * inSize;
    startAt = zeroCount;
    while (startAt < inSize) {
        size_t remainder = 0;
        size_t divLoop;

        for (divLoop = startAt; divLoop < inSize; divLoop++) {
            size_t digit256 = (size_t)(tmp[divLoop] & 0xff);
            size_t tmpDiv = remainder * 256 + digit256;
            tmp[divLoop] = (uint8_t)(tmpDiv / 58);
            remainder = (tmpDiv % 58);
        }

        if (tmp[startAt] == 0) {
            ++startAt;
        }

        buffer[--j] = (uint8_t)BASE58ALPHABET[remainder];
    }

    while ((j < (2 * inSize)) && (buffer[j] == BASE58ALPHABET[0])) {
        ++j;
    }

    while (zeroCount-- > 0) {
        buffer[--j] = BASE58ALPHABET[0];
    }

    inSize = 2 * inSize - j;
    if (maxOutSize < inSize) {
        THROW(EXCEPTION_OVERFLOW);
    }

    os_memmove(out, (buffer + j), inSize);

    return inSize;
}

////////////////////////////////////////////////////////////////////////////////

uint16_t encodeBase58PublicKey(uint8_t WIDE *in,
                               size_t inSize,
                               uint8_t *out,
                               size_t outSize,
                               uint16_t version,
                               uint8_t alreadyHashed) {
    uint8_t temp[inSize + 4];
    uint8_t checksum[CX_SHA256_SIZE];
    size_t versionSize = (version > 255U ? 2 : 1);
    size_t ripeLength = versionSize + CX_RIPEMD160_SIZE;

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
        os_memmove(&temp[versionSize], &in[versionSize], CX_RIPEMD160_SIZE);
    }

    cx_sha256_t hash;
    hash256(&hash, temp, ripeLength, checksum);
    hash256(&hash, checksum, CX_SHA256_SIZE, checksum);

    os_memmove(&temp[ripeLength], checksum, 4);

    return encodeBase58(temp, ripeLength + 4, out, outSize);
}

////////////////////////////////////////////////////////////////////////////////
