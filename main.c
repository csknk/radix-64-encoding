#include <stdio.h>
#include <stdlib.h>
#include "getinput.h"
#include "base64.h"
#include "utilities.h"

int main(void)
{
	char *hexstring = NULL;
	getDynamicInput(&hexstring);

	unsigned char *bytes = NULL;
	size_t nBytes = hexstringToBytes(hexstring, &bytes);
	free(hexstring);

	unsigned char *b64Buffer = NULL;
	base64Encode(bytes, &b64Buffer, nBytes); 
	free(bytes);
	printf("base64 encoded:\t%s\n", b64Buffer);
	free(b64Buffer);

	return 0;
}
