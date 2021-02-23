#ifndef INC_SnapshotMaker_IMAGE_H
#define INC_SnapshotMaker_IMAGE_H

#include "libavutil/timestamp.h"

#endif //INC_SnapshotMaker_IMAGE_H


// Writes an Image to the Disk.
// Returns the Target File Path on Success.
const char *writeImage(const char *output_folder_path, int frame_number, int width, int height, uint8_t *buffer);

// Writes an Image to the Disk using the JPEG Format.
// Returns Zero on Success.
int writeJpegImage(char *filename, int width, int height, uint8_t *buffer);

// Writes an Image to the Disk using the PNG Format.
// Returns Zero on Success.
// This Library is bugged and does not work.
int writePngImageWithLibpng(char *filename, int width, int height, uint8_t *buffer, char *title);

// Writes an Image to the Disk using the PNG Format.
// Returns Zero on Failure.
// Does not support Compression.
int writePngImageWithStb(char *filename, int width, int height, uint8_t *buffer);
