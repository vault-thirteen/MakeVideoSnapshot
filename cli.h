#ifndef MAKEVIDEOSNAPSHOT_CLI_H
#define MAKEVIDEOSNAPSHOT_CLI_H

/*
 * Parses a pixel format ID.
 *
 * @param pix_fmt_id    pixel format ID
 * @param s             text to parse
 *
 * @return              negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_pixel_format_arg(int *pix_fmt_id, char *s);

/*
 * Parses a stream index argument.
 *
 * @param stream_idx    stream index
 * @param s             text to parse
 *
 * @return              negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_stream_index_arg(int *stream_idx, char *s);

#endif //MAKEVIDEOSNAPSHOT_CLI_H
