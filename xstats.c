#define REQUEST	0
#define REPLY	1
#define ERROR	2
#define EVENT	3

string_to_action (s)
    char    *s;
{
    if (!strcmp (s, "@@REQUEST")) return REQUEST;
    if (!strcmp (s, "@@REPLY")) return REPLY;
    if (!strcmp (s, "@@ERROR")) return ERROR;
    if (!strcmp (s, "@@EVENT")) return EVENT;
}
    
typedef struct {
    double  time;
    int	    action;
    int	    client;
    int	    major;
    int	    minor;
    int	    len;
} ActionRec, *ActionPtr;

unsigned long	requestCount[256][256];
unsigned long	replyCount[256][256];
unsigned long	eventCount[256][256];
unsigned long	errorCount[256][256];

unsigned long	requestBytes[256][256];
unsigned long	replyBytes[256][256];
unsigned long	eventBytes[256][256];
unsigned long	errorBytes[256][256];

unsigned long	tRequestBytes;
unsigned long	tReplyBytes;
unsigned long	tEventBytes;
unsigned long	tErrorBytes;
    
unsigned long	tRequestCount;
unsigned long	tReplyCount;
unsigned long	tEventCount;
unsigned long	tErrorCount;
    
dump (c, b)
    unsigned long   c[256][256];
    unsigned long   b[256][256];
{
    int		    i, j;
    unsigned long   count, bytes;

    for (i = 0; i < 256; i++)
    {
	for (j = 0; j < 256; j++)
	{
	    if (count = c[i][j])
	    {
		bytes = b[i][j];
		printf ("%3d %3d count %5d bytes %7d\n", i, j, count, bytes);
	    }
	}
    }
}

main ()
{
    ActionRec	a;
    char	aname[128];
    int		i, j;
    
    while (scanf ("%lf: %s %d %d %d %d\n",
		  &a.time, aname, &a.client, &a.major, &a.minor, &a.len) == 6)
    {
	a.action = string_to_action (aname);
	switch (a.action) {
	case REQUEST:
	    requestCount[a.major][a.minor]++;
	    requestBytes[a.major][a.minor] += a.len;
	    tRequestCount++;
	    tRequestBytes += a.len;
	    break;
	case REPLY:
	    replyCount[a.major][a.minor]++;
	    replyBytes[a.major][a.minor] += a.len;
	    tReplyCount++;
	    tReplyBytes += a.len;
	    break;
	case EVENT:
	    eventCount[a.major][a.minor]++;
	    eventBytes[a.major][a.minor] += a.len;
	    tEventCount++;
	    tEventBytes += a.len;
	    break;
	case ERROR:
	    errorCount[a.major][a.minor]++;
	    errorBytes[a.major][a.minor] += a.len;
	    tErrorCount++;
	    tErrorBytes += a.len;
	    break;
	}
    }
    printf ("requests:\n");
    dump (requestCount, requestBytes);
    printf ("replies:\n");
    dump (replyCount, replyBytes);
    printf ("events:\n");
    dump (eventCount, eventBytes);
    printf ("errors:\n");
    dump (errorCount, errorBytes);
    printf ("send    count %5d bytes %7d\n",
	    tRequestCount, tRequestBytes);
    printf ("recv    count %5d bytes %7d\n",
	    tEventCount + tErrorCount + tReplyCount,
	    tEventBytes + tErrorBytes + tReplyBytes);
}
