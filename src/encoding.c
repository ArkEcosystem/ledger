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

#include "encoding.h"

#include "os.h"

#include "global.h"

#include "base58.h"
#include "encoding.h"
#include "hashing.h"

//////////////////////////////////////////////////////////////////////

unsigned short base58_decode_address(unsigned char WIDE *in,
                                     unsigned short inlen,
                                     unsigned char *out,
                                     unsigned short outlen) {
    unsigned char hashBuffer[HASH_32_LENGTH];
    cx_sha256_t hash;
    outlen = base58_decode(in, inlen, out, outlen);

    hash_sha256(&hash, out, outlen - RIPEMD160_TMP_LENGTH, hashBuffer);
    hash_sha256(&hash, hashBuffer, HASH_32_LENGTH, hashBuffer);

    if (os_memcmp(out + outlen - RIPEMD160_TMP_LENGTH,
                  hashBuffer,
                  RIPEMD160_TMP_LENGTH)) {
        THROW(INVALID_CHECKSUM);
    }

    return outlen;
}

//////////////////////////////////////////////////////////////////////

unsigned short base58_encode_address(unsigned char WIDE *in,
                                     unsigned short inlen,
                                     unsigned char *out,
                                     unsigned short outlen) {
    unsigned char tmpBuffer[inlen + RIPEMD160_TMP_LENGTH];
    unsigned char checksumBuffer[HASH_32_LENGTH];
    cx_sha256_t hash;

    hash_sha256(&hash, in, inlen, checksumBuffer);
    hash_sha256(&hash, checksumBuffer, HASH_32_LENGTH, checksumBuffer);

    os_memmove(tmpBuffer + inlen, checksumBuffer, RIPEMD160_TMP_LENGTH);

    return base58_encode(tmpBuffer,
                         inlen + RIPEMD160_TMP_LENGTH,
                         out,
                         outlen);
}

//////////////////////////////////////////////////////////////////////

unsigned short base58_encode_public_key(unsigned char WIDE *in,
                                        unsigned short inlen,
                                        unsigned char *out,
                                        unsigned short outlen,
                                        unsigned short version,
                                        unsigned char alreadyHashed) {
    unsigned char tmpBuffer[26U];
    unsigned char checksumBuffer[HASH_32_LENGTH];
    cx_sha256_t hash;
    unsigned char versionSize = (version > 255U ? 2U : 1U);

    if (version > 255U) {
        tmpBuffer[0] = (version >> 8);
        tmpBuffer[1] = version;
    } else {
        tmpBuffer[0] = version;
    }

    if (!alreadyHashed) {
        public_key_hash160(in, inlen, tmpBuffer + versionSize);
    } else {
        os_memmove(tmpBuffer + versionSize,
                   in + versionSize,
                   RIPEMD160_HASH_LENGTH);
    }

    hash_sha256(&hash,
                tmpBuffer,
                versionSize + RIPEMD160_HASH_LENGTH,
                checksumBuffer);
    hash_sha256(&hash, checksumBuffer, HASH_32_LENGTH, checksumBuffer);

    os_memmove(tmpBuffer + RIPEMD160_HASH_LENGTH + versionSize,
               checksumBuffer,
               RIPEMD160_TMP_LENGTH);

    return base58_encode(tmpBuffer,
                         RIPEMD160_HASH_LENGTH + RIPEMD160_TMP_LENGTH + versionSize,
                         out,
                         outlen);
}
