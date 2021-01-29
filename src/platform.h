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

#ifndef ARK_PLATFORM_H
#define ARK_PLATFORM_H

// This file detects available features and improves portability.
//
// Use this file (vs including '<os.h>') before detecting platform variables,
// such as 'TARGET_NANOS' or 'TARGET_NANOX'.
//
// 'HAVE_BOLOS_SDK' is a variable unique to this app;
// use it in instances where we only need to detect the BOLOS environment.
//
// ---
#if defined(HAVE_BOLOS_SDK)
    #include <os.h>

    #if defined(TARGET_NANOX)
        #define SUPPORTS_LARGE_OPERATIONS
        #define SUPPORTS_MULTISIGNATURE
    #endif
#endif  // HAVE_BOLOS_SDK
#endif  // ARK_PLATFORM_H
