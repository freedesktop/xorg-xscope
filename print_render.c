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

#include "scope.h"
#include "x11.h"
#include "renderscope.h"

RenderQueryVersion (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderQueryVersionReply (FD fd, unsigned char *buf)
{
  short   n;
  long    m;
  long   k;

  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* RenderRequest reply */ ;
  PrintField(RBf, 1, 1, RENDERREPLY, RENDERREPLYHEADER) /* RenderQueryVersion reply */;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
  PrintField(buf, 8, 2, CARD16, "major-version");
  PrintField(buf, 10, 2, CARD16, "minor-version");
}
RenderQueryPictFormats (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderQueryPictFormatsReply (FD fd, unsigned char *buf)
{
  long	n;
  long	f;
  long  s;
  long  d;
  long  v;
  long  m;
  long  k;

  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* RenderRequest reply */ ;
  PrintField(RBf, 1, 1, RENDERREPLY, RENDERREPLYHEADER) /* RenderQueryVersion reply */;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
  n = ILong (&buf[8]);
  f = ILong (&buf[12]);
  s = ILong (&buf[16]);
  d = ILong (&buf[20]);
  v = ILong (&buf[24]);
  PrintList (&buf[32], (long) f, PICTFORMINFO, "pict-formats");
}
RenderQueryPictIndexValues (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderQueryPictIndexValuesReply (FD fd, unsigned char *buf)
{
  short   n;
  long    m;
  long   k;

  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* RenderRequest reply */ ;
  PrintField(RBf, 1, 1, RENDERREPLY, RENDERREPLYHEADER) /* RenderQueryVersion reply */;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
}
RenderQueryDithers (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderQueryDithersReply (FD fd, unsigned char *buf)
{
  short   n;
  long    m;
  long   k;

  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* RenderRequest reply */ ;
  PrintField(RBf, 1, 1, RENDERREPLY, RENDERREPLYHEADER) /* RenderQueryVersion reply */;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
}
RenderCreatePicture (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, DVALUE2(5+n), "request length");
  PrintField(buf, 4, 4, PICTURE, "picture");
  PrintField(buf, 8, 4, DRAWABLE, "drawable");
  PrintField(buf, 12, 4, PICTFORMAT, "format");
  PrintField(buf, 16, 4, PICTURE_BITMASK, "value-mask");
  PrintValues(&buf[16], 4, PICTURE_BITMASK, &buf[20], "value-list");
}
RenderChangePicture (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, DVALUE2(3+n), "request length");
  PrintField(buf, 4, 4, PICTURE, "picture");
  PrintField(buf, 8, 4, PICTURE_BITMASK, "value-mask");
  PrintValues(&buf[8], 4, PICTURE_BITMASK, &buf[12], "value-list");
}
RenderSetPictureClipRectangles (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderFreePicture (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderComposite (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderScale (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderTrapezoids (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderTriangles (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderTriStrip (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderTriFan (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderColorTrapezoids (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderColorTriangles (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderTransform (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderCreateGlyphSet (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderReferenceGlyphSet (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderFreeGlyphSet (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderAddGlyphs (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderAddGlyphsFromPicture (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}
RenderFreeGlyphs (FD fd, unsigned char *buf)
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}

extern char Leader[];

void
PrintGlyphList(unsigned char *buf, int length, char *name, int size)
{
  short   n;

  fprintf(stdout, "%s%20s:\n", Leader, name);
  while (length >= 8)
  {
    n = IByte(&buf[0]);
    if (n != 255)
    {
      printfield(buf, 0, 1, CARD8, "length");
      PrintField(buf, 4, 2, INT16, "delta x");
      PrintField(buf, 6, 2, INT16, "delta y");
      switch (size) {
      case 1:
	PrintTString8(&buf[8], (long) n, "glyph item 8 string"); break;
      case 2:
	PrintTString16(&buf[8], (long) n, "glyph item 16 string"); break;
      case 4:
	break;
      }
      buf += n*size + 8;
      length -= n*size+8;
    }
    else
    {
      PrintField(buf, 4, 4, GLYPHSET, "glyphset");
      buf += 8;
      length -= 8;
    }
  }
}
	       
RenderCompositeGlyphs8 (FD fd, unsigned char *buf)
{
  int n;
  
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  n = (IShort(&buf[2]) - 7) * 4;
  PrintField(buf, 4, 1, PICTOP, "op");
  PrintField(buf, 8, 4, PICTURE, "source");
  PrintField(buf, 12, 4, PICTURE, "dest");
  PrintField(buf, 16, 4, PICTFORMAT, "mask format");
  PrintField(buf, 20, 4, GLYPHSET, "glyphset");
  PrintField(buf, 24, 2, INT16, "x-src");
  PrintField(buf, 26, 2, INT16, "y-src");
  PrintGlyphList(&buf[28], n, "items", 1);
}

RenderCompositeGlyphs16 (FD fd, unsigned char *buf)
{
  int n;
  
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  n = (IShort(&buf[2]) - 7) * 4;
  
  PrintField(buf, 4, 1, PICTOP, "op");
  PrintField(buf, 8, 4, PICTURE, "source");
  PrintField(buf, 12, 4, PICTURE, "dest");
  PrintField(buf, 16, 4, PICTFORMAT, "mask format");
  PrintField(buf, 20, 4, GLYPHSET, "glyphset");
  PrintField(buf, 24, 2, INT16, "x-src");
  PrintField(buf, 26, 2, INT16, "y-src");
  PrintGlyphList(&buf[28], n, "items", 2);
}
RenderCompositeGlyphs32 (FD fd, unsigned char *buf)
{
  int n;
  
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  n = (IShort(&buf[2]) - 7) * 4;
  PrintField(buf, 4, 1, PICTOP, "op");
  PrintField(buf, 8, 4, PICTURE, "source");
  PrintField(buf, 12, 4, PICTURE, "dest");
  PrintField(buf, 16, 4, PICTFORMAT, "mask format");
  PrintField(buf, 20, 4, GLYPHSET, "glyphset");
  PrintField(buf, 24, 2, INT16, "x-src");
  PrintField(buf, 26, 2, INT16, "y-src");
  PrintGlyphList(&buf[28], n, "items", 4);
}
RenderFillRectangles (FD fd, unsigned char *buf)
{
  int n;
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  n = (IShort(&buf[2]) - 5) / 2;
  PrintField(buf, 4, 1, PICTOP, "op");
  PrintField(buf, 8, 4, PICTURE, "dest");
  PrintField(buf, 12, 8, RENDERCOLOR, "color");
  (void)PrintList(&buf[20], (long)n, RECTANGLE, "rectangles");
}
RenderPictFormatError (fd, buf)
{
}
RenderPictureError (fd, buf)
{
}
RenderPictOpError (fd, buf)
{
}
RenderGlyphSetError (fd, buf)
{
}
RenderGlyphError (fd, buf)
{
}
