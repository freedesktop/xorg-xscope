/* ******************************************************
 *						      	*
 *  A spy program to reveal X11  traffic	    	*
 *						      	*
 *	James Peterson, 1988	       			*
 * Copyright (C) 1988 MCC
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
 *							*
 *							*
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
 ***************************************************** */

#include "scope.h"

#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#ifdef SYSV
#define bzero(s,l) memset(s, 0, l)
#define bcopy(s,d,l) memmove(d,s,l)
#endif

/* ********************************************** */
/*                                                */
/* ********************************************** */

#define DefaultPort 6000

static char    ServerHostName[MAXHOSTNAMELEN];
       long    ServerBasePort = DefaultPort;
static long    ServerInPort = 1;
static long    ServerOutPort = 0;
static long    ServerDisplay = 0;

#ifdef USE_XTRANS
#undef DNETCONN
#undef DNETSVR4
#endif

#ifdef DNETCONN
#include <X11/dni.h>
#include <sys/fcntl.h>
int	decnet_in = 0;
int	decnet_out = 0;
int	decnet_server = 0;
#endif
#ifdef DNETSVR4
#include <X11/dni8.h>
extern struct hostent *(*dnet_gethostbyname)();
extern int (*dnet_gethostname)();
short initialize_libdni();
int     decnet_in = 0;
int     decnet_out = 0;
int     decnet_server = 0;
#endif

static FD ConnectToClient(FD ConnectionSocket);
static FD ConnectToServer(int report);
static void SetUpStdin(void);


/* ********************************************** */
/*                                                */
/*                                                */
/* ********************************************** */

static short	GetServerport ()
{
  short     port;

  enterprocedure("GetServerport");

#if defined(DNETCONN) || defined(DNETSVR4)
  if (decnet_server) {
	  port = ServerDisplay + ServerOutPort;
	  return(port);
  }
#endif
  port = ServerBasePort + ServerOutPort + ServerDisplay;
  debug(4,(stderr, "Server service is on port %d\n", port));
  return(port);
}

