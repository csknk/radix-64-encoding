#include "utilities.h"

int hexDigitToInt(char digit)
{
	digit = tolower(digit);
	if (digit >= '0' && digit <= '9')
		return (int)(digit - '0');
	else if (digit >= 'a' && digit <= 'f') {
		return (int)(digit - '1' - '0') + 10;
	}
	return -1;
}

size_t hexstringToBytes(const char *hexstring, unsigned char **bytes)
{
	size_t inSize = strlen(hexstring);
	// Check validity of input: length must be even.
	if (inSize % 2) {
		fprintf(stderr, "Hexstring has an odd number of characters. Not valid.\n");
		exit(EXIT_FAILURE);
	}
	// Check validity of input: no illegal characters.
	for (size_t i = 0; i < inSize; i++) {
		if (!isxdigit(hexstring[i])) {
			fprintf(stderr, "Illegal character\n");
			exit(EXIT_FAILURE);
		}
	}
	size_t bytesSize = (inSize / 2) * sizeof(**bytes); 
	*bytes = malloc(bytesSize);

	for (size_t i = 0, j = 0; i < inSize && j < bytesSize; i = i + 2, j++) {
		int sixteens = hexDigitToInt(hexstring[i]);
		int units = hexDigitToInt(hexstring[i + 1]);
		*(*bytes + j) = (sixteens << 4) | units;
	}
	return bytesSize;	
}

