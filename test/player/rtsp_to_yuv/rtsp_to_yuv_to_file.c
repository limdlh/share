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
 
int main()
{
	av_log_set_level(AV_LOG_INFO);
	AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
	AVCodec *pCodec = NULL;
	AVDictionary *options = NULL;
	AVPacket *packet = NULL;
	AVFrame *pFrameYUV = NULL;
	//char filepath[] = "rtsp://admin:hk12345678@172.168.0.161:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";
	char filepath[] = "rtsp://192.168.20.250:554/media/video3";
	
	//av_register_all();  //函数在ffmpeg4.0以上版本已经被废弃，所以4.0以下版本就需要注册初始函数
	
	av_dict_set(&options, "buffer_size", "1024000", 0); //设置缓存大小,1080p可将值跳到最大
	av_dict_set(&options, "rtsp_transport", "tcp", 0); //以tcp的方式打开,
	//av_dict_set(&options, "stimeout", "5000000", 0); //设置超时断开链接时间，单位us
	//av_dict_set(&options, "max_delay", "500000", 0); //设置最大时延
	av_dict_set(&options, "stimeout", "50000000", 0); //设置超时断开链接时间，单位us
	av_dict_set(&options, "max_delay", "5000000", 0); //设置最大时延
	
	pFormatCtx = avformat_alloc_context(); //用来申请AVFormatContext类型变量并初始化默认参数,申请的空间

	//打开网络流或文件流
	printf("now start to avformat_open_input() \n");
	if (avformat_open_input(&pFormatCtx, filepath, NULL, &options) != 0)
	{
		printf("Couldn't open input stream.\n");
		return 0;
	}
	
	//获取视频文件信息
	printf("now start to avformat_find_stream_info() \n");
	if (avformat_find_stream_info(pFormatCtx, NULL)<0)
	{
		printf("Couldn't find stream information.\n");
		return 0;
	}
	
	//查找码流中是否有视频流
	int videoindex = -1;
	unsigned i = 0;
	for (i = 0; i<pFormatCtx->nb_streams; i++){
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
        }
    }
	printf("videoindex: %d\n", videoindex);
	if (videoindex == -1)
	{
		printf("Didn't find a video stream.\n");
		return 0;
	}
 
    pCodecCtx = (AVCodecContext *) pFormatCtx->streams[videoindex]->codecpar;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);  //寻找解码器
	//pCodec = avcodec_find_decoder(AV_CODEC_ID_H265);
	printf("pCodecCtx->codec_id: %d, AV_CODEC_ID_NUV: %d\n", pCodecCtx->codec_id, AV_CODEC_ID_NUV);
	if (pCodec == NULL)
	{
		printf("codecpar not found.\n");
		return -1;
	}
	printf("now start to avcodec_open2() \n");
	if (avcodec_open2(pCodecCtx, pCodec, NULL)<0)  //打开解码器
	{
		printf("Could not open codecpar.\n");
		return -1;
	}
	printf("now finished to avcodec_open2() >>>>>>>>>>>>>>>>>>>\n");
	
	packet = (AVPacket *)av_malloc(sizeof(AVPacket)); // 申请空间，存放的每一帧数据 （h264、h265）
	pFrameYUV = av_frame_alloc(); // 申请空间，存放每一帧编码后的YUV数据
    AVStream *video_stream = pFormatCtx->streams[videoindex];
	
	FILE *fp_YUV = NULL;
    int got_picture = 0;
	fp_YUV = fopen("getYUV.yuv", "wb");
 
    //这边可以调整i的大小来改变文件中的视频时间,每 +1 就是一帧数据
	for (i = 0; i < 200; i++)
	{
		if (av_read_frame(pFormatCtx, packet) >= 0)
		{
			if (packet->stream_index == videoindex)
			{      
                avcodec_decode_video2(pCodecCtx,pFrameYUV, &got_picture, packet);
                fwrite(pFrameYUV->data[0], 1, video_stream->codecpar->width*video_stream->codecpar->height,fp_YUV);  //Y
                fwrite(pFrameYUV->data[1], 1, video_stream->codecpar->width*video_stream->codecpar->height/4,fp_YUV);//U
                fwrite(pFrameYUV->data[2], 1, video_stream->codecpar->width*video_stream->codecpar->height/4,fp_YUV);//V
                printf("w: %d, H: %d\n", video_stream->codecpar->width, video_stream->codecpar->height);
			}
			av_packet_unref(packet);
		}
	}
 
	fclose(fp_YUV);
    av_free(packet);
    av_frame_free(&pFrameYUV);
	avformat_close_input(&pFormatCtx);
    
    return 0;
}
