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
        fclose(fptr);
        return 1;
    }
    else
        return 0;
}