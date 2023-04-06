#ifndef INC_SnapshotMaker_SNAPSHOT_H
#define INC_SnapshotMaker_SNAPSHOT_H

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"


/*
 * Decode the best video stream from the file, make snapshot images,
 * save them to disk.
 *
 * If pixel format ID is negative, e.g. -1, it is ignored. If pixel format ID
 * is >= 0, it overrides the automatically selected pixel format.
 *
 * If stream index is negative, e.g. -1, it is ignored. If stream index is >= 0,
 * it overrides the automatically selected stream index.
 *
 * If 'N-th' parameter is 0, all frames will be saved. If it is >0, every N-th
 * frame will be saved and the first frame too.
 *
 * @param infp          input file path
 * @param outfdp        output folder path
 * @param writer        file writer type
 * @param si            stream index
 * @param fn            number of processed frames
 * @param pix_fmt_id    ID of a pixel format
 * @param nth           N-th frame to save
 *
 * @return              negative error code in case of failure, otherwise >= 0.
 */
errno_t make_video_snapshots(const char *infp,
                             const char *outfp,
                             char *writer,
                             int *fn,
                             int si,
                             int pix_fmt_id,
                             int nth);

/*
 * Read frames, decode them, save images to disk.
 *
 * Documentation:
 * https://ffmpeg.org/doxygen/3.3/group__lavc__encdec.html
 *
 * @param outfdp    output folder path
 * @param fn        number of processed frames
 * @param writer    file writer type
 * @param nth       N-th frame to save
 *
 * @return          negative error code in case of failure.
 */
errno_t decode_and_process_frames(const char *outfdp,
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
                                  char *writer,
                                  int nth);

/*
 * Receive available decoded frames from the decoder, save them to disk.
 *
 * @param outfdp    output folder path
 * @param fn        current frame number
 * @param writer    file writer type
 * @param nth       N-th frame to save
 *
 * @return          negative error code in case of failure, otherwise >= 0.
 */
errno_t process_decoded_frames(AVCodecContext *ctx,
                               AVFrame *fr,
                               uint8_t *buf,
                               int buf_size,
                               struct SwsContext *conv_ctx,
                               uint8_t *const rgb_dst[1],
                               const int rgb_dstStride[1],
                               uint8_t *rgb_buf,
                               const char *outfdp,
                               int *fn,
                               char *writer,
                               int nth);

#endif //INC_SnapshotMaker_SNAPSHOT_H
