CFLAGS = -Wall -g
LDFLAGS=-lgd -lmysqlclient -g

hagraph: hagraph.o

clean:
	rm hagraph *.o
