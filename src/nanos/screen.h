/*******************************************************************************
*   Ark Wallet
*   (c) 2017 Ledger
*   (c) ARK Ecosystem
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#pragma once

#include "os_io_seproxyhal.h"

extern const bagl_element_t ui_address_nanos[7];
extern const bagl_element_t ui_approval_nanos[7];

/* Placeholder macros for readability. */
#define _B_FIELD_TYPE(a) (a)
#define _B_FIELD_USERID(a) (a)
#define _B_FIELD_X(a) (a)
#define _B_FIELD_Y(a) (a)
#define _B_FIELD_WIDTH(a) (a)
#define _B_FIELD_HEIGHT(a) (a)
#define _B_FIELD_STROKE(a) (a)
#define _B_FIELD_RADIUS(a) (a)
#define _B_FIELD_FILL(a) (a)
#define _B_FIELD_FGCOLOR(a) (a)
#define _B_FIELD_BGCOLOR(a) (a)
#define _B_FIELD_FONTID(a) (a)
#define _B_FIELD_ICONID(a) (a)
#define _B_FIELD_TEXT(a) (a)
#define _B_FIELD_TOUCH_AREA_BRIM(a) (a)
#define _B_FIELD_OVERFGCOLOR(a) (a)
#define _B_FIELD_OVERBGCOLOR(a) (a)
#define _B_FIELD_TAP(a) (a)
#define _B_FIELD_OUT(a) (a)
#define _B_FIELD_OVER(a) (a)

// This macro calculates the time needed to display scrolling text. 
// Milliseconds are used to produce a value in units of "100ms".
// For example, the SDK interprets 10 as "10 x 100ms":
// (== 1000ms == 1sec).
#define _B_SCROLL_WAIT_MS(ms) ((ms) / 100)

#define _B_CLEAN_SCREEN { \
    { \
        _B_FIELD_TYPE(BAGL_RECTANGLE), \
        _B_FIELD_USERID(0x00), \
        _B_FIELD_X(0), \
        _B_FIELD_Y(0), \
        _B_FIELD_WIDTH(128), \
        _B_FIELD_HEIGHT(32), \
        _B_FIELD_STROKE(0), \
        _B_FIELD_RADIUS(0), \
        _B_FIELD_FILL(BAGL_FILL), \
        _B_FIELD_FGCOLOR(0x000000), \
        _B_FIELD_BGCOLOR(0xFFFFFF), \
        _B_FIELD_FONTID(0), \
        _B_FIELD_ICONID(0) \
    }, \
    _B_FIELD_TEXT(NULL), \
    _B_FIELD_TOUCH_AREA_BRIM(0), \
    _B_FIELD_OVERFGCOLOR(0), \
    _B_FIELD_OVERBGCOLOR(0), \
    _B_FIELD_TAP(NULL), \
    _B_FIELD_OUT(NULL), \
    _B_FIELD_OVER(NULL) \
}

#define _B_ICON(iconid, userid, x, y, w, h) { \
    { \
        _B_FIELD_TYPE(BAGL_ICON), \
        _B_FIELD_USERID(userid), \
        _B_FIELD_X(x), \
        _B_FIELD_Y(y), \
        _B_FIELD_WIDTH(w), \
        _B_FIELD_HEIGHT(h), \
        _B_FIELD_STROKE(0), \
        _B_FIELD_RADIUS(0), \
        _B_FIELD_FILL(0), \
        _B_FIELD_FGCOLOR(0xFFFFFF), \
        _B_FIELD_BGCOLOR(0x000000), \
        _B_FIELD_FONTID(0), \
        _B_FIELD_ICONID(iconid) \
    }, \
    _B_FIELD_TEXT(NULL), \
    _B_FIELD_TOUCH_AREA_BRIM(0), \
    _B_FIELD_OVERFGCOLOR(0), \
    _B_FIELD_OVERBGCOLOR(0), \
    _B_FIELD_TAP(NULL), \
    _B_FIELD_OUT(NULL), \
    _B_FIELD_OVER(NULL) \
}

