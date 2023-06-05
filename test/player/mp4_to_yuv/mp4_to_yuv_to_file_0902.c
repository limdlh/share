#include <stdio.h>
#include <stdlib.h>
 
#ifdef __cplusplus 
extern "C"
{
#endif
	/*Include ffmpeg header file*/
#include <libavformat/avformat.h> 
#include <libavcodec/avcodec.h> 
#include <libswscale/swscale.h> 
 
#ifdef __cplusplus
}
#endif

const char* SRC_FILE = "Hard_Sun.mp4";
#define PIX_FORMAT AV_PIX_FMT_YUV420P
int main()
{
	AVPacket *pkt = NULL;
    FILE *yuv_file = fopen("yuv_file","wb");
    if (!yuv_file)
        return 0;
    AVFormatContext* pFormat = NULL;
    if (avformat_open_input(&pFormat, SRC_FILE, NULL, NULL) < 0)
    {
        return 0;
    }
    AVCodecContext* video_dec_ctx = avcodec_alloc_context3(NULL);
    AVCodec* video_dec = NULL;
    if (avformat_find_stream_info(pFormat, NULL) < 0)
    {
        return 0;
    }
    av_dump_format(pFormat,0,SRC_FILE,0);
    avcodec_parameters_to_context(video_dec_ctx, pFormat->streams[0]->codecpar);
    video_dec = avcodec_find_decoder(video_dec_ctx->codec_id);
    if (avcodec_open2(video_dec_ctx, video_dec, NULL) < 0)
    {
        return 0;
    }
	pkt = av_packet_alloc();
    av_init_packet(pkt);
    // YUV格式转化以及去掉黑边
    struct SwsContext* swsCtx = NULL;
    AVFrame* newFrame = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(PIX_FORMAT, video_dec_ctx->width, video_dec_ctx->height,1);
    uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(newFrame->data, newFrame->linesize, buffer, PIX_FORMAT, video_dec_ctx->width, video_dec_ctx->height,1);
    
    while (1)
    {
        if (av_read_frame(pFormat, pkt) < 0)
        {
            fclose(yuv_file);
            //delete pkt;
			av_free(pkt);
            return 0;
        }
        if (pkt->stream_index == 0)
        {
            //AVFrame *pFrame = avcodec_alloc_frame();
            int err = avcodec_send_packet(video_dec_ctx, pkt);
            if (err != AVERROR(EAGAIN))
            {
                AVFrame *pFrame = av_frame_alloc();
                int got_picture = 0,ret = 0;
                ret = avcodec_receive_frame(video_dec_ctx, pFrame);
                if (ret !=0)
                {
                    av_frame_free(&pFrame);
                    av_packet_unref(pkt);
                    continue;
                }
                //char* buf = new char[video_dec_ctx->height * video_dec_ctx->width * 3 / 2];
                swsCtx = sws_getCachedContext(swsCtx, pFrame->width, pFrame->height, video_dec_ctx->pix_fmt, video_dec_ctx->width, video_dec_ctx->height, PIX_FORMAT, SWS_X, NULL, NULL, NULL);
                // 原数据进行转化及裁剪
                int height = sws_scale(swsCtx, pFrame->data, pFrame->linesize, 0, video_dec_ctx->height, newFrame->data, newFrame->linesize);
                //int height = video_dec_ctx->height;
                int width = video_dec_ctx->width;
                printf("decode video ok\n");
                int a = 0, i;
                // 写入所有Y
                fwrite(newFrame->data[0], 1, newFrame->linesize[0],yuv_file);
                fflush(yuv_file);
                
                // 写入所有U
                fwrite(newFrame->data[1], 1, newFrame->linesize[1], yuv_file);
                fflush(yuv_file);
                
                
                fwrite(newFrame->data[2],1, newFrame->linesize[2], yuv_file);
                fflush(yuv_file);

                av_frame_free(&pFrame);
                av_packet_unref(pkt);
            }
			
        }
    }

    return 0;
}