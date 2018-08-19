#include <stdio.h>
#include <gcrypt.h>
#include <math.h>
#include "radix64Chars.h"
#include "getline.h"
void base64Encode(unsigned char *buffer, unsigned char *b64Buffer, size_t inputLength, size_t *outputLength);
// void base64Encode(unsigned char *buffer, unsigned char *b64Buffer, size_t inputLength);

int main(void)
{
    // static char *decoding_table = NULL;
    const char *version = "1.6.5";
    const char *outputVersion;
    if (!(outputVersion = gcry_check_version (version)))
    {
        fputs ("libgcrypt version mismatch\n", stderr);
        exit (2);
    }
    // printf("Installed: %s\n", outputVersion);
    unsigned char buf[32];
    gcry_randomize (buf, 32, GCRY_STRONG_RANDOM);

    // Get string from user
    // -------------------------------------------------------------------------
    int rc;
    char userInputBuffer[32];
    rc = getLine ("Enter string: ", userInputBuffer, sizeof(userInputBuffer));
    if (rc == NO_INPUT) {
        printf ("\nNo input\n");
        return 1;
    }
    if (rc == TOO_LONG) {
        printf ("Input too long [%s]\n", userInputBuffer);
        return 1;
    }
    printf ("OK [%s]\n", userInputBuffer);
    printf("strlen(userInputBuffer): %lu\n", strlen(userInputBuffer));

    // Allocate memory
    // -------------------------------------------------------------------------
    unsigned char *bytesBuffer = malloc(strlen(userInputBuffer));
    size_t minReq = ((strlen(userInputBuffer) + 3 - 1) / 3) * 4; // @NB: strlen may not be appropriate for other types of input
    // size_t b64BufferLength = !(minReq % 4) ? minReq : (((minReq / 4) + 1) * 4);
    size_t *b64BufferLength;
    // b64BufferLength should refer to OUTPUT buffer not input.
    unsigned char *b64Buffer = malloc(b64BufferLength);

    base64Encode((unsigned char *)userInputBuffer, b64Buffer, strlen(userInputBuffer), &b64BufferLength);

    *b64BufferLength = !(minReq % 4) ? minReq : (((minReq / 4) + 1) * 4)

    for (size_t i = 0; b64Buffer[i]; i++) {
        printf("Result index: b64Buffer[%lu] = %c\n", i, b64Buffer[i]);
    }
    printf("\nRadix 64 encoded: %s\n\n", b64Buffer);
    free(bytesBuffer);
    free(b64Buffer);
    return 0;
}

// void base64Encode(unsigned char *inputBuffer, unsigned char *b64Buffer, size_t inputLength)
void base64Encode(unsigned char *inputBuffer, unsigned char *b64Buffer, size_t inputLength, size_t *outputLength)
{
    unsigned char tmpByte;
    tmpByte = 0;
    tmpByte = (inputBuffer[0]) & 3;                     // Mask against 00000011
    size_t lookupVal = (inputBuffer[0] >>= 2) & 63;     // Mask against 00111111 - should be (1 << 5)
    b64Buffer[0] = encodingTable[lookupVal];            // Mask against 00111111 - should be (1 << 5)
    size_t divider = 4;                                 // First run: set the divider for next char

    size_t i = 1;
    size_t bufIndex = 1;
    while (i < inputLength) {
        tmpByte <<= divider;                                // Last bits from previous iteration. Shift bits: from 0000xxxx to 00xxxx00
        unsigned char mask = ~(0xFF >> divider);            // Get the required most significant bits from the new char
        unsigned char mostSigBits = inputBuffer[i] & mask;  // Put these in the position to be combined with the last bits from the previous iteration
        mostSigBits >>= (8 - divider);
        b64Buffer[bufIndex] = encodingTable[tmpByte ^ mostSigBits];

        // If the divider is 2, take the next 6 bits!!
        if (divider == 2) {
            unsigned char mask2 = 0xFF >> divider;
            tmpByte = 0;
            b64Buffer[++bufIndex] = encodingTable[inputBuffer[i] & mask2];
            divider = 0; // get first 6 bits of the next byte
        } else {
            // mask to get the unused bits from the current character:
            tmpByte = inputBuffer[i] & (0xFF >> divider);
        }
        bufIndex++;
        switch (divider) {
            case 2:
                divider = 6;
                break;
            case 6:
                divider = 4;
                break;
            default:
                divider = 6 - divider;
        }
        i++;
        printf("i: %lu\n", i);
    }
    printf("bufIndex = %lu\n", bufIndex);
    b64Buffer[bufIndex] = '\0';
}
