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
 
const char* SRC_FILE = "1.mp4";

int main()
{
	AVPacket *packet = NULL;
    FILE *yuv_file = fopen("yuv_file","ab");
    if (!yuv_file)
        return 0;
    av_register_all();
    AVFormatContext* pFormat = NULL;
    if (avformat_open_input(&pFormat, SRC_FILE, NULL, NULL) < 0)
    {
        return 0;
    }
    AVCodecContext* video_dec_ctx = NULL;
    AVCodec* video_dec = NULL;
    if (avformat_find_stream_info(pFormat, NULL) < 0)
    {
        return 0;
    }
    av_dump_format(pFormat,0,SRC_FILE,0);
    video_dec_ctx = pFormat->streams[0]->codecpar;
    video_dec = avcodec_find_decoder(video_dec_ctx->codec_id);
    if (avcodec_open2(video_dec_ctx, video_dec, NULL) < 0)
    {
        return 0;
    }
    AVPacket *pkt = new AVPacket();
    av_init_packet(pkt);
    while (1)
    {
        if (av_read_frame(pFormat, pkt) < 0)
        {
            fclose(yuv_file);
            delete pkt;
            return 0;
        }
        if (pkt->stream_index == 0)
        {
            AVFrame *pFrame = avcodec_alloc_frame();
            int got_picture = 0,ret = 0;
            ret = avcodec_decode_video2(video_dec_ctx, pFrame, &got_picture, pkt);
            if (ret < 0)
            {
                delete pkt;
                return 0;
            }
            if (got_picture)
            {
                char* buf = new char[video_dec_ctx->height * video_dec_ctx->width * 3 / 2];
                memset(buf, 0, video_dec_ctx->height * video_dec_ctx->width * 3 / 2);
                int height = video_dec_ctx->height;
                int width = video_dec_ctx->width;
                printf("decode video ok\n");
                int a = 0, i;
                for (i = 0; i<height; i++)
                {
                    memcpy(buf + a, pFrame->data[0] + i * pFrame->linesize[0], width);
                    a += width;
                }
                for (i = 0; i<height / 2; i++)
                {
                    memcpy(buf + a, pFrame->data[1] + i * pFrame->linesize[1], width / 2);
                    a += width / 2;
                }
                for (i = 0; i<height / 2; i++)
                {
                    memcpy(buf + a, pFrame->data[2] + i * pFrame->linesize[2], width / 2);
                    a += width / 2;
                }
                fwrite(buf, 1, video_dec_ctx->height * video_dec_ctx->width * 3 / 2, yuv_file);
                delete buf;
                buf = NULL;
            }
            avcodec_free_frame(&pFrame);
        }
    }

    return 0;
}