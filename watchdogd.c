#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/watchdog.h>

#define DEV_NAME "/dev/watchdog"

int watchdogd_main(int argc, char **argv)
{
    int fd;
    int ret;
    int interval = 10;
    int margin = 10;
    int timeout;

    fprintf(stdout, "Starting watchdogd\n");

    if (argc >= 2)
        interval = atoi(argv[1]);

    if (argc >= 3)
        margin = atoi(argv[2]);

    timeout = interval + margin;

    fd = open(DEV_NAME, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "watchdogd: Failed to open %s: %s\n", DEV_NAME, strerror(errno));
        return 1;
    }

    ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
    if (ret) {
        fprintf(stderr, "watchdogd: Failed to set timeout to %d: %s\n", timeout, strerror(errno));
        ret = ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
        if (ret) {
            fprintf(stderr, "watchdogd: Failed to get timeout: %s\n", strerror(errno));
        } else {
            if (timeout > margin)
                interval = timeout - margin;
            else
                interval = 1;
            fprintf(stderr, "watchdogd: Adjusted interval to timeout returned by driver: timeout %d, interval %d, margin %d\n",
                  timeout, interval, margin);
        }
    }

    while(1) {
        write(fd, "", 1);
        sleep(interval);
    }
}

