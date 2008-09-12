/* ************************************************************ *\
 *								*
 *    Common support routines for sockets			*
 *								*
 *       James L. Peterson	 				*
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
 * 				  				*
 \* *********************************************************** */

#include "scope.h"

#ifdef SYSV
#define bzero(s,l) memset(s, 0, l)
#define bcopy(s,d,l) memmove(d,s,l)
#endif

#include <unistd.h>

/* ********************************************** */
/*						  */
/*       Debugging support routines               */
/*						  */
/* ********************************************** */

void
enterprocedure(s)
     char   *s;
{
  debug(2,(stderr, "-> %s\n", s));
}

void
warn(s)
     char   *s;
{
  fprintf(stderr, "####### %s\n", s);
}

void
panic(s)
     char   *s;
{
  fprintf(stderr, "%s\n", s);
  exit(1);
}

/* ********************************************** */
/*						  */
/*  Debugging forms of memory management          */
/*						  */
/* ********************************************** */

void   *Malloc (long    n)
{
  void   *p;
  p = malloc(n);
  debug(64,(stderr, "%x = malloc(%d)\n", p, n));
  if (p == NULL)
    panic("no more malloc space");
  return(p);
}

void 
Free(void   *p)
{
  debug(64,(stderr, "%x = free\n", p));
  free(p);
}



/* ************************************************************ */
/*								*/
/*    Signal Handling support					*/
/*								*/
/* ************************************************************ */

#include <signal.h>

static void SignalURG(int sig)
{
  debug(1,(stderr, "==> SIGURG received\n"));
}

static void SignalPIPE(int sig)
{
  debug(1,(stderr, "==> SIGPIPE received\n"));
}

static void SignalINT(int sig)
{
  debug(1,(stderr, "==> SIGINT received\n"));
  exit(1);
}

static void SignalQUIT(int sig)
{
  debug(1,(stderr, "==> SIGQUIT received\n"));
  exit(1);
}

static void SignalTERM(int sig)
{
  debug(1,(stderr, "==> SIGTERM received\n"));
  exit(1);
}

static void SignalTSTP(int sig)
{
  debug(1,(stderr, "==> SIGTSTP received\n"));
}

static void SignalCONT(int sig)
{
  debug(1,(stderr, "==> SIGCONT received\n"));
}

void
SetSignalHandling()
{
  enterprocedure("SetSignalHandling");
  signal(SIGURG, SignalURG);
  signal(SIGPIPE, SignalPIPE);
  signal(SIGINT, SignalINT);
  signal(SIGQUIT, SignalQUIT);
  signal(SIGTERM, SignalTERM);
  signal(SIGTSTP, SignalTSTP);
  signal(SIGCONT, SignalCONT);
}



/* ************************************************************ */
/*								*/
/*   Create a socket for a service to listen for clients        */
/*								*/
/* ************************************************************ */

#ifdef USE_XTRANS

#define TRANS_CLIENT
#define TRANS_SERVER
#define X11_t
#include <X11/Xtrans/Xtrans.h>
static XtransConnInfo  *ListenTransConns = NULL;
static int             *ListenTransFds = NULL;
static int              ListenTransCount;

#else

#include <sys/types.h>	       /* needed by sys/socket.h and netinet/in.h */
#include <sys/uio.h>	       /* for struct iovec, used by socket.h */
#include <sys/socket.h>	       /* for AF_INET, SOCK_STREAM, ... */
#include <sys/ioctl.h>	       /* for FIONCLEX, FIONBIO, ... */
#include <sys/fcntl.h>	       /* for FIONCLEX, FIONBIO, ... */
#include <netinet/in.h>	       /* struct sockaddr_in */
#include <netdb.h>	       /* struct servent * and struct hostent *  */

#ifdef DNETCONN
#include <X11/dni.h>
#endif
#ifdef DNETSVR4
#include <X11/dni8.h>
#include <dlfcn.h>
struct hostent *(*dnet_gethostbyname)();
int (*dnet_gethostname)();
short initialize_libdni();
#endif
static int  ON = 1 /* used in ioctl */ ;
#define	BACKLOG	5
#endif

