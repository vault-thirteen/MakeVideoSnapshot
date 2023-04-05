#include <stdio.h>
#include "libpng/png.h"
#include "libjpeg/jpeglib.h"
#include "zlib/zlib.h"
#include "image.h"
#include "mvs.h"
#include "std.h"

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
errno_t write_with_libpng(const char *fname, int w, int h, const uint8_t *buf, const char *title) {
    errno_t exit_code = SUCCESS;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    // Open the file for writing in binary mode.
    FILE *fp = NULL;
    errno_t err = fopen_s(&fp, fname, "wb"); // [!] -> fclose.
    if (err != 0) {
        fprintf(stderr, "Can not open file for writing: %s.\n", fname);
        exit_code = ERR_FILE_OPEN;
        goto finalize;
    }

    // Initialize a write structure.
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); // [!] -> png_destroy_write_struct.
    if (png_ptr == NULL) {
        fprintf(stderr, "Can not allocate a write struct.\n");
        exit_code = ERR_LIBPNG_CWS;
        goto finalize;
    }

    // Initialize an info structure.
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Can not allocate an info struct.\n");
        exit_code = ERR_LIBPNG_CIS;
        goto finalize;
    }

    // Exception handling.
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "libpng error\n");
        exit_code = ERR_LIBPNG_SETJMP;
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
                 PNG_COLOR_TYPE_RGBA,
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

    // Allocate memory for one row of pixels (4 bytes per pixel – RGBA).
    row = (png_bytep) malloc(PIXEL_CHANNELS_COUNT * w * sizeof(png_byte)); // [!] -> free.

    // Write all the pixels.
    png_bytep row_pointer = row;
    int x, y, i, j;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            i = x * PIXEL_CHANNELS_COUNT;
            j = (y * w * PIXEL_CHANNELS_COUNT) + i;
            row[i] = buf[j]; // Red.
            i++;
            j++;
            row[i] = buf[j]; // Green.
            i++;
            j++;
            row[i] = buf[j]; // Blue.
            i++;
            j++;
            row[i] = buf[j]; // Alpha channel.
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

//TODO
// writeJpegImage writes an image to disk in JPEG format using the 'libjpeg'
// library. Returns zero on success.
errno_t writeJpegImage(const char *fname, int w, int h, uint8_t *buf) {
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
errno_t write_with_stb(const char *fname, int w, int h, uint8_t *buf) {
    return stbi_write_png(fname, w, h, PIXEL_CHANNELS_COUNT, buf, w * PIXEL_CHANNELS_COUNT);
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
errno_t write_image(const char *outfdp, int fn, int w, int h, uint8_t *buf, const char *writer) {
    char *fname = NULL;
    errno_t err;

    if (strcmp(writer, WRITER_LIBPNG) == 0) {
        // Write the image to a PNG file using the 'libpng'.
        err = make_file_path(&fname, outfdp, fn, FILE_EXT_PNG);
        if (err < 0) {
            return ERR_MAKE_FILE_PATH;
        }

        char *title = NULL;
        err = make_metadata_title(&title, fn);
        if (err < 0) {
            free(fname);
            return ERR_METADATA;
        }

        // Note: This library returns 0 on success.
        int res = write_with_libpng(fname, w, h, buf, title);
        if (res != 0) {
            free(title);
            free(fname);
            return ERR_ENCODE;
        }
        free(title);
        free(fname);
        return SUCCESS;
    }

    if (strcmp(writer, WRITER_STB) == 0) {
        // Write the image to a PNG file using the 'stb' library.
        err = make_file_path(&fname, outfdp, fn, FILE_EXT_PNG);
        if (err < 0) {
            return ERR_MAKE_FILE_PATH;
        }

        // Note: This library returns 0 on failure.
        int res = write_with_stb(fname, w, h, buf);
        if (res == 0) {
            free(fname);
            return ERR_ENCODE;
        }
        free(fname);
        return SUCCESS;
    }

    if (strcmp(writer, WRITER_LIBJPEG) == 0) {
        // Write the image to a JPEG file using the 'libjpeg'.
        err = make_file_path(&fname, outfdp, fn, FILE_EXT_JPEG);
        if (err < 0) {
            return ERR_MAKE_FILE_PATH;
        }

        // Note: This library returns 0 on success.
        int res = writeJpegImage(fname, w, h, buf);
        if (res != 0) {
            free(fname);
            return ERR_ENCODE;
        }
        free(fname);
        return SUCCESS;
    }

    return ERR_UNKNOWN_WRITER;
}

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
errno_t make_file_path(char **fp, const char *outfdp, int fn, const char *ext) {
    // INT_MAX is 2147483647, i.e. 10 ASCII symbols.
    // INT_MIN is -2147483648, i.e. 11 ASCII symbols.
    char num[11 + 1]; // Last symbol is NUL.
    snprintf(num, sizeof num, "%d", fn);

    // Template: 'C:\Files' + '\' + '123.png' + NUL.
    size_t result_len = strlen(outfdp) + 1 + strlen(num) + 1 + strlen(ext) + 1;

    char *result = (char *) malloc(result_len);
    if (result == NULL) {
        return MALLOC_ERROR;
    }

    sprintf(result, "%s\\%s.%s", outfdp, num, ext);
    *fp = result;
    return SUCCESS;
}

/*
 * Composes a title meta-data field.
 * The returned pointer must be freed after usage.
 *
 * @param title     title
 * @param fn        frame number
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t make_metadata_title(char **title, int fn) {
    // INT_MAX is 2147483647, i.e. 10 ASCII symbols.
    // INT_MIN is -2147483648, i.e. 11 ASCII symbols.
    char num[11 + 1]; // Last symbol is NUL.
    snprintf(num, sizeof num, "%d", fn);

    // Template: 'Frame #' + '123' + NUL.
    size_t result_len = strlen(META_TITLE_PREFIX) + strlen(num) + 1;

    char *result = (char *) malloc(result_len);
    if (result == NULL) {
        return MALLOC_ERROR;
    }

    sprintf(result, "%s%s", META_TITLE_PREFIX, num);
    *title = result;
    return SUCCESS;
}
