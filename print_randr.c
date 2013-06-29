/*
 * Copyright © 2001 Keith Packard, member of The XFree86 Project, Inc.
 * Copyright (c) 2012, Oracle and/or its affiliates. All rights reserved.
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
#include "randrscope.h"

void
RandrQueryVersion(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSwitch */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, CARD32, "major-version");
    PrintField(buf, 8, 4, CARD32, "minor-version");

    /*
      Todo, maybe: record major-version for distinguishing which version of
      original set of requests/replies client is using, version 0.x or 1.x.
      Alternatively: purge all support for version 0.x.
    */
}

void
RandrQueryVersionReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY,
               RANDRREPLYHEADER); /* RandrQueryVersion reply */
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 4, CARD32, "major-version");
    PrintField(buf, 12, 4, CARD32, "minor-version");
}

/* RandrOld* interfaces are for RandR 0.x only - the protocol changed
   incompatibily in RandR 1.0, and the RandR 1.0 versions come later. */
void
RandrOldGetScreenInfo(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSwitch */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
}

void
RandrOldGetScreenInfoReply(FD fd, const unsigned char *buf)
{
    unsigned short nsize;
    unsigned short nvg;
    unsigned short ngvg;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    PrintField(buf, 1, 1, CARD8, "set-of-rotations");
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 4, WINDOW, "root");
    PrintField(buf, 12, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 16, 4, TIMESTAMP, "config-timestamp");
    PrintField(buf, 20, 2, CARD16, "num-visual-groups");
    nvg = IShort(&buf[20]);
    PrintField(buf, 22, 2, CARD16, "num-groups-of-visual-groups");
    ngvg = IShort(&buf[22]);
    PrintField(buf, 24, 2, CARD16, "num-sizes");
    nsize = IShort(&buf[24]);
    PrintField(buf, 26, 2, CARD16, "size-id");
    PrintField(buf, 28, 2, CARD16, "visual-group-id");
    PrintField(buf, 30, 2, CARD16, "rotation");
}

void
RandrOldSetScreenConfig(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSwitch */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 4, TIMESTAMP, "config-timestamp");
    PrintField(buf, 16, 2, CARD16, "size-id");
    PrintField(buf, 18, 2, CARD16, "rotation");
    PrintField(buf, 20, 2, CARD16, "visual-group-id");
}

void
RandrOldSetScreenConfigReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, BOOL, "success"); /* RandrQueryVersion reply */
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "new-timestamp");
    PrintField(buf, 12, 4, TIMESTAMP, "new-config-timestamp");
    PrintField(buf, 16, 4, WINDOW, "root");
}

void
RandrOldScreenChangeSelectInput(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSwitch */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 1, BOOL, "enable");
}

static void
RandrOldScreenSizes(const unsigned char *buf)
{
    PrintField(buf, 0, 2, CARD16, "width-in-pixels");
    PrintField(buf, 2, 2, CARD16, "height-in-pixels");
    PrintField(buf, 4, 2, CARD16, "width-in-millimeters");
    PrintField(buf, 6, 2, CARD16, "height-in-millimeters");
    PrintField(buf, 8, 2, CARD16, "visual-group");
}

void
RandrOldScreenChangeNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RROldScreenChangeNotify */
    if (Verbose < 1)
        return;
    PrintField(buf, 1, 1, BOOL, "resident");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 8, 4, TIMESTAMP, "config-timestamp");
    PrintField(buf, 12, 4, WINDOW, "root");
    PrintField(buf, 16, 2, CARD16, "size id");
    PrintField(buf, 18, 2, CARD16, "rotation");
    RandrOldScreenSizes(buf + 20);
}

/* RandR 1.0 starts here */
int
PrintSCREENSIZE(const unsigned char *buf)
{
    PrintField(buf, 0, 2, CARD16, "width-in-pixels");
    PrintField(buf, 2, 2, CARD16, "height-in-pixels");
    PrintField(buf, 4, 2, CARD16, "width-in-millimeters");
    PrintField(buf, 6, 2, CARD16, "height-in-millimeters");
    return 8;
}

void
RandrSetScreenConfig(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetScreenConfig */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(6));
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 4, TIMESTAMP, "config-timestamp");
    PrintField(buf, 16, 2, CARD16, "size-id");
    PrintField(buf, 18, 2, CARD16, "rotation");
    PrintField(buf, 20, 2, CARD16, "rate");
}


