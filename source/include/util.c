#include <stdio.h>
#include <stdlib.h>

void error(char* err_msg)
{
    printf("An error has occurred: %s\n", err_msg);
    perror("Error");
    exit(1);
}

int exists(const char* file)
{
    FILE *fptr = fopen(file, "r");
    if(fptr)
    {
        printf("%s exists\n", file);
        fclose(fptr);
        return 1;
    }
    else
    {
        printf("%s does not exists\n", file);
        return 0;
    }
}