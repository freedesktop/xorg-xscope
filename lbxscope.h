/* $XConsortium: pexscope.h,v 5.1 91/02/16 09:31:57 rws Exp $ */

/***********************************************************
Copyright 1990, 1991 by Sun Microsystems, Inc. and the X Consortium.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Sun Microsystems,
the X Consortium, and MIT not be used in advertising or publicity 
pertaining to distribution of the software without specific, written 
prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef _LBXSCOPE_H_
#define _LBXSCOPE_H_

#define LBXREQUESTHEADER  "LBXREQUEST"
#define LBXREPLYHEADER	  "LBXREPLY"
#define LBXEVENTHEADER	  "LBXEVENT"

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

extern unsigned char LookForLBXFlag;
extern unsigned char LBXRequest;
extern unsigned char LBXError;
extern unsigned char LBXEvent;

extern void LbxQueryVersion		(FD fd, unsigned char *buf);
extern void LbxQueryVersionReply	(FD fd, unsigned char *buf);
extern void LbxStartProxy		(FD fd, unsigned char *buf);
extern void LbxStopProxy		(FD fd, unsigned char *buf);
extern void LbxNewClient		(FD fd, unsigned char *buf);
extern void LbxCloseClient		(FD fd, unsigned char *buf);
extern void LbxSwitch			(FD fd, unsigned char *buf);
extern void LbxModifySequence		(FD fd, unsigned char *buf);
extern void LbxSwitchEvent		(FD fd, unsigned char *buf);
extern void LbxCloseEvent		(FD fd, unsigned char *buf);

#endif

