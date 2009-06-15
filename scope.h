/* **********************************************
 *						*
 * header file for the Server spy scope         *
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

#ifndef XSCOPE_SCOPE_H
#define XSCOPE_SCOPE_H

#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/param.h>
#ifdef SVR4
#include <sys/filio.h>
#endif /* SVR4 */

#define Boolean short
#define true 1
#define false 0

#include "fd.h"

/* ********************************************** */
/*                                                */
/* ********************************************** */

#define Assert(b) (b)
#define debug(n,f) (void)((debuglevel & n) ? (fprintf f,fflush(stderr)) : 0)
extern short   debuglevel;

/* ********************************************** */
/*                                                */
/* ********************************************** */

extern short   Verbose		  /* quiet (0) or increasingly verbose  ( > 0) */ ;
extern short	XVerbose;
extern short	NasVerbose;
#ifdef RAW_MODE
extern short	Raw		  /* raw data output only */ ;
#else
#define Raw 0
#endif


extern int     ScopePort;
extern char   *ScopeHost;

extern int  Interrupt, SingleStep, BreakPoint;

extern char ServerHostName[MAXHOSTNAMELEN];
extern char AudioServerHostName[MAXHOSTNAMELEN];

/* ********************************************** */
/*                                                */
/* ********************************************** */

#include "fd.h"
#include "proto.h"

#define BUFFER_SIZE (1024 * 32)

struct fdinfo
{
  Boolean Server;
  long    ClientNumber;
  FD	  pair;
  unsigned char	  buffer[BUFFER_SIZE];
  int	  bufcount;
  int	  bufstart;
  int	  buflimit;	/* limited writes */
  int	  bufdelivered;	/* total bytes delivered */
  Boolean writeblocked;
};

extern struct fdinfo   FDinfo[StaticMaxFD];
extern int littleEndian;
extern char HandleSIGUSR1;
extern char Leader[];
extern long ServerBasePort;
extern char ScopeEnabled;
extern long TranslateText;


#endif /* XSCOPE_SCOPE_H */
