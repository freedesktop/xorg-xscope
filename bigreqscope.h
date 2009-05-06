#ifndef _BIGREQSCOPE_H_
#define _BIGREQSCOPE_H_


#define BIGREQREQUESTHEADER  "BIGREQREQUEST"
#define BIGREQREPLYHEADER    "BIGREQREPLY"

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

extern unsigned char LookForBIGREQFlag;
extern unsigned char BIGREQRequest;

extern void BigreqEnable	(FD fd, unsigned char *buf);
extern void BigreqEnableReply	(FD fd, unsigned char *buf);

#endif

