#ifndef INC_SnapshotMaker_IMAGE_H
#define INC_SnapshotMaker_IMAGE_H

#include "libavutil/timestamp.h"


// writePngImageWithLibpng writes an image to disk in PNG format using the
// 'libpng' library. Returns zero on success.
int writePngImageWithLibpng(char *fname, int w, int h, uint8_t *buf, char *title);

// writeJpegImage writes an image to disk in JPEG format using the 'libjpeg'
// library. Returns zero on success.
int writeJpegImage(char *fname, int w, int h, uint8_t *buf);

// writePngImageWithStb writes an image to disk in PNG format using the 'stb'
// library. Returns zero on failure. Does not support compression.
int writePngImageWithStb(char *fname, int w, int h, uint8_t *buf);

/*
 * Write the image to disk.
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
int write_image(const char *outfdp, int fn, int w, int h, uint8_t *buf, char *writer);


#endif //INC_SnapshotMaker_IMAGE_H
