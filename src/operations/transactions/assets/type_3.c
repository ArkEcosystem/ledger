
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

#include "transactions/assets/type_3.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "transactions/status.h"

////////////////////////////////////////////////////////////////////////////////

// Vote (Type 3)
//
// @param Vote *vote: The Vote (Type 3) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Number of Votes: 1 Byte
// - vote->n_votes = buffer[0];
//
// Vote - 1 + 33(Compressed PublicKey) Bytes:
// - os_memmove(vote->data, &buffer[1], 1U + PUBLICKEY_COMPRESSED_LENGTH)
//
// ---
StreamStatus deserializeVote(Vote *vote,
                             const uint8_t *buffer,
                             const uint32_t length) {
    if (((length - 1) % 34U) != 0) {
        return USTREAM_FAULT;
    }

    os_memmove(vote->data, &buffer[1], 1U + PUBLICKEY_COMPRESSED_LENGTH);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
