#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

void main()
{
    struct stat st = {0};
    if (stat("./quangtest", &st) == -1)
    {
        mkdir("./quangtest", 0700);
    }
    else
    {
        printf("co thu muc do roi\n");
    }

    FILE *fp;
    fp = fopen("./quangtest/quangtest.txt", "w");
    if (fp == NULL)
    {
        printf("Loi doc file");
        exit(1);
    }
    fprintf(fp, "%s %s\n", "hihi", "haha");
    fclose(fp);
}
