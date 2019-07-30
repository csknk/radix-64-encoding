#include "base64.h"
#include "radix64Chars.h"

void base64Encode(unsigned char const *inputBuffer, unsigned char **b64Buffer, size_t inputLength)
{
	size_t b64BufferLength = lenCharsBase64(inputLength);
	*b64Buffer = malloc(b64BufferLength * sizeof(*b64Buffer));
	unsigned char tmpByte = 0;
	tmpByte = inputBuffer[0] & (0xFF >> 6);
	unsigned char mask = ~(0xFF >> 6);
	unsigned char lookupVal = (inputBuffer[0] & mask) >> 2;
	*b64Buffer[0] = encodingTable[lookupVal];
	size_t divider = 4;
	size_t i = 1;
	size_t bufIndex = 1;
	while (i < inputLength) {
		tmpByte <<= divider;
		unsigned char mask = ~(0xFF >> divider);
		unsigned char mostSigBits = inputBuffer[i] & mask;
		mostSigBits >>= (8 - divider);
		*(*b64Buffer + bufIndex) = encodingTable[tmpByte ^ mostSigBits];
		bufIndex++;

		if (divider == 2) {
			*(*b64Buffer + bufIndex) = encodingTable[(inputBuffer[i] & (0xFF >> divider))];
			bufIndex++;
			tmpByte = 0;
		} else {
			tmpByte = inputBuffer[i] & (0xFF >> divider);
		}

		divider = (divider - 2) ? divider - 2 : 6;
		i++;
	}
	*(*b64Buffer + bufIndex) = encodingTable[tmpByte <<= divider];

	while (bufIndex < b64BufferLength) {
		*(*b64Buffer + (++bufIndex)) = '=';
	}
	*(*b64Buffer + bufIndex) = '\0';
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
