/*
 * $XFree86$
 *
 * Copyright © 2000 Keith Packard, member of The XFree86 Project, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */


#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#define _RENDER_SERVER_
#include "scope.h"
#include "x11.h"
#include "renderscope.h"

unsigned char LookForRENDERFlag;

unsigned char RENDERRequest, RENDERError;

render_decode_req(fd, buf)
FD fd;
unsigned char *buf;
{
  short Major = IByte (&buf[0]);
  short Minor = IByte (&buf[1]);

  switch (Minor) {
  case 0: RenderQueryVersion (fd, buf); ExtendedReplyExpected (fd, Major, Minor); break;
  case 1: RenderQueryPictFormats (fd, buf); ExtendedReplyExpected (fd, Major, Minor); break;
  case 2: RenderQueryPictIndexValues (fd, buf); ExtendedReplyExpected (fd, Major, Minor); break;
  case 3: RenderQueryDithers (fd, buf); ExtendedReplyExpected (fd, Major, Minor); break;
  case 4: RenderCreatePicture (fd, buf); break;
  case 5: RenderChangePicture (fd, buf); break;
  case 6: RenderSetPictureClipRectangles (fd, buf); break;
  case 7: RenderFreePicture (fd, buf); break;
  case 8: RenderComposite (fd, buf); break;
  case 9: RenderScale (fd, buf); break;
  case 10: RenderTrapezoids (fd, buf); break;
  case 11: RenderTriangles (fd, buf); break;
  case 12: RenderTriStrip (fd, buf); break;
  case 13: RenderTriFan (fd, buf); break;
  case 14: RenderColorTrapezoids (fd, buf); break;
  case 15: RenderColorTriangles (fd, buf); break;
  case 16: RenderTransform (fd, buf); break;
  case 17: RenderCreateGlyphSet (fd, buf); break;
  case 18: RenderReferenceGlyphSet (fd, buf); break;
  case 19: RenderFreeGlyphSet (fd, buf); break;
  case 20: RenderAddGlyphs (fd, buf); break;
  case 21: RenderAddGlyphsFromPicture (fd, buf); break;
  case 22: RenderFreeGlyphs (fd, buf); break;
  case 23: RenderCompositeGlyphs8 (fd, buf); break;
  case 24: RenderCompositeGlyphs16 (fd, buf); break;
  case 25: RenderCompositeGlyphs32 (fd, buf); break;
  case 26: RenderFillRectangles (fd, buf); break;
  default:
    break;
  }
}

render_decode_reply(fd, buf, RequestMinor)
    FD fd;
    unsigned char *buf;
    short RequestMinor;
{
    switch (RequestMinor) {
    case 0: RenderQueryVersionReply (fd, buf); break;
    case 1: RenderQueryPictFormatsReply (fd, buf); break;
    case 2: RenderQueryPictIndexValuesReply (fd, buf); break;
    case 3: RenderQueryDithersReply (fd, buf); break;
    }
}

render_decode_error(fd, buf)
    FD fd;
    unsigned char *buf;
{
    short error = IByte(&buf[0]) - RENDERError;
  
    switch (error) {
    case 0: RenderPictFormatError (fd, buf); break;
    case 1: RenderPictureError (fd, buf); break;
    case 2: RenderPictOpError (fd, buf); break;
    case 3: RenderGlyphSetError (fd, buf); break;
    case 4: RenderGlyphError (fd, buf); break;
    default:
	break;
    }
}


PrintPICTURE(buf)
       unsigned char *buf;
{
    /* print a WINDOW -- CARD32  plus 0 = None */
    long    n = ILong (buf);
    if (n == 0)
          fprintf(stdout, "None");
    else
          fprintf(stdout, "PICTURE %08x", n);
    return(4);
}

PrintPICTFORMAT(buf)
       unsigned char *buf;
{
    /* print a WINDOW -- CARD32  plus 0 = None */
    long    n = ILong (buf);
    if (n == 0)
          fprintf(stdout, "None");
    else
          fprintf(stdout, "PICTFORMAT %08x", n);
    return(4);
}

PrintPICTFORMINFO(buf)
  unsigned char *buf;
{
  /* print a PictFormInfo */
  long	n = ILong(buf);
  short t = IByte(buf+4);
  short d = IByte(buf+5);
  
  fprintf (stdout, "PICTFORMINFO %08x %s %d ",
	   n, t == 0 ? "Indexed" : "Direct", d);
  if (t == 0) {
    long c = ILong(buf+20);
    fprintf (stdout, "cmap %08x", c);
  } else {
    short r = IShort(buf+8);
    short g = IShort(buf+12);
    short b = IShort(buf+16);
    short a = IShort(buf+20);
    fprintf (stdout, "%d %d %d %d", a, r, g, b);
  }
  return(28);
}

PrintGLYPHSET(buf)
       unsigned char *buf;
{
    /* print a GLYPHSET -- CARD32  plus 0 = None */
    long    n = ILong (buf);
    if (n == 0)
          fprintf(stdout, "None");
    else
          fprintf(stdout, "GLYPHSET %08x", n);
    return(4);
}

PrintRENDERCOLOR(buf)
       unsigned char *buf;
{
    /* print a RENDERCOLOR */
    unsigned short  r, g, b, a;
    
    r = IShort(buf);
    g = IShort(buf+2);
    b = IShort(buf+4);
    a = IShort(buf+6);
    fprintf(stdout, "COLOR r:%04x g:%04x b:%04x a:%04x", r, g, b, a);
    return(8);
}

