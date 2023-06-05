bool init_rtsp_pusher(){
    //RTSP
    if (avformat_alloc_output_context2(&fmt_ctx, NULL, "RTSP", RTSP_URL.c_str()) < 0){
        printf("Fail: avformat_alloc_output_context2\n");
        return false;
    }
 
    //使用tcp协议传输
    av_opt_set(fmt_ctx->priv_data, "rtsp_transport", "tcp", 0);
 
    //检查所有流是否都有数据，如果没有数据会等待max_interleave_delta微秒
    fmt_ctx->max_interleave_delta = 1000000;
 
    //输出视频流
    AVStream *video_s = avformat_new_stream(fmt_ctx, codec_h264);
    if (!video_s){
        printf("Fail: avformat_new_stream\n");
        return false;
    }
    video_s->time_base = { 1, 25 };
    videoindex = video_s->id = fmt_ctx->nb_streams - 1;  //加入到fmt_ctx流
 
    //复制AVCodecContext的设置
    if (avcodec_copy_context(video_s->codec, codec_ctx_h264) < 0) {
        printf("Fail: avcodec_copy_context\n");
        return false;
    }
    video_s->codec->codec_tag = 0;
    if (fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
        video_s->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    avcodec_parameters_from_context(video_s->codecpar, codec_ctx_h264);
    
	//输出音频流
	AVStream *audio_s = avformat_new_stream(fmt_ctx, codec_ctx_aac->codec);
	if (!audio_s){
		printf("Fail: avformat_new_stream\n");
		return false;
	}
	audio_s->time_base = {1, 25};
	audioindex = audio_s->id = fmt_ctx->nb_streams - 1;

	//复制AVCodecContext的设置
	if (avcodec_copy_context(audio_s->codec, codec_ctx_aac) < 0) {
		printf("Fail: avcodec_copy_context\n");
		return false;
	}
	audio_s->codec->codec_tag = 0;
	if (fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
		audio_s->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	avcodec_parameters_from_context(audio_s->codecpar, codec_ctx_aac);
	//printf("fmt_ctx nb_streams = %d\n", fmt_ctx->nb_streams);
 
    av_dump_format(fmt_ctx, 0, fmt_ctx->filename, 1);
    if (!(fmt_ctx->oformat->flags & AVFMT_NOFILE)) {    //???
        //打开输出URL（Open output URL）
        if (avio_open(&fmt_ctx->pb, fmt_ctx->filename, AVIO_FLAG_WRITE) < 0) {
            printf("Fail: avio_open('%s')\n", fmt_ctx->filename);
            return false;
        }
    }
    return true;
}