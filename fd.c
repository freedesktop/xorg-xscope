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
 \* *********************************************************** */

#include "scope.h"

#include <sys/uio.h>	       /* for struct iovec, used by socket.h */
#include <sys/socket.h>	       /* for AF_INET, SOCK_STREAM, ... */
#include <sys/ioctl.h>	       /* for FIONCLEX, FIONBIO, ... */
#ifdef SVR4
#include <sys/filio.h>
#endif
#include <fcntl.h>
#include <netinet/in.h>	       /* struct sockaddr_in */
#include <sys/un.h>	       /* struct sockaddr_un */
#include <netinet/tcp.h>
#include <netdb.h>	       /* struct servent * and struct hostent * */
#include <errno.h>	       /* for EINTR, EADDRINUSE, ... */
extern int  errno;


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

InitializeFD()
{
  register short  i;

  enterprocedure("InitializeFD");
  /* get the number of file descriptors the system will let us use */
#if defined(hpux) || defined(SVR4)
  MaxFD = _NFILE - 1;
#else
  MaxFD = getdtablesize();
#endif
  if (MaxFD > StaticMaxFD)
    {
      fprintf(stderr, "Recompile with larger StaticMaxFD value %d\n", MaxFD);
      MaxFD = StaticMaxFD;
    }

  /* allocate space for a File Descriptor (FD) Table */
  FDD = (struct FDDescriptor *)
    Malloc ((long)(MaxFD * sizeof (struct FDDescriptor)));

  /* be sure all fd's are closed and marked not busy */
  for (i = 0; i < MaxFD; i++)
    {
      /* 0, 1, 2 are special (stdin, stdout, stderr) */
      if (i > 2)
	(void)close(i);
      FDD[i].Busy = false;
    }

  /* save one FD for single file input or output like debugging */
  /* also the getservbyname call is currently using an FD */
  MaxFD -= 4;

  nFDsInUse = 0 /* stdin, stdout, stderr */ ;
  ReadDescriptors = 0;
  HighestFD = 0;

  UsingFD(fileno(stdin),  (int (*)())NULL, (int (*)())NULL);
  UsingFD(fileno(stdout), (int (*)())NULL, (int (*)())NULL);
  UsingFD(fileno(stderr), (int (*)())NULL);
}

/* ************************************************************ */

UsingFD(fd, Handler, FlushHandler)
     FD fd;
     int     (*Handler)();
     int     (*FlushHandler)();
{
  if (FDD[fd].Busy)
    NotUsingFD(fd);
  nFDsInUse += 1;

  FDD[fd].Busy = true;
  FDD[fd].InputHandler = Handler;
  FDD[fd].FlushHandler = FlushHandler;
  if (Handler == NULL)
    ReadDescriptors &= ~(1 << fd) /* clear fd bit */ ;
  else
    ReadDescriptors |= 1 << fd /* set fd bit */ ;

  if (fd > HighestFD)
    HighestFD = fd;

  if (nFDsInUse >= MaxFD)
    panic("no more FDs");

  debug(128,(stderr, "Using FD %d, %d of %d in use\n", fd, nFDsInUse, MaxFD));
}

/* ************************************************************ */

NotUsingFD(fd)
     FD fd;
{
  debug(128,(stderr, "Not Using FD %d\n", fd));

  if (FDD[fd].Busy)
    nFDsInUse -= 1;

  FDD[fd].Busy = false;
  ReadDescriptors &= ~(1 << fd) /* clear fd bit */ ;

  while (!FDD[HighestFD].Busy && HighestFD > 0)
    HighestFD -= 1;

  debug(128,(stderr, "Highest FD %d, in use %d\n", HighestFD, nFDsInUse));
}

/* ************************************************************ */

EOFonFD(fd)
     FD fd;
{
  enterprocedure("EOFonFD");
  debug(128,(stderr, "EOF on %d\n", fd));
  (void)close(fd);
  NotUsingFD(fd);
}

FD
AcceptConnection (ConnectionSocket)
  FD  ConnectionSocket;
{
  FD ClientFD;
  struct sockaddr_in  from;
  int    len = sizeof (from);
  int	 tmp = 1;

  enterprocedure("ConnectToClient");

  ClientFD = accept(ConnectionSocket, (struct sockaddr *)&from, &len);
  debug(4,(stderr, "Connect To Client: FD %d\n", ClientFD));
  if (ClientFD < 0 && errno == EWOULDBLOCK)
    {
      debug(4,(stderr, "Almost blocked accepting FD %d\n", ClientFD));
      panic("Can't connect to Client");
    }
  if (ClientFD < 0)
    {
      debug(4,(stderr, "NewConnection: error %d\n", errno));
      panic("Can't connect to Client");
    }

#ifdef FD_CLOEXEC
  (void)fcntl(ClientFD, F_SETFD, FD_CLOEXEC);
#else
  (void)ioctl(ClientFD, FIOCLEX, 0);
#endif
  /* ultrix reads hang on Unix sockets, hpux reads fail */
#if defined(O_NONBLOCK) && (!defined(ultrix) && !defined(hpux))
  (void) fcntl (ClientFD, F_SETFL, O_NONBLOCK);
#else
#ifdef FIOSNBIO
  ioctl (ClientFD, FIOSNBIO, &ON);
#else
  (void) fcntl (ClientFD, F_SETFL, FNDELAY);
#endif
#endif
  (void) setsockopt(ClientFD, IPPROTO_TCP, TCP_NODELAY, (char *) &tmp, sizeof (int));
  return(ClientFD);
}

