//https://blog.csdn.net/fanyun_01/article/details/117403651?spm=1001.2101.3001.6650.16&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-16-117403651-blog-125971410.pc_relevant_multi_platform_whitelistv3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-16-117403651-blog-125971410.pc_relevant_multi_platform_whitelistv3&utm_relevant_index=22


extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/time.h"
}
#include <iostream>
using namespace std;

 

int main(int argc, char *argv[]){
	AVCodecContext* pCodecCtx = 0;
	AVCodec* pCodec;
	int in_w, in_h, got_picture, ret;
	AVPacket pkt;
	uint8_t* picture_buf;
	AVFrame* pFrame = 0;
	int picture_size;
	struct SwsContext* img_convert_ctx;
	AVDictionary* param = 0;
	
	//传输
	AVFormatContext* ofmt_ctx = NULL;
	AVStream* video_st;
	int framecnt = 0;
	AVRational time_base_q = { 1, AV_TIME_BASE };
	int vid_next_pts = 0;
 
	
	if (avformat_alloc_output_context2(&ofmt_ctx, NULL, "rtsp", pInf->RTSP_addr) < 0)
	{
		printf("Fail: avformat_alloc_output_context2\n");
		goto o_Terminate_Coder_Thread;
	}
 
	//检查所有流是否都有数据，如果没有数据会等待max_interleave_delta微秒
	ofmt_ctx->max_interleave_delta = 1000000;
 
	//使用tcp协议传输
	av_opt_set(ofmt_ctx->priv_data, "rtsp_transport", "tcp", 0);
 
	/* Some formats want stream headers to be separate. */
	//Add a new stream to output, should be called by the user before avformat_write_header() for muxing
	video_st = avformat_new_stream(ofmt_ctx, pCodec);
	if (video_st == NULL)
	{
		goto o_Terminate_Coder_Thread;
	}
	video_st->time_base.num = 1;
	//video_st->time_base.den = 75;
	video_st->time_base.den = 10;
	avcodec_parameters_from_context(video_st->codecpar, pCodecCtx);
	printf("======= av_dump_format output =======\n");
	av_dump_format(ofmt_ctx, 0, pInf->RTSP_addr, 1);
	printf("============== dump end ========= \n");
 
	//Open output URL
	if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
	{
		ret = avio_open(&ofmt_ctx->pb, pInf->RTSP_addr, AVIO_FLAG_READ_WRITE);
		if (ret < 0)
		{
			printf("Could not open output URL '%s'", pInf->RTSP_addr);
			goto o_Terminate_Coder_Thread;
		}
	}
 
	av_dict_free(&param);
	av_dict_set(&param, "stimeout", "5000000", 0);//很关键，当推送目标没有响应时，5秒后会自动退出，要不就死这了
	ret = avformat_write_header(ofmt_ctx, &param);
	if (ret < 0)
		printf("Error occurred when opening output URL\n");
	else
		printf("avformat_write_header return: %d\n", ret);
 
 
	while (1)
	{
 
		pkt.data = NULL;
		pkt.size = 0;
		av_init_packet(&pkt);
 
		//注意，在这里获得编码数据
		//pkt = 。。。
		///
 
		framecnt++;
		pkt.stream_index = video_st->index;
 
		AVRational time_base = ofmt_ctx->streams[0]->time_base;//{ 1, 1000 };
		AVRational r_framerate1 = { 1, 25 };
		//Duration between 2 frames (us)
		int64_t calc_duration = (double)(AV_TIME_BASE) * (1 / av_q2d(r_framerate1));	//内部时间戳
		//Parameters
		//enc_pkt.pts = (double)(framecnt*calc_duration)*(double)(av_q2d(time_base_q)) / (double)(av_q2d(time_base));
		pkt.pts = av_rescale_q(framecnt * calc_duration, time_base_q, time_base);
		pkt.dts = pkt.pts;
		pkt.duration = av_rescale_q(calc_duration, time_base_q, time_base); //(double)(calc_duration)*(double)(av_q2d(time_base_q)) / (double)(av_q2d(time_base));
		pkt.pos = -1;
 
		vid_next_pts = framecnt * calc_duration; //general timebase
 
		if(pInf->close_coder_Thread == 0)SendToRTSP(pInf, pkt.data + 4, pkt.size - 4);
 
		//av_write_frame(ofmt_ctx, &pkt);
		ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
 
		av_packet_unref(&pkt);
		delete[] p;
		pInf->VideoBuffer_Count--;
	}
 
	av_write_trailer(ofmt_ctx);
}