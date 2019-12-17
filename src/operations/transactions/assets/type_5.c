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

#include "transactions/assets/type_5.h"

#include <stddef.h>
#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

// IPFS (Type 5) - 0 <=> 255 Bytes
//
// @param Ipfs *ipfs: The Ipfs (Type 5) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// ---
// Internals:
//
// Length - 1 Byte
// - ipfs->length = buffer[1] + 2U;
//
// Dag - 0 <=> 255 Bytes
// - os_memmove(ipfs->dag, buffer, ipfs->length);
//
// ---
StreamStatus deserializeIpfs(Ipfs *ipfs,
                             const uint8_t *buffer,
                             size_t size) {
    // 2nd byte of IPFS hash contains its len.
    //
    // byte[0] == hash-type (sha256).
    // byte[1] == hash-type length (32-bytes).
    // byte[[2...]] == 32-byte hash.
    ipfs->length = buffer[1] + 2U;

    // Let's make sure the length isn't > 255,
    // and that the lengths match.
    if (size > 255 || size != ipfs->length) {
        return USTREAM_FAULT;
    }

    os_memmove(ipfs->dag, buffer, size);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
