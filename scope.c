/*
 *  A spy program to reveal X11  traffic
 *
 *	James Peterson, 1988
 *
 * Copyright (C) 1988 MCC
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
 *
 */
/*
 * Copyright (c) 2002, 2012, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "scope.h"
#include "nas.h"
#include "extensions.h"

#include <ctype.h>
#include <unistd.h>

#include <sys/types.h>          /* needed by sys/socket.h and netinet/in.h */
#include <sys/uio.h>            /* for struct iovec, used by socket.h */
#include <sys/socket.h>         /* for AF_INET, SOCK_STREAM, ... */
#include <sys/ioctl.h>          /* for FIONCLEX, FIONBIO, ... */
#if !defined(FIOCLEX) && defined(HAVE_SYS_FILIO_H)
#include <sys/filio.h>
#endif
#include <fcntl.h>
#include <netinet/in.h>         /* struct sockaddr_in */
#include <netinet/tcp.h>
#include <netdb.h>              /* struct servent * and struct hostent * */
#include <errno.h>              /* for EINTR, EADDRINUSE, ... */

#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif


/* ********************************************** */
/*                                                */
/* ********************************************** */

#define DefaultPort 6000

char ServerHostName[MAXHOSTNAMELEN];
char AudioServerHostName[MAXHOSTNAMELEN];
long ServerBasePort = DefaultPort;
static long ServerInPort = 1;
static long ServerOutPort = 0;
static long ServerDisplay = 0;
static char *RawFile;

static FD ConnectToClient(FD ConnectionSocket);
static void DataFromClient(FD fd);
static void SetUpStdin(void);
static void SetUpRawFile(const char *filename);

char TranslateText = 0;
char IsUTF8locale = 0;
char ScopeEnabled = 1;
char HandleSIGUSR1 = 0;
char DoAudio = 0;
char TerminateClose = 0;
int Interrupt = 0;

struct FDDescriptor *FDD = NULL;
int MaxFD = 0;
int nFDsInUse = 0;
fd_set ReadDescriptors;
fd_set WriteDescriptors;
fd_set BlockedReadDescriptors;
int HighestFD;

short debuglevel = 0;
short Verbose = 0;
short XVerbose = 0;
short NasVerbose = 0;
short Raw = 0;
int ScopePort = 0;
char *ScopeHost = NULL;


typedef struct _BreakPoint {
    struct _BreakPoint *next;
    int number;
    unsigned char request;
    int minorop;
    Boolean enabled;
} BP;

typedef enum _CMDResult { CMDProceed, CMDDebug, CMDSyntax, CMDError } CMDResult;

static CMDResult CMDStep(int argc, char **argv);
static CMDResult CMDCont(int argc, char **argv);
static CMDResult CMDBreak(int argc, char **argv);
static CMDResult CMDDelete(int argc, char **argv);
static CMDResult CMDDisable(int argc, char **argv);
static CMDResult CMDEnable(int argc, char **argv);
static CMDResult CMDLevel(int argc, char **argv);
static CMDResult CMDAudio(int argc, char **argv);
static CMDResult CMDQuit(int argc, char **argv) _X_NORETURN;
static CMDResult CMDHelp(int argc, char **argv);

typedef struct _CMDFunc {
    const char *name;
    const char *alias;
     CMDResult(*func) (int argc, char **argv);
    const char *usage;
    const char *help;
} CMDFuncRec;
typedef const CMDFuncRec *CMDFuncPtr;

static const CMDFuncRec CMDFuncs[] = {
    {"audio", "a", CMDAudio, "[a]udio",
     "Set audio output level\n"},
    {"break", "b", CMDBreak, "[b]reak",
     "Create breakpoints\n"},
    {"cont", "c", CMDCont, "[c]ont",
     "Continue scoping\n"},
    {"delete", "del", CMDDelete, "[del]ete",
     "Delete breakpoints\n"},
    {"disable", "d", CMDDisable, "[d]isable",
     "Disable breakpoints\n"},
    {"enable", "e", CMDEnable, "[e]nable",
     "Enable breakpoints\n"},
    {"help", "?", CMDHelp, "help",
     "Print this list\n"},
    {"level", "l", CMDLevel, "[l]evel",
     "Set output level\n"},
    {"quit", "q", CMDQuit, "[q]uit",
     "Quit Xscope\n"},
    {"step", "s", CMDStep, "[s]tep",
     "Step trace one request\n"},
};

#define NumCMDFuncs (sizeof CMDFuncs / sizeof CMDFuncs[0])

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

static int
CMDStringToInt(char *s, int *v)
{
    int sign = 1;

    switch (*s) {
    case '-':
        sign = -1;
        s++;
        break;
    case '+':
        s++;
        break;
    }

    if (!strncmp(s, "0x", 2)) {
        sscanf(s + 2, "%x", v);
    }
    else if (*s == '0') {
        sscanf(s, "%o", v);
    }
    else if (isdigit(*s)) {
        sscanf(s, "%d", v);
    }
    else if (*s == '\'') {
        s++;
        if (*s == '\\') {
            s++;
            switch (*s) {
            case 'n':
                *v = '\n';
                break;
            case 't':
                *v = '\t';
                break;
            default:
                *v = (int) *s;
                break;
            }
        }
        else
            *v = (int) *s;
        s++;
        if (*s != '\'')
            return FALSE;
    }
    else
        return FALSE;
    *v *= sign;
    return TRUE;
}

