CFLAGS = `pkg-config --cflags gtk+-2.0` -Wall -fPIC
LDFLAGS= `pkg-config --libs gtk+-2.0` -export-dynamic -lhac -lmysqlclient

hagraph: hagraph.o data.o libhagraph.o

clean:
	$(RM) hagraph *.o
