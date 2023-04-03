#include "image.h"

#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"


// Decodes the best Video Steam from the File,
// Takes the first Video Frame Image,
// Saves it to the Disk,
// Returns the Path of the saved Image File.
const char *make_video_snapshot(
        const char *input_filename,
        const char *output_folder_path) {

    const AVCodec *codec = NULL;
    AVCodecContext *ctx = NULL;
    AVCodecParameters *origin_par = NULL;
    AVFrame *fr = NULL;
    uint8_t *byte_buffer = NULL;
    uint8_t *rgb_byte_buffer = NULL;
    AVPacket *pkt;
    AVFormatContext *fmt_ctx = NULL;
    int number_of_written_bytes;
    int video_stream;
    int i = 0;
    int result;
    int frame_number = 0;
    struct SwsContext *conversion_ctx = NULL;
    int byte_buffer_size;
    int rgb_byte_buffer_size;
    int h = 0;
    enum AVPixelFormat dstPixelFormat = AV_PIX_FMT_RGB24;
    const char *output_file_path;

    result = avformat_open_input(&fmt_ctx, input_filename, NULL, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file\n");
        return NULL;
    }

    result = avformat_find_stream_info(fmt_ctx, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't get stream info\n");
        return NULL;
    }

    video_stream = av_find_best_stream(
            fmt_ctx, AVMEDIA_TYPE_VIDEO, -1,
            -1, NULL, 0);
    if (video_stream < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't find video stream in input file\n");
        return NULL;
    }

    origin_par = fmt_ctx->streams[video_stream]->codecpar;

    codec = avcodec_find_decoder(origin_par->codec_id);
    if (!codec) {
        av_log(NULL, AV_LOG_ERROR, "Can't find decoder\n");
        return NULL;
    }

    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate decoder context\n");
        return NULL;
    }

    result = avcodec_parameters_to_context(ctx, origin_par);
    if (result) {
        av_log(NULL, AV_LOG_ERROR, "Can't copy decoder context\n");
        return NULL;
    }

    result = avcodec_open2(ctx, codec, NULL);
    if (result < 0) {
        av_log(ctx, AV_LOG_ERROR, "Can't open decoder\n");
        return NULL;
    }

    fr = av_frame_alloc();
    if (!fr) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate frame\n");
        return NULL;
    }

    pkt = av_packet_alloc();
    if (!pkt) {
        av_log(NULL, AV_LOG_ERROR, "Cannot allocate packet\n");
        av_frame_unref(fr);
        return NULL;
    }

    byte_buffer_size = av_image_get_buffer_size(
            ctx->pix_fmt, ctx->width, ctx->height, 32);
    byte_buffer = av_malloc(byte_buffer_size);
    if (!byte_buffer) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate buffer\n");
        av_frame_unref(fr);
        return NULL;
    }

    rgb_byte_buffer_size = av_image_get_buffer_size(
            dstPixelFormat, ctx->width, ctx->height, 32);
    rgb_byte_buffer = av_malloc(rgb_byte_buffer_size);
    if (!rgb_byte_buffer) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate RGB buffer\n");
        av_frame_unref(fr);
        return NULL;
    }

    uint8_t *const rgb_dst[1] = {rgb_byte_buffer};
    const int rgb_dstStride[1] = {ctx->width * 3};

    conversion_ctx = sws_getContext(
            ctx->width,
            ctx->height,
            ctx->pix_fmt,
            ctx->width,
            ctx->height,
            dstPixelFormat,
            SWS_BICUBIC,
            NULL,
            NULL,
            NULL);
    if (conversion_ctx == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can't create conversion context\n");
        av_frame_unref(fr);
        return NULL;
    }

//    printf("#tb %d: %d/%d\n", video_stream, fmt_ctx->streams[video_stream]->time_base.num,
//           fmt_ctx->streams[video_stream]->time_base.den);
    i = 0;

    result = 0;
    while (result >= 0) {
        result = av_read_frame(fmt_ctx, pkt);
        if (result >= 0 && pkt->stream_index != video_stream) {
            av_packet_unref(pkt);
            continue;
        }

        if (result < 0)
            result = avcodec_send_packet(ctx, NULL);
        else {
            if (pkt->pts == AV_NOPTS_VALUE)
                pkt->pts = pkt->dts = i;
            result = avcodec_send_packet(ctx, pkt);
        }
        av_packet_unref(pkt);

        if (result < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error submitting a packet for decoding\n");
            return NULL;
        }

        while (result >= 0) {
            result = avcodec_receive_frame(ctx, fr);
            if (result == AVERROR_EOF)
                goto finish;
            else if (result == AVERROR(EAGAIN)) {
                result = 0;
                break;
            } else if (result < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error decoding frame\n");
                return NULL;
            }

            number_of_written_bytes = av_image_copy_to_buffer(
                    byte_buffer,
                    byte_buffer_size,
                    (const uint8_t *const *) fr->data,
                    (const int *) fr->linesize,
                    ctx->pix_fmt,
                    ctx->width,
                    ctx->height,
                    1);
            if (number_of_written_bytes < 0) {
                av_log(NULL, AV_LOG_ERROR, "Can't copy image to buffer\n");
                av_frame_unref(fr);
                return NULL;
            }
//            printf("%d, %s, %s, %8"PRId64", %8d, 0x%08lx, %dx%d\n", video_stream,
//                   av_ts2str(fr->pts), av_ts2str(fr->pkt_dts), fr->pkt_duration,
//                   number_of_written_bytes,
//                   av_adler32_update(0, (const uint8_t *) byte_buffer, number_of_written_bytes),
//                   fr->width, fr->height);


            // Convert the Colour Space (YUV -> RGB).
            h = sws_scale(
                    conversion_ctx,
                    (const uint8_t *const *) fr->data,
                    (const int *) fr->linesize,
                    0,
                    ctx->height,
                    rgb_dst,
                    rgb_dstStride);
            if (h != ctx->height) {
                av_log(NULL, AV_LOG_ERROR, "sws_scale\n");
                av_frame_unref(fr);
                return NULL;
            }

            frame_number++;

            output_file_path = writeImage(
                    output_folder_path,
                    frame_number,
                    ctx->width,
                    ctx->height,
                    rgb_byte_buffer);
            if (output_file_path == NULL) {
                av_log(NULL, AV_LOG_ERROR, "Error writing image file\n");
                av_frame_unref(fr);
                return NULL;
            }

            av_frame_unref(fr);
        }
        i++;

        // Exit after the first Frame.
        if (i >= 1) {
            goto finish;
        }
    }

    finish:
    av_packet_free(&pkt);
    av_frame_free(&fr);
    avformat_close_input(&fmt_ctx);
    avcodec_free_context(&ctx);
    av_freep(&byte_buffer);
    av_freep(&rgb_byte_buffer);
    sws_freeContext(conversion_ctx);
    return output_file_path;
}
