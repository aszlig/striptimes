#include <sys/types.h>
#include <unistd.h>

#include "patch.h"

int patch_gzip(int fd)
{
    const int32_t ts = 0;

    if (lseek(fd, 4, SEEK_SET) == -1)
        return -1;

    if (write(fd, &ts, 4) == -1)
        return -1;

    return 0;
}
