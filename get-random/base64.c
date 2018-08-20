#include "base64.h"
#include "radix64Chars.h"

void base64Encode(unsigned char *inputBuffer, unsigned char *b64Buffer, size_t inputLength)
{
    size_t b64BufferLength = lenCharsBase64(inputLength);

    // The first character
    // -------------------------------------------------------------------------
    unsigned char tmpByte;
    tmpByte = 0;
    tmpByte = (inputBuffer[0]) & (0xFF >> 6);                   // Mask against 00000011
    size_t lookupVal = (inputBuffer[0] >>= 2) & (0xFF >> 2);    // Mask against 00111111
    b64Buffer[0] = encodingTable[lookupVal];                    // Get the b64 character
    size_t divider = 4;                                         // First run: set the divider for next char

    // Loop to the penultimate character
    // The last 2 bits of tmpByte is the last 2 bits of the first char
    // -------------------------------------------------------------------------
    size_t i = 1;
    size_t bufIndex = 1;
    while (i <= inputLength) {
        // Combine current char data with the previous (tmpByte)
        // ---------------------------------------------------------------------
        tmpByte <<= divider;                                        // Shift bits from previous iteration to the correct position
        unsigned char mask = ~(0xFF >> divider);                    // Mask representing the first `divider` bits of a byte
        unsigned char mostSigBits = inputBuffer[i] & mask;          // Collect the first `divider` bits of the current char
        mostSigBits >>= (8 - divider);                              // Shift to correct position
        b64Buffer[bufIndex] = encodingTable[tmpByte ^ mostSigBits]; // Add combined bytes to the return buffer

        // If the divider == 2, set the last 6 bits as a new radix 64 character
        // ---------------------------------------------------------------------
        if (divider == 2) {
            b64Buffer[++bufIndex] = encodingTable[(inputBuffer[i] & (0xFF >> divider))];
            tmpByte = 0; // Don't carry data forward
        } else {
            // mask to get the unused bits from the current character:
            tmpByte = inputBuffer[i] & (0xFF >> divider);
            bufIndex++;
        }

        // Set divider for the next char
        // ---------------------------------------------------------------------
        divider = (divider - 2) ? divider - 2 : 6;
        i++;
    }
    b64Buffer[bufIndex] = '\0';
}

/**
 * Calculate the correct total length for a base 64 string
 */
size_t lenCharsBase64(size_t inputLength)
{
    size_t ret;
    if (inputLength % 3) {
        ret = ((inputLength / 3) + 1) * 4;
    } else {
        ret = (inputLength / 3) * 4;
    }
    return ret;
}