void
RandrSetScreenConfigReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "new-timestamp");
    PrintField(buf, 12, 4, TIMESTAMP, "new-config-timestamp");
    PrintField(buf, 16, 4, WINDOW, "root");
    PrintField(buf, 20, 2, SUBPIXEL, "subpixel-order");
}


void
RandrSelectInput(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSelectInput */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 2, SETofRREVENT, "select-mask");
}


void
RandrGetScreenInfo(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetScreenInfo */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
}

void
RandrGetScreenInfoReply(FD fd, const unsigned char *buf)
{
    uint16_t s, r;
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, SETofROTATION, "rotations");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 4, WINDOW, "root");
    PrintField(buf, 12, 4, TIMESTAMP, "new-timestamp");
    PrintField(buf, 16, 4, TIMESTAMP, "new-config-timestamp");
    PrintField(buf, 20, 2, CARD16, "num-sizes");
    s = IShort(&buf[20]);
    PrintField(buf, 22, 2, CARD16, "size-id");
    PrintField(buf, 24, 2, SETofROTATION, "rotation");
    PrintField(buf, 26, 2, CARD16, "rate");
    printfield(buf, 28, 2, CARD16, "rates-length");

    buf += 32; /* screen sizes start 32 bytes from head of reply */
    PrintList(buf, s, SCREENSIZE, "sizes");
    buf += s * 8;

    r = IShort(buf);
    PrintField(buf, 0, 2, CARD16, "num-rates");
    buf += 2;
    PrintList(buf, r, CARD16, "refresh-rates");
}

void
RandrScreenChangeNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RRScreenChangeNotify */
    if (Verbose < 1)
        return;
    PrintField(buf, 1, 1, CARD8, "rotation");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 8, 4, TIMESTAMP, "config-timestamp");
    PrintField(buf, 12, 4, WINDOW, "root");
    PrintField(buf, 16, 4, WINDOW, "window");
    PrintField(buf, 20, 2, CARD16, "size id");
    PrintField(buf, 22, 2, SUBPIXEL, "subpixel-order");
    PrintSCREENSIZE(buf + 24);
}

/* Randr 1.2 additions */
int
PrintMODEINFO(const unsigned char *buf)
{
    PrintField(buf, 0, 4, CARD32, "id");
    PrintField(buf, 4, 2, CARD16, "width-in-pixels");
    PrintField(buf, 6, 2, CARD16, "height-in-pixels");
    PrintField(buf, 8, 4, CARD32, "dot clock");
    PrintField(buf, 12, 2, CARD16, "h sync start");
    PrintField(buf, 14, 2, CARD16, "h sync end");
    PrintField(buf, 16, 2, CARD16, "h total");
    PrintField(buf, 18, 2, CARD16, "h skew");
    PrintField(buf, 20, 2, CARD16, "v sync start");
    PrintField(buf, 22, 2, CARD16, "v sync end");
    PrintField(buf, 24, 2, CARD16, "v total");
    PrintField(buf, 26, 2, CARD16, "name length");
    PrintField(buf, 28, 4, SETofMODEFLAG, "mode flags");
    return 32;
}

int PrintMODE(const unsigned char *buf)
{
    /* print a MODE -- CARD32  plus 0 = None */
    long n = ILong(buf);

    if (n == 0)
        fprintf(stdout, "None");
    else
        fprintf(stdout, "MODE %08lx", n);
    return (4);
}

int
PrintCRTC(const unsigned char *buf)
{
    /* print a CRTC -- CARD32  plus 0 = None */
    long n = ILong(buf);

    if (n == 0)
        fprintf(stdout, "None");
    else
        fprintf(stdout, "CRTC %08lx", n);
    return (4);
}

int
PrintOUTPUT(const unsigned char *buf)
{
    /* print an OUTPUT -- CARD32  plus 0 = None */
    long n = ILong(buf);

    if (n == 0)
        fprintf(stdout, "None");
    else
        fprintf(stdout, "OUTP %08lx", n);
    return (4);
}


void
RandrGetScreenSizeRange(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetScreenSizeRange */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
}


