/* **********************************************
 *						*
 * header file file descriptor (FD) code        *
 *						*
 *	James Peterson, 1987			*
 * Copyright (C) 1987 MCC
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
 *						*
 ********************************************** */


/*
   the following structure remembers for each file descriptor its
   state.  In particular, we need to know if it is busy or free
   and if it is in use, by whom.
*/

typedef int FD;

struct FDDescriptor
{
    Boolean Busy;
    int     (*InputHandler)();
    int	    (*FlushHandler)();
};

extern struct FDDescriptor *FDD /* array of FD descriptors */ ;
extern short   MaxFD /* maximum number of FD's possible */ ;

extern short   nFDsInUse /* number of FD's actually in use */ ;

extern long    ReadDescriptors /* bit map of FD's in use -- for select  */ ;
extern long	WriteDescriptors /* bit map of write blocked FD's -- for select */;
extern long	BlockedReadDescriptors /* bit map of FD's blocked from reading */;
extern short   HighestFD /* highest FD in use -- for select */ ;
