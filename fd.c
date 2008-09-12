/* ************************************************************ *\
 *								*
 *    Support routines for file descriptors (FD) 		*
 *								*
 *	James Peterson, 1987  	 				*
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
 * 				  				*
 * 				  				*
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
 \* *********************************************************** */

#include "scope.h"

#ifdef SYSV
#include <unistd.h>
#define getdtablesize() sysconf(_SC_OPEN_MAX)
#define bzero(s,l) memset(s, 0, l)
#define bcopy(s,d,l) memmove(d,s,l)
#endif

/*
  All of this code is to support the handling of file descriptors (FD).
  The idea is to keep a table of the FDs that are in use and why.
  For each FD that is open for input, we keep the name of a procedure
  to call if input arrives for that FD.  When an FD is created
  (by an open, pipe, socket, ...) declare that by calling UsingFD.
  When it is no longer in use (close ...), call NotUsingFD.
*/

/* ************************************************************ */
/*								*/
/*								*/
/* ************************************************************ */

void
InitializeFD()
{
  register short  i;

  enterprocedure("InitializeFD");
  /* get the number of file descriptors the system will let us use */
  MaxFD = getdtablesize();
  if (MaxFD > FD_SETSIZE) {
      MaxFD = FD_SETSIZE;
  }

  /* allocate space for a File Descriptor (FD) Table */
  FDD = (struct FDDescriptor *)
    Malloc ((long)(MaxFD * sizeof (struct FDDescriptor)));
  if (FDD == NULL) {
      panic("Can't allocate memory!");
  }
  bzero(FDD, (MaxFD * sizeof (struct FDDescriptor)));

  /* be sure all fd's are closed and marked not busy */
  for (i = 0; i < MaxFD; i++)
    {
      /* 0, 1, 2 are special (stdin, stdout, stderr) */
      if (i > 2)
	close(i);
      /* FDD[i].Busy = false; - not needed since false==0*/
    }

  /* save one FD for single file input or output like debugging */
  /* also the getservbyname call is currently using an FD */
  MaxFD -= 4;

  nFDsInUse = 0 /* stdin, stdout, stderr */ ;
  FD_ZERO(&ReadDescriptors);
  HighestFD = 0;

  UsingFD(fileno(stdin),  (void (*)(int))NULL, NULL);
  UsingFD(fileno(stdout), (void (*)(int))NULL, NULL);
  UsingFD(fileno(stderr), (void (*)(int))NULL, NULL);
}

/* ************************************************************ */

void
UsingFD(fd, Handler, trans_conn)
     FD fd;
     void     (*Handler)(int);
     XtransConnInfo trans_conn;
{
  if (FDD[fd].Busy)
    NotUsingFD(fd);
  nFDsInUse += 1;

  FDD[fd].Busy = true;
  FDD[fd].InputHandler = Handler;
#ifdef USE_XTRANS
  FDD[fd].trans_conn = trans_conn;
#endif
  if (Handler == NULL)
    FD_CLR(fd,&ReadDescriptors) /* clear fd bit */ ;
  else
    FD_SET(fd,&ReadDescriptors) /* set fd bit */ ;

  if (fd > HighestFD)
    HighestFD = fd;

  if (nFDsInUse >= MaxFD)
    panic("no more FDs");

  debug(128,(stderr, "Using FD %d, %d of %d in use\n", fd, nFDsInUse, MaxFD));
}

/* ************************************************************ */

void
NotUsingFD(fd)
     FD fd;
{
  debug(128,(stderr, "Not Using FD %d\n", fd));

  if (FDD[fd].Busy)
    nFDsInUse -= 1;

  FDD[fd].Busy = false;
  FD_CLR(fd,&ReadDescriptors) /* clear fd bit */ ;

  while (!FDD[HighestFD].Busy && HighestFD > 0)
    HighestFD -= 1;

  debug(128,(stderr, "Highest FD %d, in use %d\n", HighestFD, nFDsInUse));
}

/* ************************************************************ */

#ifdef USE_XTRANS
XtransConnInfo GetXTransConnInfo(FD fd)
{
    return FDD[fd].trans_conn;
}
#endif

/* ************************************************************ */

static void
EOFonFD(fd)
     FD fd;
{
  enterprocedure("EOFonFD");
  debug(128,(stderr, "EOF on %d\n", fd));
#ifdef USE_XTRANS
  if (FDD[fd].trans_conn) 
      _X11TransClose(FDD[fd].trans_conn);
  else
#endif
  close(fd);
  NotUsingFD(fd);
}


/* ************************************************************ */
/*								*/
/*     Main Loop -- wait for input from any source and Process  */
/*								*/
/* ************************************************************ */

#include <errno.h>	       /* for EINTR, EADDRINUSE, ... */
extern int  errno;

int
MainLoop(void)
{
  enterprocedure("MainLoop");

  while (true)
    {
      fd_set  rfds, xfds;
      short   nfds;
      short   fd;

      /* wait for something */
      rfds = ReadDescriptors;
      xfds = rfds;

      debug(128,(stderr, "select %d, rfds = 0%o\n", HighestFD + 1, rfds));
      nfds = select(HighestFD + 1, (struct fd_set *) &rfds, 
	 (struct fd_set *) NULL,  (struct fd_set *) &xfds, NULL);
      debug(128,(stderr, "select nfds = 0%o, rfds = 0%o, xfds 0%o\n",
		 nfds, rfds, xfds));

      if (nfds < 0)
	{
	  if (errno == EINTR)
	    continue /* to end of while loop */ ;
	  debug(1,(stderr, "Bad select - errno = %d\n", errno));
	  if (errno == EBADF)
	    {
	      /* one of the bits in rfds is invalid, close down
		 files until it goes away */
	      EOFonFD(HighestFD);
	      continue;
	    }

	  panic("Select returns error");
	  continue /* to end of while loop */ ;
	}

      if (nfds == 0)
	{
	  TimerExpired();
	  continue;
	}

      /* check each fd to see if it has input */
      for (fd = 0; 0 < nfds && fd <= HighestFD; fd++)
	{
	  /*
	    check all returned fd's; this prevents
	    starvation of later clients by earlier clients
	  */

	  if (FD_ISSET(fd,&rfds) == 0)
	    continue;

	  nfds -= 1;

	  if (FDD[fd].InputHandler == NULL)
	    {
	      panic("FD selected with no handler");
	      debug(1,(stderr, "FD %d has NULL handler\n", fd));
	    }
	  else
	    (FDD[fd].InputHandler)(fd);
	}
    }
  return 0;
}
