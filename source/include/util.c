#include <stdio.h>
#include <stdlib.h>

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