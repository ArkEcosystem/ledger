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

#ifndef ARK_OPERATIONS_TRANSACTIONS_TYPES_ENTITY_H
#define ARK_OPERATIONS_TRANSACTIONS_TYPES_ENTITY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
static const size_t ENTITY_NAME_MAX_LEN = 40;

////////////////////////////////////////////////////////////////////////////////
enum EntityType {
    BUSINESS    = 0,
    PRODUCT     = 1,
    PLUGIN      = 2,
    MODULE      = 3,
    DELEGATE    = 4,
};

////////////////////////////////////////////////////////////////////////////////
enum EntityAction {
    ENTITY_REGISTER = 0,
    ENTITY_UPDATE   = 1,
    ENTITY_RESIGN   = 2,
};

////////////////////////////////////////////////////////////////////////////////
typedef struct entity_data_t {
    uint8_t length;
    const uint8_t *data;
} EntityData;

////////////////////////////////////////////////////////////////////////////////
typedef struct entity_asset_t {
    uint8_t         type;
    uint8_t         subType;
    uint8_t         action;
    EntityData      name;
    EntityData      ipfs;
    EntityData      registrationId;
} Entity;

////////////////////////////////////////////////////////////////////////////////
bool deserializeEntity(Entity *entity, const uint8_t *buffer, size_t size);

#endif  // #define ARK_OPERATIONS_TRANSACTIONS_TYPES_ENTITY_H
