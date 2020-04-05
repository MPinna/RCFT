CFLAGS = -Wall

all: tftp_server tftp_client

tftp_server:	tftp_server.o include/util.o
	gcc $(CFLAGS) tftp_server.o include/util.o -o tftp_server

tftp_client: 	tftp_client.o include/util.o
	gcc $(CFLAGS) tftp_client.o include/util.o -o tftp_client

include/util.o:	include/util.c
	gcc $(CFLAGS) -c include/util.c -o include/util.o

clean:
	rm *.o tftp_server tftp_client include/*.o
