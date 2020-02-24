AUTOMAKE_OPTIONS = gnu

AM_CFLAGS = ${PW13_CFLAGS}

if OS_WIN32
win32_libs = -lws2_32
endif

pw13_clusterdir = ${includedir}/pw13-0/pw13_cluster/common
pw13_cluster_HEADERS =	broadcast.h		\
			win32.h

noinst_LTLIBRARIES = libcommon.la
libcommon_la_SOURCES = \
	nieme_str.c \
	nieme_str.h \
	transmit.c \
	transmit.h \
	type_message.h \
	data_transfer.c \
	data_transfer.h

libcommon_la_CFLAGS = ${PW13_CFLAGS}
libcommon_la_LIBADD = ${PW13_LIBS} -lpthread ${win32_libs}
