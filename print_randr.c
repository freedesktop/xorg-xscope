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
#include "randrscope.h"

void
RandrQueryVersion (FD fd, const unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RandrRequest */ ;
  PrintField (buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER) /* RandrSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printreqlen(buf, fd, CONST2(2));
}

void
RandrQueryVersionReply (FD fd, const unsigned char *buf)
{
  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* RandrRequest reply */ ;
  PrintField(RBf, 1, 1, RANDRREPLY, RANDRREPLYHEADER) /* RandrQueryVersion reply */;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
  PrintField(buf, 8, 2, CARD16, "major-version");
  PrintField(buf, 10, 2, CARD16, "minor-version");
}

void
RandrGetScreenInfo (FD fd, const unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RandrRequest */ ;
  PrintField (buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER) /* RandrSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printreqlen(buf, fd, CONST2(2));
  PrintField(buf, 4, 4, WINDOW, "window");
}

void
RandrGetScreenInfoReply (FD fd, const unsigned char *buf)
{
  unsigned short  nsize;
  unsigned short  nvg;
  unsigned short  ngvg;

  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* RandrRequest reply */ ;
  if (Verbose < 1)
    return;
  PrintField(buf, 1, 1, CARD8, "set-of-rotations");
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
  PrintField(buf, 8, 4, WINDOW, "root");
  PrintField(buf,12, 4, TIMESTAMP, "timestamp");
  PrintField(buf,16, 4, TIMESTAMP, "config-timestamp");
  PrintField(buf,20, 2, CARD16, "num-visual-groups");
  nvg = IShort (&buf[20]);
  PrintField(buf,22, 2, CARD16, "num-groups-of-visual-groups");
  ngvg = IShort (&buf[22]);
  PrintField(buf,24, 2, CARD16, "num-sizes");
  nsize = IShort (&buf[24]);
  PrintField(buf,26, 2, CARD16, "size-id");
  PrintField(buf,28, 2, CARD16, "visual-group-id");
  PrintField(buf,30, 2, CARD16, "rotation");
}

void
RandrSetScreenConfig (FD fd, const unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RandrRequest */ ;
  PrintField (buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER) /* RandrSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printreqlen(buf, fd, CONST2(2));
  PrintField(buf, 4, 4, DRAWABLE, "drawable");
  PrintField(buf, 8, 4, TIMESTAMP, "timestamp");
  PrintField(buf,12, 4, TIMESTAMP, "config-timestamp");
  PrintField(buf,16, 2, CARD16, "size-id");
  PrintField(buf,18, 2, CARD16, "rotation");
  PrintField(buf,20, 2, CARD16, "visual-group-id");
}

void
RandrSetScreenConfigReply (FD fd, const unsigned char *buf)
{
  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* RandrRequest reply */ ;
  PrintField(buf, 1, 1, BOOL, "success") /* RandrQueryVersion reply */;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
  PrintField(buf, 8, 4, TIMESTAMP, "new-timestamp");
  PrintField(buf,12, 4, TIMESTAMP, "new-config-timestamp");
  PrintField(buf,16, 4, WINDOW, "root");
}

void
RandrScreenChangeSelectInput (FD fd, const unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RandrRequest */ ;
  PrintField (buf, 1, 1, RANDRREQUEST, RANDRREQUESTHEADER) /* RandrSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printreqlen(buf, fd, CONST2(2));
  PrintField(buf, 4, 4, WINDOW, "window");
  PrintField(buf, 8, 1, BOOL, "enable");
}

void
RandrScreenSizes (const unsigned char *buf)
{
  PrintField(buf, 0, 2, CARD16, "width-in-pixels");
  PrintField(buf, 2, 2, CARD16, "height-in-pixels");
  PrintField(buf, 4, 2, CARD16, "width-in-millimeters");
  PrintField(buf, 6, 2, CARD16, "height-in-millimeters");
  PrintField(buf, 8, 2, CARD16, "visual-group");
}

void
RandrScreenChangeNotifyEvent (const unsigned char *buf)
{
  PrintField(buf, 0, 1, EVENT, EVENTHEADER) /* RRScreenChangeNotify */ ;
  if (Verbose < 1)
    return;
  PrintField(buf, 1, 1, BOOL, "resident");
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, TIMESTAMP, "timestamp");
  PrintField(buf, 8, 4, TIMESTAMP, "config-timestamp");
  PrintField(buf,12, 4, WINDOW, "root");
  PrintField(buf,16, 2, CARD16, "size id");
  PrintField(buf,18, 2, CARD16, "rotation");
  RandrScreenSizes (buf + 20);
}
