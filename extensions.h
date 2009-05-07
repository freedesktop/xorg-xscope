/*
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * of the copyright holder.
 */

#ifndef XSCOPE_EXTENSIONS_H
#define XSCOPE_EXTENSIONS_H

#include "scope.h"

#define EXTENSION_MIN	128	/* lowest possible extension request code */
#define EXTENSION_MAX	255	/* highest possible extension request code */
#define NUM_EXTENSIONS	128	/* maximum possible number of extensions */

/* special processing in extensions.c to capture extension info */
extern void ProcessQueryExtensionRequest(long seq, const unsigned char *buf);
extern void ProcessQueryExtensionReply(long seq, const unsigned char *buf);

extern void ExtensionRequest(FD fd, const unsigned char *buf, short Request);
extern void ExtensionReply(FD fd, const unsigned char *buf,
			   short Request, short RequestMinor);
extern void ExtensionError(FD fd, const unsigned char *buf, short Error);
extern void ExtensionEvent(FD fd, const unsigned char *buf, short Event);


/* X11 Extension decoders in decode_*.c */
#define extension_decode_prototypes(ext) \
	extern void ext##_decode_req	(FD fd, const unsigned char *buf); \
	extern void ext##_decode_reply	(FD fd, const unsigned char *buf,  \
					 short RequestMinor);		   \
	extern void ext##_decode_error	(FD fd, const unsigned char *buf); \
	extern void ext##_decode_event	(FD fd, const unsigned char *buf)

extension_decode_prototypes(bigreq);
extension_decode_prototypes(lbx);
extension_decode_prototypes(mitshm);
extension_decode_prototypes(randr);
extension_decode_prototypes(render);
extension_decode_prototypes(wcp);

extern void InitializeBIGREQ	(const unsigned char *buf);
extern void InitializeLBX	(const unsigned char *buf);
extern void InitializeMITSHM	(const unsigned char *buf);
extern void InitializeRANDR	(const unsigned char *buf);
extern void InitializeRENDER	(const unsigned char *buf);
extern void InitializeWCP	(const unsigned char *buf);

#endif /* XSCOPE_EXTENSIONS_H */