static short     GetScopePort ()
{
  short     port;

  enterprocedure("GetScopePort");

#if defined(DNETCONN) || defined(DNETSVR4)
  if (decnet_in) {
	  port = ServerInPort + ServerDisplay;
	  return(port);
  }
#endif
  port = ServerBasePort + ServerInPort + ServerDisplay;
  debug(4,(stderr, "scope service is on port %d\n", port));
  return(port);
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

static void 
Usage()
{
  fprintf(stderr, "Usage: xscope\n");
  fprintf(stderr, "              [-h<server-host>]\n");
  fprintf(stderr, "              [-i<in-port>]\n");
  fprintf(stderr, "              [-o<out-port>]\n");
  fprintf(stderr, "              [-d<display-number>]\n");
  fprintf(stderr, "              [-v<n>]  -- verbose output\n");
#ifdef RAW_MODE
  fprintf(stderr, "              [-r]  -- raw output\n");
#endif
  fprintf(stderr, "              [-q]  -- quiet output\n");
  fprintf(stderr, "              [-D<debug-level>]\n");
  exit(1);
}

static void
ScanArgs(argc, argv)
     int     argc;
     char  **argv;
{
#if defined(DNETCONN) || defined(DNETSVR4)
  char *ss;
#endif
  Verbose = 1 /* default verbose-ness level */;
#ifdef RAW_MODE
  Raw = 0 ;
#endif

  /* Scan argument list */
  while (--argc > 0)
    {
      ++argv;
      if (**argv == '-')
	switch (*++*argv)
	  {
	    /*
	      debug levels:
	      	2 - trace each procedure entry
	      	4 - I/O, connections
	      	8 - Scope internals
	      	16 - Message protocol
		32 - 64 - malloc
		128 - 256 - really low level
	    */
	  case 'D':
	    debuglevel = atoi(++*argv);
	    if (debuglevel == 0)
	      debuglevel = 255;
	    debuglevel |= 1;
	    Verbose = 7;
	    debug(1,(stderr, "debuglevel = %d\n", debuglevel));
	    break;

	  case 'q': /* quiet mode */
	    Verbose = 0;
	    debug(1,(stderr, "Verbose = %d\n", Verbose));
	    break;

	  case 'v': /* verbose mode */
	    Verbose = atoi(++*argv);
	    debug(1,(stderr, "Verbose = %d\n", Verbose));
	    break;

#ifdef RAW_MODE
	  case 'r': /* raw mode */
	    Raw = 1 ;
	    debug(1,(stderr, "Raw = %d\n", Raw));
	    break;	    
#endif

	  case 'o':
	    ServerOutPort = atoi(++*argv);
	    if (ServerOutPort <= 0)
	      ServerOutPort = 0;
	    debug(1,(stderr, "ServerOutPort = %d\n", ServerOutPort));
	    break;

	  case 'd':
	    ServerDisplay = atoi(++*argv);
	    if (ServerDisplay <= 0)
	      ServerDisplay = 0;
	    debug(1,(stderr, "ServerDisplay=%d\n", ServerDisplay));
	    break;

	  case 'i':
#if defined(DNETCONN) || defined(DNETSVR4)
	    if (ss = (char *)strchr(*argv,':')) {
		decnet_in = 1;
		*ss = NULL;
	    }
#endif
	    ServerInPort = atoi(++*argv);
	    if (ServerInPort <= 0)
	      ServerInPort = 0;
	    debug(1,(stderr, "ServerInPort = %d\n", ServerInPort));
	    break;

	  case 'h':
#if defined(DNETCONN) || defined(DNETSVR4)
	    if (ss = (char *)strchr(*argv,':')) {
		decnet_server = 1;
		*ss = NULL;
		strcpy(ServerHostName,++*argv);
		break;
	    }
#endif
	    if (++*argv != NULL && **argv != '\0' 
	      && (strlen(*argv) < sizeof(ServerHostName)))
	      strcpy(ServerHostName, *argv);
	    debug(1,(stderr, "ServerHostName=%s\n", ServerHostName));
	    break;

	  default:
	    fprintf(stderr, "Unknown option %c\n", **argv);
	    Usage();
	    break;

	  }
      else
	{
	  /* file argument to scope -- error */
	  Usage();
	}
    }

  /* check for different port numbers or different machines */
  if (ServerInPort == ServerOutPort)
    if (ServerHostName[0] == '\0')
      {
	fprintf(stderr, "Can't have xscope on same port as server (%d)\n",
		ServerInPort);
	Usage();
      }

}


/* ********************************************** */
/*                                                */
/* ********************************************** */

main(argc, argv)
     int     argc;
     char  **argv;
{
  ScanArgs(argc, argv);
  InitializeFD();
  InitializeX11();
  SetUpStdin();
#if defined(DNETCONN) || defined(DNETSVR4)
  if (decnet_in) 
	SetUpDECnetConnection(GetScopePort());
  else
	SetUpConnectionSocket(GetScopePort());
#else
  SetUpConnectionSocket(GetScopePort());
  SetSignalHandling();
#endif

  return MainLoop();
}

void
TimerExpired()
{
  debug(16,(stderr, "Timer tick\n"));
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

/*
  here is where we would add code to allow control from
  the keyboard.  We would want to read a command and
  interpret it.  Possibilties:

  (a) verbose level setting
  (b) reset time
  (c) save X requests to a file.
  (d) replay X requests from a file.
  (e) allow fake events, errors to be generated.
*/

static void
ReadStdin(fd)
     FD fd;
{
  char    buf[2048];
  long    n;

  enterprocedure("ReadStdin");
  n = read(fd, buf, 2048);
  debug(4,(stderr, "read %d bytes from stdin\n", n));
}

static void
SetUpStdin()
{
  enterprocedure("SetUpStdin");
  UsingFD(fileno(stdin), ReadStdin, NULL);
}

/* ************************************************************ */
/*								*/
/*								*/
/* ************************************************************ */

/*
  xscope is really meant to look at one client at a time.  However,
  it can easily handle multiple clients and servers.  To do so,
  we need to have a pair of FDs: one for the client and one for the
  server for that client.  If either goes away, so does the other.
  We need to be able to identify the other FD of a pair, so that if
  we get input from one, we can write it to the other.
*/

struct fdinfo
{
  Boolean Server;
  long    ClientNumber;
  FD pair;
};

static long ClientNumber = 0;
static struct fdinfo   FDinfo[StaticMaxFD];

static void
SetUpPair(client, server)
     FD client;
     FD server;
{
  if (client >= 0)
    {
      ClientNumber += 1;
      FDinfo[client].Server = false;
      FDinfo[client].pair = server;
      FDinfo[client].ClientNumber = ClientNumber;
      if (server >= 0)
	{
	  FDinfo[server].Server = true;
	  FDinfo[server].pair = client;
	  FDinfo[server].ClientNumber = FDinfo[client].ClientNumber;
	}
    }
  else if (server >= 0)
      {
	close(server);
	NotUsingFD(server);
      }
}

static void
CloseConnection(fd)
     FD fd;
{
  debug(4,(stderr, "close %d and %d\n", fd, FDPair(fd)));
  StopClientConnection(ServerHalf(fd));
  StopServerConnection(ClientHalf(fd));

#ifdef USE_XTRANS
  _X11TransClose(GetXTransConnInfo(fd));
  _X11TransClose(GetXTransConnInfo(FDPair(fd)));
#else
  close(fd);
  close(FDPair(fd));
#endif
  NotUsingFD(fd);
  NotUsingFD(FDPair(fd));
}

/* ************************************************************ */

FD FDPair(fd)
     FD fd;
{
  return(FDinfo[fd].pair);
}

FD ClientHalf(fd)
     FD fd;
{
  if (FDinfo[fd].Server)
    return(FDinfo[fd].pair);
  return(fd);
}

FD ServerHalf(fd)
     FD fd;
{
  if (FDinfo[fd].Server)
    return(fd);
  return(FDinfo[fd].pair);
}

char   *ClientName (fd)
     FD fd;
{
  static char name[12];

  if (ClientNumber <= 1)
    return("");
  sprintf(name, " %d", FDinfo[fd].ClientNumber);
  return(name);
}


/* ********************************************** */
/*                                                */
/* ********************************************** */

/* when we get data from a client, we read it in, copy it to the
   server for this client, then dump it to the client. Note, we don't
   have to have a server, if there isn't one. */

static void
DataFromClient(fd)
     FD fd;
{
  unsigned char    buf[2048];
  long    n;
  FD ServerFD;

  enterprocedure("DataFromClient");
  n = read(fd, (char *)buf, 2048);
  debug(4,(stderr, "read %d bytes from Client%s\n", n, ClientName(fd)));
  if (n < 0)
    {
      PrintTime();
      perror("Client --> read error:");
      CloseConnection(fd);
      return;
    }
  if (n == 0)
    {
      PrintTime();
      fprintf(stdout, "Client%s --> EOF\n", ClientName(fd));
      CloseConnection(fd);
      return;
    }

  ServerFD = FDPair(fd);
  if (ServerFD < 0)
    {
      ServerFD = ConnectToServer(false);
      SetUpPair(fd, ServerFD);
    }

  /* write bytes from client to server, allow for server to fail */
  if (ServerFD >= 0)
    {
      long    BytesToWrite = n;
      unsigned char   *p = buf;

      while (BytesToWrite > 0)
	{
	  int     n1 = write (ServerFD, (char *)p, (int)BytesToWrite);
	  debug(4,(stderr, "write %d bytes to Server%s\n", n1, ClientName(fd)));
	  if (n1 > 0)
	    {
	      BytesToWrite -= n1;
	      p += n1;
	    }
      /*
         B U G : 4156754

         Write may fail with an error code of EAGAIN if
         BytesToWrite is less than PIPE_BUF, but bigger
         than available free space. This error says, wait
         and try again till PIPE is flushed and has more
         free space.
       */
      else if (errno == EAGAIN) 
      {
         /* Wait for some time and try again */
         sleep (1);
      }
	  else
	    {
			    perror("Error on write to Server");
	      CloseConnection(fd);
	      BytesToWrite = 0;
	    }
	}
    }

  /* also report the bytes to standard out */
  ReportFromClient(fd, buf, n);
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

/* similar situation for the server, but note that if there is no client,
   we close the connection down -- don't need a server with no client. */

static void
DataFromServer(fd)
     FD fd;
{
  unsigned char    buf[2048];
  long    n;
  FD ClientFD;

  enterprocedure("DataFromServer");
  n = read(fd, (char *)buf, 2048);
  debug(4,(stderr, "read %d bytes from Server%s\n", n, ClientName(fd)));
  if (n < 0)
    {
      PrintTime();
      perror("read error <- Server");
      CloseConnection(fd);
      return;
    }
  if (n == 0)
    {
      PrintTime();
      fprintf(stdout, "EOF <-- Server%s\n", ClientName(fd));
      CloseConnection(fd);
      return;
    }

  ClientFD = FDPair(fd);
  if (ClientFD < 0)
    {
      CloseConnection(fd);
      return;
    }

  /* write bytes from server to client, allow for client to fail */
  {
    long    BytesToWrite = n;
    unsigned char   *p = buf;
    while (BytesToWrite > 0)
      {
	int     n1 = write (ClientFD, (char *)p, (int)BytesToWrite);
	debug(4,(stderr, "write %d bytes to Client%s\n", n1, ClientName(fd)));
	if (n1 > 0)
	  {
	    BytesToWrite -= n1;
	    p += n1;
	  }
      /*
         B U G : 4156754

         Write may fail with an error code of EAGAIN if
         BytesToWrite is less than PIPE_BUF, but bigger
         than available free space. This error says, wait
         and try again till PIPE is flushed and has more
         free space.
       */
      else if (errno == EAGAIN) 
      {
         /* Wait for some time and try again */
         sleep (1);
      }
	else
	  {
			perror("Error on write to Client");
	    CloseConnection(fd);
	    BytesToWrite = 0;
	  }
      }
  }

  /* also report the bytes to standard out */
  ReportFromServer(fd, buf, n);
}



/* ************************************************************ */
/*								*/
/*     Create New Connection to a client program and to Server  */
/*								*/
/* ************************************************************ */

#include <sys/types.h>	       /* needed by sys/socket.h and netinet/in.h */
#include <sys/uio.h>	       /* for struct iovec, used by socket.h */
#include <sys/socket.h>	       /* for AF_INET, SOCK_STREAM, ... */
#include <sys/ioctl.h>	       /* for FIONCLEX, FIONBIO, ... */
#include <netinet/in.h>	       /* struct sockaddr_in */
#include <netdb.h>	       /* struct servent * and struct hostent * */
#include <errno.h>	       /* for EINTR, EADDRINUSE, ... */
extern int  errno;

static int  ON = 1 /* used in ioctl */ ;

void
NewConnection(fd)
     FD fd;
{
  FD ServerFD = -1;
  FD ClientFD = -1;

#ifdef DNETCONN
  if (decnet_in)
	  ClientFD = ConnectToDECnetClient(fd);
  else
	  ClientFD = ConnectToClient(fd);
  if (decnet_server)
	  ServerFD = ConnectToDECnetServer(true);
  else
	  ServerFD = ConnectToServer(true);
#endif
#ifdef DNETSVR4
  ClientFD = ConnectToClient(fd);
  if (decnet_server) {
	  if (!initialize_libdni()) {
                fprintf(stderr,"Unable to open libdni.so\n");
                exit(0);
	  }
          ServerFD = ConnectToDECnetSVR4Server(true);
  }
  else
          ServerFD = ConnectToServer(true);
#endif
#if !(defined(DNETCONN)) && !(defined(DNETSVR4)) 
  ClientFD = ConnectToClient(fd);
  ServerFD = ConnectToServer(true);

#endif
  SetUpPair(ClientFD, ServerFD);
}


/* ************************************************************ */

static FD ConnectToClient(ConnectionSocket)
     FD ConnectionSocket;
{
  FD ClientFD;
  XtransConnInfo trans_conn = NULL;
#ifdef USE_XTRANS
  XtransConnInfo listen_conn;
  int status;
#else
  struct sockaddr_in  from;
  int    len = sizeof (from);
#endif

  enterprocedure("ConnectToClient");

#ifdef USE_XTRANS
  listen_conn = GetXTransConnInfo(ConnectionSocket);
  if ((trans_conn = _X11TransAccept (listen_conn, &status)) == NULL) {
      debug(4,(stderr, "Failed to accept connection\n"));
      return -1;
  }
  _X11TransSetOption(trans_conn, TRANS_NONBLOCKING, 1);
  ClientFD = _X11TransGetConnectionNumber(trans_conn);  
#else
  ClientFD = accept(ConnectionSocket, (struct sockaddr *)&from, &len);
#endif
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

  UsingFD(ClientFD, DataFromClient, trans_conn);
#ifndef USE_XTRANS
  ioctl(ClientFD, FIOCLEX, 0);
  ioctl(ClientFD, FIONBIO, &ON);
#endif
  StartClientConnection(ClientFD);
  return(ClientFD);
}

/* ************************************************************ */
/*								*/
/*								*/
/* ************************************************************ */



static FD ConnectToServer(report)
     Boolean report;
{
  FD ServerFD;
  XtransConnInfo trans_conn = NULL;   /* transport connection object */
#ifdef USE_XTRANS
  char address[256];
  int connect_stat;
  extern long ServerBasePort;
#else
  struct sockaddr_in  sin;
  struct hostent *hp;
#endif
  short port;

  enterprocedure("ConnectToServer");

  /* determine the host machine for this process */
  if (ServerHostName[0] == '\0')
    (void) gethostname(ServerHostName, sizeof (ServerHostName));
  debug(4,(stderr, "try to connect on %s\n", ServerHostName));
  port = GetServerport ();

  if (port == ScopePort
      && strcmp(ServerHostName, ScopeHost) == 0)
    {
      char error_message[100];
      sprintf(error_message, "Trying to attach to myself: %s,%d\n",
	      ServerHostName, port);
      panic(error_message);
    }

#ifdef USE_XTRANS
  snprintf (address, sizeof(address), "%s:%d", ServerHostName, 
    port - ServerBasePort);
  if ( (trans_conn = _X11TransOpenCOTSClient(address)) == NULL ) {
      debug(1,(stderr, "OpenCOTSClient failed\n"));
      panic("Can't open connection to Server");
  }
  if ((connect_stat = _X11TransConnect(trans_conn,address)) < 0 ) {
      _X11TransClose(trans_conn);
      trans_conn = NULL;
      debug(1,(stderr, "TransConnect failed\n"));
      panic("Can't open connection to Server");
  }

  ServerFD = _X11TransGetConnectionNumber(trans_conn);
#else
  /* establish a socket to the name server for this host */
  bzero((char *)&sin, sizeof(sin));
  ServerFD = socket(AF_INET, SOCK_STREAM, 0);
  if (ServerFD < 0)
    {
      perror("socket() to Server failed");
      debug(1,(stderr, "socket failed\n"));
      panic("Can't open connection to Server");
    }
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR,  (char *) NULL, 0);
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_USELOOPBACK,(char *) NULL, 0);
#ifdef SO_DONTLINGER
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_DONTLINGER, (char *) NULL, 0);
#endif

  hp = gethostbyname(ServerHostName);
  if (hp == 0)
    {
      perror("gethostbyname failed");
      debug(1,(stderr, "gethostbyname failed for %s\n", ServerHostName));
      panic("Can't open connection to Server");
    }

  sin.sin_family = AF_INET;
  bcopy((char *)hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
  sin.sin_port = htons (port);

  /* ******************************************************** */
  /* try to connect to Server */

  if (connect(ServerFD, (struct sockaddr *)&sin, sizeof(sin)) < 0)
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
	  close(ServerFD);
	  debug(1,(stderr, "No Server\n"));
	  if (report)
	    warn("Can't open connection to Server");
	  return(-1);

	default:
	  close(ServerFD);
	  panic("Can't open connection to Server");
	}
    }
