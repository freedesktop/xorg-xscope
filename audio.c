/*
 * $NCDOr: audio.c,v 1.1 1996/07/18 23:25:52 keithp Exp keithp $
 *
 * Copyright 1996 Network Computing Devices
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of NCD. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCD. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCD. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NCD.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, Network Computing Devices
 */

#include "scope.h"
#include "nas.h"

#include <sys/uio.h>	       /* for struct iovec, used by socket.h */
#include <sys/socket.h>	       /* for AF_INET, SOCK_STREAM, ... */
#include <sys/ioctl.h>	       /* for FIONCLEX, FIONBIO, ... */
#ifdef SVR4
#include <sys/filio.h>
#endif
#include <fcntl.h>
#include <netinet/in.h>	       /* struct sockaddr_in */
#include <netinet/tcp.h>
#include <netdb.h>	       /* struct servent * and struct hostent * */
#include <errno.h>	       /* for EINTR, EADDRINUSE, ... */
extern int  errno;

extern char ScopeEnabled;

ReportFromAudioClient (fd, buf, n)
    FD	fd;
    unsigned char   *buf;
    long	    n;
{
    if (NasVerbose) {
	if (ScopeEnabled) {
	    PrintTime ();
	    fprintf(stdout, "NAS Client%s --> %4d %s\n",
		    ClientName(fd), n, (n == 1 ? "byte" : "bytes"));
	}
    }
    ProcessBuffer (fd, buf, n);
}

ReportFromAudioServer(fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
    if (NasVerbose) {
	if (ScopeEnabled) {
	    PrintTime();
	    fprintf(stdout, "\t\t\t\t\t%4d %s <-- NAS Server%s\n",
		    n, (n == 1 ? "byte" : "bytes"), ClientName(fd));
	}
    }
    ProcessBuffer(fd, buf, n);
}

extern long	StartAudioSetUpMessage ();
extern long	FinishAudioSetUpMessage ();
extern long	StartAudioRequest ();
extern long	FinishAudioRequest ();
extern long	StartAudioSetUpReply ();
extern long	FinishAudioSetUpReply ();
extern long	AudioServerPacket ();
extern long	FinishAudioReply ();
extern long	FlushFD ();

StartAudioClientConnection(fd)
     FD fd;
{
  enterprocedure("StartAudioClientConnection");
  /* when a new connection is started, we have no saved bytes */
  CS[fd].SavedBytes = NULL;
  CS[fd].SizeofSavedBytes = 0;
  CS[fd].NumberofSavedBytes = 0;
  CS[fd].NumberofBytesProcessed = 0;

  /* when a new connection is started, we have no reply Queue */
  FlushReplyQ(fd);

  /* each new connection gets a request sequence number */
  CS[fd].SequenceNumber = 0;

  /* we need 12 bytes to start a SetUp message */
  CS[fd].ByteProcessing = StartAudioSetUpMessage;
  CS[fd].NumberofBytesNeeded = 12;
}

StopAudioClientConnection(fd)
     FD fd;
{
  enterprocedure("StopAudioClientConnection");
  /* when a new connection is stopped, discard the old buffer */

  if (CS[fd].SizeofSavedBytes > 0)
    Free((char*)CS[fd].SavedBytes);
}

long    StartAudioSetUpMessage (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  short   namelength;
  short   datalength;
  extern int  littleEndian;

  enterprocedure("StartSetUpMessage");
  /*
    we need the first 12 bytes to be able to determine if, and how many,
    additional bytes we need for name and data authorization.  However, we
    can't process the first 12 bytes until we get all of them, so
    return zero bytes used, and increase the number of bytes needed
  */

  CS[fd].littleEndian = (buf[0] == 'l');
  CS[ServerHalf(fd)].littleEndian = CS[fd].littleEndian;
  littleEndian = CS[fd].littleEndian;
  namelength = IShort(&buf[6]);
  datalength = IShort(&buf[8]);
  CS[fd].ByteProcessing = FinishAudioSetUpMessage;
  CS[fd].NumberofBytesNeeded = n
                               + pad((long)namelength) + pad((long)datalength);
  debug(8,(stderr, "need %d bytes to finish startup\n",
	   CS[fd].NumberofBytesNeeded - n));
  return(0);
}

long    FinishAudioSetUpMessage (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  enterprocedure("FinishSetUpMessage");
  if (ScopeEnabled)
    PrintAudioSetUpMessage(buf);

  /* after a set-up message, we expect a string of requests */
  CS[fd].ByteProcessing = StartAudioRequest;
  CS[fd].NumberofBytesNeeded = 4;
  return(n);
}


