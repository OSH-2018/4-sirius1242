//#include "libkdump.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LEN 11

const char *secret = "You got the flag!! flag is: flag{";

int main(int argc, char const *argv[])
{
	int i;
//    libkdump_config_t config;
//    config = libkdump_config_t();
//    libkdump_init(config);

    srand(time(NULL));
    char flag[LEN];
    for (i=0;i<LEN-2;i++)
    {
        flag[i] = rand() % 94 + 32;
    }
    flag[LEN-2] = '}';
    flag[LEN-1] = '\n';
    printf("%s%s", secret, flag);
    /* code */
    return 0;
}
