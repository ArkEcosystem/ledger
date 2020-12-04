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

#ifndef ARK_OPERATIONS_TRANSACTIONS_UX_SIGNATURES_UX_H
#define ARK_OPERATIONS_TRANSACTIONS_UX_SIGNATURES_UX_H

#include "platform.h"

#if defined(SUPPORTS_MULTISIGNATURE)

#include <stddef.h>

#include "transactions/transaction.h"

////////////////////////////////////////////////////////////////////////////////
static const char *const UX_SIGNATURES_LABEL = "Signature";

////////////////////////////////////////////////////////////////////////////////
size_t SetUxSignatures(const Transaction *transaction, size_t offset);

#endif  // SUPPORTS_MULTISIGNATURE
#endif  // #define ARK_OPERATIONS_TRANSACTIONS_UX_SIGNATURES_UX_H
