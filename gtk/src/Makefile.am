##  pw13_gtk - A Patchwork13! GTK+2 User Interface
##  Copyright (C) 2005  Thomas de Grivel <billitch@gmail.com>
##
##  This program is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, write to the Free Software
##  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

if OS_WIN32
  prefix_flags =
else
  prefix_flags = -DPW13_GTK_DATA_DIR=\"$(pw13_datadir)\"
endif

AM_CFLAGS = $(PW13_CFLAGS) $(PW13_GTK_CFLAGS) $(prefix_flags)

bin_PROGRAMS = pw13_gtk
pw13_gtk_SOURCES = \
	about.c \
	about.h \
	connect.c \
	connect.h \
	drag_and_drop.h \
	icons.c \
	icons.h \
	input.c \
	input.h \
	main.c \
	output.c \
	output.h \
	patch.c \
	patch.h \
	patchwork.c \
	patchwork.h \
	paths.c \
	paths.h \
	play.c \
	play.h \
	pw_close.c \
	pw_close.h \
	pw_draw.c \
	pw_draw.h \
	quit.c \
	quit.h \
	tb_tree.c \
	tb_tree.h \
	toolbox.c \
	toolbox.h \
	types.h \
	xml.c \
	xml.h \
	xml_signals.h


pw13_gtk_CFLAGS = $(PW13_CFLAGS) $(PW13_GTK_CFLAGS) $(prefix_flags)
pw13_gtk_LDADD = $(PW13_LIBS) $(PW13_GTK_LIBS) -lpthread -lm
