/* **********************************************
 *						*
 * header file file descriptor (FD) code        *
 *						*
 *	James Peterson, 1987			*
 * Copyright (C) 1987 MCC
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of MCC not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  MCC makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * MCC DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL MCC BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *						*
 *						*
 * Copyright 2002 Sun Microsystems, Inc.  All rights reserved.
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
 *
 ********************************************** */


/* 
   the following structure remembers for each file descriptor its
   state.  In particular, we need to know if it is busy or free
   and if it is in use, by whom.
*/
#ifdef USE_XTRANS
#include <X11/Xtrans/Xtrans.h>
#else
typedef void *XtransConnInfo;
#endif
#include <sys/select.h>

typedef int FD;

struct FDDescriptor
{
    Boolean Busy;
    void     (*InputHandler)(int);
#ifdef USE_XTRANS
    XtransConnInfo trans_conn;
#endif
};

struct FDDescriptor *FDD /* array of FD descriptors */ ;
int	MaxFD 		/* maximum number of FD's possible */ ;
int	nFDsInUse	/* number of FD's actually in use */ ;
fd_set  ReadDescriptors /* bit map of FD's in use -- for select  */ ;
int	HighestFD 	/* highest FD in use -- for select */ ;

/* need to change the MaxFD to allow larger number of fd's */
#define StaticMaxFD FD_SETSIZE

extern void InitializeFD(void);
extern void UsingFD(FD fd, void (*Handler)(int), XtransConnInfo trans_conn);
extern void NotUsingFD(FD fd);
extern int MainLoop(void);
#ifdef USE_XTRANS
extern XtransConnInfo GetXTransConnInfo(FD fd);
#endif
