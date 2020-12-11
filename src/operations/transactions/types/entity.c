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

#include "transactions/types/entity.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
// Magistrate Transaction (TypeGroup 2)
// Entity (Type 6)
//
// @param Entity *entity            asset object prt.
// @param const uint8_t *buffer:    of the serialized transaction[asset offset].
// @param size_t size:              of the buffer.
//
// @return bool: true if deserialization was successful.
//
// ---
bool deserializeEntity(Entity *entity, const uint8_t *buffer, size_t size) {
    if (entity == NULL || buffer == NULL || size == 0U) {
        return false;
    }

    size_t cursor = 0U;

    entity->type        = buffer[cursor++];
    entity->subType     = buffer[cursor++];
    entity->action      = buffer[cursor++];

    entity->registrationId.length = buffer[cursor++];
    if (entity->registrationId.length > HASH_64_LEN) { return false; }
    if ((entity->registrationId.length > 0U) &&
        (entity->action == ENTITY_UPDATE || entity->action == ENTITY_RESIGN)) {
        if (entity->registrationId.length < HASH_32_LEN) { return false; }
        entity->registrationId.data = &buffer[cursor];
        cursor += entity->registrationId.length;
    }

    entity->name.length = buffer[cursor++];
    if (entity->name.length > ENTITY_NAME_MAX_LEN) { return false; }
    if (entity->name.length > 0U) {
        entity->name.data = &buffer[cursor];
        cursor += entity->name.length;
    }

    entity->ipfs.length = buffer[cursor];
    if (entity->ipfs.length > HASH_64_LEN) { return false; }
    if (entity->ipfs.length > 0U) {
        entity->ipfs.data = &buffer[cursor];
    }

    return true;
}
