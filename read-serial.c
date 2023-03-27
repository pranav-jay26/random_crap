//
//  wheew.c
//  sus
//
//  Created by Pranav Jayakumar on 3/27/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BAUDRATE B115200
#define SERIALPORT "/dev/ttyUSB0"

int main() {
    int fd;
    struct termios options;
    char buffer[256];
    int n;

    fd = open(SERIALPORT, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyUSB0 - ");
        return 1;
    }

    fcntl(fd, F_SETFL, 0);

    tcgetattr(fd, &options);
    cfsetispeed(&options, BAUDRATE);
    cfsetospeed(&options, BAUDRATE);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    tcsetattr(fd, TCSANOW, &options);

    while (1) {
        memset(buffer, '\0', sizeof(buffer));
        n = read(fd, buffer, sizeof(buffer));

        if (n > 0) {
            printf("Received: %s", buffer);
        }

        usleep(100000);
    }

    close(fd);

    return 0;
}

