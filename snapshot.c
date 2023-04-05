#include "image.h"
#include "snapshot.h"

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "mvs.h"

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
                         const char *outfdp,
                         char *writer,
                         int *fn) {
    AVFormatContext *fmt_ctx = NULL;
    int err = avformat_open_input(&fmt_ctx, infp, NULL, NULL); // [!] The stream must be closed with avformat_close_input().
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not open the file: %s.\n", infp);
        return err;
    }

    err = avformat_find_stream_info(fmt_ctx, NULL);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not get the stream, file: %s.\n", infp);
        avformat_close_input(&fmt_ctx);
        return err;
    }

    int stream = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0); // Video stream index.
    if (stream == AVERROR_STREAM_NOT_FOUND) {
        av_log(NULL, AV_LOG_ERROR, "Can not find video stream, file: %s.\n", infp);
        avformat_close_input(&fmt_ctx);
        return stream;
    } else if (stream == AVERROR_DECODER_NOT_FOUND) {
        av_log(NULL, AV_LOG_ERROR, "Can not find decoder for video stream, file: %s.\n", infp);
        avformat_close_input(&fmt_ctx);
        return stream;
    } else if (stream < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not find video stream, file: %s.\n", infp);
        avformat_close_input(&fmt_ctx);
        return stream;
    }

    AVCodecParameters *codecpar = fmt_ctx->streams[stream]->codecpar; // Codec parameters associated with the stream.
    if (codecpar == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can not find codec parameters, file: %s.\n", infp);
        avformat_close_input(&fmt_ctx);
        return ERR_CODEC_PARAMETER;
    }

    //TODO: avcodec_find_decoder_by_name ...
    const AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
    if (codec == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can not find a decoder, file: %s.\n", infp);
        avformat_close_input(&fmt_ctx);
        return ERR_CODEC;
    }

    AVCodecContext *ctx = avcodec_alloc_context3(codec); // [!] The resulting struct should be freed with avcodec_free_context().
    if (ctx == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can not allocate decoder context, file: %s.\n", infp);
        avformat_close_input(&fmt_ctx);
        return ERR_CODEC_CONTEXT;
    }

    err = avcodec_parameters_to_context(ctx, codecpar);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not copy decoder context, file: %s.\n", infp);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return err;
    }

    err = avcodec_open2(ctx, codec, NULL);
    if (err < 0) {
        av_log(ctx, AV_LOG_ERROR, "Can not open decoder, file: %s.\n", infp);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return err;
    }

    int buf_size = av_image_get_buffer_size(ctx->pix_fmt, ctx->width, ctx->height, 32);
    if (buf_size < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not get buffer size, file: %s.\n", infp);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return ERR_BUFFER_SIZE;
    }

    uint8_t *buf = av_malloc(buf_size); // [!] -> av_free.
    if (buf == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can not allocate buffer, file: %s.\n", infp);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return ERR_BUFFER;
    }

    enum AVPixelFormat dstPixelFormat = AV_PIX_FMT_RGBA;    // RGBA 8:8:8:8, 32bpp, RGBA RGBA ...
    //enum AVPixelFormat dstPixelFormat = AV_PIX_FMT_RGB24; // RGB 8:8:8, 24bpp, RGB RGB ...
    int rgb_buf_size = av_image_get_buffer_size(dstPixelFormat, ctx->width, ctx->height, 32);
    if (rgb_buf_size < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can not get image buffer size, file: %s.\n", infp);
        av_free(buf);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return ERR_RGB_BUFFER_SIZE;
    }

    uint8_t *rgb_buf = av_malloc(rgb_buf_size); // [!] -> av_free.
    if (rgb_buf == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can not allocate RGB buffer, file: %s.\n", infp);
        av_free(buf);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return ERR_RGB_BUFFER;
    }

    uint8_t *const rgb_dst[1] = {rgb_buf};
    const int rgb_dstStride[1] = {ctx->width * PIXEL_CHANNELS_COUNT}; // RGBA.

    struct SwsContext *conv_ctx = sws_getContext(
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
    if (conv_ctx == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can not create conversion context, file: %s.\n", infp);
        av_free(rgb_buf);
        av_free(buf);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return ERR_SWS_CONTEXT;
    }

//    printf("#tb %d: %d/%d\n", stream, fmt_ctx->streams[stream]->time_base.num, fmt_ctx->streams[stream]->time_base.den);

    AVPacket *pkt = av_packet_alloc(); // [!] The resulting struct must be freed using av_packet_free().
    if (pkt == NULL) {
        av_free(rgb_buf);
        av_free(buf);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return ERR_PACKET;
    }

    AVFrame *fr = av_frame_alloc(); // [!] The resulting struct must be freed using av_frame_free().
    if (fr == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can not allocate frame, file: %s.\n", infp);
        av_packet_free(&pkt);
        av_free(rgb_buf);
        av_free(buf);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return err;
    }

    // FFmpeg library uses a default decoder for WebM video. The default
    // decoder does not recognize YUVA420p format and sees it as YUV420p.
    // Even if I set the pixel format manually like so:
    //      ctx->pix_fmt = AV_PIX_FMT_YUVA420P;
    // the decoder is unable to see the alpha channel. This is disgusting.
    // https://stackoverflow.com/questions/66702932/is-there-a-way-to-force-ffmpeg-to-decode-a-video-stream-with-alpha-from-a-webm
    // There is a dirty trick to set the decoder manually. :\
    //TODO

    err = decode_and_process_frames(outfdp, fn, buf, buf_size, fmt_ctx, stream, ctx, conv_ctx, rgb_dst, rgb_dstStride, rgb_buf, pkt, fr, writer);
    if (err < 0) {
        av_frame_free(&fr);
        av_packet_free(&pkt);
        av_free(rgb_buf);
        av_free(buf);
        avcodec_free_context(&ctx);
        avformat_close_input(&fmt_ctx);
        return err;
    }

    av_frame_free(&fr);
    av_packet_free(&pkt);
    av_free(rgb_buf);
    av_free(buf);
    avcodec_free_context(&ctx);
    avformat_close_input(&fmt_ctx);
    return 0;
}