#endif

  debug(4,(stderr, "Connect To Server: FD %d\n", ServerFD));
  if (ServerFD >= 0)
    {
      UsingFD(ServerFD, DataFromServer, trans_conn);
      StartServerConnection(ServerFD);
    }
  return(ServerFD);
}

#ifdef DNETSVR4
FD ConnectToDECnetSVR4Server(report)
     Boolean report;
{
  FD ServerFD;
  struct sockaddr_dn  sdn;
  struct hostent *hp;

  enterprocedure("ConnectToServer");

  /* establish a socket to the name server for this host */
  ServerFD = socket(AF_DECnet, SOCK_STREAM, 0);
  if (ServerFD < 0)
    {
      perror("socket() to Server failed");
      debug(1,(stderr, "socket failed\n"));
      panic("Can't open connection to Server");
    }
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR,  (char *) NULL, 0);
  (void) setsockopt(ServerFD, SOL_SOCKET, SO_USELOOPBACK,(char *) NULL, 0);
  /* determine the host machine for this process */
  initialize_libdni();
  if (ServerHostName[0] == '\0')
  	(dnet_gethostname)(ServerHostName);
  debug(4,(stderr, "try to connect on %s\n", ServerHostName));

  hp = (struct hostent *)(dnet_gethostbyname)(ServerHostName);
  if (hp == 0)
    {
      perror("gethostbyname failed");
      debug(1,(stderr, "gethostbyname failed for %s\n", ServerHostName));
      panic("Can't open connection to Server");
    }

  sdn.sdn_family = AF_DECnet;
  sdn.sdn_format = DNADDR_FMT1;
  sdn.sdn_port = 0;
  sprintf (sdn.sdn_name, "X$X%d", GetServerport() );
  sdn.sdn_namelen = strlen(sdn.sdn_name);
  sdn.sdn_addr = *(u_short *)hp->h_addr_list[0];

  /* ******************************************************** */
  /* try to connect to Server */

  if (connect(ServerFD, (struct sockaddr *)&sdn, sizeof(sdn)) < 0)
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
	  close(ServerFD);
	  debug(1,(stderr, "No Server\n"));
	  if (report)
	    warn("Can't open connection to Server");
	  return(-1);

	default:
	  close(ServerFD);
	  panic("Can't open connection to Server");
	}
    }

  debug(4,(stderr, "Connect To Server: FD %d\n", ServerFD));
  if (ServerFD >= 0)
    {
      UsingFD(ServerFD, DataFromServer, NULL);
      StartServerConnection(ServerFD);
    }
  return(ServerFD);
}
#endif