long    StartAudioRequest (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  unsigned short   requestlength;
  enterprocedure("StartRequest");

  /* bytes 0,1 are ignored now; bytes 2,3 tell us the request length */
  requestlength = IShort(&buf[2]);
  CS[fd].ByteProcessing = FinishAudioRequest;
  CS[fd].NumberofBytesNeeded = 4 * requestlength;
  debug(8,(stderr, "need %d more bytes to finish request\n",
	   CS[fd].NumberofBytesNeeded - n));
  return(0);
}


long    FinishAudioRequest (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  enterprocedure("FinishRequest");
  CS[fd].ByteProcessing = StartAudioRequest;
  CS[fd].NumberofBytesNeeded = 4;
  if (ScopeEnabled)
      DecodeAudioRequest(fd, buf, n);
  return(n);
}

StartAudioServerConnection(fd)
     FD fd;
{
  enterprocedure("StartAudioServerConnection");
  /* when a new connection is started, we have no saved bytes */
  CS[fd].SavedBytes = NULL;
  CS[fd].SizeofSavedBytes = 0;
  CS[fd].NumberofSavedBytes = 0;
  CS[fd].NumberofBytesProcessed = 0;

  /* when a new connection is started, we have no reply Queue */
  FlushReplyQ(fd);

  /* we need 8 bytes to start a SetUp reply */
  CS[fd].ByteProcessing = StartAudioSetUpReply;
  CS[fd].NumberofBytesNeeded = 8;
}

StopAudioServerConnection(fd)
     FD fd;
{
  enterprocedure("StopAudioServerConnection");
  /* when a new connection is stopped, discard the old buffer */

  if (CS[fd].SizeofSavedBytes > 0)
    Free((char *)CS[fd].SavedBytes);
}

long    StartAudioSetUpReply (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  short   replylength;

  enterprocedure("StartSetUpReply");
  replylength = IShort(&buf[6]);
  CS[fd].ByteProcessing = FinishAudioSetUpReply;
  CS[fd].NumberofBytesNeeded = n + 4 * replylength;
  debug(8,(stderr, "need %d bytes to finish startup reply\n",
	   CS[fd].NumberofBytesNeeded - n));
  return(0);
}

long    FinishAudioSetUpReply (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  enterprocedure("FinishSetUpReply");
  if (ScopeEnabled)
      PrintAudioSetUpReply(buf);
  CS[fd].ByteProcessing = AudioServerPacket;
  CS[fd].NumberofBytesNeeded = 32;
  return(n);
}

/* ************************************************************ */

long    AudioErrorPacket (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  CS[fd].ByteProcessing = AudioServerPacket;
  CS[fd].NumberofBytesNeeded = 32;
  DecodeAudioError(fd, buf, n);
  return(n);
}


long    AudioEventPacket (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  CS[fd].ByteProcessing = AudioServerPacket;
  CS[fd].NumberofBytesNeeded = 32;
  if (ScopeEnabled)
    DecodeAudioEvent(fd, buf, n);
  return(n);
}


long    AudioReplyPacket (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  long   replylength;

  replylength = ILong(&buf[4]);

  /*
    Replies may need more bytes, so we compute how many more
    bytes are needed and ask for them, not using any of the bytes
    we were given (return(0) to say that no bytes were used).
    If the replylength is zero (we don't need any more bytes), the
    number of bytes needed will be the same as what we have, and
    so the top-level loop will call the next routine immediately
    with the same buffer of bytes that we were given.
  */

  CS[fd].ByteProcessing = FinishAudioReply;
  CS[fd].NumberofBytesNeeded = n + 4 * replylength;
  debug(8,(stderr, "need %d bytes to finish reply\n", (4 * replylength)));
  return(0);
}

long    FinishAudioReply (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  CS[fd].ByteProcessing = AudioServerPacket;
  CS[fd].NumberofBytesNeeded = 32;
  enterprocedure("FinishAudioReply");
  if (ScopeEnabled)
    DecodeAudioReply(fd, buf, n);
  return(n);
}

long    AudioServerPacket (fd, buf, n)
     FD fd;
     unsigned char *buf;
     long    n;
{
  short   PacketType;
  enterprocedure("ServerPacket");

  PacketType = IByte(&buf[0]);
  if (PacketType == 0)
    return(AudioErrorPacket(fd, buf, n));
  if (PacketType == 1)
    return(AudioReplyPacket(fd, buf, n));
  return(AudioEventPacket(fd, buf, n));
}

