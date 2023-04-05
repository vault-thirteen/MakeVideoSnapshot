#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "std.h"

/*
 * Parses a text into 'int' type.
 *
 * @param n         parsed int
 * @param s         text to be parsed
 * @param base      base of integer number
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_int(int *n, char *s, int base) {
    if (s == NULL) {
        return -1;
    }

    char *end;
    long l = strtol(s, &end, base);

    if (end == s) {
        fprintf(stderr, "string is not a number: %s.\n", s);
        return INT_PARSE_ERROR_NOT_A_NUMBER;
    } else if ('\0' != *end) {
        fprintf(stderr, "extra characters at end of input: %s.\n", end);
        return INT_PARSE_ERROR_EXTRA_CHARS_AT_END;
    } else if ((LONG_MIN == l || LONG_MAX == l) && ERANGE == errno) {
        fprintf(stderr, "out of range of long type: %s.\n", s);
        return INT_PARSE_ERROR_LONG_OVERFLOW;
    } else if (l > INT_MAX) {
        fprintf(stderr, "out of range of int type: %ld.\n", l);
        return INT_PARSE_ERROR_INT_OVERFLOW;
    } else if (l < INT_MIN) {
        fprintf(stderr, "out of range of int type: %ld.\n", l);
        return INT_PARSE_ERROR_INT_OVERFLOW;
    } else {
        *n = (int) l;
        return SUCCESS;
    }
    // https://wiki.sei.cmu.edu/confluence/display/c/ERR34-C.+Detect+errors+when+converting+a+string+to+a+number
}
