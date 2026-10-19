#ifndef INC_SnapshotMaker_IMAGE_H
#define INC_SnapshotMaker_IMAGE_H

#include "libavutil/timestamp.h"

#define WRITER_LIBPNG "libpng"
#define WRITER_STB "stb"
#define WRITER_LIBJPEG "libjpeg"

#define FILE_EXT_PNG "png"
#define FILE_EXT_JPEG "jpeg"

#define JPEG_QUALITY 100

#define META_TITLE_PREFIX "Frame #"

#define ERR_MAKE_FILE_PATH (-1)
#define ERR_METADATA (-2)
#define ERR_ENCODE (-3)
#define ERR_UNKNOWN_WRITER (-4)

/*
 * Write an image to disk in PNG format using the 'libpng' library.
 *
 * @param fname     output file path
 * @param w         width of image
 * @param h         height of image
 * @param buf       pixel data from FFmpeg, RGBA format, 8 bits per channel
 * @param title     image's title (meta-data field)
 *
 * @doc             http://www.libpng.org/pub/png/libpng-manual.txt
 * @doc             https://www.w3.org/TR/PNG-Rationale.html
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t write_with_libpng(const char *fname, int w, int h, const uint8_t *buf, const char *title);

/*
 * Write an image to disk in JPEG format using the 'libjpeg' library.
 *
 * @param fname     output file path
 * @param w         width of image
 * @param h         height of image
 * @param buf       pixel data from FFmpeg, RGB format, 8 bits per channel
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t write_with_libjpeg(const char *fname, int w, int h, uint8_t *buf);

/*
 * Write an image to disk in PNG format using the 'stb' library.
 * Uses no compression.
 *
 * @param fname     output file path
 * @param w         width of image
 * @param h         height of image
 * @param buf       pixel data from FFmpeg, RGBA format, 8 bits per channel
 *
 * @return          zero of failure
 */
errno_t write_with_stb(const char *fname, int w, int h, uint8_t *buf);

/*
 * Write the image to disk.
 *
 * @param outfdp    output folder path
 * @param fn        current frame number
 * @param w         width of the image
 * @param h         height of the image
 * @param buf       RGB(A) image data taken from FFmpeg
 * @param writer    file writer type
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t write_image(const char *outfdp, int fn, int w, int h, uint8_t *buf, const char *writer);

/*
 * Composes a file path by concatenating the folder path and frame number.
 * The returned pointer must be freed after usage.
 *
 * @param fp        file path
 * @param outfdp    output folder path
 * @param fn        frame number
 * @param ext       file extension
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t make_file_path(char **fp, const char *outfdp, int fn, const char *ext);

/*
 * Composes a title meta-data field.
 * The returned pointer must be freed after usage.
 *
 * @param title     title
 * @param fn        frame number
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t make_metadata_title(char **title, int fn);

#endif //INC_SnapshotMaker_IMAGE_H
