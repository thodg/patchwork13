AUTOMAKE_OPTIONS = gnu

AM_CFLAGS = ${PW13_CFLAGS}

if OS_WIN32
pw13_cluster_def = libpw13_cluster_c-0.def
win32_libs = -lws2_32 -export-symbols $(pw13_cluster_def)
endif

pw13_clusterdir = ${includedir}/pw13-0/pw13_cluster/client
pw13_cluster_HEADERS = \
	broadcast.h \
	client.h \
	pw13_server.h \
	servlist.h


lib_LTLIBRARIES = libpw13_cluster_c-0.la

libpw13_cluster_c_0_la_SOURCES = \
	broadcast.c \
	broadcast.h \
	servlist.c \
	servlist.h \
	tcp.c \
	tcp.h

libpw13_cluster_c_0_la_CFLAGS = ${PW13_CFLAGS}
libpw13_cluster_c_0_la_LDFLAGS = ${PW13_LIBS} ../common/libcommon.la \
				-lpthread -no-undefined ${win32_libs}


bin_PROGRAMS = pw13_cluster_c

pw13_cluster_c_SOURCES = \
	broadcast.c \
	broadcast.h \
	client_message_server.c \
	client_message_server.h \
	client_message_server_dupli.c \
	client_message_server_dupli.h \
	client_message_server_scat.c \
	client_message_server_scat.h \
	iplist.c \
	iplist.h \
	main.c \
	pw13_server.c \
	pw13_server.h \
	servlist.c \
	servlist.h \
	tcp.c \
	tcp.h

pw13_cluster_c_CFLAGS = ${PW13_CFLAGS}
pw13_cluster_c_LDFLAGS = ../common/libcommon.la -lpthread ${PW13_LIBS}
