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
#include <string.h>

#include "constants.h"

#include "crypto/hashing.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////

static const uint8_t BASE58ALPHABET[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

////////////////////////////////////////////////////////////////////////////////

uint8_t encodeBase58(uint8_t *in,
                     size_t inSize,
                     uint8_t *out,
                     size_t maxOutSize) {
    uint8_t tmp[164];
    uint8_t buffer[164];
    size_t j;
    size_t startAt;
    size_t zeroCount = 0;

    if (inSize > sizeof(tmp)) {
        return 0;
    }

    bytecpy(tmp, in, inSize);
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
        explicit_bzero(out, sizeof(out));
        return 0;
    }

    bytecpy(out, (buffer + j), inSize);

    return inSize;
}

////////////////////////////////////////////////////////////////////////////////

uint16_t encodeBase58PublicKey(uint8_t *in,
                               size_t inSize,
                               uint8_t *out,
                               size_t outSize,
                               uint16_t version,
                               uint8_t alreadyHashed) {
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

    return encodeBase58(temp, ripeLength + 4, out, outSize);
}

////////////////////////////////////////////////////////////////////////////////
