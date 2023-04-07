#include "main.h"
#include "snapshot.h"
#include "std.h"
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
        av_log(NULL, AV_LOG_ERROR, "Usage: <executable> <input_file> <stream_index> <pixel_format_id> <output_folder> <writer> <nth>\n");
        return APP_EXIT_CODE_ARGS_COUNT_IS_BAD;
    }

    //char *exe = argv[0];
    char *inf = argv[1];                // Input file.
    char *stream_idx_text = argv[2];    // Video stream index.
    char *pix_fmt_text = argv[3];       // Input pixel format.
    char *outfd = argv[4];              // Output folder path.
    char *writer = argv[5];             // Image writer.
    char *nth_text = argv[6];           // N-th frame to save. If 0, all frames will be saved.

    int pix_fmt_id = -1;
    errno_t err = parse_int(&pix_fmt_id, pix_fmt_text, 10);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not parse pixel format: '%s', error code: %d.\n", pix_fmt_text, err);
        return APP_EXIT_CODE_ARG_PIXEL_FORMAT_IS_BAD;
    }

    int stream_index = -1;
    err = parse_int(&stream_index, stream_idx_text, 10);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not parse stream index: '%s', error code: %d.\n", stream_idx_text, err);
        return APP_EXIT_CODE_ARG_STREAM_INDEX_IS_BAD;
    }

    int nth = 0;
    err = parse_int(&nth, nth_text, 10);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not parse 'N-th' setting: '%s', error code: %d.\n", nth_text, err);
        return APP_EXIT_CODE_ARG_NTH_IS_BAD;
    }

    int frame_number = 1; // First frame number is 1, not 0. We are humans, not machines.
    err = make_video_snapshots(inf, outfd, writer, &frame_number, stream_index, pix_fmt_id, nth);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Snapshot maker returned an error, error code: %d.\n", err);
        return APP_EXIT_CODE_SNAPSHOT_MAKER_ERROR;
    }

    av_log(NULL, AV_LOG_INFO, "Snapshots have been made, number of frames: %d.\n", frame_number);

    return APP_EXIT_CODE_ALL_CLEAR;
}
