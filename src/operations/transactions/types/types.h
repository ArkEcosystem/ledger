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

#ifndef ARK_OPERATIONS_TRANSACTIONS_ASSETS_TYPES_H
#define ARK_OPERATIONS_TRANSACTIONS_ASSETS_TYPES_H

////////////////////////////////////////////////////////////////////////////////
// ARK Transaction Types
//
// Supported Tx Types:
// - Transfer                           (type 0)
// - Vote/Unvote                        (type 3)
// - MultiSignature Registration        (type 4)
// - Ipfs                               (type 5)
// - Htlc Lock                          (type 8)
// - Htlc Claim                         (type 9)
// - Htlc Refund                        (type 10)
//
// ---
//
// Unsupported Tx Types:
// - Second Signature Registration      (type 1)
// - Delegate Registration              (type 2)
// - MultiPayment                       (type 6)
// - Delegate Resignation               (type 7)
//
// ---
enum TransactionTypes {
    TRANSFER_TYPE                   = 0,
/*  SECOND_SIGNATURE_TYPE           = 1, */
/*  DELEGATE_REGISTRATION_TYPE      = 2, */
    VOTE_TYPE                       = 3,
    MULTI_SIG_REGISTRATION_TYPE     = 4,
    IPFS_TYPE                       = 5,
/*  MULTI_PAYMENT_TYPE              = 6, */
/*  DELEGATE_RESIGNATION_TYPE       = 7, */
    HTLC_LOCK_TYPE                  = 8,
    HTLC_CLAIM_TYPE                 = 9,
    HTLC_REFUND_TYPE                = 10
};

#endif  // ARK_OPERATIONS_TRANSACTIONS_ASSETS_TYPES_H
