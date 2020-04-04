all: tftp_server tftp_client

tftp_server:	tftp_server.o
	gcc -Wall tftp_server.o -o tftp_server

tftp_client: 	tftp_client.o
	gcc -Wall tftp_client.o -o tftp_client

clean:
	rm *.o tftp_server tftp_client