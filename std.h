#ifndef MAKEVIDEOSNAPSHOT_STD_H
#define MAKEVIDEOSNAPSHOT_STD_H

#define NUL '\0' /* NUL byte for NUL-terminated C strings */

#define SUCCESS 0

#define INT_PARSE_ERROR_NOT_A_NUMBER (-1)
#define INT_PARSE_ERROR_EXTRA_CHARS_AT_END (-2)
#define INT_PARSE_ERROR_LONG_OVERFLOW (-3)
#define INT_PARSE_ERROR_INT_OVERFLOW (-4)

#define MALLOC_ERROR (-1)

/*
 * Parses a text into 'int' type.
 *
 * @param n         parsed int
 * @param s         text to be parsed
 * @param base      base of integer number
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_int(int *n, char *s, int base);

#endif //MAKEVIDEOSNAPSHOT_STD_H
