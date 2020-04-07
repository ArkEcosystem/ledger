
////////////////////////////////////////////////////////////////////////////////

// The Following are only examples and places where this code could be implemented.
//  It is not final or guaranteed working.
//  This should only serve as a reference for implementing.

////////////////////////////////////////////////////////////////////////////////

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

#include "transactions/types/delegate_registration.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Deserialize Transfer (Type 2) - 4 <=> 20 Bytes
//
// @param DelegateRegistrationAsset *registration: The Delegate Registration (Type 2) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Username Length - 1 Byte:
// - registration->length = buffer[0];
//
// Username - 3 <=> 20 Bytes:
// - MEMCOPY(registration->username, &buffer[1], registration->length)
//
// ---
bool deserializeDelegateRegistration(DelegateRegistration *registration,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
    // usernameLength + username
    if (length < DELEGATE_REGISTRATION_USERNAME_MIN ||
        length > DELEGATE_REGISTRATION_USERNAME_MAX) {
        return false;  // Incorrect Username Length
    }

    registration->length = (int)buffer[0];
    MEMCOPY(registration->username,
            &buffer[1],
            registration->length);

    return true;
}