void
RandrGetScreenSizeRangeReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 2, CARD16, "min-width");
    PrintField(buf, 10, 2, CARD16, "min-height");
    PrintField(buf, 12, 2, CARD16, "max-width");
    PrintField(buf, 24, 2, CARD16, "max-height");
}


void
RandrSetScreenSize(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetScreenSize */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(5));
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintSCREENSIZE(buf + 8);
}


void
RandrGetScreenResources(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetScreenResources */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
}


void
RandrGetScreenResourcesReply(FD fd, const unsigned char *buf)
{
    uint16_t c, o, m, b;
    const unsigned char *nl, *end;
    int i;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(c+o+8m+(b+p)/4), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 4, TIMESTAMP, "config-timestamp");
    printfield(buf, 16, 2, CARD16, "num-crtcs");	/* c */
    printfield(buf, 18, 2, CARD16, "num-outputs");	/* o */
    printfield(buf, 20, 2, CARD16, "num-modes");	/* m */
    printfield(buf, 22, 2, CARD16, "num-bytes-names");	/* b */
    /* 8 bytes unused */
    c = IShort(&buf[16]);
    o = IShort(&buf[18]);
    m = IShort(&buf[20]);
    b = IShort(&buf[22]);

    buf += 32;
    PrintList(buf, c, CRTC, "crtcs");
    buf += c * 4;
    PrintList(buf, o, OUTPUT, "outputs");
    buf += o * 4;
    nl = buf + 26; /* offset of name length field in first modeinfo */
    PrintList(buf, m, MODEINFO, "modeinfos");
    buf += m * 32;
    end = buf + b;
    for (i = 0 ; i < m; i++, nl += 32) {
        uint16_t len = IShort(nl);
        if ((buf + len) > end) {
            fprintf(stdout, "**INVALID NAME LENGTH**");
            break;
        }
        buf += PrintString8(buf, len, "mode name");
    }
}


void
RandrGetOutputInfo(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetOutputInfo */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, TIMESTAMP, "config-timestamp");
}


void
RandrGetOutputInfoReply(FD fd, const unsigned char *buf)
{
    uint16_t c, m, p, o, n;
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(1+c+m+(n+p)/4), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 4, CRTC, "crtc");
    PrintField(buf, 16, 4, CARD32, "width-in-millimeters");
    PrintField(buf, 20, 4, CARD32, "height-in-millimeters");
    PrintField(buf, 24, 1, CARD8, "connection");
    PrintField(buf, 25, 1, SUBPIXEL, "subpixel-order");
    printfield(buf, 26, 2, CARD16, "num-crtcs");
    printfield(buf, 28, 2, CARD16, "num-modes");
    printfield(buf, 30, 2, CARD16, "num-preferred");
    printfield(buf, 32, 2, CARD16, "num-clones");
    printfield(buf, 34, 2, CARD16, "name-length");
    c = IShort(&buf[26]);
    m = IShort(&buf[28]);
    p = IShort(&buf[30]);
    o = IShort(&buf[32]);
    n = IShort(&buf[34]);

    buf += 36;
    PrintList(buf, c, CRTC, "crtcs");
    buf += c * 4;
    PrintList(buf, p, MODE, "preferred-modes");
    buf += p * 4;
    PrintList(buf, m - p, MODE, "other-modes");
    buf += (m - p) * 4;
    PrintList(buf, o, OUTPUT, "clones");
    buf += o * 4;
    PrintString8(buf, n, "name");
}


void
RandrListOutputProperties(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrListOutputProperties */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, OUTPUT, "output");
}


void
RandrListOutputPropertiesReply(FD fd, const unsigned char *buf)
{
    uint16_t n;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    printfield(buf, 8, 2, DVALUE2(n), "number of atoms");
    n = IShort(&buf[8]);
    PrintList(&buf[32], (long) n, ATOM, "atoms");
}


void
RandrQueryOutputProperty(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrQueryOutputProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, ATOM, "property");
}


void
RandrQueryOutputPropertyReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(n), "reply length");
    PrintField(buf, 8, 1, BOOL, "pending");
    printfield(buf, 9, 1, BOOL, "range");
    PrintField(buf, 10, 1, BOOL, "immutable");

    if (IBool(&buf[9]) && (Verbose <= 1)) {     /* range of min-max values */
        PrintField(buf, 32, 4, INT32, "minimum value");
        PrintField(buf, 36, 4, INT32, "maximum value");
    } else {                                    /* list of values */
        uint32_t n = ILong(&buf[4]);
        PrintList(&buf[32], n, INT32, "accepted values");
    }
}


