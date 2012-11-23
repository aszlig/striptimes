#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "patch.h"

void progress(const char *path)
{
    int fd;
    unsigned char buf[2];

    if ((fd = open(path, O_RDWR)) == -1) {
        perror("open");
        return;
    }

    if ((read(fd, &buf, 2)) == -1)
        perror("read");
    else if (*buf == 0x1f && *(buf + 1) == 0x8b)
        if (patch_gzip(fd) == -1) perror("patch_gzip");

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s FILENAME...\n", argv[0]);
        return 1;
    }

    while (argc-- > 1)
        progress(argv[argc]);

    return 0;
}
