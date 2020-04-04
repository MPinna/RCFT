#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "const.h"

#define QUIET_MODE_FLAG "-q"
#define MAX_USER_CMD_LENGTH 1024
#define NETASCII_MODE 0
#define OCTET_MODE 1

void print_help_text()
{
    char* help_text = "Sono disponibili i seguenti comandi:\n!help\t\t\t\t--> mostra l'elenco dei comandi disponibili\n!mode {txt|bin} \t\t--> imposta il modo di trasferimento dei files (testo o binario)\n!get <filename> <nome_locale>   --> richiede al server il file <filename>  e lo salva localmente con il nome <nome_locale>\n!quit\t\t\t\t--> termina il client\n";
    printf("%s", help_text);
}

int get_user_command(char *cmd)
{
    if(strncmp(cmd, "!help ", 6) == 0)
        return 0;
    if(strncmp(cmd, "!mode ", 6) == 0)
        return 1;
    if(strncmp(cmd, "!get ", 5) == 0)
        return 2;
    if(strncmp(cmd, "!quit ", 6) == 0)
        return 3;
    return -1;
}

int get_transfer_mode(char *cmd)
{
    if(strcmp(cmd, "!get txt") == 0)
        return 0;
    if(strcmp(cmd, "!get bin") == 0)
        return 1;
    return -1;
}

int main(int argc, char const *argv[])
{
    int quiet_mode = 0;
    int cmd_code = 0;
    short port_number;
    int temp_mode, transfer_mode;
    char* NETASCII_MODE = "netascii",
          *OCTET_MODE = "octet",
          user_cmd[MAX_USER_CMD_LENGTH];

    if(argc < 3)
    {
        printf("Usage: ./tftp_server <server IP> <server port>\n");
        exit(1);
    }

    // set quiet mode if user passed '-q' flag
    if(argc == 4 && !(strcmp(argv[3], QUIET_MODE_FLAG)))
        quiet_mode = 1;

    if(!quiet_mode)
        print_help_text();

    port_number = atoi(argv[2]);

    print_help_text();
    return 0;
}