void
RandrConfigureOutputProperty(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrConfigureOutputProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, DVALUE2(4 + n));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, ATOM, "property");
    PrintField(buf, 12, 1, BOOL, "pending");
    printfield(buf, 13, 1, BOOL, "range");

    if (IBool(&buf[12]) && (Verbose <= 1)) {     /* range of min-max values */
        PrintField(buf, 16, 4, INT32, "minimum value");
        PrintField(buf, 20, 4, INT32, "maximum value");
    } else {                                    /* list of values */
        uint32_t n = ILong(&buf[4]) - 4;
        PrintList(&buf[16], n, INT32, "accepted values");
    }
}


void
RandrChangeOutputProperty(FD fd, const unsigned char *buf)
{
    uint32_t n;
    short unit;
    long type;

    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrChangeOutputProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, DVALUE2(6 + (n + p) / 4));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, ATOM, "property");
    PrintField(buf, 12, 4, ATOM, "type");
    PrintField(buf, 16, 1, CARD8, "format");
    PrintField(buf, 17, 1, CHANGEMODE, "mode");
    /* 2 bytes unused */
    printfield(buf, 20, 4, CARD32, "length of data");

    type = ILong(&buf[12]);
    unit = IByte(&buf[16]) / 8;
    n = ILong(&buf[20]);
    PrintPropertyValues(&buf[24], type, unit, n, "data");
}


void
RandrDeleteOutputProperty(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrDeleteOutputProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, ATOM, "property");
}


void
RandrGetOutputProperty(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetOutputProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(7));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, ATOM, "property");
    PrintField(buf, 12, 4, ATOMT, "type");
    PrintField(buf, 16, 4, CARD32, "long-offset");
    printfield(buf, 20, 4, CARD32, "long-length");
    PrintField(buf, 24, 1, BOOL, "delete");
    PrintField(buf, 25, 1, BOOL, "pending");
}


void
RandrGetOutputPropertyReply(FD fd, const unsigned char *buf)
{
    uint32_t n;
    short unit;
    long type;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    PrintField(buf, 1, 1, CARD8, "format");
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
    PrintField(buf, 8, 4, ATOM, "type");
    PrintField(buf, 12, 4, CARD32, "bytes-after");
    printfield(buf, 16, 4, CARD32, "length of value");

    unit = IByte(&buf[1]) / 8;
    type = ILong(&buf[8]);
    n = ILong(&buf[16]);
    PrintPropertyValues(&buf[32], type, unit, n, "value");
}


void
RandrCreateMode(FD fd, const unsigned char *buf)
{
    uint16_t n;
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrCreateMode */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, DVALUE2(12 + (n + p) / 4));
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 32, MODEINFO, "mode");
    n = IShort(&buf[34]); /* name length field of MODEINFO */
    PrintString8(&buf[40], n, "name");
}


void
RandrCreateModeReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    PrintField(buf, 8, 4, MODE, "mode");
}


void
RandrDestroyMode(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrDestroyMode */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, MODE, "mode");
}


void
RandrAddOutputMode(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrAddOutputMode */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, MODE, "mode");
}


void
RandrDeleteOutputMode(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrDeleteOutputMode */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, OUTPUT, "output");
    PrintField(buf, 8, 4, MODE, "mode");
}


void
RandrGetCrtcInfo(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetCrtcInfo */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, CRTC, "crtc");
    PrintField(buf, 8, 4, TIMESTAMP, "config-timestamp");
}


void
RandrGetCrtcInfoReply(FD fd, const unsigned char *buf)
{
    uint16_t o, p;
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(o+p), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 2, INT16, "x");
    PrintField(buf, 14, 2, INT16, "y");
    PrintField(buf, 16, 2, CARD16, "width");
    PrintField(buf, 18, 2, CARD16, "height");
    PrintField(buf, 20, 4, MODE, "mode");
    PrintField(buf, 24, 2, SETofROTATION, "current rotation and reflection");
    PrintField(buf, 26, 2, SETofROTATION, "set of possible rotations");
    PrintField(buf, 28, 2, CARD16, "number of outputs");
    PrintField(buf, 30, 2, CARD16, "number of possible outputs");

    o = IShort(&buf[28]);
    p = IShort(&buf[30]);
    PrintList(&buf[32], (long) o, OUTPUT, "outputs");
    PrintList(&buf[32 + (4 * o)], (long) p, OUTPUT, "possible outputs");
}


