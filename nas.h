/* ************************************************************ *
 *						     		*
 *  Type definitions and Connection State for the  NAS protocol *
 *						      		*
 *	James Peterson, 1988			      		*
 *	(c) Copyright MCC, 1988 		      		*
 *						      		*
 * ************************************************************ */


#include "x11.h"

/* ************************************************************ */
/*								*/
/*								*/
/* ************************************************************ */

/* 
  In general, we are called with a buffer of bytes and are supposed to
  try to make sense of these bytes according to the NAS protocol.  There
  are two different types of communication: requests from the client to
  the server and replies/errors/events from the server to the client.
  We must interpret these two differently.

  Also, we must consider that the bytes on the communication socket may
  be sent asynchronously in any amount.  This means that we must be prepared
  to read in and save some bytes until we get enough to do something with
  them.  For example, suppose that we get a buffer from a client.  We would
  expect it to be a request.  The request may be 24 bytes long.  We may find,
  however, that only 16 bytes have actually arrived -- the other 8 are stuck
  in a buffer somewhere.  We must be prepared to simply hold the 16 bytes we
  have until more bytes arrive.

  In general, we do two things: we wait for some number of bytes, and
  then we interpret this set of bytes.  To interpret this data we use 
  a modified state machine.  We keep two pieces of information:

  (1) the number of bytes that we need
  (2) what to do with those bytes.

  This last piece of information is the "state" of the interpretation.
  We keep the state as a pointer to the procedure that is to be executed.


  CLIENTS:

  The data going from the client to the x11 server consists of a
  set-up message followed by an infinite stream of variable length
  requests.  

  Our overall flow is then:

  (a) Wait for 12 bytes.
  (b) Interpret these first 12 bytes of the set-up message to get the
      length of the rest of the message.
  (c) Wait for the rest of the set-up message.
  (d) Interpret and print the set-up message.
  
  *** end of set-up phase -- start normal request loop ***

  (e) Wait for 4 bytes.
  (f) Interpret these 4 bytes to get the length of the rest of the command.
  (g) Wait for the rest of the command.
  (h) Interpret and print the command.
  (i) Go back to step (e).

  SERVERS:

  Again, we have a set-up reply followed by an infinite stream of variable
  length replies/errors/events.

  Our overall flow is then:

  (a) Wait for 8 bytes.
  (b) Interpret the 8 bytes to get the length of the rest of the set-up reply.
  (c) Wait for the rest of the set-up reply.
  (d) Interpret and print the set-up reply.

  *** end of set-up phase -- start normal reply/error/event loop ***

  We have the following properties of NAS replies, errors, and events:

  Replies:  32 bytes plus a variable amount.  Byte 0 is 1.
            Bytes 2-3 are a sequence number; bytes 4-7 are length (n).  The
	    complete length of the reply is 32 + 4 * n.

  Errors:   32 bytes.  Byte 0 is 0.
            Byte 1 is an error code; bytes 2-3 are a sequence number.
	    Bytes 8-9 are a major opcode; byte 10 is a minor opcode.

  Events:   32 bytes.  Byte 0 is 2, 3, 4, ....

  Looking at this we have two choices:  wait for one byte and then separately
  wait for replies, errors, and events, or wait for 32 bytes, then separately
  process each type.  We may have to wait for more, in the event of a reply.
  This latter seems more effective.  It appears reply/error/event formats
  were selected to allow waiting for 32 bytes, and it will allow short packets
  which are only 32 bytes long, to be processed completely in one step.
  
  Thus, For normal reply/error/event processing we have 

  (e) Wait for 32 bytes.
  (f) Interpret these 32 bytes.  If possible, go back to step (e).
  (g) If the packet is a reply with bytes 4-7 non-zero, wait for the
      remainder of the the reply.
  (h) Interpret and print the longer reply.  Go back to step (e).
  

  The similarity in approach to how both the client and server are handled
  suggests we can use the same control structure to drive the interpretation
  of both types of communication client->server and server->client.  
  Accordingly, we package up the relevant variables in a ConnState
  record.  The ConnState record contains the buffer of saved bytes (if any),
  the size and length of this buffer, the number of bytes we are waiting for
  and what to do when we get them.  A separate ConnState record is kept
  for the client and server.

  In addition, we may have several different client or server connections.
  Thus we need an array of all the necessary state for each client or server.
  A client/server is identified with a file descriptor (fd), so we use the
  fd to identify the client/server and use it as an index into an array of
  state variables.
*/

