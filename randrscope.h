#ifndef _RANDRSCOPE_H_
#define _RANDRSCOPE_H_

#define RANDRREQUESTHEADER  "RANDRREQUEST"
#define RANDRREPLYHEADER    "RANDRREPLY"

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

extern unsigned char LookForRANDRFlag;
extern unsigned char RANDRRequest;
extern unsigned char RANDREvent;

extern void RandrQueryVersion (FD fd, unsigned char *buf);
extern void RandrQueryVersionReply (FD fd, unsigned char *buf);
extern void RandrGetScreenInfo (FD fd, unsigned char *buf);
extern void RandrGetScreenInfoReply (FD fd, unsigned char *buf);
extern void RandrSetScreenConfig (FD fd, unsigned char *buf);
extern void RandrSetScreenConfigReply (FD fd, unsigned char *buf);
extern void RandrScreenChangeSelectInput (FD fd, unsigned char *buf);
extern void RandrScreenSizes (unsigned char *buf);
extern void RandrScreenChangeNotifyEvent (unsigned char *buf);

#endif

