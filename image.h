#ifndef INC_SnapshotMaker_IMAGE_H
#define INC_SnapshotMaker_IMAGE_H

#include "libavutil/timestamp.h"

#define WRITER_LIBPNG "libpng"
#define WRITER_STB "stb"
#define WRITER_LIBJPEG "libjpeg"

#define ERR_ENCODE (-1)
#define ERR_UNKNOWN_WRITER (-2)

/*
 * Write an image to disk in PNG format using the 'libpng' library.
 *
 * @param fname     output file path
 * @param w         width of image
 * @param h         height of image
 * @param buf       pixel data from FFmpeg, RGB format, 8 bits per channel
 * @param title     image's title (meta-data field)
 *
 * @doc             http://www.libpng.org/pub/png/libpng-manual.txt
 * @doc             https://www.w3.org/TR/PNG-Rationale.html
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t write_with_libpng(char *fname, int w, int h, const uint8_t *buf, char *title);

//TODO
// writeJpegImage writes an image to disk in JPEG format using the 'libjpeg'
// library. Returns zero on success.
errno_t writeJpegImage(char *fname, int w, int h, uint8_t *buf);

/*
 * Write an image to disk in PNG format using the 'stb' library.
 * Uses no compression.
 *
 * @param fname     output file path
 * @param w         width of image
 * @param h         height of image
 * @param buf       pixel data from FFmpeg, RGB format, 8 bits per channel
 *
 * @return          zero of failure
 */
errno_t write_with_stb(char *fname, int w, int h, uint8_t *buf);

/*
 * Write the image to disk.
 * File path length is limited to 1024 bytes. //TODO: Dynamic length.
 *
 * @param outfdp    output folder path
 * @param fn        current frame number
 * @param w         width of the image
 * @param h         height of the image
 * @param buf       RGB image data taken from FFmpeg
 * @param writer    file writer type
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t write_image(const char *outfdp, int fn, int w, int h, uint8_t *buf, char *writer);


#endif //INC_SnapshotMaker_IMAGE_H