static CMDFuncPtr
CMDStringToFunc(const char *name)
{
    int i;

    for (i = 0; i < NumCMDFuncs; i++) {
        if (!strcmp(name, CMDFuncs[i].name) || !strcmp(name, CMDFuncs[i].alias)) {
            return &CMDFuncs[i];
        }
    }
    return NULL;
}

static int
CMDSplitIntoWords(char *line, char **argv)
{
    char quotechar;
    int argc;

    argc = 0;
    while (*line) {
        while (isspace(*line))
            line++;
        if (!*line)
            break;
        if (*line == '"') {
            quotechar = *line++;
            *argv++ = line;
            argc++;
            while (*line && *line != quotechar)
                line++;
            if (*line)
                *line++ = '\0';
        }
        else {
            *argv++ = line;
            argc++;
            while (*line && !isspace(*line))
                line++;
            if (*line)
                *line++ = '\0';
        }
    }
    *argv = NULL;
    return argc;
}

static CMDResult
CMDHelp(int argc, char **argv)
{
    int i;
    CMDFuncPtr func;

    if (argc == 1) {
        for (i = 0; i < NumCMDFuncs; i++)
            printf("%-10s%s\n", CMDFuncs[i].name, CMDFuncs[i].usage);
    }
    else {
        for (i = 1; i < argc; i++) {
            func = CMDStringToFunc(argv[i]);
            if (!func) {
                printf("%-10s unknown command\n", argv[i]);
                return CMDSyntax;
            }
            printf("%-10s %s\n%s", func->name, func->usage, func->help);
        }
    }
    return CMDDebug;
}

static void
CMDSyntaxError(int argc, char **argv)
{
    printf("Syntax error in:");
    while (*argv)
        printf(" %s", *argv++);
    printf("\n");
}

void
ReadCommands(void)
{
    int argc;
    char line[1024];
    char *argv[20];
    CMDResult result;
    CMDFuncPtr func;
    static int here;

    if (here)
        return;
    here = 1;
    for (;;) {
        printf("> ");
        if (!fgets(line, sizeof line, stdin)) {
            if (feof(stdin)) {
                strncpy(line, "quit", sizeof(line));
            }
            else {
                printf("Error: %s\n", strerror(errno));
                break;
            }
        }
        argc = CMDSplitIntoWords(line, argv);
        if (argc > 0) {
            func = CMDStringToFunc(argv[0]);
            if (!func)
                CMDSyntaxError(argc, argv);
            else {
                result = (*func->func) (argc, argv);
                switch (result) {
                case CMDSyntax:
                    CMDSyntaxError(argc, argv);
                    break;
                case CMDError:
                    printf("Error\n");
                    break;
                case CMDProceed:
                    here = 0;
                    return;
                default:
                    break;
                }
            }
        }
    }
    printf("...\n");
    here = 0;
}

int SingleStep;
int BreakPoint;
BP *breakPoints;
int breakPointNumber;

void
TestBreakPoints(const unsigned char *buf, long n)
{
    BP *bp;

    for (bp = breakPoints; bp; bp = bp->next) {
        if (bp->enabled) {
            if (bp->request == buf[0]) {
                if (bp->request < EXTENSION_MIN_REQ)    /* Core protocol, not extension */
                    break;
                else if ((bp->minorop == -1) || (bp->minorop == buf[1]))
                    /* extension, either matching minor opcode or all minor opcodes */
                    break;
            }
        }
    }
    if (bp) {
        if (bp->request < EXTENSION_MIN_REQ)
            printf("Breakpoint on request %d\n", bp->request);
        else if (bp->minorop == -1)
            printf("Breakpoint on extension %d\n", bp->request);
        else
            printf("Breakpoint on extension %d, minor request %d\n",
                   bp->request, bp->minorop);
        ReadCommands();
    }
}

static void
setBreakPoint(void)
{
    Boolean b = false;
    BP *bp;
    FD fd;

    if (SingleStep)
        b = true;
    else {
        for (bp = breakPoints; bp; bp = bp->next) {
            if (bp->enabled) {
                b = true;
                break;
            }
        }
    }
    if (b != BreakPoint) {
        BreakPoint = b;
        for (fd = 0; fd < HighestFD; fd++) {
            if (FDD[fd].Busy && FDD[fd].InputHandler == DataFromClient) {
                if (BreakPoint)
                    SetBufLimit(fd);
                else
                    ClearBufLimit(fd);
            }
        }
    }
}

