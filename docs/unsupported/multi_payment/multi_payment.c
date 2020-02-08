
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

#include "transactions/types/multi_payment.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
//
// Incomplete. Needs more consideration on safe logic behind displaying values.
//
// MultiPayment (Type 6) - 0 <=> 255 Bytes
//
// @param MultiPaymentAsset *multipayment: The MultiPayment (Type 6) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Number of Payments - 2 Bytes:
// - payments->n_payments = U2BE(buffer, 0);
//
// Amounts[] - 8 Bytes * n_payments
// - payments->amounts[i] = U8LE(&buffer[sizeof(uint16_t) + (i * sizeof(uint64_t))], sizeof(uint64_t))
//
// Addresses[] - 21 Bytes * n_payments
// - bytecpy(&payments->addresses[i * 21], &buffer[(sizeof(uint16_t) + (i * (sizeof(uint64_t) + 21))], 21);
//
// ---
bool deserializeMultiPayment(MultiPaymentAsset *payments,
                                     const uint8_t *buffer,
                                     const uint32_t length) {
    payments->n_payments = U2BE(buffer, 0);

    for (uint8_t i = 0U; i < payments->n_payments; ++i) {
        payments->amounts[i] = U8LE(&buffer[sizeof(uint16_t) + i * sizeof(uint64_t)],
                                    sizeof(uint64_t));

        bytecpy(&payments->addresses[i * ADDRESS_HASH_LEN],
                &buffer[sizeof(uint16_t) + payments->n_payments * sizeof(uint64_t) + i * ADDRESS_HASH_LEN],
                ADDRESS_HASH_LEN);
    }

    return true;
}