void
RandrSetCrtcConfig(FD fd, const unsigned char *buf)
{
    uint16_t n;
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetCrtcConfig */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, DVALUE2(7 + 2n));
    PrintField(buf, 4, 4, CRTC, "crtc");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 4, TIMESTAMP, "config timestamp");
    PrintField(buf, 16, 2, INT16, "x");
    PrintField(buf, 18, 2, INT16, "y");
    PrintField(buf, 20, 4, MODE, "mode");
    PrintField(buf, 24, 2, SETofROTATION, "rotation/reflection");
    /* 26 = 2 bytes padding */
    n = IShort(&buf[2]) - 7;
    PrintList(&buf[28], (long) n, OUTPUT, "outputs");
}


void
RandrSetCrtcConfigReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, CONST4(0), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
}


void
RandrGetCrtcGammaSize(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetCrtcGammaSize */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, CRTC, "crtc");
}


void
RandrGetCrtcGammaSizeReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, CONST4(0), "reply length");
    PrintField(buf, 8, 2, CARD16, "size");
}


void
RandrGetCrtcGamma(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetCrtcGamma */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, CRTC, "crtc");
}


void
RandrGetCrtcGammaReply(FD fd, const unsigned char *buf)
{
    uint16_t n;
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(((6 * n) + p)/4), "reply length");
    PrintField(buf, 8, 2, CARD16, "size");
    n = IShort(&buf[8]);
    PrintList(&buf[12], (long) n, CARD16, "red");
    PrintList(&buf[12 + (2 * n)], (long) n, CARD16, "green");
    PrintList(&buf[12 + (4 * n)], (long) n, CARD16, "blue");
}


void
RandrSetCrtcGamma(FD fd, const unsigned char *buf)
{
    uint16_t n;
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetCrtcGamma */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, DVALUE2(3+((6 * n) + p)/4));
    PrintField(buf, 4, 4, CRTC, "crtc");
    PrintField(buf, 8, 2, CARD16, "size");
    n = IShort(&buf[8]);
    /* 10 = 2 bytes padding */
    PrintList(&buf[12], (long) n, CARD16, "red");
    PrintList(&buf[12 + (2 * n)], (long) n, CARD16, "green");
    PrintList(&buf[12 + (4 * n)], (long) n, CARD16, "blue");
}

void
RandrCrtcChangeNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RRNotify */
    PrintField(buf, 1, 1, RANDREVENT, "minor code");  /* RRCrtcChangeNotify */
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 8, 4, WINDOW, "window");
    PrintField(buf, 12, 4, CRTC, "crtc");
    PrintField(buf, 16, 4, MODE, "mode");
    PrintField(buf, 20, 2, SETofROTATION, "rotation");
    /* 22 = 2 bytes padding */
    PrintField(buf, 24, 2, INT16, "x");
    PrintField(buf, 26, 2, INT16, "y");
    PrintField(buf, 28, 2, CARD16, "width");
    PrintField(buf, 30, 2, CARD16, "height");
}

void
RandrOutputChangeNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RRNotify */
    PrintField(buf, 1, 1, RANDREVENT, "minor code");  /* RROutputChangeNotify */
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 8, 4, TIMESTAMP, "config-timestamp");
    PrintField(buf, 12, 4, WINDOW, "window");
    PrintField(buf, 16, 4, OUTPUT, "output");
    PrintField(buf, 20, 4, CRTC, "crtc");
    PrintField(buf, 24, 4, MODE, "mode");
    PrintField(buf, 28, 2, SETofROTATION, "rotation");
    PrintField(buf, 30, 1, CONNECTION, "connection");
    PrintField(buf, 31, 1, SUBPIXEL, "subpixel-order");

}

void
RandrOutputPropertyNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RRNotify */
    PrintField(buf, 1, 1, RANDREVENT, "minor code");  /* RROutputPropertyNotify */
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 4, OUTPUT, "output");
    PrintField(buf, 12, 4, ATOM, "atom");
    PrintField(buf, 16, 4, TIMESTAMP, "time");
    PrintField(buf, 20, 1, PROPCHANGE, "state");
    /* 11 bytes unused */
}


