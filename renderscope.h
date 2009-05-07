#ifndef _RENDERSCOPE_H_
#define _RENDERSCOPE_H_

#define RENDERREQUESTHEADER  "RENDERREQUEST"
#define RENDERREPLYHEADER	  "RENDERREPLY"

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

extern unsigned char LookForRENDERFlag;
extern unsigned char RENDERRequest;
extern unsigned char RENDERError;
#define RENDERNError	5

extern void RenderQueryVersion		(FD fd, const unsigned char *buf);
extern void RenderQueryVersionReply	(FD fd, const unsigned char *buf);
extern void RenderQueryPictFormats	(FD fd, const unsigned char *buf);
extern void RenderQueryPictFormatsReply	(FD fd, const unsigned char *buf);
extern void RenderQueryPictIndexValues	(FD fd, const unsigned char *buf);
extern void RenderQueryPictIndexValuesReply (FD fd, const unsigned char *buf);
extern void RenderQueryDithers		(FD fd, const unsigned char *buf);
extern void RenderQueryDithersReply	(FD fd, const unsigned char *buf);
extern void RenderCreatePicture		(FD fd, const unsigned char *buf);
extern void RenderChangePicture		(FD fd, const unsigned char *buf);
extern void RenderSetPictureClipRectangles (FD fd, const unsigned char *buf);
extern void RenderFreePicture		(FD fd, const unsigned char *buf);
extern void RenderComposite		(FD fd, const unsigned char *buf);
extern void RenderScale			(FD fd, const unsigned char *buf);
extern void RenderTrapezoids		(FD fd, const unsigned char *buf);
extern void RenderTriangles		(FD fd, const unsigned char *buf);
extern void RenderTriStrip		(FD fd, const unsigned char *buf);
extern void RenderTriFan		(FD fd, const unsigned char *buf);
extern void RenderColorTrapezoids	(FD fd, const unsigned char *buf);
extern void RenderColorTriangles	(FD fd, const unsigned char *buf);
extern void RenderTransform		(FD fd, const unsigned char *buf);
extern void RenderCreateGlyphSet	(FD fd, const unsigned char *buf);
extern void RenderReferenceGlyphSet	(FD fd, const unsigned char *buf);
extern void RenderFreeGlyphSet		(FD fd, const unsigned char *buf);
extern void RenderAddGlyphs		(FD fd, const unsigned char *buf);
extern void RenderAddGlyphsFromPicture	(FD fd, const unsigned char *buf);
extern void RenderFreeGlyphs		(FD fd, const unsigned char *buf);
extern void RenderCompositeGlyphs8	(FD fd, const unsigned char *buf);
extern void RenderCompositeGlyphs16	(FD fd, const unsigned char *buf);
extern void RenderCompositeGlyphs32	(FD fd, const unsigned char *buf);
extern void RenderFillRectangles	(FD fd, const unsigned char *buf);

extern void RenderPictFormatError	(FD fd, const unsigned char *buf);
extern void RenderPictureError		(FD fd, const unsigned char *buf);
extern void RenderPictOpError		(FD fd, const unsigned char *buf);
extern void RenderGlyphSetError		(FD fd, const unsigned char *buf);
extern void RenderGlyphError		(FD fd, const unsigned char *buf);

#endif

