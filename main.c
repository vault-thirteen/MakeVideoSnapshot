#include "snapshot.h"

#include "libavcodec/avcodec.h"
#include "libavutil/log.h"


int main(int argc, char **argv) {
    if (argc < 3) {
        av_log(NULL, AV_LOG_ERROR, "Usage: <executable> <input_file> <output_folder>\n");
        return 1;
    }

    const char *savedFilePath;
    savedFilePath = make_video_snapshot(argv[1], argv[2]);
    if (savedFilePath == NULL)
        return 1;
    printf("%s\n", savedFilePath);

    return 0;
}