void
RandrOutputError(FD fd, const unsigned char *buf)
{
    printErrorWithValue(buf, OUTPUT, "bad output");
}

void
RandrCrtcError(FD fd, const unsigned char *buf)
{
    printErrorWithValue(buf, CRTC, "bad crtc");
}

void
RandrModeError(FD fd, const unsigned char *buf)
{
    printErrorWithValue(buf, MODE, "bad mode");
}



/* Randr 1.3 additions */
void
RandrGetScreenResourcesCurrent(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetScreenResourcesCurrent */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
}


void
RandrGetScreenResourcesCurrentReply(FD fd, const unsigned char *buf)
{
    /* Reply format is exact same as RandrGetScreenResources */
    RandrGetScreenResourcesReply(fd, buf);
}


void
RandrSetCrtcTransform(FD fd, const unsigned char *buf)
{
    uint16_t n;
    uint32_t reqlen, p, v;
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetCrtcTransform */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    reqlen = getreqlen(fd, buf);
    printreqlen(buf, fd, DVALUE2(12+(n+p)/4+v));
    PrintField(buf, 4, 4, CRTC, "crtc");
    PrintField(buf, 8, 36, RENDERTRANSFORM, "transform");
    printfield(buf, 44, 2, CARD16, "filter name length");
    /* 2 bytes unused */
    n = IShort(&buf[44]);
    PrintString8(&buf[48], n, "filter name");
    /* pad to p=pad(n) */
    p = pad(n);
    v = reqlen - (12 + (p/4));
    PrintList(&buf[48 + p], (long) v, FIXED, "filter params");
}


void
RandrGetCrtcTransform(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetCrtcTransform */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, CRTC, "crtc");
}


void
RandrGetCrtcTransformReply(FD fd, const unsigned char *buf)
{
    uint16_t pn, pf, cn, cf;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(16+(pn+pnp)/4+(cn+cnp)/4+pf+cf), "reply length");
    PrintField(buf, 8, 36, RENDERTRANSFORM, "pending transform");
    PrintField(buf, 44, 1, BOOL, "has transforms");
    /* 3 bytes unused */
    PrintField(buf, 48, 36, RENDERTRANSFORM, "current transform");
    /* 4 bytes unused */

    printfield(buf, 88, 2, CARD16, "pending filter name length"); /* pn */
    printfield(buf, 90, 2, CARD16, "pending filter num params");  /* pf */
    printfield(buf, 92, 2, CARD16, "current filter name length"); /* cn */
    printfield(buf, 94, 2, CARD16, "current filter num params");  /* cf */

    pn = IShort(&buf[88]);
    pf = IShort(&buf[90]);
    cn = IShort(&buf[92]);
    cf = IShort(&buf[94]);

    buf += 96;

    PrintString8(buf, pn, "pending filter name");
    buf += pad(pn);
    PrintList(buf, pf, FIXED, "pending filter params");
    buf += 4 * pf;

    PrintString8(buf, cn, "current filter name");
    buf += pad(cn);
    PrintList(buf, cf, FIXED, "current filter params");
}


void
RandrGetPanning(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetPanning */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, CRTC, "crtc");
}


void
RandrGetPanningReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(1), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 2, CARD16, "left");
    PrintField(buf, 14, 2, CARD16, "top");
    PrintField(buf, 16, 2, CARD16, "width");
    PrintField(buf, 18, 2, CARD16, "height");
    PrintField(buf, 20, 2, CARD16, "track_left");
    PrintField(buf, 22, 2, CARD16, "track_top");
    PrintField(buf, 24, 2, CARD16, "track_width");
    PrintField(buf, 26, 2, CARD16, "track_height");
    PrintField(buf, 28, 2, INT16, "border_left");
    PrintField(buf, 30, 2, INT16, "border_top");
    PrintField(buf, 32, 2, INT16, "border_right");
    PrintField(buf, 34, 2, INT16, "border_bottom");
}