/* audio.c */
extern void NewAudio (FD fd);
extern void InitializeAudio (void);

/* decodenas.c */
extern void DecodeAudioRequest	(FD fd, unsigned char *buf, long n);
extern void DecodeAudioReply	(FD fd, unsigned char *buf, long n);
extern void DecodeAudioEvent	(FD fd, unsigned char *buf, long n);
extern void DecodeAudioError	(FD fd, unsigned char *buf, long n);

extern void InitializeAudioDecode (void);

/* printnas.c */
extern void PrintAudioSetUpMessage		(unsigned char *buf);
extern void PrintAudioSetUpReply		(unsigned char *buf);
extern void AudioElementNotify			(unsigned char *buf);
extern void AudioGrabNotify			(unsigned char *buf);
extern void AudioMonitorNotify			(unsigned char *buf);
extern void AudioBucketNotify			(unsigned char *buf);
extern void AudioDeviceNotify			(unsigned char *buf);
extern void UnknownAudioReply			(unsigned char *buf);
extern void AudioListDevices			(unsigned char *buf);
extern void AudioListDevicesReply		(unsigned char *buf);
extern void AudioGetDeviceAttributes		(unsigned char *buf);
extern void AudioGetDeviceAttributesReply	(unsigned char *buf);
extern void AudioSetDeviceAttributes		(unsigned char *buf);
extern void AudioCreateBucket			(unsigned char *buf);
extern void AudioDestroyBucket			(unsigned char *buf);
extern void AudioListBuckets			(unsigned char *buf);
extern void AudioListBucketsReply		(unsigned char *buf);
extern void AudioGetBucketAttributes		(unsigned char *buf);
extern void AudioGetBucketAttributesReply	(unsigned char *buf);
extern void AudioSetBucketAttributes		(unsigned char *buf);
extern void AudioCreateRadio			(unsigned char *buf);
extern void AudioDestroyRadio			(unsigned char *buf);
extern void AudioListRadios			(unsigned char *buf);
extern void AudioListRadiosReply		(unsigned char *buf);
extern void AudioGetRadioAttributes		(unsigned char *buf);
extern void AudioGetRadioAttributesReply	(unsigned char *buf);
extern void AudioSetRadioAttributes		(unsigned char *buf);
extern void AudioCreateFlow			(unsigned char *buf);
extern void AudioDestroyFlow			(unsigned char *buf);
extern void AudioGetFlowAttributes		(unsigned char *buf);
extern void AudioGetFlowAttributesReply		(unsigned char *buf);
extern void AudioSetFlowAttributes		(unsigned char *buf);
extern void AudioGetElements			(unsigned char *buf);
extern void AudioGetElementsReply		(unsigned char *buf);
extern void AudioSetElements			(unsigned char *buf);
extern void AudioGetElementStates		(unsigned char *buf);
extern void AudioElementState			(unsigned char *buf);
extern void AudioGetElementStatesReply		(unsigned char *buf);
extern void AudioSetElementStates		(unsigned char *buf);
extern void AudioGetElementParameters		(unsigned char *buf);
extern void AudioGetElementParametersReply	(unsigned char *buf);
extern void AudioSetElementParameters		(unsigned char *buf);
extern void AudioWriteElement			(unsigned char *buf);
extern void AudioReadElement			(unsigned char *buf);
extern void AudioReadElementReply		(unsigned char *buf);
extern void AudioGrabComponent			(unsigned char *buf);
extern void AudioUngrabComponent		(unsigned char *buf);
extern void AudioSendEvent			(unsigned char *buf);
extern void AudioGetAllowedUsers		(unsigned char *buf);
extern void AudioGetAllowedUsersReply		(unsigned char *buf);
extern void AudioSetAllowedUsers		(unsigned char *buf);
extern void AudioListExtensions			(unsigned char *buf);
extern void AudioListExtensionsReply		(unsigned char *buf);
extern void AudioQueryExtension			(unsigned char *buf);
extern void AudioQueryExtensionReply		(unsigned char *buf);
extern void AudioGetCloseDownMode		(unsigned char *buf);
extern void AudioGetCloseDownModeReply		(unsigned char *buf);
extern void AudioSetCloseDownMode		(unsigned char *buf);
extern void AudioKillClient			(unsigned char *buf);
extern void AudioGetServerTime			(unsigned char *buf);
extern void AudioGetServerTimeReply		(unsigned char *buf);
extern void AudioNoOperation			(unsigned char *buf);
