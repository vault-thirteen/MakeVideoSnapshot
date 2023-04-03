#include "snapshot.h"

#include "libavutil/log.h"


int main(int argc, char **argv) {
    if (argc < 3) {
        av_log(NULL, AV_LOG_ERROR, "Usage: <executable> <input_file> <output_folder>\n");
        return 1;
    }

    int fn = 0;
    int err = make_video_snapshots(argv[1], argv[2], &fn);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Something went wrong, error code: %d.\n", err);
    }

    av_log(NULL, AV_LOG_INFO, "Snapshots have been made, number of frames: %d.\n", fn);

    return 0;
}
