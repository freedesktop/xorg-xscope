/* **********************************************
 *						*
 * header file for the Server spy scope           *
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
 ********************************************** */

#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <stdio.h>
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


extern int     ScopePort;
extern char   *ScopeHost;

extern int  Interrupt, SingleStep, BreakPoint;

extern void ReadCommands ();

extern char ServerHostName[255];
extern char AudioServerHostName[255];

/* external function type declarations */

extern char   *Malloc ();
extern char   *ClientName ();

/* ********************************************** */
/*                                                */
/* ********************************************** */

/* need to change the MaxFD to allow larger number of fd's */
#define StaticMaxFD 1024

#define BUFFER_SIZE (1024 * 32)

struct fdinfo
{
  Boolean Server;
  long    ClientNumber;
  FD	  pair;
  char	  buffer[BUFFER_SIZE];
  int	  bufcount;
  int	  bufstart;
  int	  buflimit;	/* limited writes */
  int	  bufdelivered;	/* total bytes delivered */
  Boolean writeblocked;
};

extern struct fdinfo   FDinfo[StaticMaxFD];
