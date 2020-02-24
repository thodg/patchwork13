AUTOMAKE_OPTIONS = gnu

AM_CFLAGS = ${PW13_CFLAGS}

if OS_WIN32
win32_libs = -lws2_32
endif

bin_PROGRAMS = pw13_cluster_d
pw13_cluster_d_SOURCES = \
	broadcast.c \
	broadcast.h \
	client.server.c \
	client_of_server.c \
	client_of_server.h \
	client_of_server_common.c \
	client_of_server_common.h \
	main.c \
	output_server.c \
	output_server.h \
	perf.c \
	perf.h \
	server.c \
	server.h \
	signals.c \
	signals.h \
	struct_server.h \
	tcp.c \
	tcp.h

pw13_cluster_d_CFLAGS = ${PW13_CFLAGS}
pw13_cluster_d_LDADD =	${PW13_LIBS} -lpthread ../common/libcommon.la \
			${win32_libs}
