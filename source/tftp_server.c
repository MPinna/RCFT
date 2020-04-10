#include "include/const.h"
#include "include/util.h"
#include <time.h>

#define MAX_CLIENTS 10
#define MAX_DIR_LENGTH 200
#define LOG_FILE "./log.txt"
#define LOG_LOCK_FILE "./log.lock"

void log(char* message)
{
    FILE *logptr;
    logptr = fopen(LOG_FILE, "w+");
    if(logptr == 0)
    {
        printf("Error: can't open %s\n", LOG_FILE);
        return;
    }


}

int send_error()
{
    return 0;
}

int main(int argc, char const *argv[])
{
    int ret, listening_socket, transfer_socket;
    unsigned short port_number;
    FILE* log;

    pid_t PID;

    struct sockaddr_in my_addr, sv_transfer_addr, cl_addr;

    char local_directory[MAX_DIR_LENGTH];

    if(argc != 3)
    {
        printf("Usage: ./tftp_server <port> <path/to/files>\n");
        exit(1);
    }


    port_number = atoi(argv[1]);
    if(strlen(argv[2]) >= MAX_DIR_LENGTH)
    {
        printf("Pathname too long. Quitting...");
        exit(0);
    }
    strcpy(local_directory, argv[2]);
    
    printf("Providing files in directory %s on port %d\n", local_directory, port_number);

    listening_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(listening_socket < 0)
        error("creation of the socket");

    printf("Listening socket successfully created: %d\n", listening_socket);

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port_number);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listening_socket, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(ret)
        error("bind on listening_socket");

    printf("bind() succesful on socket %d\n", listening_socket);
    printf("Ready to serve requests on port %d\n", port_number);


    return 0;
}
