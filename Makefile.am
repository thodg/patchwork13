##  Patchwork13! The generalist modular synthesis toolkit.
##  Copyright (C) 2003-2007  Thomas de Grivel <billitch@gmail.com>
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

SUBDIRS = pw13 std gtk stk snd sdl
.NOTPARALLEL:

dist_bin_SCRIPTS = pw13-makefile-am

doc:

EXTRA_DIST = \
	debian/changelog \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/rules \
	debian/pw13_gtk.xml

dist_gzip = ${PACKAGE_TARNAME}-${PACKAGE_VERSION}.tar.gz
dist_orig = ${PACKAGE_TARNAME}_${PACKAGE_VERSION}.orig.tar.gz

deb: dist-orig
	test ! -d $(distdir) || rm -rf $(distdir)
	tar xzf $(dist_orig)
	cd $(distdir) && debuild

dist-orig: dist-gzip
	mv $(dist_gzip) $(dist_orig)

CLEANFILES = $(dist_orig) $(distdir).tar.bz2 $(distdir).tar.gz
