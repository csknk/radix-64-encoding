#include "getinput.h"

int getInput(char *userInputBuffer, size_t length)
{
    int rc;
    rc = getLine("Enter string: ", userInputBuffer, length);
    if (rc == NO_INPUT) {
        printf ("\nNo input\n");
        return 1;
    }
    if (rc == TOO_LONG) {
        printf ("Input too long [%s]\n", userInputBuffer);
        return 1;
    }
    return 0;
}

int getLine(char *prompt, char *buff, size_t maxInputLength)
{
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
