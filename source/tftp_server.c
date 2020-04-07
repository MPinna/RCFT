#include "include/const.h"
#include "include/util.h"

#define MAX_CLIENTS 10


void error(char* err_msg)
{
    printf("An error has occurred in: %s\n", err_msg);
    perror("Error");
    exit(1);
}

    return 0;
}

int main(int argc, char const *argv[])
{
    short port_number;

    if(argc != 3)
    {
        printf("Usage: ./tftp_server <port> <path/to/files>\n");
        exit(1);
    }


    port_number = atoi(argv[1]);
    
    // printf("Providing files in directory %s on port %d\n", argv[2], port_number);
    printf("%d %d %d %d %d", RRQ_OPCODE, WRQ_OPCODE, DTA_OPCODE, ACK_OPCODE, ERR_OPCODE);
    return 0;
}
