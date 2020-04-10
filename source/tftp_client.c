#include "include/const.h"
#include "include/util.h"

#define QUIET_MODE_FLAG "-q"
#define MAX_USER_CMD_LENGTH 1024
#define MAX_FILENAME_LENGTH 200


#define RRQ_PACKET_MAX_SIZE 212
// 2 + 200 + 1 + 8 + 1 [OPCODE + FILENAME + \0 + strlen("netascii") + \0]


void print_help_text()
{
    char* help_text = "The following commands are available:\n!help\t\t\t\t--> lists the available commands\n!mode {txt|bin} \t\t--> set file transfer mode (text or binary)\n!get <filename> <local_name>    --> request the file <filename> to the server and save it locally as <local_name>\n!quit\t\t\t\t--> exit the client\n\n";
    printf("%s", help_text);
}

// Returns an integer associated to the command passed by the user.
// Returns -1 for unknown commands.
int get_user_command(char *cmd)
{
    if(strncmp(cmd, "!help", 5) == 0)
        return 0;
    if(strncmp(cmd, "!mode", 5) == 0)
        return 1;
    if(strncmp(cmd, "!get", 4) == 0)
        return 2;
    if(strncmp(cmd, "!quit", 5) == 0)
        return 3;
    return -1;
}

int get_transfer_mode(char *cmd)
{
    if(strcmp(cmd, "!mode txt") == 0)
        return 0;
    if(strcmp(cmd, "!mode bin") == 0)
        return 1;
    return -1;
}

int get_filenames(char* user_get_command, char *remote, char *local)
{
    int i;
    char* token = strtok(user_get_command, " "); //discard "!get " part of user command
    if(token == NULL)
    {
        error("Malformed get input");
    }

    for(i = 0; token != NULL && i < 2; ++i)// any additional parameters are ignored
    {
        token = strtok(NULL, " ");
        if(token == NULL)
            return -1;
        if(i == 0)
            strcpy(remote, token);
        if(i == 1)
            strcpy(local, token);
    }
    return 0;
}

void txt_transfer(int sd, char* buffer, struct sockaddr_in sv_addr, char* local_filename, int len)
{
    short opcode;

    FILE *dest;
    dest = fopen(local_filename, "w");
    if(!dest)
    {
        printf("Can't open %s\n", local_filename);
        return;
    }
}

void bin_transfer(int sd, char* buffer, struct sockaddr_in sv_addr, char* local_filename, int len)
{
    return;
}

void request_file(int sd, struct sockaddr_in *sv_addr, char *remote_filename, char *local_filename, int transfer_mode)
{
    short opcode = htons(RRQ_OPCODE);
    short errcode;
    char buffer[MAX_PKT_SIZE];

    struct sockaddr_in sv_transfer_addr;

    // socket used to receive the file from the server
    socklen_t sv_transfer_addr_len = sizeof(sv_transfer_addr);
    int ret, response_length, pos = 0;

    // craft the RRQ packet to be sent
    memcpy(buffer + pos, &opcode, sizeof(opcode));
    pos += sizeof(opcode);

    strcpy(buffer + pos, remote_filename);
    pos += strlen(remote_filename) + 1;

    printf("Transfer mode: ");
    if(transfer_mode == 0)
    {
        strcpy(buffer + pos, OCTET_MODE_S);
        pos += strlen(OCTET_MODE_S) + 1;
    }
    else
    {
        strcpy(buffer + pos, NETASCII_MODE_S);
        pos += strlen(NETASCII_MODE_S) + 1;
    }

    // send the RRQ packet
    ret = sendto(sd, buffer, pos, 0, (struct sockaddr*)&sv_addr, sizeof(sv_addr));

    // receive server response
    response_length = recvfrom(sd, buffer, MAX_PKT_SIZE, 0, (struct sockaddr*)&sv_transfer_addr, &sv_transfer_addr_len);

    pos = 0;

    memcpy(&opcode, buffer + pos, sizeof(opcode));
    pos += sizeof(opcode);

    // handle error
    opcode = ntohs(opcode);
    if(opcode == ERR_OPCODE)
    {
        memcpy(&errcode, buffer + pos, sizeof(errcode));
        switch (errcode)
        {
        case NOT_FOUND_ERRCODE:
            printf("Error: file %s not found on server. Quitting...\n", remote_filename);
            break;
        case ILLEGAL_OP_ERRCODE:
            printf("Error: illegal operation. Quitting...\n");
            break;
        default:
            printf("Unknow error type. Quitting...\n");
            break;
        }
        return;
    }

    printf("Starting download...\n");
    if(transfer_mode == NETASCII_MODE_S)
        txt_transfer();
    else
        bin_transfer();
    return;
}

