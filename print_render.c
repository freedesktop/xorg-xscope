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
  n = ILong (&buf[4]);
  f = ILong (&buf[8]);
  s = ILong (&buf[12]);
  d = ILong (&buf[16]);
  v = ILong (&buf[20]);
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
  int n;
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  PrintField (buf, 4, 1, PICTOP, "op");
  PrintField(buf, 8, 4, PICTURE, "source");
  PrintField(buf, 12, 4, PICTURE, "dest");
  PrintField(buf, 16, 4, PICTFORMAT, "mask format");
  PrintField(buf, 20, 2, CARD16, "source x");
  PrintField(buf, 22, 2, CARD16, "source y");
  n = (CS[fd].requestLen - 6) / 10;
  PrintList(&buf[24], (long)n, TRAPEZOID, "trapezoids");
}

RenderTriangles (FD fd, unsigned char *buf)
{
  long n;
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  PrintField (buf, 4, 1, PICTOP, "op");
  PrintField(buf, 8, 4, PICTURE, "source");
  PrintField(buf, 12, 4, PICTURE, "dest");
  PrintField(buf, 16, 4, PICTFORMAT, "mask format");
  PrintField(buf, 20, 2, CARD16, "source x");
  PrintField(buf, 22, 2, CARD16, "source y");
  n = (CS[fd].requestLen - 6) / 6;
  PrintList(&buf[24], (long)n, TRIANGLE, "triangles");
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
  PrintField(buf, 4, 4, GLYPHSET, "glyphset");
  PrintField(buf, 8, 4, PICTFORMAT, "format");
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
  PrintField(buf, 4, 4, GLYPHSET, "glyphset");
}
extern char Leader[];

void
PrintGlyphs(unsigned char *buf, int n, char *name)
{
  unsigned char *gids;
  unsigned char *glyphs;
  int i;
    
  fprintf(stdout, "%s%20s:\n", Leader, name);
  gids = buf;
  glyphs = gids + 4 * n;
  for (i = 0; i < n; i++)
  {
    PrintField(gids, 0, 4, CARD32, "glyphid");
    PrintField(glyphs, 0, 2, CARD16, "width");
    PrintField(glyphs, 2, 2, CARD16, "height");
    PrintField(glyphs, 4, 2, INT16, "x");
    PrintField(glyphs, 6, 2, INT16, "y");
    PrintField(glyphs, 8, 2, INT16, "xOff");
    PrintField(glyphs, 10, 2, INT16, "yOff");
    glyphs += 12;
  }
}

RenderAddGlyphs (FD fd, unsigned char *buf)
{
  long n;
  long i;
  long g;
  long nb;
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  PrintField(buf, 2, 2, CONST2(2), "request length");
  PrintField(buf, 4, 4, GLYPHSET, "glyphset");
  PrintField(buf, 8, 4, CARD32, "nglyphs");
  PrintGlyphs(&buf[12], ILong(&buf[8]), "glyphs");
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
  unsigned short n;
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* RenderRequest */ ;
  PrintField (buf, 1, 1, RENDERREQUEST, RENDERREQUESTHEADER) /* RenderSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  PrintField(buf, 4, 4, GLYPHSET, "glyphset");
  n = (IShort(&buf[2]) - 2);
  (void)PrintList(&buf[8], (long)n, CARD32, "glyphs");
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
  n = (CS[fd].requestLen - 7) * 4;
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
  n = (CS[fd].requestLen - 7) * 4;
  
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
  n = (CS[fd].requestLen - 7) * 4;
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
  n = (CS[fd].requestLen - 5) / 2;
  PrintField(buf, 4, 1, PICTOP, "op");
  PrintField(buf, 8, 4, PICTURE, "dest");
  PrintField(buf, 12, 8, RENDERCOLOR, "color");
  (void)PrintList(&buf[20], (long)n, RECTANGLE, "rectangles");
}
RenderPictFormatError (fd, buf)
{
  PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Request */ ;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, PICTFORMAT, "format");
  PrintField(buf, 8, 2, CARD16, "minor opcode");
  PrintField(buf, 10, 1, CARD8, "major opcode");
}

RenderPictureError (fd, buf)
{
  PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Request */ ;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, PICTURE, "picture");
  PrintField(buf, 8, 2, CARD16, "minor opcode");
  PrintField(buf, 10, 1, CARD8, "major opcode");
}

RenderPictOpError (fd, buf)
{
  PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Request */ ;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, PICTOP, "pictop");
  PrintField(buf, 8, 2, CARD16, "minor opcode");
  PrintField(buf, 10, 1, CARD8, "major opcode");
}

RenderGlyphSetError (fd, buf)
{
  PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Request */ ;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, GLYPHSET, "glyphset");
  PrintField(buf, 8, 2, CARD16, "minor opcode");
  PrintField(buf, 10, 1, CARD8, "major opcode");
}

RenderGlyphError (fd, buf)
{
  PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Request */ ;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, CARD32, "glyph");
  PrintField(buf, 8, 2, CARD16, "minor opcode");
  PrintField(buf, 10, 1, CARD8, "major opcode");
}