void
RandrSetPanning(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetPanning */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(9));
    PrintField(buf, 4, 4, CRTC, "crtc");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 2, CARD16, "left");
    PrintField(buf, 14, 2, CARD16, "top");
    PrintField(buf, 16, 2, CARD16, "width");
    PrintField(buf, 18, 2, CARD16, "height");
    PrintField(buf, 20, 2, CARD16, "track_left");
    PrintField(buf, 22, 2, CARD16, "track_top");
    PrintField(buf, 24, 2, CARD16, "track_width");
    PrintField(buf, 26, 2, CARD16, "track_height");
    PrintField(buf, 28, 2, INT16, "border_left");
    PrintField(buf, 30, 2, INT16, "border_top");
    PrintField(buf, 32, 2, INT16, "border_right");
    PrintField(buf, 34, 2, INT16, "border_bottom");
}


void
RandrSetPanningReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, CONST4(0), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "new-timestamp");
    /* 20 bytes unused */
}


void
RandrSetOutputPrimary(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetOutputPrimary */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 4, OUTPUT, "output");
}


void
RandrGetOutputPrimary(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetOutputPrimary */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
}


void
RandrGetOutputPrimaryReply(FD fd, const unsigned char *buf)
{
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, CONST4(0), "reply length");
    PrintField(buf, 8, 4, OUTPUT, "output");
}



/* Randr 1.4 additions */
void
RandrGetProviders(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetProviders */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, WINDOW, "window");
}


void
RandrGetProvidersReply(FD fd, const unsigned char *buf)
{
    uint16_t p;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(p), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 2, CARD16, "number of providers");

    p = IShort(&buf[12]);
    PrintList(&buf[32], p, PROVIDER, "providers");
}


void
RandrGetProviderInfo(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetProviderInfo */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, TIMESTAMP, "config-timestamp");
}


void
RandrGetProviderInfoReply(FD fd, const unsigned char *buf)
{
    uint16_t c, o, a, n;
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    PrintField(buf, 1, 1, CARD8, "status");
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(1+c+o+(a*2)+(n+p)/4), "reply length");
    PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 12, 4, SETofPROVIDER_CAPS, "capabilities");
    PrintField(buf, 16, 2, CARD16, "number of crtcs");		/* c */
    PrintField(buf, 18, 2, CARD16, "number of outputs");	/* o */
    PrintField(buf, 20, 2, CARD16, "number of associated providers"); /* a */
    PrintField(buf, 22, 2, CARD16, "length of name");		/* n */
    /* 8 bytes unused */
    c = IShort(&buf[16]);
    o = IShort(&buf[18]);
    a = IShort(&buf[20]);
    n = IShort(&buf[22]);

    buf += 32;
    PrintList(buf, c, CRTC, "crtcs");
    buf += c * 4;
    PrintList(buf, o, OUTPUT, "outputs");
    buf += o * 4;
    PrintList(buf, a, PROVIDER, "associated providers");
    buf += a * 4;
    PrintList(buf, a, SETofPROVIDER_CAPS, "associated provider capability");
    buf += a * 4;
    PrintString8(buf, n, "name");
}


void
RandrSetProviderOffloadSink(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetProviderOffloadSink */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(4));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, PROVIDER, "offload sink provider");
    PrintField(buf, 12, 4, TIMESTAMP, "timestamp");
}


void
RandrSetProviderOutputSource(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrSetProviderOutputSource */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");
    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, PROVIDER, "output source provider");
    PrintField(buf, 12, 4, TIMESTAMP, "timestamp");
}


void
RandrListProviderProperties(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrListProviderProperties */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(2));
    PrintField(buf, 4, 4, PROVIDER, "provider");
}


void
RandrListProviderPropertiesReply(FD fd, const unsigned char *buf)
{
    uint16_t n;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(0), "reply length");
    printfield(buf, 8, 2, DVALUE2(n), "number of atoms");
    /* 22 bytes unused */
    n = IShort(&buf[8]);
    PrintList(&buf[32], (long) n, ATOM, "atoms");
}


void
RandrQueryProviderProperty(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrQueryProviderProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, ATOM, "property");
}


void
RandrQueryProviderPropertyReply(FD fd, const unsigned char *buf)
{
    uint32_t n;
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(n), "reply length");
    PrintField(buf, 8, 1, BOOL, "pending");
    PrintField(buf, 9, 1, BOOL, "range");
    PrintField(buf, 10, 1, BOOL, "immutable");
    /* 21 bytes unused */
    n = ILong(&buf[4]);
    PrintList(&buf[32], n, INT32, "valid values");
}


