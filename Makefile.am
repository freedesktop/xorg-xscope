#
# Copyright 2008-2009 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, provided that the above
# copyright notice(s) and this permission notice appear in all copies of
# the Software and that both the above copyright notice(s) and this
# permission notice appear in supporting documentation.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
# INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
# 
# Except as contained in this notice, the name of a copyright holder
# shall not be used in advertising or otherwise to promote the sale, use
# or other dealings in this Software without prior written authorization
# of the copyright holder.
#

bin_PROGRAMS = xscope

AM_CFLAGS = $(CWARNFLAGS) $(XSCOPE_CFLAGS)
xscope_LDADD = $(XSCOPE_LIBS)

xscope_SOURCES =	\
	audio.c \
	bigreqscope.h \
	common.c \
	decode11.c \
	decode_bigreq.c \
	decode_lbx.c \
	decode_randr.c \
	decode_render.c \
	decode_shm.c \
	decode_wcp.c \
	decodenas.c \
	extensions.c \
	extensions.h \
	fd.c \
	fd.h \
	lbxscope.h \
	nas.h \
	print11.c \
	print_bigreq.c \
	print_lbx.c \
	print_randr.c \
	print_render.c \
	print_shm.c \
	print_wcp.c \
	printnas.c \
	proto.h \
	prtype.c \
	randrscope.h \
	renderscope.h \
	scope-transport.c \
	scope.c \
	scope.h \
	server.c \
	shmscope.h \
	table11.c \
	wcpscope.h \
	x11.h

appman_PRE = \
        xscope.man

appmandir = $(APP_MAN_DIR)

appman_DATA = $(appman_PRE:man=@APP_MAN_SUFFIX@)

EXTRA_DIST = $(appman_PRE) ChangeLog autogen.sh
CLEANFILES = $(appman_DATA)
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog

SED = sed

# Strings to replace in man pages
XORGRELSTRING = @PACKAGE_STRING@
  XORGMANNAME = X Version 11

MAN_SUBSTS = \
	-e 's|__vendorversion__|"$(XORGRELSTRING)" "$(XORGMANNAME)"|' \
	-e 's|__xorgversion__|"$(XORGRELSTRING)" "$(XORGMANNAME)"|' \
	-e 's|__xservername__|Xorg|g' \
	-e 's|__xconfigfile__|xorg.conf|g' \
	-e 's|__projectroot__|$(prefix)|g' \
	-e 's|__apploaddir__|$(appdefaultdir)|' \
	-e 's|__appmansuffix__|$(APP_MAN_SUFFIX)|g' \
	-e 's|__libmansuffix__|$(LIB_MAN_SUFFIX)|g' \
	-e 's|__adminmansuffix__|$(ADMIN_MAN_SUFFIX)|g' \
	-e 's|__miscmansuffix__|$(MISC_MAN_SUFFIX)|g' \
	-e 's|__filemansuffix__|$(FILE_MAN_SUFFIX)|g'

SUFFIXES = .$(APP_MAN_SUFFIX) .man

.man.$(APP_MAN_SUFFIX):
	$(AM_V_GEN)$(SED) $(MAN_SUBSTS) < $< > $@

if LINT
# Check source code with tools like lint & sparse

ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS)

lint:	$(BUILT_SOURCES)
	@for f in $(xscope_SOURCES) ; do \
	    case $$f in *.c) \
		$(LINT) $(ALL_LINT_FLAGS) $(srcdir)/$$f ;; esac ; \
        done
endif LINT
