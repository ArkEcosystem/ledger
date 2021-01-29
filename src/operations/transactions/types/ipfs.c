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

#include "transactions/types/ipfs.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// IPFS (Type 5) - 0 <=> 64 Bytes
//
// @param Ipfs *ipfs
// @param uint8_t *buffer: The serialized buffer at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// @return   0: error
// @return > 0: asset size
//
// ---
// Internals:
//
// Length - 1 Byte
// - ipfs->length = buffer[1] + 2U;
//
// Dag - 0 <=> 64 Bytes
// - MEMCOPY(ipfs->dag, buffer, ipfs->length);
//
// ---
size_t deserializeIpfs(Ipfs *ipfs, const uint8_t *buffer, size_t size) {
    // 2nd byte of IPFS hash contains its len.
    //
    // byte[0] == hash-type (sha256).
    // byte[1] == hash-type length (32-bytes).
    // byte[[2...]] == 32-byte hash.
    ipfs->length = buffer[sizeof(uint8_t)] + 2;

    // Let's make sure the length isn't > 64,
    // and that the lengths match.
    if (size < ipfs->length) {
        return 0U;
    }

    MEMCOPY(ipfs->dag, buffer, ipfs->length);       // 0 <=> 64 Bytes

    return ipfs->length;
}
