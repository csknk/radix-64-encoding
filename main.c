#include <stdio.h>
#include <stdlib.h>
#include "getinput.h"
#include "base64.h"

int main(void)
{
    // Get string from user
    // -------------------------------------------------------------------------
    char userInputBuffer[32]; // Max of 32 chars
    getInput(userInputBuffer, 32);

    // Allocate memory
    // @NB: strlen may not be appropriate for other types of input (e.g. binary data)
    // -------------------------------------------------------------------------
    size_t minReq = ((strlen(userInputBuffer) + 3 - 1) / 3) * 4;
    size_t b64BufferLength = !(minReq % 4) ? minReq : (((minReq / 4) + 1) * 4);
    unsigned char *b64Buffer = malloc(b64BufferLength);

    // Base 64 Encode
    // -------------------------------------------------------------------------
    base64Encode((unsigned char *)userInputBuffer, b64Buffer, strlen(userInputBuffer));
    printf("base64 encoded:\t%s\n", b64Buffer);

    return 0;
}
