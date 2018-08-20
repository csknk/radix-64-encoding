#include "base64.h"
#include "radix64Chars.h"

void base64Encode(unsigned char *inputBuffer, unsigned char *b64Buffer, size_t inputLength)
{
    size_t b64BufferLength = !(inputLength % 4) ? inputLength : (((inputLength / 4) + 1) * 4);

    unsigned char tmpByte;
    tmpByte = 0;
    tmpByte = (inputBuffer[0]) & 3;                     // Mask against 00000011
    size_t lookupVal = (inputBuffer[0] >>= 2) & 63;     // Mask against 00111111 - should be (1 << 5)
    b64Buffer[0] = encodingTable[lookupVal];            // Mask against 00111111 - should be (1 << 5)
    size_t divider = 4;                                 // First run: set the divider for next char

    size_t i = 1;
    size_t bufIndex = 1;

    // The indexing is not right. Need to step through chars, but have different indexing for output
    while (i < inputLength) {
        if (bufIndex > b64BufferLength)
            break;

        if (i == inputLength) {
            printf("Last element!!!!\n");
        }

        tmpByte <<= divider;                                // Last bits from previous iteration. Shift bits: from 0000xxxx to 00xxxx00
        unsigned char mask = ~(0xFF >> divider);            // Get the required most significant bits from the new char
        unsigned char mostSigBits = inputBuffer[i] & mask;  // Put these in the position to be combined with the last bits from the previous iteration
        mostSigBits >>= (8 - divider);
        b64Buffer[bufIndex] = encodingTable[tmpByte ^ mostSigBits];

        // If the divider is 2, take the next 6 bits from the current char
        if (divider == 2) {
            unsigned char mask2 = 0xFF >> divider;
            tmpByte = 0;
            b64Buffer[++bufIndex] = encodingTable[inputBuffer[i] & mask2];
            divider = 0; // get first 6 bits of the next byte
        } else {
            // mask to get the unused bits from the current character:
            tmpByte = inputBuffer[i] & (0xFF >> divider);
        }
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
        bufIndex++;
        i++;
        // last element, an incomplete sextuplet
        if (i == inputLength && ((inputLength * 8) % 6)) {
            divider = 2;
        }
    }
    // Figure out how to get teh last char here.
    // b64Buffer[bufIndex] = 'X';
    // at this point tmpByte is WRONG @TODO
    b64Buffer[bufIndex] = encodingTable[tmpByte >> divider];
    while (i == inputLength && bufIndex < b64BufferLength) {
        // printf("here bufIndex %lu\n", bufIndex);
        b64Buffer[++bufIndex] = '=';
    }
    b64Buffer[bufIndex] = '\0';
}

size_t lenCharsBase64(size_t inputLength)
{
    return inputLength * 2;
}
