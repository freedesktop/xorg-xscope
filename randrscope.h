#ifndef _RANDRSCOPE_H_
#define _RANDRSCOPE_H_

#define RANDRREQUEST	141
#define RANDRREPLY	142
#define RANDRERROR	143


#define RANDRREQUESTHEADER  "RANDRREQUEST"
#define RANDRREPLYHEADER    "RANDRREPLY"

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

#endif

