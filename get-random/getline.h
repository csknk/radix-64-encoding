#include <stdio.h>
#include <string.h>

// See https://stackoverflow.com/a/4023921/3590673

enum returnVals {
    OK,
    NO_INPUT,
    TOO_LONG
};
int getLine (char *prompt, char *buff, size_t maxInputLength);
