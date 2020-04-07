#include "include/const.h"
#include "include/util.h"

#define QUIET_MODE_FLAG "-q"
#define MAX_USER_CMD_LENGTH 1024
#define MAX_FILENAME_LENGTH 200
#define NETASCII_MODE 0
#define OCTET_MODE 1

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
    if(strcmp(cmd, "!mode txt\n") == 0)
        return 0;
    if(strcmp(cmd, "!mode bin\n") == 0)
        return 1;
    return -1;
}

int get_filenames(char* user_get_command, char** remote, char** local)
{
    char* token = strtok(user_get_command, " "); //discard "!get " part of user command
    *remote = strtok(NULL, " ");    // save the first two arguments into remote and local
    if(!(*remote))
        return -1;

    *local = strtok(NULL, " ");
    if(!(*local))
        return -1;

    return 0;
}



int main(int argc, char const *argv[])
{
    int quiet_mode = 0, cmd_code = 0, ret,
        temp_mode, transfer_mode = 1; //default transfer mode: bin
    char* NETASCII_MODE_S = "netascii",
          *OCTET_MODE_S = "octet",
          user_cmd[MAX_USER_CMD_LENGTH],
          replace_choice[3],
          *remote_filename,
          *local_filename;

    // sockets variables
    int sd;
    char sv_addr_string[ADDR_STRING_LENGTH];
    short sv_port;
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
    {
        printf("An error has occurred during the creation of the socket\n");
        perror("Error");
        exit(1);
    }

    printf("Socket %d created succesfully\n", sd);

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(sv_port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(sd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(ret == -1)
    {
        printf("An error has occurred during the bind() function\n");
        perror("Error");
        exit(0);
    }

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
                ret = get_filenames(user_cmd, &remote_filename, &local_filename);
                if(ret == -1)
                    printf("Usage: !get <filename> <local_name>\n");
                else
                {
                    printf("remote_filename: %s\n", remote_filename);
                    printf("local_filename: %s\n", local_filename);
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
                    printf("Trasferisco il file remoto %s dal server %s:%d in %s\n", remote_filename, sv_addr_string, sv_port, local_filename);
                    // ret = 
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
    return 0;
}