void
RandrConfigureProviderProperty(FD fd, const unsigned char *buf)
{
    uint32_t reqlen, n;
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrConfigureProviderProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    reqlen = getreqlen(fd, buf);
    printreqlen(buf, fd, DVALUE2(4+n));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, ATOM, "property");
    PrintField(buf, 12, 1, BOOL, "pending");
    PrintField(buf, 13, 1, BOOL, "range");
    /* 2 bytes unused */
    n = reqlen - 4;
    PrintList(&buf[16], n, INT32, "valid-values");
}


void
RandrChangeProviderProperty(FD fd, const unsigned char *buf)
{
    uint32_t n;
    short unit;
    long type;

    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrChangeProviderProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, DVALUE2(6 + (n + p) / 4));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, ATOM, "property");
    PrintField(buf, 12, 4, ATOM, "type");
    PrintField(buf, 16, 1, CARD8, "format");
    PrintField(buf, 17, 1, CHANGEMODE, "mode");
    /* 2 bytes unused */
    printfield(buf, 20, 4, CARD32, "length of data");

    type = ILong(&buf[12]);
    unit = IByte(&buf[16]) / 8;
    n = ILong(&buf[20]);
    PrintPropertyValues(&buf[24], type, unit, n, "data");
}


void
RandrDeleteProviderProperty(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrDeleteProviderProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(3));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, ATOM, "property");
}


void
RandrGetProviderProperty(FD fd, const unsigned char *buf)
{
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER); /* RandrRequest */
    PrintField(buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER); /* RandrGetProviderProperty */
    if (Verbose < 1)
        return;
    if (Verbose > 1)
        PrintField(SBf, 0, 4, CARD32, "sequence number");

    printreqlen(buf, fd, CONST2(7));
    PrintField(buf, 4, 4, PROVIDER, "provider");
    PrintField(buf, 8, 4, ATOM, "property");
    PrintField(buf, 12, 4, ATOMT, "type");
    PrintField(buf, 16, 4, CARD32, "long-offset");
    printfield(buf, 20, 4, CARD32, "long-length");
    PrintField(buf, 24, 1, BOOL, "delete");
    PrintField(buf, 25, 1, BOOL, "pending");
}


void
RandrGetProviderPropertyReply(FD fd, const unsigned char *buf)
{
    uint32_t n;
    short unit;
    long type;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER); /* RandrRequest reply */
    PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER);
    if (Verbose < 1)
        return;
    PrintField(buf, 1, 1, CARD8, "format");
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
    PrintField(buf, 8, 4, ATOM, "type");
    PrintField(buf, 12, 4, CARD32, "bytes-after");
    printfield(buf, 16, 4, CARD32, "length of value");

    unit = IByte(&buf[1]) / 8;
    type = ILong(&buf[8]);
    n = ILong(&buf[16]);
    PrintPropertyValues(&buf[32], type, unit, n, "value");
}


void
RandrProviderChangeNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RRNotify */
    PrintField(buf, 1, 1, RANDREVENT, "minor code");  /* RRProviderChangeNotify */
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 8, 4, WINDOW, "window");
    PrintField(buf, 12, 4, PROVIDER, "provider");
    /* 16 bytes unused */
}


void
RandrProviderPropertyNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RRNotify */
    PrintField(buf, 1, 1, RANDREVENT, "minor code");  /* RRProviderPropertyNotify */
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 4, PROVIDER, "provider");
    PrintField(buf, 12, 4, ATOM, "atom");
    PrintField(buf, 16, 4, TIMESTAMP, "time");
    PrintField(buf, 20, 1, PROPCHANGE, "state");
    /* 11 bytes unused */
}


void
RandrResourceChangeNotifyEvent(const unsigned char *buf)
{
    PrintField(buf, 0, 1, EVENT, EVENTHEADER); /* RRNotify */
    PrintField(buf, 1, 1, RANDREVENT, "minor code");  /* RRResourceChangeNotify */
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "timestamp");
    PrintField(buf, 8, 4, WINDOW, "window");
    /* 20 bytes unused */
}


void
RandrProviderError(FD fd, const unsigned char *buf)
{
    printErrorWithValue(buf, PROVIDER, "bad provider");
}
