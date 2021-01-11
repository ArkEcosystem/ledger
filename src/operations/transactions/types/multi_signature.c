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

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// MultiSignature Asset / Registration (Type 4) - 68 <=> 530 Bytes
//
// @param MultiSignature *multiSignature
// @param uint8_t *buffer: The serialized buffer at the Assets offset.
// @param int size: The Asset Buffer Size.
//
// @return   0: error
// @return > 0: asset size
//
// This is a generic deserializer for the MultiSignature Asset.
// All participant publicKeys are copied.
//
// The biggest difference between MultiSignature Registration & a Multi-Signed
// Transaction is that a MultiSignature Registration is a 'Type 4';
// The MultiSignature asset will always be at the end of the transaction payload
// regardless.
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
// Signatures - 66N Bytes
// - MEMCOPY(&multiSig->keys[i], &buffer[ofsset + 1(idx)], 64);
//
// ---
size_t deserializeMultiSignature(MultiSignature *multiSig,
                                 const uint8_t *buffer,
                                 size_t size) {
    if (multiSig == NULL || buffer == NULL || size == 0U) {
        return 0U;
    }

    multiSig->min = buffer[0];
    multiSig->count = buffer[1];

    if (multiSig->min < MULTI_SIG_MIN || multiSig->min > MULTI_SIG_MAX ||
        multiSig->count < multiSig->min || multiSig->count > MULTI_SIG_MAX) {
        return 0U;
    }

    for (uint8_t i = 0U; i < multiSig->count; ++i) {
        MEMCOPY(&multiSig->keys[i],
                &buffer[2U + (i * PUBLICKEY_COMPRESSED_LEN)],
                PUBLICKEY_COMPRESSED_LEN);
    }

    return 2U + (multiSig->count * PUBLICKEY_COMPRESSED_LEN);
}

#endif  // SUPPORTS_MULTISIGNATURE
