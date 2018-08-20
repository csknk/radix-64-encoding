#include <stdio.h>
#include "getinput.h"
#include "random.h"
#include "base64.h"

int main(void)
{
    unsigned char printableRandomChars[32];
    generateRandom(printableRandomChars, 32, PRINTABLE);
    // printf("generateRandomPrintable = %s\n", printableRandomChars);

    // Get string from user
    // -------------------------------------------------------------------------
    char userInputBuffer[32];
    getInput(userInputBuffer, 32);

    // Allocate memory
    // -------------------------------------------------------------------------
    // unsigned char *bytesBuffer = malloc(strlen(userInputBuffer));
    size_t minReq = ((strlen(userInputBuffer) + 3 - 1) / 3) * 4; // @NB: strlen may not be appropriate for other types of input
    size_t b64BufferLength = !(minReq % 4) ? minReq : (((minReq / 4) + 1) * 4);
    unsigned char *b64Buffer = malloc(b64BufferLength);

    base64Encode((unsigned char *)userInputBuffer, b64Buffer, strlen(userInputBuffer));
    printf("base64 encoded:\t%s\n", b64Buffer);

    return 0;
}
