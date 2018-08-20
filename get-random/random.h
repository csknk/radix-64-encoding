#include <gcrypt.h>
#include <ctype.h>

enum randomType {
    ALL_CHARS,
    PRINTABLE
};

int generateRandom(unsigned char *buf, size_t length, int printable);
