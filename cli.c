#include <stdlib.h>
#include "cli.h"
#include "std.h"

/*
 * Parses a pixel format ID argument.
 *
 * @param pix_fmt_id    pixel format ID
 * @param s             text to parse
 *
 * @return              negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_pixel_format_arg(int *pix_fmt_id, char *s) {
    return parse_int(pix_fmt_id, s, 10);
}

/*
 * Parses a stream index argument.
 *
 * @param stream_idx    stream index
 * @param s             text to parse
 *
 * @return              negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_stream_index_arg(int *stream_idx, char *s) {
    return parse_int(stream_idx, s, 10);
}
