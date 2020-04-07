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
 ******************************************************************************/

#include "crypto/keys.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Uncompressed Key must be 65-bytes.           (0x04 + (x,y))
// Compressed output buffer must be 33-bytes.   (0x02/0x03 + (x))
//
// returns the compressed publicKey length if successful.
//
// ---
size_t compressPublicKey(const uint8_t *uncompressed, uint8_t *compressed) {
    if (uncompressed == NULL || compressed == NULL) {
        return 0;
    }

    const uint8_t EC_PUBLICKEY_EVEN_PREFIX          = 0x02;
    const uint8_t EC_PUBLICKEY_ODD_PREFIX           = 0x03;
    const uint8_t EC_PUBLICKEY_UNCOMPRESSED_PREFIX  = 0x04;

    if (uncompressed[0] != EC_PUBLICKEY_UNCOMPRESSED_PREFIX) {
        return 0;
    }

    // prepend the 'x'-value to represent whether 'y' is odd or even.
    compressed[0] = ((uncompressed[HASH_64_LEN] & 1)
            ? EC_PUBLICKEY_ODD_PREFIX
            : EC_PUBLICKEY_EVEN_PREFIX);

    MEMCOPY(&compressed[1], &uncompressed[1], HASH_32_LEN);

    return PUBLICKEY_COMPRESSED_LEN;
}
