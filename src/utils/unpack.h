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

#ifndef ARK_UTILS_UNPACK_H
#define ARK_UTILS_UNPACK_H

#include <stddef.h>
#include <stdint.h>

#if defined(HAVE_BOLOS_SDK)
// Undef the BOLOS macros in favor of additional type-safety implemented below.
    #undef U2LE
    #undef U4LE
#endif

////////////////////////////////////////////////////////////////////////////////
// Unsigned Integer Unpacking Constants
static const size_t U1_SIZE         = sizeof(uint8_t);
static const size_t U2_SIZE         = sizeof(uint16_t);
static const size_t U4_SIZE         = sizeof(uint32_t);
static const size_t U8_SIZE         = sizeof(uint64_t);

static const uint8_t U1_MAX         = 0xFFU;
static const uint16_t U2_MAX        = 0xFFFFUL;
static const uint32_t U4_MAX        = 0xFFFFFFFFUL;
static const uint64_t U8_MAX        = 0xFFFFFFFFFFFFFFFFULL;

static const uint16_t U1_SHIFT      = U1_SIZE * U8_SIZE;
static const uint32_t U2_SHIFT      = U2_SIZE * U8_SIZE;
static const uint64_t U4_SHIFT      = U4_SIZE * U8_SIZE;

////////////////////////////////////////////////////////////////////////////////
// Unpack a 2-byte packed number ot its unsigned (uint16_t/unsigned short) form.
//
// example:
// - uint16_t u16 = U2LE(src, offset);
//
// @param uint8_t src:     packed unsigned integer 2-byte array.
// @param size_t offset:   offset in the input/src stream.
//
// @return uint16_t: then unpacked 2-byte value.
//
// ---
#define U2LE(src, offset)                                                   \
    ((uint16_t)((src)[(offset)]                 & U1_MAX)               |   \
    ((uint16_t)((src)[(offset) + U1_SIZE]       & U1_MAX)   << U1_SHIFT)    \
                                                & U2_MAX)

////////////////////////////////////////////////////////////////////////////////
// Unpack a 4-byte packed number to its unsigned (uint32_t/unsigned int) form.
//
// example:
// - uint32_t u32 = U4LE(src, offset);
//
// @param: uint8_t src:     packed unsigned integer 4-byte array.
// @param: size_t offset:   offset in the input/src stream.
//
// @return uint32_t: then unpacked 4-byte value.
//
// ---
#define U4LE(src, offset)                                                   \
    ((uint32_t)(U2LE((src), (offset))           & U2_MAX)               |   \
    ((uint32_t)(U2LE((src), (offset) + U2_SIZE) & U2_MAX)   << U2_SHIFT)    \
                                                & U4_MAX)

////////////////////////////////////////////////////////////////////////////////
// Unpack an 8-byte packed number to its unsigned (uint64_t/unsigned long) form.
//
// example:
// - uint64_t u64 = U8LE(src, offset);
//
// @param: uint8_t src:     packed unsigned integer 8-byte array.
// @param: size_t offset:   offset in the input/src stream.
//
// @return uint64_t: then unpacked 8-byte value.
//
// ---
#define U8LE(src, offset)                                                   \
    ((uint64_t)(U4LE((src), (offset))           & U4_MAX)               |   \
    ((uint64_t)(U4LE((src), (offset) + U4_SIZE) & U4_MAX)   << U4_SHIFT)    \
                                                & U8_MAX)

#endif  // ARK_UTILS_UNPACK_H
