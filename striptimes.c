#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

void patch_gzip(int fd)
{
    const int32_t ts = 0;

    if (lseek(fd, 4, SEEK_SET) == -1)
        perror("lseek");
    else if (write(fd, &ts, 4) == -1)
        perror("write");
}

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
        patch_gzip(fd);

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
