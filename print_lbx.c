/*
 * $NCDId$
 *
 * Copyright 1992 Network Computing Devices
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
#include "x11.h"
#include "lbxscope.h"

static unsigned long	sequences[256];
static unsigned char	starting_server[256];
static unsigned char    starting_client[256];
static int		client_client, server_client;

LbxQueryVersion (fd, buf)
register unsigned char *buf;
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* LbxRequest */ ;
  PrintField (buf, 1, 1, LBXREQUEST, LBXREQUESTHEADER) /* LbxSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}

LbxQueryVersionReply (fd, buf)
register unsigned char *buf;
{
  short   n;
  long    m;
  long   k;

  PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* LbxRequest reply */ ;
  PrintField(RBf, 1, 1, LBXREPLY, LBXREPLYHEADER) /* LbxQueryVersion reply */;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  printfield(buf, 4, 4, DVALUE4(0), "reply length");
  PrintField(buf, 8, 2, CARD16, "major-version");
  PrintField(buf, 10, 2, CARD16, "minor-version");
}
    
LbxStartProxy (fd, buf)
  unsigned char	*buf;
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* LbxRequest */ ;
  PrintField (buf, 1, 1, LBXREQUEST, LBXREQUESTHEADER) /* LbxSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}

LbxStopProxy (fd, buf)
  unsigned char	*buf;
{
  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* LbxRequest */ ;
  PrintField (buf, 1, 1, LBXREQUEST, LBXREQUESTHEADER) /* LbxSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
}

LbxNewClient (fd, buf)
register unsigned char	*buf;
{
  unsigned long   c;

  c = ILong(&buf[4]);
  starting_client[c] = 1;
  starting_server[c] = 1;
  sequences[c] = 0;

  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* LbxRequest */ ;
  PrintField (buf, 1, 1, LBXREQUEST, LBXREQUESTHEADER) /* LbxSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  PrintField(buf, 4, 4, CARD32, "new-client-id");
}

LbxCloseClient (fd, buf)
  unsigned char	*buf;
{
}

long    StartSetUpMessage ();
long	StartSetUpReply ();

LbxSwitch (fd, buf)
register unsigned char	*buf;
{
  unsigned long	c;
  c = ILong(&buf[4]);
  CS[fd].SequenceNumber--;
  sequences[client_client] = CS[fd].SequenceNumber;
  if (starting_client[c]) 
  {
    CS[fd].ByteProcessing = StartSetUpMessage;
    CS[fd].NumberofBytesNeeded = 12;
    starting_client[c] = 0;
  }
  CS[fd].SequenceNumber = sequences[c];
  client_client = c;


  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* LbxRequest */ ;
  PrintField (buf, 1, 1, LBXREQUEST, LBXREQUESTHEADER) /* LbxSwitch */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  PrintField(buf, 4, 4, CARD32, "client number");
}

LbxModifySequence (fd, buf)
  unsigned char	*buf;
{
  int	mod;

  mod = ILong(&buf[4]);
  CS[fd].SequenceNumber += mod;

  PrintField (buf, 0, 1, REQUEST, REQUESTHEADER) /* LbxRequest */ ;
  PrintField (buf, 1, 1, LBXREQUEST, LBXREQUESTHEADER) /* LbxModifySequence */ ;
  if (Verbose < 1)
    return;
  if (Verbose > 1)
    PrintField(SBf, 0, 4, CARD32, "sequence number");

  printfield(buf, 2, 2, CONST2(2), "request length");
  printfield(buf, 4, 4, INT32, "adjustment");
}

LbxSwitchEvent (fd, buf)
  unsigned char	*buf;
{
  unsigned long	c;

  c = ILong(&buf[4]);
  if (starting_server[c])
  {
    CS[fd].ByteProcessing = StartSetUpReply;
    CS[fd].NumberofBytesNeeded = 8;
    starting_server[c] = 0;
  }

  PrintField(buf, 0, 1, EVENT, EVENTHEADER) /* LbxEvent */ ;
  PrintField(buf, 1, 1, LBXEVENT, LBXEVENTHEADER) /* LbxSwitchEvent */ ;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, CARD32, "client");
}

LbxCloseEvent (fd, buf)
  unsigned char *buf;
{
  PrintField(buf, 0, 1, EVENT, EVENTHEADER) /* LbxEvent */ ;
  PrintField(buf, 1, 1, LBXEVENT, LBXEVENTHEADER) /* LbxSwitchEvent */ ;
  if (Verbose < 1)
    return;
  printfield(buf, 2, 2, CARD16, "sequence number");
  PrintField(buf, 4, 4, CARD32, "client");
}
