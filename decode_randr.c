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


#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#define _RANDR_SERVER_
#include "scope.h"
#include "x11.h"
#include "randrscope.h"
#include "extensions.h"

static unsigned char RANDRRequest, RANDRError, RANDREvent;

static void
randr_decode_req (
    FD fd,
    const unsigned char *buf)
{
  short Major = IByte (&buf[0]);
  short Minor = IByte (&buf[1]);

  switch (Minor) {
  case 0: RandrQueryVersion (fd, buf); ExtendedReplyExpected (fd, Major, Minor); break;
  case 1: RandrGetScreenInfo (fd, buf); ExtendedReplyExpected (fd, Major, Minor); break;
  case 2: RandrSetScreenConfig (fd, buf); ExtendedReplyExpected (fd, Major, Minor); break;
  case 3: RandrScreenChangeSelectInput (fd, buf);
  default:
    ExtendedRequest(fd, buf);
    ExtendedReplyExpected(fd, Major, Minor);
    break;
  }
}

static void
randr_decode_reply (
    FD fd,
    const unsigned char *buf,
    short RequestMinor)
{
    switch (RequestMinor) {
    case 0: RandrQueryVersionReply (fd, buf); break;
    case 1: RandrGetScreenInfoReply (fd, buf); break;
    case 2: RandrSetScreenConfigReply (fd, buf); break;
    default: UnknownReply(buf); break;
    }
}

static void
randr_decode_event (
    FD fd,
    const unsigned char *buf)
{
  RandrScreenChangeNotifyEvent (buf);
}

void
InitializeRANDR (
    const unsigned char *buf)
{
  TYPE    p;

  RANDRRequest = (unsigned char)(buf[9]);
  RANDREvent = (unsigned char)(buf[10]);
  RANDRError = (unsigned char)(buf[11]);

  DefineEValue (&TD[REQUEST], (unsigned long) RANDRRequest, "RandrRequest");
  DefineEValue (&TD[REPLY], (unsigned long) RANDRRequest, "RandrReply");
  DefineEValue (&TD[EVENT], (unsigned long) RANDREvent, "RRScreenChangeNotify");

  p = DefineType(RANDRREQUEST, ENUMERATED, "RANDRREQUEST", (PrintProcType) PrintENUMERATED);
  DefineEValue(p, 0L, "RandrQueryVersion");
  DefineEValue(p, 1L, "RandrGetScreenInfo");
  DefineEValue(p, 2L, "RandrSetScreenConfig");
  DefineEValue(p, 3L, "RandrScreenChangeSelectInput");

  p = DefineType(RANDRREPLY, ENUMERATED, "RANDRREPLY", (PrintProcType) PrintENUMERATED);
  DefineEValue (p, 0L, "QueryVersion");
  DefineEValue (p, 1L, "GetScreenInfo");
  DefineEValue (p, 2L, "SetScreenConfig");

  InitializeExtensionDecoder(RANDRRequest, randr_decode_req,
			     randr_decode_reply);
  /* Not yet implemented:
     InitializeExtensionErrorDecoder(RANDRError, randr_decode_error); */
  InitializeExtensionEventDecoder(RANDREvent, randr_decode_event);

}
