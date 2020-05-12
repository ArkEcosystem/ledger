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

#include "platform.h"

#if defined(SUPPORTS_MULTISIGNATURE)

#include "transactions/types/multi_signature.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// MultiSignature Registration (Type 54) - 0 <=> 0 Bytes
//
// @param MultiSignature *multiSignature
// @param uint8_t *buffer: The serialized buffer at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// *This implementation handles MultiSignature Reg. differently than ARK Core.
// The ARK Ledger app has no need to deserialize Signatures and treats them as
// part of the MultiSignature Asset; ARK Core handles this logic separately.
//
// ---
// Internals:
//
// Minimum Participants - 1 Byte:
// - multiSig->min = buffer[0];
//
// Key Count
// - multiSig->count = buffer[1];
//
// PublicKeys - 33N Bytes
// - MEMCOPY(&multiSig->keys[i], &buffer[offset], 33);
//
// Signatures - 64N Bytes
// - MEMCOPY(&multiSig->keys[i], &buffer[ofsset], 64);
//
// ---
bool deserializeMultiSignature(MultiSignature *registration,
                               const uint8_t *buffer,
                               size_t size) {
    if (registration == NULL || buffer == NULL || size == 0U) {
        return false;
    }

    registration->min = buffer[0];
    registration->count = buffer[1];

    if (registration->min < MULTI_SIG_KEY_MIN ||
        registration->min > MULTI_SIG_KEY_MAX ||
        registration->count < registration->min ||
        registration->count > MULTI_SIG_KEY_MAX) {
        return false;
    }

    const size_t keysOffset = sizeof(registration->min) +
                              sizeof(registration->count);
    const size_t sigsOffset = keysOffset +
                              registration->count * PUBLICKEY_COMPRESSED_LEN;

    for (size_t i = 0U; i < registration->count; ++i) {
        // PublicKey
        const size_t keyOffset = keysOffset + (i * PUBLICKEY_COMPRESSED_LEN);
        MEMCOPY(&registration->keys[i],
                &buffer[keyOffset],
                PUBLICKEY_COMPRESSED_LEN);

        // Signature
        const size_t sigOffset = (i + sizeof(uint8_t) + (i * SIG_SCHNORR_LEN));
        MEMCOPY(&registration->signatures[i],
                &buffer[sigsOffset + sigOffset],
                SIG_SCHNORR_LEN);
    }

    return true;
}

#endif  // SUPPORTS_MULTISIGNATURE
