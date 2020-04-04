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

int main(int argc, char const *argv[])
{
    short port_number;

    if(argc != 3)
    {
        printf("Usage: ./tftp_server <server IP> <server port>\n");
        exit(1);
    }

    port_number = atoi(argv[2]);

    print_help_text();
    return 0;
}
