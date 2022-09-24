#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm-generic/termbits.h>

typedef unsigned int uint;

uint parse_uint(char *str) {
    long speed = atol(str);
    if (speed <= 0) {
        dprintf(2, "invalid baud rate: %s\n", str);
        return 0;
    }
    return speed;
}

int set_baud_rate(int fd, uint ispeed, uint ospeed) {
    struct termios2 tio;
    if (ioctl(fd, TCGETS2, &tio) == -1) {
        perror("ioctl");
        return -1;
    }

    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_ispeed = ispeed;
    tio.c_ospeed = ospeed;
    if (ioctl(fd, TCSETS2, &tio) == -1) {
        perror("ioctl");
        return -1;
    }
    return 0;
}

int get_baud_rate(int fd, uint *ispeed, uint *ospeed) {
    struct termios2 tio;
    if (ioctl(fd, TCGETS2, &tio) == -1) {
        perror("ioctl");
        return -1;
    }
    *ispeed = tio.c_ispeed;
    *ospeed = tio.c_ospeed;
    return 0;
}

int main(int argc, char *argv[]) {
    if (!(argc == 2 || argc == 3 || argc == 4) || strcmp("--help", argv[1]) == 0) {
        dprintf(2,
                "usage (get baud rate): %s device\n"
                "usage (set baud rate): %s device baudrate\n"
                "usage (set baud rate i&o): %s device ispeed ospeed\n",
                argv[0], argv[0], argv[0]);
        return -1;
    }
    if (strcmp("--help", argv[1]) == 0) {
    }

    int fd;
    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror("open");
        return -1;
    }

    if (argc == 2) {
        // get
        uint ispeed, ospeed;
        if (get_baud_rate(fd, &ispeed, &ospeed) == -1) {
            return -1;
        }
        printf("device=%s, ispeed=%u, ospeed=%u\n", argv[1], ispeed, ospeed);
        return 0;
    } else if (argc == 3) {
        // set
        uint speed;
        if ((speed = parse_uint(argv[2])) == 0) {
            return -1;
        }
        return set_baud_rate(fd, speed, speed);
    } else if (argc == 4) {
        // set i&o
        uint ispeed, ospeed;
        if ((ispeed = parse_uint(argv[2])) == 0
                || (ospeed = parse_uint(argv[3])) == 0) {
            return -1;
        }
        return set_baud_rate(fd, ispeed, ospeed);
    }
    return -1;
}
