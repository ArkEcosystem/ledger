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
 * -----
 * 
 * Parts of this software are based on Ledger Nano SDK
 * 
 * (c) 2017 Ledger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef ARK_UTILS_H
#define ARK_UTILS_H

#if defined(HAVE_BOLOS_SDK)
    #include <os.h>
    #include <os_io_seproxyhal.h>

    #define MEMCOPY_ os_memmove
    #define MEMSET_ os_memset
    #define MEMSET_BZERO_ explicit_bzero
#else  // if not HAVE_BOLOS_SDK
    #include <string.h>

    #define MEMCOPY_ memcpy
    #define MEMSET_ memset
    #define MEMSET_BZERO_ explicit_bzero
#endif  // HAVE_BOLOS_SDK

////////////////////////////////////////////////////////////////////////////////
// A platform wrapper for 'memcpy' / 'os_memmove'.
//
// @param void *dst:        pointer to the destination buffer.
// @param const void *src:  pointer to the source buffer.
// @param size_t n:         number of bytes to copy.
//
// ---
#define MEMCOPY MEMCOPY_


////////////////////////////////////////////////////////////////////////////////
// A platform wrapper for 'memset'. Fills a block of memory with a given value.
//
// @param void *ptr:    pointer to the object.
// @param int value:    value to fill the block of memory.
// @param size_t n:     number of bytes to set.
//
// ---
#define MEMSET MEMSET_

////////////////////////////////////////////////////////////////////////////////
// Zero-out a block of memory, avoiding compiler optimization.
//
// @param void *ptr:    pointer to the object.
// @param size_t len:   number of bytes to zero-out.
//
// ---
#define MEMSET_BZERO MEMSET_BZERO_

////////////////////////////////////////////////////////////////////////////////
// Zero-out a block of memory with additional type-safety.
//
// TODO: use Static Assertion
//
// @param void *ptr:        pointer to the object.
// @param void expected_t:  type of object to be zeroed-out.
//
// ---
#define MEMSET_TYPE_BZERO(ptr, expected_t) do {     \
    if (sizeof(expected_t) == sizeof(*(ptr))) {     \
        MEMSET_BZERO(ptr, sizeof(expected_t));      \
    }                                               \
} while(0);


#endif  // ARK_UTILS_H