void
SetUpConnectionSocket(iport)
     int     iport;
{
#ifdef USE_XTRANS
  char	port[20];
  int	partial;     
  int   i;
  extern long ServerBasePort;
#else
  FD ConnectionSocket;
  struct sockaddr_in  sin;
  short	port;
#endif

  enterprocedure("SetUpConnectionSocket");

#ifdef USE_XTRANS
  ScopePort = iport - ServerBasePort;
  sprintf (port, "%d", ScopePort);
  if ((_X11TransMakeAllCOTSServerListeners (port, &partial,
        &ListenTransCount, &ListenTransConns) >= 0) &&
        (ListenTransCount >= 1)) {
      if (partial) {
	  debug(4,(stderr, 
	    "Warning: Failed to establish listening connections on some transports\n"));
      } 
      ListenTransFds = (int *) malloc (ListenTransCount * sizeof (int));

      for (i = 0; i < ListenTransCount; i++)
      {
	  int fd = _X11TransGetConnectionNumber (ListenTransConns[i]);
                
	  ListenTransFds[i] = fd;
	  debug(4,(stderr, "Listening on FD %d\n", fd));
	  UsingFD(fd, NewConnection, ListenTransConns[i]);
      }
  } else {
      panic("Could not open any listening connections");
  }
#else

  /* create the connection socket and set its parameters of use */
  ConnectionSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (ConnectionSocket < 0)
    {
      perror("socket");
      exit(-1);
    }
  (void)setsockopt(ConnectionSocket, SOL_SOCKET, SO_REUSEADDR,   (char *)NULL, 0);
  (void)setsockopt(ConnectionSocket, SOL_SOCKET, SO_USELOOPBACK, (char *)NULL, 0);
#ifdef SO_DONTLINGER
  (void)setsockopt(ConnectionSocket, SOL_SOCKET, SO_DONTLINGER,  (char *)NULL, 0);
#endif

  /* define the name and port to be used with the connection socket */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;

  /* the address of the socket is composed of two parts: the host machine and
     the port number.  We need the host machine address for the current host
   */
  {
    /* define the host part of the address */
    char    MyHostName[256];
    struct hostent *hp;

    (void) gethostname(MyHostName, sizeof(MyHostName));
    ScopeHost = (char *) Malloc((long)(1+strlen(MyHostName)));
    strcpy(ScopeHost, MyHostName);
    hp = gethostbyname(MyHostName);
    if (hp == NULL)
      panic("No address for our host");
    bcopy((char *)hp->h_addr, (char*)&sin.sin_addr, hp->h_length);
  }
    /* new code -- INADDR_ANY should be better than using the name of the
       host machine.  The host machine may have several different network
       addresses.  INADDR_ANY should work with all of them at once. */
  sin.sin_addr.s_addr = INADDR_ANY;

  port = iport;
  sin.sin_port = htons (port);
  ScopePort = port;

  /* bind the name and port number to the connection socket */
  if (bind(ConnectionSocket, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
      perror("bind");
      exit(-1);
    }

  debug(4,(stderr, "Socket is FD %d for %s,%d\n",
	   ConnectionSocket, ScopeHost, ScopePort));

  /* now activate the named connection socket to get messages */
  if (listen(ConnectionSocket, BACKLOG) < 0)
    {
      perror("listen");
      exit(-1);
    };

  /* a few more parameter settings */
  ioctl(ConnectionSocket, FIOCLEX, 0);
  ioctl(ConnectionSocket, FIONBIO, &ON);

  debug(4,(stderr, "Listening on FD %d\n", ConnectionSocket));
  UsingFD(ConnectionSocket, NewConnection);
#endif

}

#ifndef USE_XTRANS
#ifdef DNETSVR4
SetUpDECnetConnection(display)
int display;
{
	struct sockaddr_dn *sdn,sdnn;          /* DECnet socket */
	struct hostent *hp;
	int sock;
	char hostname[6];

	if (!initialize_libdni()) {
		fprintf(stderr,"Unable to open libdni.so\n");
		exit(0);
	}
	sdn = &sdnn;
	if (!(dnet_gethostname)(hostname)) {
                fprintf(stderr,"Can't get DECnet local host name\n");
		exit(0);
        }
        if ((hp = (struct hostent *)(dnet_gethostbyname)(hostname)) == NULL) {
                fprintf(stderr,"xhost: can't get name %s\n",hostname);
		exit(0);
        }
	sdn->sdn_family = AF_DECnet;
	sdn->sdn_format = DNADDR_FMT1;
	sdn->sdn_port = 0;
	sprintf ((char *)sdn->sdn_name, "X$X%d", display);
	sdn->sdn_namelen = strlen((char *)sdn->sdn_name);
	sdn->sdn_addr = *(u_short *)hp->h_addr_list[0];

	if ((sock = socket (AF_DECnet, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(0);
        }
	(void)setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,   (char *)NULL, 0);
	(void)setsockopt(sock, SOL_SOCKET, SO_USELOOPBACK, (char *)NULL, 0);

	if (bind(sock, (struct sockaddr_dn *)sdn, 
			sizeof(struct sockaddr_dn)) < 0) {
		perror("DNI Bind");
		exit(0);
	}
	if (listen(sock, 100) < 0) {
		perror("DNI Listen");
		exit(0);
	}
			
	UsingFD(sock, NewConnection);
}
#endif

#ifdef DNETCONN
SetUpDECnetConnection(display)
int display;
{
	struct ses_io_type sesopts;
	char objname[6];
	FD fd;

	enterprocedure("SetUpDECnetConnection");
	sprintf (objname, "X$X%d", display);
	if ((fd = open("/dev/dni", O_RDWR)) < 0) {
		fprintf(stderr,"xscope: dni: open failed\n");
		exit(-1);
	}
	if (ioctl(fd, SES_GET_LINK, 0)) {
		fprintf(stderr,"xscope: dni: can't get link\n");
		exit(-1);
	}
	/* set nonblocking here since dni ignores fcntls */
	/* set asyncronous to avoid blocking on SES_GET_AI */
	sesopts.io_flags = SES_IO_NBIO + SES_IO_ASYNCH_MODE;
	sesopts.io_io_signal = SIGIO;
	sesopts.io_int_signal = 0;
	
	if (ioctl(fd, SES_IO_TYPE, &sesopts) < 0) {
		fprintf(stderr,"xscope: dni: ioctl failed\n");
		exit(-1);
	}
	
	/* register as server */
	if (ioctl(fd, SES_NAME_SERVER, objname) < 0) {
		fprintf(stderr,"xscope: dni: ioctl failed\n");
		exit(-1);
	}
	debug(4,(stderr, "Listening on DECnet FD %d\n", fd));
	UsingFD(fd, NewConnection);
}
#endif

#ifdef DNETSVR4
short  
initialize_libdni()
{
        void *handle;
        char *home;
        char *path;

	if (dnet_gethostname && dnet_gethostbyname)
		return(1);
	if (!(handle = dlopen("/etc/openwin/lib/libdni.so",RTLD_NOW))) {
                if ((home = (char *)getenv("DNI_X_ENABLE")) == NULL)
                        return(0);
                path = (char *)malloc(strlen(home) + 12);
                sprintf(path, "%s%s",home, "/libdni.so");
                if (!(handle = dlopen(path,RTLD_NOW)))
                        return(0);
		free(path);
        }
        if (!(dnet_gethostbyname = (struct hostent *(*)())dlsym(handle,"dni_gethostbyname")))
                return(0);
        if (!(dnet_gethostname = (int (*)())dlsym(handle,"dni_gethostname")))
                return(0);
        return(1);
}
#endif
#endif /* USE_XTRANS */
