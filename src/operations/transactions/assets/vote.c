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

#include "transactions/assets/vote.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////

// Vote (Type 3) - 34 Bytes
//
// @param Vote *vote
// @param const uint8_t *buffer: The serialized buffer at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// @return bool: true if deserialization was successful.
//
// ---
// Internals:
//
// Number of Votes - 1 Byte: Not Needed
// - vote->n_votes = buffer[0];
//
// Vote - 1 + 33(Compressed PublicKey) Bytes:
// - bytecpy(vote->data, &buffer[1], 34);
//
// ---
bool deserializeVote(Vote *vote, const uint8_t *buffer, size_t size) {
    if (size != sizeof(uint8_t) + VOTE_LEN) {
        return false;
    }

    // skip vote count
    bytecpy(vote->data, &buffer[sizeof(uint8_t)], VOTE_LEN);        // 34 Bytes

    return true;
}

////////////////////////////////////////////////////////////////////////////////
