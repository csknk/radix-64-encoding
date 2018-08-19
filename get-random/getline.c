#include "getline.h"

int getLine (char *prompt, char *buff, size_t maxInputLength) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prompt != NULL) {
        printf ("%s", prompt);
        fflush (stdout);
    }
    if (fgets (buff, maxInputLength, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. Flush to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}
