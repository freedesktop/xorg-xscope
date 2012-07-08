/*
 * Copyright © 2000 Keith Packard, member of The XFree86 Project, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#include "scope.h"
#include "x11.h"
#include "bigreqscope.h"
#include "extensions.h"

static unsigned char BIGREQRequest;

static void
bigreq_decode_req(FD fd, const unsigned char *buf)
{
    short Major = IByte(&buf[0]);
    short Minor = IByte(&buf[1]);

    switch (Minor) {
    case 0:
        CS[fd].bigreqEnabled = 1;
        BigreqEnable(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    default:
        break;
    }
}

static void
bigreq_decode_reply(FD fd, const unsigned char *buf, short RequestMinor)
{
    switch (RequestMinor) {
    case 0:
        BigreqEnableReply(fd, buf);
        break;
    }
}

void
InitializeBIGREQ(const unsigned char *buf)
{
    TYPE p;

    BIGREQRequest = (unsigned char) (buf[9]);

    DefineEValue(&TD[REQUEST], (unsigned long) BIGREQRequest, "BigreqRequest");
    DefineEValue(&TD[REPLY], (unsigned long) BIGREQRequest, "BigreqReply");

    p = DefineType(BIGREQREQUEST, ENUMERATED, "BIGREQREQUEST",
                   (PrintProcType) PrintENUMERATED);
    DefineEValue(p, 0L, "BigreqEnable");

    p = DefineType(BIGREQREPLY, ENUMERATED, "BIGREQREPLY",
                   (PrintProcType) PrintENUMERATED);
    DefineEValue(p, 0L, "BigreqEnable");

    InitializeExtensionDecoder(BIGREQRequest, bigreq_decode_req,
                               bigreq_decode_reply);
}
