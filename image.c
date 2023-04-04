#include <stdio.h>

#include "libavutil/adler32.h"
#include "libpng/png.h"
#include "libjpeg/jpeglib.h"
#include "zlib/zlib.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

#endif //STB_IMAGE_WRITE_IMPLEMENTATION


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
int write_with_libpng(char *fname, int w, int h, const uint8_t *buf, char *title) {
    int exit_code = 0;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    // Open the file for writing in binary mode.
    FILE *fp = NULL;
    errno_t err = fopen_s(&fp, fname, "wb"); // [!] -> fclose.
    if (err != 0) {
        fprintf(stderr, "Can not open file for writing: %s.\n", fname);
        exit_code = -1; //TODO
        goto finalize;
    }

    // Initialize a write structure.
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); // [!] -> png_destroy_write_struct.
    if (png_ptr == NULL) {
        fprintf(stderr, "Can not allocate a write struct.\n");
        exit_code = -2; //TODO
        goto finalize;
    }

    // Initialize an info structure.
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Can not allocate an info struct.\n");
        exit_code = -3; //TODO
        goto finalize;
    }

    // Exception handling.
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "libpng error\n");
        exit_code = -4; //TODO
        goto finalize;
    }

    png_init_io(png_ptr, fp);

    // Write a header.
    /*
     * [ A quote from the manual ]
     *
     * If you wish, you can reset the compression_type, interlace_type, or
     * filter_method later by calling png_set_IHDR() again; if you do this, the
     * width, height, bit_depth, and color_type must be the same in each call.
     */
    png_set_IHDR(png_ptr, info_ptr, w, h, 8,
                 PNG_COLOR_TYPE_RGB, // TODO: Change to PNG_COLOR_TYPE_RGBA if the source has alpha channel.
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // Disable PNG signature for MNG.
    //png_set_sig_bytes(png_ptr, 8);

    // Filtering. It will be adaptive, so we enable all possible filters.
    /*
     * [ A quote from the manual ]
     *
     * If an application wants to start and stop using particular filters
     * during compression, it should start out with all of the filters (to
     * ensure that the previous row of pixels will be stored in case it's
     * needed later), and then add and remove them after the start of
     * compression.
     */
    png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);

    // Compression.
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);

    // Meta-data: Title.
    if (title != NULL) {
        png_text title_text = {PNG_TEXT_COMPRESSION_NONE, "Title", title,
                               strlen(title), 0, NULL, NULL};
        png_set_text(png_ptr, info_ptr, &title_text, 1);
    }

    png_write_info(png_ptr, info_ptr);

    //png_set_filler(png_ptr, 0, PNG_FILLER_AFTER; // RGBX.
    //png_set_bgr(png_ptr); //BGR.

    // Allocate memory for one row of pixels (3 bytes per pixel – RGB).
    // TODO: Change to 4 if the source has alpha channel.
    row = (png_bytep) malloc(3 * w * sizeof(png_byte)); // [!] -> free.

    // Write all the pixels.
    png_bytep row_pointer = row;
    int x, y, i, j;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            // TODO: Add alpha channel byte if the source has it.
            i = x * 3;
            j = (y * w * 3) + i;
            row[i] = buf[j]; // Red.
            i++;
            j++;
            row[i] = buf[j]; // Green.
            i++;
            j++;
            row[i] = buf[j]; // Blue.
        }
        png_write_row(png_ptr, row_pointer); // For some reason CLion's debugger crashes here when y=88. [WTF ?!]
    }

    // Finish writing the PNG file.
    png_write_end(png_ptr, info_ptr);

    finalize:
    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, &info_ptr);
    if (row != NULL) free(row);
    if (fp != NULL) fclose(fp);

    return exit_code;
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
int write_with_stb(char *fname, int w, int h, uint8_t *buf) {
    return stbi_write_png(fname, w, h, 3, buf, w * 3);
}

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
int write_image(const char *outfdp, int fn, int w, int h, uint8_t *buf, char *writer) {
    char fname[1024];

    if (strcmp(writer, "png-stb") == 0) {
        // Write the image to a PNG file using the 'stb' library.
        // Note: This library returns 0 on failure.
        sprintf(fname, "%s\\%d.png", outfdp, fn);
        int res = write_with_stb(fname, w, h, buf);
        if (res == 0) {
            return -1;//TODO
        }
        return 0;
    }

    if (strcmp(writer, "libpng") == 0) {
        // Write the image to a PNG file using the 'libpng'.
        // Note: This library returns 0 on success.
        sprintf(fname, "%s\\%d.png", outfdp, fn);
        int res = write_with_libpng(fname, w, h, buf, "image");
        if (res != 0) {
            return -1;//TODO
        }
        return 0;
    }

    if (strcmp(writer, "libjpeg") == 0) {
        // Write the image to a JPEG file using the 'libjpeg'.
        // Note: This library returns 0 on success.
        sprintf(fname, "%s\\%d.jpg", outfdp, fn);
        int res = writeJpegImage(fname, w, h, buf);
        if (res != 0) {
            return -1;//TODO
        }
        return 0;
    }

    return -2;//TODO
}