int main(int argc, char const *argv[])
{
    int quiet_mode = 0, cmd_code = 0, ret,
        temp_mode, transfer_mode = 1; //default transfer mode: bin
    char  user_cmd[MAX_USER_CMD_LENGTH],
          replace_choice[3],
          remote_filename[MAX_FILENAME_LENGTH],
          local_filename[MAX_FILENAME_LENGTH];

    // sockets variables
    int sd;
    char sv_addr_string[ADDR_STRING_LENGTH];
    unsigned short sv_port;
    struct sockaddr_in sv_addr, my_addr;
    socklen_t addrlen = sizeof(sv_addr);

    if(argc < 3)
    {
        printf("Usage: ./tftp_server <server IP> <server port> [flags]\n");
        exit(0);
    }

    strcpy(sv_addr_string, argv[1]);
    sv_port = atoi(argv[2]);
    
    // set quiet mode if user passed '-q' flag
    if(argc == 4 && (strcmp(argv[3], QUIET_MODE_FLAG)) == 0)
        quiet_mode = 1;
    if(!quiet_mode)
        print_help_text();


    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd == -1)
        error("Creation of the socket failed");

    printf("Socket %d created succesfully\n", sd);

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = 0; // bind socket to first available port
    my_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(sd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(ret == -1)
        error("bind function failed");

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(sv_port);
    inet_pton(AF_INET, sv_addr_string, &sv_addr.sin_addr);

    while(1)
    {
        fgets(user_cmd, MAX_USER_CMD_LENGTH, stdin);
        user_cmd[strcspn(user_cmd, "\n")] = 0; //remove trailing newline
        cmd_code = get_user_command(user_cmd);
        switch (cmd_code)
        {
            case 0: // !help
                print_help_text();
                break;
            case 1: // !mode
                temp_mode = get_transfer_mode(user_cmd);
                if(temp_mode == -1)
                    printf("Invalid transfer mode\nUsage: !mode {txt|bin}\n");
                else
                {
                    transfer_mode = temp_mode;
                    if(transfer_mode == 0) // txt
                        printf("Transfer mode set to txt\n");
                    else // bin
                        printf("Transfer mode set to bin\n");
                }
                break;
            case 2: // !get
                ret = get_filenames(user_cmd, remote_filename, local_filename);
                if(ret == -1)
                    printf("Usage: !get <filename> <local_name>\n");
                else
                {
                    if(exists(local_filename))
                    {
                        printf("%s already exists. Do you wish to replace it? Y/n\n", local_filename);
                        fgets(replace_choice, 3, stdin);
                        if(strcmp(replace_choice, "n\n") == 0)
                        {
                            printf("You chose not to replace the file\n");
                            break;
                        }
                        if(strcmp(replace_choice, "y\n") != 0)
                        {
                            printf("Invalid choice\n");
                            break;
                        }
                    }
                    printf("Downloading file %s from server at %s:%d to %s. Transfer mode: %s\n", remote_filename, sv_addr_string, sv_port, local_filename, ((transfer_mode == OCTET_MODE)?OCTET_MODE_S:NETASCII_MODE_S));
                    request_file(sd, &sv_addr, remote_filename, local_filename, transfer_mode);

                }
                break;
            case 3: // !quit
                printf("Goodbye\n");
                return 0;
            default:
                printf("Unknow command\n\n");
                print_help_text();
                break;
        }
    }
    close(sd);
    return 0;
}