static CMDResult
CMDBreak(int argc, char **argv)
{
    BP *bp, **prev;
    int request, minorop;
    char *minorname;

    if (argc == 1) {
        for (bp = breakPoints; bp; bp = bp->next) {
            printf("%3d: %s %3d ", bp->number,
                   bp->enabled ? "enabled" : "disabled", bp->request);
            PrintENUMERATED(&bp->request, 1, TD[REQUEST].ValueList);
            if (bp->minorop != -1)
                printf(":%d", bp->minorop);
            printf("\n");
        }
    }
    else {
        for (prev = &breakPoints; *prev; prev = &(*prev)->next);
        while (*++argv) {
            if ((minorname = strchr(*argv, ':'))) {
                *minorname = '\0';
                if (!CMDStringToInt(minorname + 1, &minorop) ||
                    (minorop < 0) || (minorop > 255)) {
                    *minorname = ':';   /* restore string for error message */
                    return CMDSyntax;
                }
            }
            else {
                minorop = -1;
            }
            request = GetXRequestFromName(*argv);
            if (request < 0 && !CMDStringToInt(*argv, &request))
                return CMDSyntax;
            if ((request < EXTENSION_MIN_REQ) && (minorname != NULL)) {
                *minorname = ':';       /* restore string for error message */
                return CMDSyntax;
            }
            bp = malloc(sizeof(BP));
            bp->number = ++breakPointNumber;
            bp->request = request;
            bp->minorop = minorop;
            bp->enabled = true;
            bp->next = NULL;
            *prev = bp;
            prev = &bp->next;
        }
    }
    setBreakPoint();
    return CMDDebug;
}

static CMDResult
CMDCont(int argc, char **argv)
{
    SingleStep = 0;
    return CMDProceed;
}

static CMDResult
CMDDisable(int argc, char **argv)
{
    BP *bp;
    int number;

    if (argc == 1) {
        printf("Disabling all breakpoints...\n");
        for (bp = breakPoints; bp; bp = bp->next)
            bp->enabled = false;
    }
    else {
        while (*++argv) {
            if (!CMDStringToInt(*argv, &number))
                return CMDSyntax;
            for (bp = breakPoints; bp; bp = bp->next)
                if (bp->number == number) {
                    bp->enabled = false;
                    break;
                }
            if (!bp) {
                printf("No such breakpoint %s\n", *argv);
            }
        }
    }
    setBreakPoint();
    return CMDDebug;
}

static CMDResult
CMDDelete(int argc, char **argv)
{
    BP *bp, **prev;
    int number;

    if (argc == 1) {
        printf("Deleting all breakpoints...\n");
        while ((bp = breakPoints) != NULL) {
            breakPoints = bp->next;
            free(bp);
        }
    }
    else {
        while (*++argv) {
            if (!CMDStringToInt(*argv, &number))
                return CMDSyntax;
            for (prev = &breakPoints; (bp = *prev) != NULL; prev = &bp->next) {
                if (bp->number == number) {
                    *prev = bp->next;
                    free(bp);
                    break;
                }
            }
            if (!bp) {
                printf("No such breakpoint %s\n", *argv);
            }
        }
    }
    setBreakPoint();
    return CMDDebug;
}

static CMDResult
CMDEnable(int argc, char **argv)
{
    BP *bp;
    int number;

    if (argc == 1) {
        printf("Enabling all breakpoints...\n");
        for (bp = breakPoints; bp; bp = bp->next)
            bp->enabled = true;
    }
    else {
        while (*++argv) {
            if (!CMDStringToInt(*argv, &number))
                return CMDSyntax;
            for (bp = breakPoints; bp; bp = bp->next)
                if (bp->number == number) {
                    bp->enabled = true;
                    break;
                }
            if (!bp) {
                printf("No such breakpoint %s\n", *argv);
            }
        }
    }
    setBreakPoint();
    return CMDDebug;
}

static CMDResult
CMDStep(int argc, char **argv)
{
    SingleStep = 1;
    setBreakPoint();
    return CMDProceed;
}

#ifdef __SUNPRO_C
/* prevent "Function has no return statement" error for CMDQuit */
#pragma does_not_return(exit)
#endif

static CMDResult
CMDQuit(int argc, char **argv)
{
    printf("exiting...\n");
    exit(0);
}

static CMDResult
CMDLevel(int argc, char **argv)
{
    int level;

    if (argc == 1)
        printf("Level: %d\n", XVerbose);
    else if (argc == 2 && CMDStringToInt(argv[1], &level))
        XVerbose = level;
    else
        return CMDSyntax;
    return CMDDebug;
}

static CMDResult
CMDAudio(int argc, char **argv)
{
    int level;

    if (argc == 1)
        printf("Audio Level: %d\n", NasVerbose);
    else if (argc == 2 && CMDStringToInt(argv[1], &level))
        NasVerbose = level;
    else
        return CMDSyntax;
    return CMDDebug;
}

/* ********************************************** */
/*                                                */
/*                                                */
/* ********************************************** */

short
GetServerport(void)
{
    short port;

    enterprocedure("GetServerport");

    port = ServerBasePort + ServerOutPort + ServerDisplay;
    debug(4, (stderr, "Server service is on port %d\n", port));
    return (port);
}

