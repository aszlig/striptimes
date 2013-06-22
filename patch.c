#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "patch.h"

#define CATCH_MATCH_ERR(expr, val) \
    if (expr != val) return -1

#define CATCH_NEG_ERR(expr) \
    if (expr == -1) return -1

int patch_gzip(int fd)
{
    const int32_t ts = 0;

    CATCH_NEG_ERR(lseek(fd, 4, SEEK_SET));
    CATCH_NEG_ERR(write(fd, &ts, 4));

    return 0;
}

int patch_ar(int fd)
{
    int len;
    char buf[2];
    char size[10];

    /* skip "!<arch>" */
    CATCH_NEG_ERR(lseek(fd, 7, SEEK_SET));

    for (;;) {
        /* skip newlines */
        while ((len = read(fd, buf, 1)) == 1)
            if (*buf != '\n') break;

        if (len == 0)
            break;

        CATCH_NEG_ERR(lseek(fd, 15, SEEK_CUR));
        CATCH_NEG_ERR(write(fd, "0           ", 12));
        CATCH_NEG_ERR(lseek(fd, 20, SEEK_CUR));
        CATCH_MATCH_ERR(read(fd, size, 10), 10);
        CATCH_MATCH_ERR(read(fd, buf, 2), 2);

        if (buf[0] != '`' || buf[1] != '\n') {
            errno = EINVAL;
            return -1;
        }

        CATCH_NEG_ERR(lseek(fd, atoi(size), SEEK_CUR));
    }

    return 0;
}
