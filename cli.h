#ifndef MAKEVIDEOSNAPSHOT_CLI_H
#define MAKEVIDEOSNAPSHOT_CLI_H

/*
 * Parses a pixel format ID.
 *
 * @param pix_fmt_id    pixel format ID
 * @param pix_fmt_text  text to parse
 *
 * @return              negative error code in case of failure, otherwise >= 0.
 */
errno_t parse_pixel_format_arg(int *pix_fmt_id, char *pix_fmt_text);

#endif //MAKEVIDEOSNAPSHOT_CLI_H
