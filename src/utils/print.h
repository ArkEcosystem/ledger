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

#ifndef ARK_UTILS_PRINT_H
#define ARK_UTILS_PRINT_H

#if defined(HAVE_BOLOS_SDK)
    #include <os.h>
    #include <os_io_seproxyhal.h>

    #undef SPRINTF  // redefine BOLOS version as 'SPRINTF_'
    #define SPRINTF_(strbuf, ...) snprintf(strbuf, sizeof(strbuf), __VA_ARGS__)

    #define SNPRINTF_ snprintf
#else  // BOLOS NOT detected
    #include <string.h>

    #define SPRINTF_ sprintf
    #define SNPRINTF_ snprintf
#endif  // HAS_BOLOS_SDK

////////////////////////////////////////////////////////////////////////////////
// A platform wrapper for 'sprintf'.
//
// @param char *s:              pointer to the destination buffer.
// @param const char *format:   string print format.
// @param ...:                  variadic arguments.
//
// ---
#define SPRINTF SPRINTF_

////////////////////////////////////////////////////////////////////////////////
// A platform wrapper for 'snprintf'.
//
// @param char *s:              pointer to the destination buffer.
// @param size_t size:          maximum number of bytes to be written. 
// @param const char *format:   string print format.
// @param ...:                  variadic arguments.
//
// ---
#define SNPRINTF SNPRINTF_

#endif  // ARK_UTILS_PRINT_H