CloseAudioConnection(fd)
     FD fd;
{
  debug(4,(stderr, "close %d and %d\n", fd, FDPair(fd)));
  StopAudioClientConnection(ServerHalf(fd));
  StopAudioServerConnection(ClientHalf(fd));

  (void)close(fd);
  NotUsingFD(fd);
  (void)close(FDPair(fd));
  NotUsingFD(FDPair(fd));
}

/* when we get data from a client, we read it in, copy it to the
   server for this client, then dump it to the client. Note, we don't
   have to have a server, if there isn't one. */

DataFromAudioClient(fd)
     FD fd;
{
  long    n;
  FD ServerFD;

  Verbose = NasVerbose;
  enterprocedure("DataFromAudioClient");
  ServerFD = FDPair(fd);
  if (ServerFD < 0)
    {
      ServerFD = ConnectToServer(false);
      if (ServerFD < 0)
      {
	CloseAudioConnection(fd);
	return;
      }
      SetUpPair(fd, ServerFD);
    }

  n = read(fd, FDinfo[ServerFD].buffer, BUFFER_SIZE);
  debug(4,(stderr, "read %d bytes from Client%s\n", n, ClientName(fd)));
  if (n < 0)
    {
      PrintTime();
      perror("Client --> read error:");
      CloseAudioConnection(fd);
      return;
    }
  if (n == 0)
    {
      PrintTime();
      if (NasVerbose >= 0)
	fprintf(stdout, "Client%s --> EOF\n", ClientName(fd));
      CloseAudioConnection(fd);
      return;
    }

  FDinfo[ServerFD].bufcount = n;
  FDinfo[ServerFD].bufstart = 0;

  FlushFD (ServerFD);
  /* also report the bytes to standard out */
  ReportFromAudioClient(fd, FDinfo[ServerFD].buffer, n);
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

/* similar situation for the server, but note that if there is no client,
   we close the connection down -- don't need a server with no client. */

DataFromAudioServer(fd)
     FD fd;
{
  long    n;
  FD ClientFD;

  Verbose = NasVerbose;
  ClientFD = FDPair(fd);
  if (ClientFD < 0)
    {
      CloseAudioConnection(fd);
      return;
    }

  enterprocedure("DataFromAudioServer");
  n = read(fd, (char *)FDinfo[ClientFD].buffer, BUFFER_SIZE);
  debug(4,(stderr, "read %d bytes from Server%s\n", n, ClientName(fd)));
  if (n < 0)
    {
      PrintTime();
      perror("read error <- Server");
      CloseAudioConnection(fd);
      return;
    }
  if (n == 0)
    {
      PrintTime();
      if (NasVerbose >= 0)
	fprintf(stdout, "EOF <-- Server%s\n", ClientName(fd));
      CloseAudioConnection(fd);
      return;
    }

  FDinfo[ClientFD].bufcount = n;
  FDinfo[ClientFD].bufstart = 0;
  FlushFD (ClientFD);

  /* also report the bytes to standard out */
  ReportFromAudioServer(fd, FDinfo[ClientFD].buffer, n);
}

FD ConnectToAudioClient(ConnectionSocket)
     FD ConnectionSocket;
{
  FD ClientFD;
  ClientFD = AcceptConnection(ConnectionSocket);
  UsingFD(ClientFD, DataFromAudioClient, FlushFD);
  StartAudioClientConnection(ClientFD);
  return(ClientFD);
}

FD ConnectToAudioServer(report)
     Boolean report;
{
  FD ServerFD;
  if (!AudioServerHostName[0])
    strcpy (AudioServerHostName, ServerHostName);
  ServerFD = MakeConnection (AudioServerHostName, GetServerport () + 2000, report);
  if (ServerFD >= 0)
    {
      UsingFD(ServerFD, DataFromAudioServer, FlushFD);
      StartAudioServerConnection(ServerFD);
    }
  return(ServerFD);
}

NewAudio (fd)
  FD  fd;
{
  FD ServerFD = -1;
  FD ClientFD = -1;

  ClientFD = ConnectToAudioClient(fd);
  ServerFD = ConnectToAudioServer(true);
  SetUpPair(ClientFD, ServerFD);
}

InitializeAudio ()
{
    InitializeAudioDecode ();
}