#define _B_ICON_LEFT(iconid, userid) \
    _B_ICON( \
        _B_FIELD_ICONID(iconid), \
        _B_FIELD_USERID(userid), \
        _B_FIELD_X(3), \
        _B_FIELD_Y(12), \
        _B_FIELD_WIDTH(7), \
        _B_FIELD_HEIGHT(7) \
  )

#define _B_ICON_RIGHT(iconid, userid) \
    _B_ICON( \
        _B_FIELD_ICONID(iconid), \
        _B_FIELD_USERID(userid), \
        _B_FIELD_X(117), \
        _B_FIELD_Y(13), \
        _B_FIELD_WIDTH(8), \
        _B_FIELD_HEIGHT(6) \
    )

#define _B_ICON_CROSS(userid) \
    _B_ICON_LEFT( \
        _B_FIELD_ICONID(BAGL_GLYPH_ICON_CROSS), \
        _B_FIELD_USERID(userid) \
    )

#define _B_ICON_CHECK(userid) \
    _B_ICON_RIGHT( \
        _B_FIELD_ICONID(BAGL_GLYPH_ICON_CHECK), \
        _B_FIELD_USERID(userid) \
    )

#define _B_LABELINE(userid, x, y, w, h, stroke, fontid, iconid, text) { \
    { \
        _B_FIELD_TYPE(BAGL_LABELINE), \
        _B_FIELD_USERID(userid), \
        _B_FIELD_X(x), \
        _B_FIELD_Y(y), \
        _B_FIELD_WIDTH(w), \
        _B_FIELD_HEIGHT(h), \
        _B_FIELD_STROKE(stroke), \
        _B_FIELD_RADIUS(0), \
        _B_FIELD_FILL(0), \
        _B_FIELD_FGCOLOR(0xFFFFFF), \
        _B_FIELD_BGCOLOR(0x000000), \
        _B_FIELD_FONTID(fontid), \
        _B_FIELD_ICONID(iconid) \
    }, \
    _B_FIELD_TEXT(text), \
    _B_FIELD_TOUCH_AREA_BRIM(0), \
    _B_FIELD_OVERFGCOLOR(0), \
    _B_FIELD_OVERBGCOLOR(0), \
    _B_FIELD_TAP(NULL), \
    _B_FIELD_OUT(NULL), \
    _B_FIELD_OVER(NULL) \
}

#define _B_LABELINE_REGULAR(userid, y, text) \
    _B_LABELINE( \
        _B_FIELD_USERID(userid), \
        _B_FIELD_X(0), \
        _B_FIELD_Y(y), \
        _B_FIELD_WIDTH(128), \
        _B_FIELD_HEIGHT(32), \
        _B_FIELD_STROKE(0), \
        _B_FIELD_FONTID(BAGL_FONT_OPEN_SANS_REGULAR_11px | BAGL_FONT_ALIGNMENT_CENTER), \
        _B_FIELD_ICONID(0), \
        _B_FIELD_TEXT(text) \
    )

#define _B_LABELINE_UPPER_REGULAR(userid, text) \
    _B_LABELINE_REGULAR( \
        _B_FIELD_USERID(userid), \
        _B_FIELD_Y(12), \
        _B_FIELD_TEXT(text) \
    )

#define _B_LABELINE_BOLD(userid, y, text) \
    _B_LABELINE( \
        _B_FIELD_USERID(userid), \
        _B_FIELD_X(0), \
        _B_FIELD_Y(y), \
        _B_FIELD_WIDTH(128), \
        _B_FIELD_HEIGHT(32), \
        _B_FIELD_STROKE(0), \
        _B_FIELD_FONTID(BAGL_FONT_OPEN_SANS_EXTRABOLD_11px | BAGL_FONT_ALIGNMENT_CENTER), \
        _B_FIELD_ICONID(0), \
        _B_FIELD_TEXT(text) \
  )

#define _B_LABELINE_UPPER_BOLD(userid, text) \
    _B_LABELINE_BOLD( \
        _B_FIELD_USERID(userid), \
        _B_FIELD_Y(12), \
        _B_FIELD_TEXT(text) \
  )

#define _B_LABELINE_LOWER_BOLD(userid, text) \
    _B_LABELINE_BOLD( \
        _B_FIELD_USERID(userid), \
        _B_FIELD_Y(26), \
        _B_FIELD_TEXT(text) \
  )