/* ********************************************** */
/*                                                */
/* ********************************************** */

#ifdef DNETCONN
FD ConnectToDECnetClient(fd)
     FD fd;
{
	struct ses_io_type sesopts;
	static SessionData sd= {0, {0, ""}};
	
	if (ioctl(fd, SES_ACCEPT, &sd) < 0) {
		fprintf(stderr,"xscope: dni: SES_ACCEPT failed\n");
		exit(-1);
        }
	UsingFD(fd, DataFromClient, NULL);
	StartClientConnection(fd);
	/* unlike sockets, dni consumes the fd on which it was listening */
        /* in order to accept new logical link requests using the same name */
        /* we must re-open the logical link device and re-supply the */
        /* appropriate access control information */	

	SetUpDECnetConnection(GetScopePort());

	return(fd);
}


FD ConnectToDECnetServer(report)
     Boolean report; 
{
	FD fd;
	OpenBlock opblk;
	struct ses_io_type sesopts;
	struct nodeent *np;

	if ((fd = open("/dev/dni", O_RDWR)) < 0) {
		fprintf(stderr,"xscope: dni: open failed\n");
		exit(-1);
	}
	if (ioctl(fd, SES_GET_LINK, 0)) {
		fprintf(stderr,"xscope: dni: can't get link\n");
		exit(-1);
	}

	/* set nonblocking here since dni can't handle fcntls */
	sesopts.io_flags = SES_IO_NBIO;
	sesopts.io_io_signal = sesopts.io_int_signal = 0;

	if (ioctl(fd, SES_IO_TYPE, &sesopts) < 0) {
		fprintf(stderr,"xscope: dni: ioctl failed\n");
		exit(-1);
	}

	strncpy(opblk.op_node_name,ServerHostName , 6);  /* dni server name */
	opblk.op_node_name[6] = '\0';
	sprintf(opblk.op_task_name,  "X$X%d", GetServerport());
	opblk.op_userid[0] = '\0';          /* No one checks our id */
	opblk.op_object_nbr = 0;            /* Any fields not used */
	opblk.op_account[0] = '\0';         /* should be set to zero */
	opblk.op_password[0] = '\0';
	opblk.op_opt_data.im_length = 0;

	if (ioctl(fd, SES_LINK_ACCESS, &opblk) == -1) {
		fprintf(stderr,"xscope: dni: cannot connect to server\n");
		exit(-1);
	}
        UsingFD(fd, DataFromServer, NULL);
        StartServerConnection(fd);
	return(fd);
		
}
#endif