static short
GetScopePort(void)
{
    short port;

    enterprocedure("GetScopePort");

    port = ServerBasePort + ServerInPort + ServerDisplay;
    debug(4, (stderr, "scope service is on port %d\n", port));
    return (port);
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

static void _X_NORETURN
Usage(void)
{
    fprintf(stderr,
            "Usage: xscope\n"
            "              [-h<server-host>]\n"
            "              [-i<in-port>]\n"
            "              [-o<out-port>]\n"
            "              [-d<display-number>]\n"
            "              [-v<n>]  -- verbose output\n"
            "              [-r]  -- raw output\n"
            "              [-a<n>]  -- audio verbose output\n"
            "              [-q]  -- quiet output\n"
            "              [-D<debug-level>]\n"
            "              [-I]  -- start in interactive mode\n"
            "              [-S<n>] -- start/stop on SIGUSR1\n"
            "              [-V]  -- output version information and exit\n"
            "              [-t]  -- terminate when all clients close\n");
    exit(1);
}

static void
InitializeLocale(void)
{
    setlocale(LC_CTYPE, "");

#ifdef HAVE_LANGINFO_H
    {
        const char *charmap = nl_langinfo (CODESET);

        if (charmap != NULL && strcmp(charmap, "UTF-8") == 0)
            IsUTF8locale = 1;
    }
#endif
}

static void
ScanArgs(int argc, char **argv)
{
    XVerbose = 1; /* default verbose-ness level */
    NasVerbose = 1;
    Raw = 0;

    /* Scan argument list */
    while (--argc > 0) {
        ++argv;
        if (**argv == '-')
            switch (*++*argv) {
                /*
                   debug levels:
                   2 - trace each procedure entry
                   4 - I/O, connections
                   8 - Scope internals
                   16 - Message protocol
                   32 - 64 - was malloc, now unused
                   128 - 256 - really low level
                 */
            case 'D':
                debuglevel = atoi(++*argv);
                if (debuglevel == 0)
                    debuglevel = 255;
                XVerbose = 7;
                debug(1, (stderr, "debuglevel = %d\n", debuglevel));
                break;

            case 'I':
                Interrupt = 1;
                break;

            case 'S':
                HandleSIGUSR1 = 1;
                ScopeEnabled = atoi(++*argv);
                break;

            case 'V':          /* print version & exit */
                printf("%s\n", PACKAGE_STRING);
                exit(0);

            case 'q':          /* quiet mode */
                XVerbose = 0;
                debug(1, (stderr, "Verbose = %d\n", XVerbose));
                break;

            case 'r':          /* raw mode */
                Raw = 1;
                debug(1, (stderr, "Raw = %d\n", Raw));
                break;

            case 'v':          /* verbose mode */
                XVerbose = atoi(++*argv);
                debug(1, (stderr, "Verbose = %d\n", XVerbose));
                break;

            case 'o':
                ServerOutPort = atoi(++*argv);
                if (ServerOutPort <= 0)
                    ServerOutPort = 0;
                debug(1, (stderr, "ServerOutPort = %ld\n", ServerOutPort));
                break;

            case 'd':
                ServerDisplay = atoi(++*argv);
                if (ServerDisplay <= 0)
                    ServerDisplay = 0;
                debug(1, (stderr, "ServerDisplay = %ld\n", ServerDisplay));
                break;

            case 'i':
                ServerInPort = atoi(++*argv);
                if (ServerInPort <= 0)
                    ServerInPort = 0;
                debug(1, (stderr, "ServerInPort = %ld\n", ServerInPort));
                break;

            case 'h':
                if (++*argv != NULL && **argv != '\0'
                    && (strlen(*argv) < sizeof(ServerHostName)))
                    strcpy(ServerHostName, *argv);
                debug(1, (stderr, "ServerHostName = %s\n", ServerHostName));
                break;

            case 'f':
                if (++*argv != NULL && **argv != '\0')
                    RawFile = *argv;
                else if (argv[1] != NULL) {
                    RawFile = argv[1];
                    argv++; argc--;
                }
                debug(1, (stderr, "RawFile = %s\n", RawFile));
                break;

            case 'T':
                TranslateText = 1;
                break;

            case 'A':
                DoAudio = 1;
                break;

            case 'a':          /* verbose mode */
                NasVerbose = atoi(++*argv);
                debug(1, (stderr, "NasVerbose = %d\n", NasVerbose));
                break;

            case 'n':          /* NAS server host */
                if (++*argv != NULL && **argv != '\0')
                    (void) strcpy(AudioServerHostName, *argv);
                debug(1, (stderr, "AudioServerHostName = %s\n",
                          AudioServerHostName));
                break;
            case 't':
                TerminateClose = 1;
                break;

            default:
                fprintf(stderr, "Unknown option %c\n", **argv);
                Usage();
                break;

            }
        else {
            /* file argument to scope -- error */
            Usage();
        }
    }

    /* check for different port numbers or different machines */
    if (ServerInPort == ServerOutPort)
        if (ServerHostName[0] == '\0') {
            fprintf(stderr, "Can't have xscope on same port as server (%ld)\n",
                    ServerInPort);
            Usage();
        }

}


/* ********************************************** */
/*                                                */
/* ********************************************** */

int
main(int argc, char **argv)
{
    InitializeLocale();
    ScanArgs(argc, argv);
    InitializeFD();
    InitializeX11();
    if (DoAudio)
        InitializeAudio();
    SetUpStdin();
    if (RawFile) {
        SetUpRawFile(RawFile);
    } else {
        SetUpConnectionSocket(GetScopePort(), NewConnection);
        if (DoAudio)
            SetUpConnectionSocket(GetScopePort() + 2000, NewAudio);
    }
    SetSignalHandling();

    return MainLoop();
}

void
TimerExpired(void)
{
    debug(16, (stderr, "Timer tick\n"));
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

/*
  here is where we would add code to allow control from
  the keyboard.  We would want to read a command and
  interpret it.  Possibilties:

  (a) verbose level setting
  (b) reset time
  (c) save X requests to a file.
  (d) replay X requests from a file.
  (e) allow fake events, errors to be generated.
*/

static void
ReadStdin(FD fd)
{
    char buf[2048];
    long n;

    enterprocedure("ReadStdin");
    n = read(fd, buf, 2048);
    debug(4, (stderr, "read %ld bytes from stdin\n", n));
}

static void
SetUpStdin(void)
{
    enterprocedure("SetUpStdin");
    UsingFD(fileno(stdin), ReadStdin, NULL, NULL);
}

/* ************************************************************ */
/*								*/
/*								*/
/* ************************************************************ */

/*
  xscope is really meant to look at one client at a time.  However,
  it can easily handle multiple clients and servers.  To do so,
  we need to have a pair of FDs: one for the client and one for the
  server for that client.  If either goes away, so does the other.
  We need to be able to identify the other FD of a pair, so that if
  we get input from one, we can write it to the other.
*/

static long clientNumber = 0;
struct fdinfo *FDinfo;

void
SetUpPair(FD client, FD server)
{
    if (client >= 0) {
        clientNumber += 1;
        FDinfo[client].Server = false;
        FDinfo[client].pair = server;
        FDinfo[client].ClientNumber = clientNumber;
        if (FDinfo[client].buffer == NULL) {
            FDinfo[client].buffer = calloc(1, BUFFER_SIZE);
            if (FDinfo[client].buffer == NULL)
                panic("unable to allocate client buffer");
        }
        FDinfo[client].bufcount = 0;
        FDinfo[client].buflimit = -1;
        FDinfo[client].bufdelivered = 0;
        if (server >= 0) {
            FDinfo[server].Server = true;
            FDinfo[server].pair = client;
            FDinfo[server].ClientNumber = FDinfo[client].ClientNumber;
            if (FDinfo[server].buffer == NULL) {
                FDinfo[server].buffer = calloc(1, BUFFER_SIZE);
                if (FDinfo[server].buffer == NULL)
                    panic("unable to allocate server buffer");
            }
            FDinfo[server].bufcount = 0;
            FDinfo[server].buflimit = -1;
            FDinfo[server].bufdelivered = 0;
        }
    }
    else if (server >= 0) {
        close(server);
        NotUsingFD(server);
    }
}

static void
ResetPair(FD client, FD server)
{
    if (client >= 0) {
        free(FDinfo[client].buffer);
        FDinfo[client].buffer = NULL;
        FDinfo[client].bufcount = 0;
        FDinfo[client].buflimit = -1;
        FDinfo[client].bufdelivered = 0;
    }
    if (server >= 0) {
        free(FDinfo[server].buffer);
        FDinfo[server].buffer = NULL;
        FDinfo[server].bufcount = 0;
        FDinfo[server].buflimit = -1;
        FDinfo[server].bufdelivered = 0;
    }
}

static void
CloseConnection(FD fd)
{
    debug(4, (stderr, "close %d and %d\n", fd, FDPair(fd)));
    ResetPair(ClientHalf(fd), ServerHalf(fd));
    StopClientConnection(ServerHalf(fd));
    StopServerConnection(ClientHalf(fd));

    CloseFD(fd);
    CloseFD(FDPair(fd));

    if (TerminateClose)
        exit(0);
}

/* ************************************************************ */

FD
FDPair(FD fd)
{
    return (FDinfo[fd].pair);
}

FD
ClientHalf(FD fd)
{
    if (FDinfo[fd].Server)
        return (FDinfo[fd].pair);
    return (fd);
}

FD
ServerHalf(FD fd)
{
    if (FDinfo[fd].Server)
        return (fd);
    return (FDinfo[fd].pair);
}

const char *
ClientName(FD fd)
{
    static char name[12];

    if (clientNumber <= 1)
        return ("");
    snprintf(name, sizeof(name), " %d", FDinfo[fd].ClientNumber);
    return (name);
}

int
ClientNumber(FD fd)
{
    return FDinfo[fd].ClientNumber;
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

/*
 * Write as much of the queued data as the receiver will accept
 * Block reads from the sender until the receiver gets all of the
 * data
 */
void
FlushFD(FD fd)
{
    long BytesToWrite = FDinfo[fd].bufcount - FDinfo[fd].bufstart;
    unsigned char *p = FDinfo[fd].buffer + FDinfo[fd].bufstart;
    int PeerFD;

    PeerFD = FDPair(fd);
    if (FDinfo[fd].buflimit >= 0) {
        if (FDinfo[fd].bufdelivered + BytesToWrite > FDinfo[fd].buflimit)
            BytesToWrite = FDinfo[fd].buflimit - FDinfo[fd].bufdelivered;
    }
    while (BytesToWrite > 0) {
        int n1 = write(fd, (char *) p, (int) BytesToWrite);

        debug(4, (stderr, "write %d bytes\n", n1));
        if (n1 < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                perror("Error on write");
                if (PeerFD >= 0)
                    CloseConnection(PeerFD);
                BytesToWrite = 0;
            }
            break;
        }
        else {
            FDinfo[fd].bufstart += n1;
            FDinfo[fd].bufdelivered += n1;
            BytesToWrite -= n1;
            p += n1;
        }
    }
    if (FDinfo[fd].bufstart == FDinfo[fd].bufcount) {
        if (PeerFD >= 0) {
            FD_CLR(PeerFD, &BlockedReadDescriptors);
        }
        FD_CLR(fd, &WriteDescriptors);
        FDinfo[fd].bufcount = FDinfo[fd].bufstart = 0;
    }
    else {
        if (PeerFD >= 0) {
            FD_SET(PeerFD, &BlockedReadDescriptors);
        }
        if (FDinfo[fd].buflimit != FDinfo[fd].bufdelivered) {
            FD_SET(fd, &WriteDescriptors);
        }
    }
}

/* when we get data from a client, we read it in, copy it to the
   server for this client, then dump it to the client. Note, we don't
   have to have a server, if there isn't one. */

static void
DataFromClient(FD fd)
{
    long n;
    FD ServerFD;

    Verbose = XVerbose;
    enterprocedure("DataFromClient");
    ServerFD = FDPair(fd);
    if (ServerFD < 0) {
        ServerFD = ConnectToServer(false);
        if (ServerFD < 0) {
            CloseConnection(fd);
            return;
        }
        SetUpPair(fd, ServerFD);
    }

    n = read(fd, FDinfo[ServerFD].buffer, BUFFER_SIZE);
    debug(4, (stderr, "read %ld bytes from Client%s\n", n, ClientName(fd)));
    if (n < 0) {
        PrintTime();
        perror("Client --> read error:");
        CloseConnection(fd);
        return;
    }
    if (n == 0) {
        PrintTime();
        if (Verbose >= 0)
            fprintf(stdout, "Client%s --> EOF\n", ClientName(fd));
        CloseConnection(fd);
        return;
    }

    FDinfo[ServerFD].bufcount = n;
    FDinfo[ServerFD].bufstart = 0;

    FlushFD(ServerFD);
    /* also report the bytes to standard out */
    ReportFromClient(fd, FDinfo[ServerFD].buffer, n);
}

/* ********************************************** */
/*                                                */
/* ********************************************** */

/* similar situation for the server, but note that if there is no client,
   we close the connection down -- don't need a server with no client. */

static void
DataFromServer(FD fd)
{
    long n;
    FD ClientFD;

    Verbose = XVerbose;
    ClientFD = FDPair(fd);
    if (ClientFD < 0) {
        CloseConnection(fd);
        return;
    }

    enterprocedure("DataFromServer");
    n = read(fd, (char *) FDinfo[ClientFD].buffer, BUFFER_SIZE);
    debug(4, (stderr, "read %ld bytes from Server%s\n", n, ClientName(fd)));
    if (n < 0) {
        PrintTime();
        perror("read error <- Server");
        CloseConnection(fd);
        return;
    }
    if (n == 0) {
        PrintTime();
        if (Verbose >= 0)
            fprintf(stdout, "EOF <-- Server%s\n", ClientName(fd));
        CloseConnection(fd);
        return;
    }

    FDinfo[ClientFD].bufcount = n;
    FDinfo[ClientFD].bufstart = 0;
    FlushFD(ClientFD);

    /* also report the bytes to standard out */
    ReportFromServer(fd, FDinfo[ClientFD].buffer, n);
}


/* ************************************************************ */
/*								*/
/*     Create New Connection to a client program and to Server  */
/*								*/
/* ************************************************************ */

void
NewConnection(FD fd)
{
    FD ClientFD = ConnectToClient(fd);
    FD ServerFD = ConnectToServer(true);

    SetUpPair(ClientFD, ServerFD);
}


/* ************************************************************ */

static FD
ConnectToClient(FD ConnectionSocket)
{
    FD ClientFD;
    XtransConnInfo trans_conn = NULL;

#ifdef USE_XTRANS
    XtransConnInfo listen_conn;
    int status;
#else
    struct sockaddr_in from;
    int len = sizeof(from);
    int ON = 1; /* used in ioctl */
#endif

    enterprocedure("ConnectToClient");

#ifdef USE_XTRANS
    listen_conn = GetXTransConnInfo(ConnectionSocket);
    if ((trans_conn = _X11TransAccept(listen_conn, &status)) == NULL) {
        debug(4, (stderr, "Failed to accept connection\n"));
        return -1;
    }
    _X11TransSetOption(trans_conn, TRANS_NONBLOCKING, 1);
    ClientFD = _X11TransGetConnectionNumber(trans_conn);
#else
    ClientFD = AcceptConnection(ConnectionSocket);
#endif
    debug(4, (stderr, "Connect To Client: FD %d\n", ClientFD));
    if (ClientFD < 0 && errno == EWOULDBLOCK) {
        debug(4, (stderr, "Almost blocked accepting FD %d\n", ClientFD));
        panic("Can't connect to Client");
    }
    if (ClientFD < 0) {
        debug(4, (stderr, "NewConnection: error %d\n", errno));
        panic("Can't connect to Client");
    }

    UsingFD(ClientFD, DataFromClient, FlushFD, trans_conn);
#ifndef USE_XTRANS
    ioctl(ClientFD, FIOCLEX, 0);
    ioctl(ClientFD, FIONBIO, &ON);
#endif
    StartClientConnection(ClientFD);
    return (ClientFD);
}

/* ************************************************************ */
/*								*/
/*								*/
/* ************************************************************ */


FD
ConnectToServer(Boolean report)
{
    FD ServerFD;
    XtransConnInfo trans_conn = NULL;   /* transport connection object */
    short port;

    enterprocedure("ConnectToServer");

    port = GetServerport();

    if (port == ScopePort &&
        ((ServerHostName[0] == '\0') || strcmp(ServerHostName, ScopeHost) == 0))
    {
        char error_message[100];

        snprintf(error_message, sizeof(error_message),
                 "Trying to attach to myself: %s,%d\n", ServerHostName, port);
        panic(error_message);
    }

    ServerFD = MakeConnection(ServerHostName, port, report, &trans_conn);

    debug(4, (stderr, "Connect To Server: FD %d\n", ServerFD));
    if (ServerFD >= 0) {
        UsingFD(ServerFD, DataFromServer, FlushFD, trans_conn);
        StartServerConnection(ServerFD);
    }
    return (ServerFD);
}


/* ************************************************************ */
/*                                                              */
/* Read & decode data from a pre-recorded raw file (-r -v0)     */
/*                                                              */
/* ************************************************************ */

static FILE *raw;
static FD *fdMap;	/* map of fds from file to current runtime */
static int fdMapSize;

static void
SetUpRawClient(FD *returnClientFD, FD *returnServerFD)
{
    FD ClientFD, ServerFD;

    /* need to fake a pair of fd's */
    ClientFD = open("/dev/null", O_RDONLY);
    if (ClientFD < 1) {
        perror("/dev/null");
        panic("Can't open /dev/null for reading");
    }
    ServerFD = dup(ClientFD);
    if (ServerFD < 1) {
        perror("ClientFD");
        panic("Can't dup ClientFD for reading");
    }
    UsingFD(ClientFD, NULL, NULL, NULL);
    UsingFD(ServerFD, NULL, NULL, NULL);
    SetUpPair(ClientFD, ServerFD);
    StartClientConnection(ClientFD);
    StartServerConnection(ServerFD);
    *returnClientFD = ClientFD;
    *returnServerFD = ServerFD;
}

static inline FD
GetRawFDMap(FD origFD)
{
    if (origFD >= fdMapSize)
        return -1;
    return fdMap[origFD];
}

static void
SetRawFDMap(FD origFD, FD currentFD)
{
    if (origFD >= fdMapSize) {
        int newSize = fdMapSize + 32;
        if (origFD >= newSize)
            newSize = origFD;
        fdMap = realloc(fdMap, newSize);
        if (fdMap == NULL)
            panic("Can't allocate memory for fdMap");
        memset(fdMap + fdMapSize, -1, newSize - fdMapSize);
        fdMapSize = newSize;
    }
    fdMap[origFD] = currentFD;
    debug(16, (stderr, "FD Map %d => %d\n", origFD, currentFD));
}

static void
UnmapRawFD(FD currentFD)
{
    FD fd;

    for (fd = 0; fd < fdMapSize; fd++) {
        if (fdMap[fd] == currentFD) {
            debug(16, (stderr, "Clearing FD Map %d => %d\n", fd, currentFD));
            fdMap[fd] = -1;
            return;
        }
    }
}


static void
DataFromRawFile(FD rawfd)
{
    Boolean fromClient = true;
    FD fd = -1;
    char *in, *n;
    unsigned char *out;
    static FD newServerFD;

    enterprocedure("DataFromRawFile");

    Verbose = XVerbose;

    for (;;) {

        /* If we already read a line last time, process it first,
           else get the next line for processing */
        if (FDinfo[rawfd].bufcount == 0) {
            if (fgets(FDinfo[rawfd].buffer, BUFFER_SIZE, raw) == NULL) {
                CloseConnection(rawfd);
                exit(0);
            }
            FDinfo[rawfd].bufcount = strlen(FDinfo[rawfd].buffer);
            debug(16, (stderr, "raw input = %s", FDinfo[rawfd].buffer));
        }

        in = FDinfo[rawfd].buffer;

        /* lines starting with space indicate change of sender */
        if (isspace(*in)) {
            /* If we already have data in buffer, process it */
            if ((fd != -1) && (FDinfo[fd].bufcount > 0)) {
                debug(16, (stderr, "reporting %d bytes from: %s %s\n",
                           FDinfo[fd].bufcount,
                           fromClient ? "client" : "server",
                           ClientName(fd)));
                if (fromClient)
                    ReportFromClient(fd, FDinfo[fd].buffer,
                                     FDinfo[fd].bufcount);
                else
                    ReportFromServer(fd, FDinfo[fd].buffer,
                                     FDinfo[fd].bufcount);
                FDinfo[fd].bufcount = 0;
                return;
            }
            else {
                /* Need to parse header to figure out which direction */
                while (isspace(*in)) {
                    in++;
                }
                if (strncmp(in, "Client Connect (fd ", 19) == 0) {
                    /* New client connection */
                    FD ClientFD, ServerFD, origFD;

                    origFD = strtol(in + 19, NULL, 10);

                    SetUpRawClient(&ClientFD, &ServerFD);
                    SetRawFDMap(origFD, ClientFD);
                    fd = ClientFD;
                    fromClient = true;
                    newServerFD = ServerFD;
                }
                else if (strncmp(in, "Server Connect (fd ", 19) == 0) {
                    /* New client connection */
                    FD origFD;

                    if (newServerFD == 0)
                        panic("Server connection without matching client");

                    origFD = strtol(in + 19, NULL, 10);
                    SetRawFDMap(origFD, newServerFD);
                    fd = newServerFD;
                    fromClient = false;
                    newServerFD = 0;
                }
                else if (strstr(in, " --> EOF")) {
                    /* Closing client connection */
                    n = strstr(in, ": Client");
                    if (n != NULL) {
                        int clientid;

                        in = n + 8;
                        if (isdigit(*in))
                            clientid = strtol(in, NULL, 10);
                        else
                            clientid = 1;

                        for (fd = 0; fd < HighestFD; fd++) {
                            if (FDinfo[fd].ClientNumber == clientid) {
                                if (Verbose >= 0) {
                                    PrintTime();
                                    fprintf(stdout, "Client%s --> EOF\n",
                                            ClientName(fd));
                                }
                                UnmapRawFD(fd);
                                UnmapRawFD(FDPair(fd));
                                CloseConnection(fd);
                                break;
                            }
                        }
                    }
                    fd = -1;
                }
                else {
                    FD origFD;

                    if ((strncmp(in, "Request ", 8) == 0))
                        fromClient = true;
                    else
                        fromClient = false;

                    in = strstr(in, "(fd ");
                    if (in == NULL) {
                        warn("Did not find fd string in input entry");
                        warn(FDinfo[rawfd].buffer);
                        FDinfo[rawfd].bufcount = 0;
                        continue;
                    }

                    origFD = strtol(in + 4, NULL, 10);
                    fd = GetRawFDMap(origFD);
                    if (fd == -1) {
                        debug(16, (stderr, "origFD = %d\n", origFD));
                        warn("Unknown fd in input entry");
                        FDinfo[rawfd].bufcount = 0;
                        continue;
                    }
                }
                debug(16, (stderr, "raw data from: %s\n",
                           fromClient ? "client" : "server"));
                /* skip over rest of header */
                n = strchr(in, ':');
                if (n != NULL)
                    in = n + 1;
                while (isspace(*in))
                    in++;
            }
        }

        if (fd == -1) {
            /* dump data we don't know what to do with */
            FDinfo[rawfd].bufcount = 0;
            continue;
        }

        out = FDinfo[fd].buffer + FDinfo[fd].bufcount;

        while (*in != '\0') {
            unsigned char val;

            if ((in[0] >= '0') && in[0] <= '9') {
                val = (in[0] - '0') << 4;
            }
            else if ((in[0] >= 'a') && in[0] <= 'f') {
                val = (in[0] - 'a' + 0x0a) << 4;
            }
            else {
                warn(in);
                warn("invalid raw file input");
                break;
            }

            if ((in[1] >= '0') && in[1] <= '9') {
                val += (in[1] - '0');
            }
            else if ((in[1] >= 'a') && in[1] <= 'f') {
                val += (in[1] - 'a' + 0x0a);
            }
            else {
                warn(in + 1);
                warn("invalid raw file input");
                break;
            }

            if (in[2] == ' ') {
                in += 3;
            } else {
                in += 2;
            }
            while ((*in == '\r') || (*in == '\n'))
                in++;

            *out++ = val;
            FDinfo[fd].bufcount++;

            /* If buffer is full, process what we have now */
            if (FDinfo[fd].bufcount >= BUFFER_SIZE) {
                if (fromClient)
                    ReportFromClient(fd, FDinfo[fd].buffer,
                                     FDinfo[fd].bufcount);
                else
                    ReportFromServer(fd, FDinfo[fd].buffer,
                                     FDinfo[fd].bufcount);
                FDinfo[fd].bufcount = 0;
                out = FDinfo[fd].buffer;
            }
        }
        FDinfo[rawfd].bufcount = 0;
    }
}

static void
SetUpRawFile(const char *filename)
{
    FD fd;
    enterprocedure("SetUpRawFile");

    if (strcmp(filename, "-") == 0)
        raw = stdin;
    else
        raw = fopen(filename, "r");
    if (raw == NULL) {
        perror(filename);
        panic("Can't open raw file for reading");
    }
    fd = fileno(raw);
    debug(4, (stderr, "Opened raw file %s: FD %d\n", filename, fd));

    if (FDinfo[fd].buffer == NULL) {
        FDinfo[fd].buffer = calloc(1, BUFFER_SIZE);
        if (FDinfo[fd].buffer == NULL)
            panic("unable to allocate client buffer");
    }
    FDinfo[fd].bufcount = 0;
    FDinfo[fd].buflimit = -1;
    FDinfo[fd].bufdelivered = 0;

    UsingFD(fd, DataFromRawFile, NULL, NULL);
}
