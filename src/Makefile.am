bin_PROGRAMS = hagraph

hagraph_SOURCES = hagraph.c

LIBS = @LIBS@ @gtk_LIBS@
AM_CFLAGS = @CFLAGS@ @gtk_CFLAGS@

LIBS+=@libhagraph_LIBS@
AM_CFLAGS+=@libhagraph_CFLAGS@
if WIN32
LIBS+= -lmysql
else
LIBS+= -lmysqlclient -L/usr/lib64/mysql -lpq
endif

