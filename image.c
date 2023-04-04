#include <stdio.h>

#include "libavutil/adler32.h"
#include "libpng/png.h"
#include "libjpeg/jpeglib.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

#endif //STB_IMAGE_WRITE_IMPLEMENTATION


// writePngImageWithLibpng writes an image to disk in PNG format using the
// 'libpng' library. Returns zero on success.
int writePngImageWithLibpng(char *fname, int w, int h, const uint8_t *buf, char *title) {
    //TODO
    int code = 0;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    // Open file for writing (binary mode)
    FILE *fp = NULL;
    errno_t err = fopen_s(&fp, fname, "wb");
    if (err != 0) {
        fprintf(stderr, "Could not open file %s for writing\n", fname);
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

    // Setup exception handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error during png creation\n");
        code = 1;
        goto finalize;
    }

    png_init_io(png_ptr, fp);

    // Write header (8-bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, w, h,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Set title
    if (title != NULL) {
        png_text title_text = {
                PNG_TEXT_COMPRESSION_NONE,
                "Title",
                title,
                strlen(title),
                0,
                NULL,
                NULL,
        };
        png_set_text(png_ptr, info_ptr, &title_text, 1);
    }

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    row = (png_bytep) malloc(3 * w * sizeof(png_byte));

    // Write image data
    int x, y, i, j;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            i = x * 3;
            j = (y * w) + i;
            row[i] = buf[j]; // Red.
            i++;
            j++;
            row[i] = buf[j]; // Green.
            i++;
            j++;
            row[i] = buf[j]; // Blue.
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

// writeJpegImage writes an image to disk in JPEG format using the 'libjpeg'
// library. Returns zero on success.
int writeJpegImage(char *fname, int w, int h, uint8_t *buf) {
    //TODO
    int quality = 100;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;              /* target file */
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
    int row_stride;             /* physical row w in image buf */

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    errno_t err = fopen_s(&outfile, fname, "wb");
    if (err != 0) {
        fprintf(stderr, "can't open %s\n", fname);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = w;          /* image w and h, in pixels */
    cinfo.image_height = h;
    cinfo.input_components = 3;        /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;    /* colorspace of input image */
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = w * 3;             /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &buf[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return 0;
}

// writePngImageWithStb writes an image to disk in PNG format using the 'stb'
// library. Returns zero on failure. Does not support compression.
int writePngImageWithStb(char *fname, int w, int h, uint8_t *buf) {
    return stbi_write_png(fname, w, h, 3, buf, w * 3);
}

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
int write_image(const char *outfdp,
                int fn,
                int w,
                int h,
                uint8_t *buf,
                char *writer) {
    char fname[1024];

    if (strcmp(writer, "png-stb") == 0) {
        // Write the image to a PNG file using the 'stb' library.
        // Note: This library returns 0 on failure.
        sprintf(fname, "%s\\%d.png", outfdp, fn);
        int res = writePngImageWithStb(fname, w, h, buf);
        if (res == 0) {
            return -1;
        }
        return 0;
    }

    if (strcmp(writer, "libpng") == 0) {
        // Write the image to a PNG file using the 'libpng'.
        // Note: This library returns 0 on success.
        sprintf(fname, "%s\\%d.png", outfdp, fn);
        int res = writePngImageWithLibpng(fname, w, h, buf, "image");
        if (res != 0) {
            return -1;
        }
        return 0;
    }

    if (strcmp(writer, "libjpeg") == 0) {
        // Write the image to a JPEG file using the 'libjpeg'.
        // Note: This library returns 0 on success.
        sprintf(fname, "%s\\%d.jpg", outfdp, fn);
        int res = writeJpegImage(fname, w, h, buf);
        if (res != 0) {
            return -1;
        }
        return 0;
    }

    return -2;
}
