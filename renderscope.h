#ifndef _RENDERSCOPE_H_
#define _RENDERSCOPE_H_

#define RENDERREQUEST	132
#define RENDERREPLY	133
#define RENDERERROR	134

#define PICTURE		135
#define PICTFORMAT	136
#define PICTURE_BITMASK	137
#define PICTOP		138
#define GLYPHSET	139
#define RENDERCOLOR	140
#define PICTFORMINFO    141

#define RENDERREQUESTHEADER  "RENDERREQUEST"
#define RENDERREPLYHEADER	  "RENDERREPLY"

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

#endif