FD
MakeConnection(server, port, report)
  char *server;
  short	port;
  int	report;
{
  FD ServerFD;
  char HostName[512];
  struct sockaddr_in  sin;
  struct sockaddr_un  sun;
  struct sockaddr     *saddr;
  int		      salen;
  struct hostent *hp;
  int tmp = 1;
#ifndef	SO_DONTLINGER
  struct linger linger;
#endif /* SO_DONTLINGER */

  enterprocedure("ConnectToServer");

  /* establish a socket to the name server for this host */
  /* determine the host machine for this process */
  if (*server == '\0')
  {
    sun.sun_family = AF_UNIX;
    sprintf (sun.sun_path, "/tmp/.X11-unix/X%d", port - 6000);
    salen = sizeof (sun.sun_family) + strlen (sun.sun_path) + 1;
    saddr = (struct sockaddr *) &sun;
  }
  else
  {
    debug(4,(stderr, "try to connect on %s\n", server));
  
    bzero((char *)&sin, sizeof(sin));
    sin.sin_addr.s_addr = inet_addr (server);
    if ((long) sin.sin_addr.s_addr == -1)
    {
	hp = gethostbyname(server);
	if (hp == 0)
	  {
	    perror("gethostbyname failed");
	    debug(1,(stderr, "gethostbyname failed for %s\n", server));
	    panic("Can't open connection to Server");
	  }
	bcopy((char *)hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    }
  
    sin.sin_family = AF_INET;
  
    if (port == ScopePort
	&& strcmp(server, ScopeHost) == 0)
      {
	char error_message[100];
	(void)sprintf(error_message, "Trying to attach to myself: %s,%d\n",
		server, sin.sin_port);
	panic(error_message);
      }
  
    sin.sin_port = htons (port);
    salen = sizeof (sin);
    saddr = (struct sockaddr *) &sin;
  }

  ServerFD = socket(saddr->sa_family, SOCK_STREAM, 0);
  if (ServerFD < 0)
    {
      perror("socket() to Server failed");
      debug(1,(stderr, "socket failed\n"));
      panic("Can't open connection to Server");
    }
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR,  (char *) NULL, 0);
#ifdef SO_USELOOPBACK
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_USELOOPBACK,(char *) NULL, 0);
#endif
  (void) setsockopt(ServerFD, IPPROTO_TCP, TCP_NODELAY, (char *) &tmp, sizeof (int));
#ifdef	SO_DONTLINGER
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_DONTLINGER, (char *) NULL, 0);
#else /* SO_DONTLINGER */
  linger.l_onoff = 0;
  linger.l_linger = 0;
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof linger);
#endif /* SO_DONTLINGER */

  /* ******************************************************** */
  /* try to connect to Server */

  if (connect(ServerFD, saddr, salen) < 0)
    {
      debug(4,(stderr, "connect returns errno of %d\n", errno));
      if (errno != 0)
	if (report)
	  perror("connect");
      switch (errno)
	{
	case ECONNREFUSED:
	  /* experience says this is because there is no Server
	     to connect to */
	  (void)close(ServerFD);
	  debug(1,(stderr, "No Server\n"));
	  if (report)
	    warn("Can't open connection to Server");
	  return(-1);

	default:
	  (void)close(ServerFD);
	  panic("Can't open connection to Server");
	}
    }

  debug(4,(stderr, "Connect To Server: FD %d\n", ServerFD));
  return(ServerFD);
}
  
/* ************************************************************ */
/*								*/
/*     Main Loop -- wait for input from any source and Process  */
/*								*/
/* ************************************************************ */

#include <errno.h>	    /* for EINTR, EADDRINUSE, ... */
extern int  errno;


MainLoop()
{
  enterprocedure("MainLoop");

  while (true)
    {
      int     rfds, wfds, xfds;
      short   nfds;
      short   fd;

      /* wait for something */
      rfds = ReadDescriptors & ~BlockedReadDescriptors;
      wfds = ReadDescriptors & WriteDescriptors;
      xfds = rfds;

      debug(128,(stderr, "select %d, rfds = 0%o\n", HighestFD + 1, rfds));
      if (Interrupt || (rfds == 0 && wfds == 0))
      {
	ReadCommands ();
	Interrupt = 0;
	continue;
      }
      nfds = select(HighestFD + 1, &rfds, &wfds, &xfds, (struct timeval *)NULL);
      debug(128,(stderr, "select nfds = 0%o, rfds = 0%o, 0%o, xfds 0%o\n",
		 nfds, rfds, wfds, xfds));

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

	  if (Interrupt)
	  {
	    ReadCommands ();
	    Interrupt = 0;
	  }
	  else
	  {
	    panic("Select returns error");
	  }
	  continue /* to end of while loop */ ;
	}

      if (nfds == 0)
	{
	  TimerExpired();
	  continue;
	}

      /* check each fd to see if it has input */
      for (fd = 0; fd <= HighestFD; fd++)
	{
	  /*
	    check all returned fd's; this prevents
	    starvation of later clients by earlier clients
	  */

	  if (rfds & (1 << fd))
	  {
	    if (FDD[fd].InputHandler == NULL)
	      {
		panic("FD selected with no handler");
		debug(1,(stderr, "FD %d has NULL handler\n", fd));
	      }
	    else
	      (FDD[fd].InputHandler)(fd);
	  }
	  if (wfds & (1 << fd))
	  {
	    if (FDD[fd].FlushHandler == NULL)
	    {
	      panic("FD selected with no flush handler");
	    }
	    else
	      (FDD[fd].FlushHandler)(fd);
	  }
	}
    }
}
