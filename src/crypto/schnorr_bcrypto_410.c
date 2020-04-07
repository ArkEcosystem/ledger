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
 * 
 ******************************************************************************/

#include "schnorr_bcrypto_410.h"

#include <stdint.h>
#include <string.h>

#include <os.h>
#include <cx.h>

#include "constants.h"

#include "utils/utils.h"

static unsigned char const SECP256K1_G[] = {
    // Gx: 0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798
    0x79, 0xbe, 0x66, 0x7e, 0xf9, 0xdc, 0xbb, 0xac,
    0x55, 0xa0, 0x62, 0x95, 0xce, 0x87, 0x0b, 0x07,
    0x02, 0x9b, 0xfc, 0xdb, 0x2d, 0xce, 0x28, 0xd9,
    0x59, 0xf2, 0x81, 0x5b, 0x16, 0xf8, 0x17, 0x98,
    // Gy:  0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8
    0x48, 0x3a, 0xda, 0x77, 0x26, 0xa3, 0xc4, 0x65,
    0x5d, 0xa4, 0xfb, 0xfc, 0x0e, 0x11, 0x08, 0xa8,
    0xfd, 0x17, 0xb4, 0x48, 0xa6, 0x85, 0x54, 0x19,
    0x9c, 0x47, 0xd0, 0x8f, 0xfb, 0x10, 0xd4, 0xb8 };

static unsigned char const SECP256K1_N[] = {
    // n: 0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0xba, 0xae, 0xdc, 0xe6, 0xaf, 0x48, 0xa0, 0x3b,
    0xbf, 0xd2, 0x5e, 0x8c, 0xd0, 0x36, 0x41, 0x41 };

