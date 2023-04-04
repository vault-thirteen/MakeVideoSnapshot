#ifndef INC_SnapshotMaker_SNAPSHOT_H
#define INC_SnapshotMaker_SNAPSHOT_H

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"


#define ERR_CODEC_PARAMETER (-256)
#define ERR_CODEC (-257)
#define ERR_CODEC_CONTEXT (-258)
#define ERR_PACKET (-259)
#define ERR_BUFFER_SIZE (-260)
#define ERR_BUFFER (-261)
#define ERR_RGB_BUFFER_SIZE (-262)
#define ERR_RGB_BUFFER (-263)
#define ERR_SWS_CONTEXT (-264)
#define ERR_SWS_SCALE (-265)
#define ERR_WRITE_IMAGE (-266)

/*
 * Decode the best video stream from the file, make snapshot images,
 * save them to disk.
 *
 * @param infp      input file path
 * @param outfdp    output folder path
 * @param writer    file writer type
 * @param fn        number of processed frames
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
int make_video_snapshots(const char *infp,
                         const char *outfp,
                         char *writer,
                         int *fn);

/*
 * Decode frames.
 *
 * @param outfp     output file path
 * @param fn        number of processed frames
 * @param writer    file writer type
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
int decode_and_process_frames(const char *outfdp,
                              int *fn,
                              uint8_t *buf,
                              int buf_size,
                              AVFormatContext *fmt_ctx,
                              int stream,
                              AVCodecContext *ctx,
                              struct SwsContext *conv_ctx,
                              uint8_t *const rgb_dst[1],
                              const int rgb_dstStride[1],
                              uint8_t *rgb_buf,
                              AVPacket *pkt,
                              AVFrame *fr,
                              char *writer);

/*
 * Receive available decoded frames from the decoder, save them to disk.
 *
 * @param outfdp    output folder path
 * @param fn        current frame number
 * @param writer    file writer type
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
int process_decoded_frames(AVCodecContext *ctx,
                           AVFrame *fr,
                           uint8_t *buf,
                           int buf_size,
                           struct SwsContext *conv_ctx,
                           uint8_t *const rgb_dst[1],
                           const int rgb_dstStride[1],
                           uint8_t *rgb_buf,
                           const char *outfdp,
                           int *fn,
                           char *writer);

#endif //INC_SnapshotMaker_SNAPSHOT_H
