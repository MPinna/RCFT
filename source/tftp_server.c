#include "include/const.h"
#include "include/util.h"
#include <time.h>

#define MAX_CLIENTS 10
#define MAX_DIR_LENGTH 200
#define BASE_DIRECTORY "./files/\0"
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

void send_ERR(int listening_socket, struct sockaddr_in cl_addr, short errcode)
{
    uint16_t opcode_n = htons(ERR_OPCODE);
    uint16_t err_code_n = htons(errcode);
    int pos = 0, ret;
    char buffer[MAX_PKT_SIZE];

    // craft ERR packet
    memcpy(buffer + pos, &opcode_n, sizeof(opcode_n));
    pos += sizeof(opcode_n);

    memcpy(buffer + pos, &err_code_n, sizeof(err_code_n));
    pos += sizeof(err_code_n);

            // strcpy(buffer + pos, "\0");
    memcpy(buffer + pos, "\0", 1); // no error string attached
    pos += 1;

    // send ERR packet to client
    ret = sendto(listening_socket, buffer, pos, 0, (struct sockaddr*)&cl_addr, sizeof(cl_addr));
    if(ret == -1)
        printf("An error has occurred while trying to send the ERR packet to the client\n");
    return;
}

int is_request_valid(int listening_socket, char *buffer, int* transfer_mode, struct sockaddr_in cl_addr)
{
    uint16_t opcode;
    char local_filename[MAX_DIR_LENGTH],
        transfer_mode_s[TRANSFER_MODE_MAX_LEN],
        filepath[2*MAX_DIR_LENGTH]; // will contain the full path of the file being requested
    
    strcpy(filepath, BASE_DIRECTORY);

    int pos = 0;
    memcpy(&opcode, buffer + pos, sizeof(opcode));
    pos += sizeof(opcode);

    opcode = ntohs(opcode);
    if(opcode != RRQ_OPCODE)
    {
        send_ERR(listening_socket, cl_addr, ILLEGAL_OP_ERRCODE);
        printf("Illegal operation\n");
        return 0;
    }

    strcpy(local_filename, buffer + pos);
    pos += strlen(local_filename) + 1; // add 1 to account for string terminator

    strcpy(transfer_mode_s, buffer + pos);
    strcat(filepath, local_filename);
    if(!exists(filepath))
    {
        send_ERR(listening_socket, cl_addr, NOT_FOUND_ERRCODE);
        printf("File not found\n");
        return 0;
    }

    if(strcmp(transfer_mode_s, NETASCII_MODE_S))
        *transfer_mode = NETASCII_MODE;
    else
        if(strcmp(transfer_mode_s, OCTET_MODE_S))
        *transfer_mode = OCTET_MODE;
    return 1;
}

int main(int argc, char const *argv[])
{
    int ret, transfer_mode, listening_socket, transfer_socket, transfer_ID = 0;
    unsigned short port_number;
    FILE* log;

    pid_t PID;

    struct sockaddr_in my_addr, sv_transfer_addr, cl_addr;
    int cl_addr_len = sizeof(cl_addr);

    char local_directory[MAX_DIR_LENGTH],
         buffer[MAX_PKT_SIZE];

    if(argc != 3)
    {
        printf("Usage: ./tftp_server <port> <path/to/files>\n");
        exit(0);
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
        error("could not create listening socket");

    printf("Listening socket successfully created: %d\n", listening_socket);

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port_number);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listening_socket, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(ret == -1)
        error("could not bind on listening_socket");

    printf("bind() successful on socket %d\n", listening_socket);
    printf("Ready to serve requests on port %d\n", port_number);

    while(1)
    {
        ret = recvfrom(listening_socket, buffer,MAX_PKT_SIZE, 0, (struct sockaddr*)&cl_addr, &cl_addr_len);
        if(ret == -1)
            error("An error has occurred while receiving the client request\n");

        if(is_request_valid(listening_socket, buffer, &transfer_mode, cl_addr))
        {
            
        }

    }


    return 0;
}
