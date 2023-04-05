#include <stdlib.h>
#include "cli.h"
#include "main.h"
#include "snapshot.h"
#include "libavutil/log.h"

/*
 * Make snapshots of the video.
 *
 * @param argc      number of command line arguments
 * @param argv      textual values of command line arguments
 *
 * @return          positive error code in case of failure, otherwise 0.
 */
errno_t main(int argc, char **argv) {
    if (argc != (APP_ARGS_COUNT + 1)) {
        av_log(NULL, AV_LOG_ERROR, "Usage: <executable> <input_file> <pixel_format_id> <output_folder> <writer>\n");
        return APP_EXIT_CODE_ARGS_COUNT_IS_BAD;
    }

    char *exe = argv[0];
    char *inf = argv[1];
    char *pix_fmt_text = argv[2];
    char *outfd = argv[3];
    char *writer = argv[4];

    int pix_fmt_id = -1;
    errno_t err = parse_pixel_format_arg(&pix_fmt_id, pix_fmt_text);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not parse pixel format: '%s', error code: %d.\n", pix_fmt_text, err);
        return APP_EXIT_CODE_ARG_PIXEL_FORMAT_IS_BAD;
    }

    int frame_number = 0;
    err = make_video_snapshots(inf, outfd, writer, &frame_number, pix_fmt_id);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Snapshot maker returned an error, error code: %d.\n", err);
        return APP_EXIT_CODE_SNAPSHOT_MAKER_ERROR;
    }

    av_log(NULL, AV_LOG_INFO, "Snapshots have been made, number of frames: %d.\n", frame_number);

    return APP_EXIT_CODE_ALL_CLEAR;
}
