#include "path.h"

#include "libavutil/adler32.h"
#include "libpng/png.h"
#include "jpeglib.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

#endif //STB_IMAGE_WRITE_IMPLEMENTATION


// Writes an Image to the Disk using the PNG Format.
// Returns Zero on Success.
// This Library is bugged and does not work.
int writePngImageWithLibpng(char *filename, int width, int height, uint8_t *buffer, char *title) {
    int code = 0;
    FILE *fp = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    // Open file for writing (binary mode)
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s for writing\n", filename);
        code = 1;
        goto finalize;
    }

    // Initialize write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fprintf(stderr, "Could not allocate write struct\n");
        code = 1;
        goto finalize;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Could not allocate info struct\n");
        code = 1;
        goto finalize;
    }

    // Setup Exception handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error during png creation\n");
        code = 1;
        goto finalize;
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Set title
    if (title != NULL) {
        png_text title_text;
        title_text.compression = PNG_TEXT_COMPRESSION_NONE;
        title_text.key = "Title";
        title_text.text = title;
        png_set_text(png_ptr, info_ptr, &title_text, 1);
    }

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    row = (png_bytep) malloc(3 * width * sizeof(png_byte));

    // Write image data
    int x, y, i, j;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            i = x * 3;
            j = (y * width) + i;
            row[i] = buffer[j]; // Red.
            i++;
            j++;
            row[i] = buffer[j]; // Green.
            i++;
            j++;
            row[i] = buffer[j]; // Blue.
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, NULL);

    finalize:
    if (fp != NULL) fclose(fp);
    if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
    if (row != NULL) free(row);

    return code;
}

// Writes an Image to the Disk using the JPEG Format.
// Returns Zero on Success.
int writeJpegImage(char *filename, int width, int height, uint8_t *buffer) {
    int quality = 100;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;        /* target file */
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
    int row_stride;        /* physical row width in image buffer */

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;    /* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = 3;        /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;    /* colorspace of input image */
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = width * 3;    /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}

// Writes an Image to the Disk using the PNG Format.
// Returns Zero on Failure.
// Does not support Compression.
int writePngImageWithStb(char *filename, int width, int height, uint8_t *buffer) {
    int result = 0;
    result = stbi_write_png(filename, width, height, 3, buffer, width * 3);
    return result;
}

// Writes an Image to the Disk.
// Returns the Target File Path on Success.
const char *writeImage(
        const char *output_folder_path,
        int frame_number,
        int width,
        int height,
        uint8_t *buffer) {

    char image_filename[1024];
    int result = 0;
    const char *target_filename;

    // Write the Frame to a PNG File using the 'stb'.
    // Note: this library returns 0 on failure.
    sprintf(image_filename, "%s/%d.png", output_folder_path, frame_number);
    target_filename = getFilePath(output_folder_path, frame_number, ".png");
    result = writePngImageWithStb(
            image_filename,
            width,
            height,
            buffer);
    if (result == 0) {
        return NULL;
    }
    return target_filename;

    // Unused Code is below.

    // Write the Frame to a PNG File using the 'libpng'.
    sprintf(image_filename, "%s/%d.png", output_folder_path, frame_number);
    target_filename = getFilePath(output_folder_path, frame_number, ".png");
    result = writePngImageWithLibpng(
            image_filename,
            width,
            height,
            buffer,
            "image");
    if (result != 0) {
        return NULL;
    }
    return target_filename;

    // Write the Frame to a JPEG File.
    sprintf(image_filename, "%s/%d.jpg", output_folder_path, frame_number);
    target_filename = getFilePath(output_folder_path, frame_number, ".jpg");
    result = writeJpegImage(
            image_filename,
            width,
            height,
            buffer);
    if (result != 0) {
        return NULL;
    }
    return target_filename;
}
