#include <stdlib.h>
#include "cli.h"
#include "std.h"

/*
 * Parses a pixel format ID.
 *
 * @param pix_fmt_id    pixel format ID
 * @param pix_fmt_text  text to parse
 *
 * @return              negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_pixel_format_arg(int *pix_fmt_id, char *pix_fmt_text) {
    return parse_int(pix_fmt_id, pix_fmt_text, 10);
}
