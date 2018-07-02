// include works of https://github.com/IAIK/meltdown
#include "libkdump/libkdump.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LEN 11

char *secret = "You got the flag!! flag is: flag{";

int main(int argc, char const *argv[])
{
	int i;
    int len;
    char *message;
    libkdump_config_t config;
    config = libkdump_get_autoconfig();
    libkdump_init(config);

    srand(time(NULL));
    char flag[LEN];
    for (i=0;i<LEN-2;i++)
    {
        flag[i] = rand() % 94 + 32;
    }
    flag[LEN-2] = '}';
    flag[LEN-1] = '\n';
    printf("Secret: %s%s", secret, flag);
    sprintf(message, "%s%s", secret, flag);
    len = strlen(message);

    size_t paddr = libkdump_virt_to_phys((size_t)secret);
    if (!paddr) 
    {
        printf("Program requires root privileges (or read access to /proc/<pid>/pagemap)!\n");
        libkdump_cleanup();
        exit(1);
    }

    printf("Physical address of secret: %zx\n", paddr);
    printf("Exit with Ctrl+C if you are done reading the secret\n");
    while (1)
    {
        volatile size_t dummy = 0, i;
        for (i = 0; i < len; i++)
        {
            dummy += message[i];
        }
        sched_yield();
    }

    libkdump_cleanup();
    /* code */
    return 0;
}