/*
 * Read frames, decode them, save images to disk.
 *
 * Documentation:
 * https://ffmpeg.org/doxygen/3.3/group__lavc__encdec.html
 *
 * @param outfdp    output folder path
 * @param fn        number of processed frames
 * @param writer    file writer type
 *
 * @return          negative error code in case of failure.
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
                              char *writer) {
    // Read frames and process them.
    int err = 0;
    while (1) {
        // Read a frame.
        err = av_read_frame(fmt_ctx, pkt); // [!] The packet must be freed with av_packet_unref() when it is no longer needed.
        if (err < 0) {
            if (err == AVERROR_EOF) {
                break;
            }
            av_log(NULL, AV_LOG_ERROR, "Can not read a frame: %d.\n", *fn);
            return err;
        }

        // It is possible to read a wrong frame. This is FFmpeg.
        if (pkt->stream_index != stream) { // Not our frame.
            av_packet_unref(pkt);
            continue;
        }

        // Send the frame to a decoder.
        send:
        err = avcodec_send_packet(ctx, pkt);
        if (err < 0) {
            if (err == AVERROR(EAGAIN)) {
                // Receive and process decoded frames.
                err = process_decoded_frames(ctx, fr, buf, buf_size, conv_ctx, rgb_dst, rgb_dstStride, rgb_buf, outfdp, fn, writer);
                if (err < 0) {
                    if (err != AVERROR(EAGAIN)) {
                        av_log(NULL, AV_LOG_ERROR, "Can not process decoded frames, frame number: %d.\n", *fn);
                        av_packet_unref(pkt);
                        return err;
                    }
                }

                // Send the frame again.
                goto send;
            } else {
                av_log(NULL, AV_LOG_ERROR, "Can not send a packet, frame number: %d.\n", *fn);
                av_packet_unref(pkt);
                return err;
            }
        }

        // Receive and process decoded frames.
        err = process_decoded_frames(ctx, fr, buf, buf_size, conv_ctx, rgb_dst, rgb_dstStride, rgb_buf, outfdp, fn, writer);
        if (err < 0) {
            if (err == AVERROR(EAGAIN)) {
                // no av_packet_unref() here because we need additional data.
                continue;
            }
            av_log(NULL, AV_LOG_ERROR, "Can not process decoded frames, frame number: %d.\n", *fn);
            av_packet_unref(pkt);
            return err;
        }

        av_packet_unref(pkt);
    }

    // Flushing the decoder.
    while (1) {
        err = avcodec_send_packet(ctx, NULL);
        if (err < 0) {
            return err;
        }

        err = process_decoded_frames(ctx, fr, buf, buf_size, conv_ctx, rgb_dst, rgb_dstStride, rgb_buf, outfdp, fn, writer);
        if (err < 0) {
            if (err == AVERROR_EOF) {
                break;
            }
            return err;
        }
    }

    avcodec_flush_buffers(ctx);
    return 0;
}

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
                           char *writer) {
    while (1) {
        int err = avcodec_receive_frame(ctx, fr); // [!] It always calls av_frame_unref(frame) before doing anything else.
        if (err < 0) {
            return err;
        }

        int cwb = av_image_copy_to_buffer( // Count of written bytes.
                buf,
                buf_size,
                (const uint8_t *const *) fr->data,
                (const int *) fr->linesize,
                ctx->pix_fmt,
                ctx->width,
                ctx->height,
                1);
        if (cwb < 0) {
            av_log(NULL, AV_LOG_ERROR, "Can not copy image to buffer, frame number: %d.\n", *fn);
            av_frame_unref(fr);
            return cwb;
        }

//            printf("%d, %s, %s, %8"PRId64", %8d, 0x%08lx, %dx%d\n", stream,
//                   av_ts2str(fr->pts), av_ts2str(fr->pkt_dts), fr->pkt_duration,
//                   cwb,
//                   av_adler32_update(0, (const uint8_t *) buf, cwb),
//                   fr->width, fr->height);


        // Convert the colour space (YUV -> RGBA).
        int h = sws_scale(
                conv_ctx,
                (const uint8_t *const *) fr->data,
                (const int *) fr->linesize,
                0,
                ctx->height,
                rgb_dst,
                rgb_dstStride);
        if (h != ctx->height) {
            av_log(NULL, AV_LOG_ERROR, "sws_scale height error: %d vs %d.\n", h, ctx->height);
            av_frame_unref(fr);
            return ERR_SWS_SCALE;
        }

        err = write_image(outfdp, *fn, ctx->width, ctx->height, rgb_buf, writer); // Output file path.
        if (err < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error writing image file, frame number: %d.\n", *fn);
            av_frame_unref(fr);
            return ERR_WRITE_IMAGE;
        }
        av_log(NULL, AV_LOG_INFO, "File was written to disk, frame number: %d.\n", *fn);

        // Next frame.
        (*fn)++;
    }
}
