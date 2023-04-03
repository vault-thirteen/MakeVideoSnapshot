#ifndef INC_SnapshotMaker_IMAGE_H
#define INC_SnapshotMaker_IMAGE_H

#include "libavutil/timestamp.h"

// getFilePath composes a target file path.
const char *getFilePath(const char *folderName, int frameNumber, const char *fileExtFull);

// write_image writes an image to the disk.
// Returns the target file path on success.
const char *write_image(const char *output_folder_path, int fn, int width, int height, uint8_t *buffer);

// writePngImageWithLibpng writes an image to the disk using the PNG format.
// Returns zero on success. This library is bugged and does not work.
int writeJpegImage(char *filename, int width, int height, uint8_t *buffer);

// writeJpegImage writes an image to the disk using the JPEG format.
// Returns zero on success.
int writePngImageWithLibpng(char *filename, int width, int height, uint8_t *buffer, char *title);

// writePngImageWithStb writes an image to the disk using the PNG format.
// Returns zero on failure. Does not support compression.
int writePngImageWithStb(char *filename, int width, int height, uint8_t *buffer);

#endif //INC_SnapshotMaker_IMAGE_H
