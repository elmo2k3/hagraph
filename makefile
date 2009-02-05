CFLAGS = `pkg-config --cflags libglade-2.0` -Wall -fPIC
LDFLAGS= `pkg-config --libs libglade-2.0` -export-dynamic -lhac -lmysqlclient

hagraph: hagraph.o data.o libhagraph.o

clean:
	$(RM) hagraph *.o