static  unsigned char const SECP256K1_P []  = { 
    //p:  0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfc, 0x2f };

static unsigned char const SECP256K1_ONE[] = {
    // ONE: 0x0000000000000000000000000000000000000000000000000000000000000001
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

////////////////////////////////////////////////////////////////////////////////
// SECP256K1 Prime Residual Exponent
//
// - ((p - 1) / 2) % p
//
// The Prime(SECP256K1_P) will always be the same in this implementation;
// we can save lots of overhead pre-calculating the residual exponent.
//
//  (fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f) - 1) /
//   0000000000000000000000000000000000000000000000000000000000000002 =
//   7fffffffffffffffffffffffffffffffffffffffffffffffffffffff7ffffe17
// 
// ---
static unsigned char const SECP256K1_RES_EXP[] = {
    // residual exponent: 7fffffffffffffffffffffffffffffffffffffffffffffffffffffff7ffffe17
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFE, 0x17 };

////////////////////////////////////////////////////////////////////////////////
// Check if a Points 'Y' coordinate is a quadratic residue of SECP256K1_P
//
// Point: must be an uncompressed curve-point (0x04, x[32].., y[32]..)[65]
//
// Prime: The SECP256K1 Prime number
// - fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f
// 
// 0: iff result != 1
// 1: iff result == 1
//
// ---
static int hasQuadY(const uint8_t *point) {
    unsigned WIDE char result[HASH_32_LEN];
    // result = pow(point.y, exponent, prime)
    cx_math_powm(result,
                 point + 1 + HASH_32_LEN, SECP256K1_RES_EXP, HASH_32_LEN,
                 SECP256K1_P, HASH_32_LEN);

    return cx_math_cmp(result, SECP256K1_ONE, 32) == 0;     // (result == 1)
}

////////////////////////////////////////////////////////////////////////////////
// Generate a Schnorr Signature
//
// BCrypto v.4.1.0 / BCH Implementation
// - https://github.com/bcoin-org/bcrypto/tree/cd3703d825567a16ea78d2bb10195d0122dc59bd
//
//
// @param [in] privateKey:      must be 32-bytes in length.
// - uint8_t[32]
//
// @param [in] hash:            must be 32-bytes in length.
// - uint8_t[32]
//
// @param [out] signature
// - uint8_t[64]
//
//
// Assumptions:
//
// - Capitalized variables denote an uncompressed Curve Point in affine coordinates.
//   - X = (0x04, x[32].., y[32]..)
//
// - Lower-case variables denote a 32-byte scalar.
//   - x = uint8_t[32]
//
// - Let `H` be a cryptographic hash function.
// - Let `m` be a 32-byte array.
// - Let `a` be a secret non-zero scalar.
// - k != 0.
//
// Computation:
//
//  k = H(a, m) mod n
//  R = G * k
//  A = G * a
//  k = -k mod n, if y(R) is not square
//  e = H( R.x, A, m) mod n
//  s = (k + e * a) mod n
//  S = (r, s)
//
// ---
uint32_t schnorr_sign_bcrypto_410(const uint8_t *privateKey,
                                  const uint8_t *hash,
                                  uint8_t *signature) {
    ////////////////////////////////////////////////////////////
    // Local vars  
    cx_sha256_t         H;
    unsigned WIDE char  a[HASH_32_LEN];
    unsigned WIDE char  k[HASH_32_LEN];
    unsigned WIDE char  R[PUBLICKEY_UNCOMPRESSED_LEN];
    unsigned WIDE char  A[PUBLICKEY_UNCOMPRESSED_LEN];
    unsigned WIDE char  e[HASH_32_LEN];

    unsigned int d_len      = HASH_32_LEN;
    unsigned int sig_len    = SIG_SCHNORR_LEN;

    ////////////////////////////////////////////////////////////
    // Construct the Base-Point (G)
    unsigned WIDE char G[PUBLICKEY_UNCOMPRESSED_LEN];
    G[0] = 0x04;
    os_memcpy(G + 1, SECP256K1_G, HASH_64_LEN);

    ////////////////////////////////////////////////////////////
    // Copy the privateKey
    os_memcpy(a, privateKey, d_len);

    // a' (privateKey) must be an integer in the range [1..n-1]
    if (cx_math_is_zero(a, d_len) ||
        cx_math_cmp(a, SECP256K1_N, d_len) > 0) {
        sig_len = 0;
        goto CLEAR_LOCALS;
    }

    ////////////////////////////////////////////////////////////
    // k' = H( d || m )
    cx_sha256_init(&H);
    cx_hash((cx_hash_t *) &H, 0, a, d_len, NULL, 0);
    cx_hash((cx_hash_t *) &H, CX_LAST | CX_NO_REINIT, hash, d_len, k, d_len);

    // k' must not be '0'
    if (cx_math_is_zero(k, d_len)) {
        sig_len = 0;
        goto CLEAR_LOCALS;
    }

    ////////////////////////////////////////////////////////////
    // R = G * k0'
    os_memcpy(R, G, sizeof(G));
    cx_ecfp_scalar_mult(CX_CURVE_SECP256K1, R, sizeof(R), k, d_len);
    os_memcpy(signature, R + 1, d_len);

    // R must not be '0'
    if (cx_math_is_zero(signature, d_len)) {
        MEMSET_BZERO(&signature, SIG_SCHNORR_LEN);
        sig_len = 0;
        goto CLEAR_LOCALS;
    }

    ////////////////////////////////////////////////////////////
    // A = G * a'
    os_memcpy(A, G, sizeof(G));
    cx_ecfp_scalar_mult(CX_CURVE_SECP256K1, A, sizeof(A), a, d_len);

    // Compress 'A'
    A[0] = (A[HASH_64_LEN] & 1) == 1 ? 0x03 : 0x02;

    ////////////////////////////////////////////////////////////
    // e = H( R.x || A.x || m ) mod n
    cx_sha256_init(&H);
    cx_hash((cx_hash_t *) &H, 0, signature, d_len, NULL, 0);
    cx_hash((cx_hash_t *) &H, 0, A, PUBLICKEY_COMPRESSED_LEN, NULL, 0);
    cx_hash((cx_hash_t *) &H, CX_LAST | CX_NO_REINIT, hash, d_len, e, d_len);
    cx_math_modm(e, d_len, SECP256K1_N, d_len);

    ////////////////////////////////////////////////////////////
    // a' %= n
    cx_math_modm(a, d_len, SECP256K1_N, d_len);

    ////////////////////////////////////////////////////////////
    // k0' %= n
    cx_math_modm(k, d_len, SECP256K1_N, d_len);

    ////////////////////////////////////////////////////////////
    // k' = n - k0' if not is_quad(R.y) else k0'
    if (!hasQuadY(R)) {
        cx_math_sub(k, SECP256K1_N, k, d_len);
    }

    ////////////////////////////////////////////////////////////
    // s' = (k' + e' * a') mod n
    cx_math_multm(e, e, a, SECP256K1_N, d_len);
    cx_math_addm(signature + d_len, k, e, SECP256K1_N, d_len);

    if (cx_math_is_zero(signature + d_len, d_len)) {
        MEMSET_BZERO(signature, SIG_SCHNORR_LEN);
        sig_len = 0;
    }

    ////////////////////////////////////////////////////////////
    CLEAR_LOCALS:
    MEMSET_BZERO((void *)&H, sizeof(H));
    MEMSET_BZERO(&a, sizeof(a));
    MEMSET_BZERO(&k, sizeof(k));
    MEMSET_BZERO(&R, sizeof(R));
    MEMSET_BZERO(&A, sizeof(A));
    MEMSET_BZERO(&e, sizeof(e));

    return sig_len;
}
