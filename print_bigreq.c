/*
 * Copyright © 2001 Keith Packard, member of The XFree86 Project, Inc.
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


#include "scope.h"
#include "x11.h"
#include "bigreqscope.h"

void
BigreqEnable(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* BigreqRequest */
    PrintField(buf, 1, 1, BIGREQREQUEST,
               BIGREQREQUESTHEADER); /* BigreqSwitch */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
}

void
BigreqEnableReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* BigreqRequest reply */
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 4, CARD32, "max-request-size");
}
