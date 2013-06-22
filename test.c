#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "patch.h"

FILE *scratchfile;
int test_failed = 0;

#define ASSERT_BINARY_INPUT \
    size_t len; unsigned int i; \
    const unsigned char new_data[] = {

#define ASSERT_BINARY_MATCH \
    }; \
    const unsigned char match_data[] = {

#define ASSERT_BINARY_BEGIN \
    }; \
    unsigned char match_buf[sizeof(match_data)]; \
    rewind(scratchfile); \
    if (ftruncate(fileno(scratchfile), 0) == -1) \
        TEST_FAILED("Could not truncate scratchfile!"); \
    if (fwrite(new_data, sizeof(unsigned char), sizeof(new_data), \
               scratchfile) != sizeof(new_data)) \
        TEST_FAILED("Could not write test input data!"); \
    rewind(scratchfile);

#define SCRATCH_FD fileno(scratchfile)

#define TEST_FAILED(...) { \
    test_failed = 1; \
    fprintf(stderr, "FAILED: " __VA_ARGS__); \
    return; \
}

#define TEST(name) void test_##name()

#define RUN_TEST(name) \
    fprintf(stderr, "Running test `" #name "'..."); \
    test_##name();

#define ASSERT_BINARY_END \
    rewind(scratchfile); \
    len = fread(match_buf, sizeof(unsigned char), sizeof(match_buf), \
                scratchfile); \
    if (len != sizeof(match_buf)) \
        TEST_FAILED("Binary stream has size %ld but we expected %ld.\n", \
                    len, sizeof(match_data)); \
    if (feof(scratchfile) != 0) \
        TEST_FAILED("Unexpected data at the end of the scratchfile!\n"); \
    for (i = 0; i < len; ++i) if (match_buf[i] != match_data[i]) \
        TEST_FAILED("Expected 0x%02X in binary stream but got 0x%02X at" \
                    " position %d.\n", match_data[i], match_buf[i], i); \
    fprintf(stderr, "PASSED.\n");

TEST(simple_gzip)
{
    ASSERT_BINARY_INPUT
        0x8b, 0x1f, 0x00, 0x08, 0x1b, 0xbb, 0x50, 0xaf, 0x03, 0x00, 0xcb, 0x4b,
        0x4f, 0xcf, 0x2c, 0x4a, 0x02, 0xe2, 0x47, 0x00, 0x2c, 0x97, 0x07, 0xb2,
        0x00, 0x00, 0x00, 0x00
    ASSERT_BINARY_MATCH
        0x8b, 0x1f, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0xcb, 0x4b,
        0x4f, 0xcf, 0x2c, 0x4a, 0x02, 0xe2, 0x47, 0x00, 0x2c, 0x97, 0x07, 0xb2,
        0x00, 0x00, 0x00, 0x00
    ASSERT_BINARY_BEGIN
        patch_gzip(SCRATCH_FD);
    ASSERT_BINARY_END
}

TEST(tooshort_gzip)
{
    ASSERT_BINARY_INPUT
        0x8b, 0x1f, 0x00, 0x01, 0xff, 0x00, 0x01, 0x12, 0xff
    ASSERT_BINARY_MATCH
        0x8b, 0x1f, 0x00, 0x01, 0, 0, 0, 0
    ASSERT_BINARY_BEGIN
        patch_gzip(SCRATCH_FD);
    ASSERT_BINARY_END
}

int main(void)
{
    if ((scratchfile = tmpfile()) == NULL)
        perror("tmpfile");

    RUN_TEST(simple_gzip);
    RUN_TEST(tooshort_gzip);

    if (fclose(scratchfile) == EOF)
        perror("fclose");

    return test_failed;
}
