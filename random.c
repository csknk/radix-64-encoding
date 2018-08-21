#include "random.h"

int generateRandom(unsigned char *buf, size_t length, int printable)
{
    const char *version = "1.6.5";
    const char *outputVersion;
    if (!(outputVersion = gcry_check_version (version)))
    {
        fputs ("libgcrypt version mismatch\n", stderr);
        exit (2);
    }
    // printf("Installed: %s\n", outputVersion);
    // unsigned char buf[32];
    // gcry_randomize (buf, length, GCRY_STRONG_RANDOM);

    size_t i = 0;
    while (i < length) {
        unsigned char tmp[1];
        gcry_randomize(tmp, 1, GCRY_STRONG_RANDOM);
        if (printable) {
            if (isprint(tmp[0])) {
                buf[i++] = tmp[0];
            }
        } else {
            buf[i++] = tmp[0];
        }
    }
    return 0;
}
