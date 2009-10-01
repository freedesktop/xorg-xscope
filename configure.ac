dnl
dnl Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
dnl Use subject to license terms.
dnl
dnl Permission is hereby granted, free of charge, to any person obtaining a
dnl copy of this software and associated documentation files (the
dnl "Software"), to deal in the Software without restriction, including
dnl without limitation the rights to use, copy, modify, merge, publish,
dnl distribute, and/or sell copies of the Software, and to permit persons
dnl to whom the Software is furnished to do so, provided that the above
dnl copyright notice(s) and this permission notice appear in all copies of
dnl the Software and that both the above copyright notice(s) and this
dnl permission notice appear in supporting documentation.
dnl 
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
dnl OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
dnl MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
dnl OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
dnl HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
dnl INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
dnl FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
dnl NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
dnl WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
dnl 
dnl Except as contained in this notice, the name of a copyright holder
dnl shall not be used in advertising or otherwise to promote the sale, use
dnl or other dealings in this Software without prior written authorization
dnl of the copyright holder.
dnl
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])
AC_INIT(xscope,[1.2], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xscope)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

# Require xorg-macros: XORG_DEFAULT_OPTIONS, XORG_WITH_LINT
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
XORG_DEFAULT_OPTIONS
AC_PROG_INSTALL

dnl Issue an error if xtrans.m4 was not found and XTRANS_CONNECTION_FLAGS macro
dnl was not expanded, since xscope with no transport types is rather useless.
dnl
dnl If you're seeing an error here, be sure you installed the lib/xtrans module
dnl first and if it's not in the default location, that you set the ACLOCAL
dnl environment variable to find it, such as:
dnl     ACLOCAL="aclocal -I ${PREFIX}/share/aclocal"
m4_pattern_forbid([XTRANS_CONNECTION_FLAGS])

# Transport selection macro from xtrans.m4
XTRANS_CONNECTION_FLAGS

# Checks for pkg-config packages
PKG_CHECK_MODULES(XSCOPE, xtrans)
XSCOPE_CFLAGS="$XSCOPE_CFLAGS -DUSE_XTRANS"
AC_SUBST(XSCOPE_CFLAGS)
AC_SUBST(XSCOPE_LIBS)

AC_CHECK_FUNCS([getdtablesize])
AC_CHECK_HEADERS([sys/filio.h])

dnl Allow checking code with lint, sparse, etc.
XORG_WITH_LINT

AC_OUTPUT([Makefile])