InitializeRENDER(buf)
  unsigned char   *buf;
{
  TYPE    p, DefineType ();

  RENDERRequest = (unsigned char)(buf[9]);
  RENDERError = (unsigned char)(buf[11]);
  LookForRENDERFlag = 0;

  DefineEValue (&TD[REQUEST], (unsigned long) RENDERRequest, "RenderRequest");
  DefineEValue (&TD[REPLY], (unsigned long) RENDERRequest, "RenderReply");
  DefineEValue (&TD[ERROR], (unsigned long) RENDERError, "RenderError");

  p = DefineType(RENDERREQUEST, ENUMERATED, "RENDERREQUEST", PrintENUMERATED);
  DefineEValue(p, 0L, "RenderQueryVersion");
  DefineEValue(p, 1L, "RenderQueryPictFormats");
  DefineEValue(p, 2L, "RenderQueryPictIndexValues");
  DefineEValue(p, 3L, "RenderQueryDithers");
  DefineEValue(p, 4L, "RenderCreatePicture");
  DefineEValue(p, 5L, "RenderChangePicture");
  DefineEValue(p, 6L, "RenderSetPictureClipRectangles");
  DefineEValue(p, 7L, "RenderFreePicture");
  DefineEValue(p, 8L, "RenderComposite");
  DefineEValue(p, 9L, "RenderScale");
  DefineEValue(p, 10L, "RenderTrapezoids");
  DefineEValue(p, 11L, "RenderTriangles");
  DefineEValue(p, 12L, "RenderTriStrip");
  DefineEValue(p, 13L, "RenderTriFan");
  DefineEValue(p, 14L, "RenderColorTrapezoids");
  DefineEValue(p, 15L, "RenderColorTriangles");
  DefineEValue(p, 16L, "RenderTransform");
  DefineEValue(p, 17L, "RenderCreateGlyphSet");
  DefineEValue(p, 18L, "RenderReferenceGlyphSet");
  DefineEValue(p, 19L, "RenderFreeGlyphSet");
  DefineEValue(p, 20L, "RenderAddGlyphs");
  DefineEValue(p, 21L, "RenderAddGlyphsFromPicture");
  DefineEValue(p, 22L, "RenderFreeGlyphs");
  DefineEValue(p, 23L, "RenderCompositeGlyphs8");
  DefineEValue(p, 24L, "RenderCompositeGlyphs16");
  DefineEValue(p, 25L, "RenderCompositeGlyphs32");
  DefineEValue(p, 26L, "RenderFillRectangles");

  p = DefineType(RENDERREPLY, ENUMERATED, "RENDERREPLY", PrintENUMERATED);
  DefineEValue (p, 0L, "QueryVersion");
  DefineEValue (p, 1L, "QueryPictFormats");
  DefineEValue (p, 2L, "QueryPictIndexValues");
  DefineEValue (p, 3L, "QueryDithers");

  DefineType(PICTURE, BUILTIN, "PICTURE", PrintPICTURE);
  DefineType(PICTFORMAT, BUILTIN, "PICTFORMAT", PrintPICTFORMAT);
  DefineType(GLYPHSET, BUILTIN, "GLYPHSET", PrintGLYPHSET);
  DefineType(RENDERCOLOR, BUILTIN, "RENDERCOLOR", PrintRENDERCOLOR);
  DefineType(PICTFORMINFO, BUILTIN, "PICTFORMINFO", PrintPICTFORMINFO);
  
  p = DefineType(PICTURE_BITMASK, SET, "PICTURE_BITMASK", PrintSET);

  DefineValues(p, 0x00000001L, 1, BOOL, "repeat");
  DefineValues(p, 0x00000002L, 1, PICTURE, "alpha-map");
  DefineValues(p, 0x00000004L, 1, INT16, "alpha-x-origin");
  DefineValues(p, 0x00000008L, 1, INT16, "alpha-y-origin");
  DefineValues(p, 0x00000010L, 1, INT16, "clip-x-origin");
  DefineValues(p, 0x00000020L, 1, INT16, "clip-y-origin");
  DefineValues(p, 0x00000040L, 1, PIXMAP, "clip-mask");
  DefineValues(p, 0x00000080L, 1, BOOL, "graphics-exposures");
  DefineValues(p, 0x00000100L, 1, SUBWINMODE, "repeat");
  DefineValues(p, 0x00000200L, 1, BOOL, "poly-edge");
  DefineValues(p, 0x00000400L, 1, BOOL, "poly-mode");
  DefineValues(p, 0x00000800L, 1, ATOM, "dither");
  DefineValues(p, 0x00001000L, 1, BOOL, "component-alpha");

  p = DefineType(PICTOP, ENUMERATED, "PICTOP", PrintENUMERATED);
  DefineEValue (p,  0L, "Clear");
  DefineEValue (p,  1L, "Src");
  DefineEValue (p,  2L, "Dst");
  DefineEValue (p,  3L, "Over");
  DefineEValue (p,  4L, "OverReverse");
  DefineEValue (p,  5L, "In");
  DefineEValue (p,  6L, "InReverse");
  DefineEValue (p,  7L, "Out");
  DefineEValue (p,  8L, "OutReverse");
  DefineEValue (p,  9L, "Atop");
  DefineEValue (p,  10L, "AtopReverse");
  DefineEValue (p,  11L, "Xor");
  DefineEValue (p,  12L, "Add");
  DefineEValue (p,  13L, "Saturate");
}
