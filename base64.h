#include <math.h>
#include <stdio.h>
// #include "radix64Chars.h"

size_t lenCharsBase64(size_t inputLength);
void base64Encode(unsigned char *inputBuffer, unsigned char *b64Buffer, size_t inputLength);
