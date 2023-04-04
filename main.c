#include "snapshot.h"

#include "libavutil/log.h"


int main(int argc, char **argv) {
    if (argc < 4) {
        av_log(NULL, AV_LOG_ERROR, "Usage: <executable> <input_file> <output_folder> <writer>\n");
        return 1;
    }

    char *inf = argv[1];
    char *outfd = argv[2];
    char *writer = argv[3];

    int fn = 0;
    int err = make_video_snapshots(inf, outfd, writer, &fn);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Something went wrong, error code: %d.\n", err);
    }

    av_log(NULL, AV_LOG_INFO, "Snapshots have been made, number of frames: %d.\n", fn);

    return 0;
}
