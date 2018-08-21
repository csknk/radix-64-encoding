#include "base64.h"
#include "radix64Chars.h"

void base64Encode(unsigned char const *inputBuffer, unsigned char *b64Buffer, size_t inputLength)
{
    size_t b64BufferLength = lenCharsBase64(inputLength);
    printf("This function should output %lu base 64 characters.\n", b64BufferLength);

    // The first character: mpByte brings data forward to the while loop,
    // lookupVal gets the first 6 bits of this byte to buuild the first b64 char
    // -------------------------------------------------------------------------
    unsigned char tmpByte = 0;
    tmpByte = inputBuffer[0] & (0xFF >> 6);                 // Mask against 00000011
    unsigned char mask = ~(0xFF >> 6);                      // Mask is 11111100
    unsigned char lookupVal = (inputBuffer[0] & mask) >> 2; // After getting the first 6 bits, shift them
    b64Buffer[0] = encodingTable[lookupVal];                // Get the b64 character
    size_t divider = 4;                                     // First run: set the divider for next char

    // Loop to the penultimate character (assuming last char is a newline)
    // The last 2 bits of tmpByte is the last 2 bits of the first char
    // -------------------------------------------------------------------------
    size_t i = 1;
    size_t bufIndex = 1;
    while (i < inputLength) {
        // Combine current char data with the previous (tmpByte)
        // ---------------------------------------------------------------------
        tmpByte <<= divider;                                        // Shift bits from previous iteration to the correct position
        unsigned char mask = ~(0xFF >> divider);                    // Mask representing the first `divider` bits of a byte
        unsigned char mostSigBits = inputBuffer[i] & mask;          // Collect the first `divider` bits of the current char
        mostSigBits >>= (8 - divider);                              // Shift to correct position
        b64Buffer[bufIndex] = encodingTable[tmpByte ^ mostSigBits]; // Add combined bytes to the return buffer
        bufIndex++;

        // If the divider == 2, set the last 6 bits as a new radix 64 character
        // ---------------------------------------------------------------------
        if (divider == 2) {
            b64Buffer[bufIndex] = encodingTable[(inputBuffer[i] & (0xFF >> divider))];
            bufIndex++;
            tmpByte = 0; // In this case, there is no data to carry forward
        } else {
            // Carry forward the unused bits from the current character
            tmpByte = inputBuffer[i] & (0xFF >> divider);   // Apply a logical & against a mask of the last bits
        }

        // Set divider for the next char
        // ---------------------------------------------------------------------
        divider = (divider - 2) ? divider - 2 : 6;
        i++;
    }
    // Add the remaining data from the tmpByte
    b64Buffer[bufIndex] = encodingTable[tmpByte <<= divider];

    // Add padding chars as required
    // -------------------------------------------------------------------------
    while (bufIndex < b64BufferLength) {
        b64Buffer[++bufIndex] = '=';
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
