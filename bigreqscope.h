#ifndef _BIGREQSCOPE_H_
#define _BIGREQSCOPE_H_

#define BIGREQREQUEST	148
#define BIGREQREPLY	149


#define BIGREQREQUESTHEADER  "BIGREQREQUEST"
#define BIGREQREPLYHEADER    "BIGREQREPLY"

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

#endif

