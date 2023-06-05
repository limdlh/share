// playerDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <cstdint>
//#include<Winsock2.h>
std::fstream fio;
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libpostproc/postprocess.h>
#include <libavfilter/avfilter.h>
}

int interrupt_callback(void* data)
{
    return false;
}

enum class MetaType
{
    META_VIDEO,
    META_AUDIO,
    META_INVALID,
};

static union {
	char c[4];
	unsigned long mylong;
} endian_test = {{'l', '?', '?', 'b'}};

class MetaData {
public:
    MetaData(std::shared_ptr<AVFormatContext>& ic, std::shared_ptr<AVCodecContext>& ctx, int index, const MetaType& type) :ic_(ic), ctx_(ctx), index_(index), type_(type)
    {
        thread = std::thread([](void* data) {
            MetaData* meta = (MetaData*)data;
            meta->decoderRun();
        }, this);
    }
    ~MetaData(){}
	
	

	#define ENDIANNESS ((char)endian_test.mylong) 
	
	#define Swap32(A) ((((int)(A) & 0xff000000) >> 24) | \
				   (((int)(A) & 0x00ff0000) >>  8) | \
				   (((int)(A) & 0x0000ff00) <<  8) | \
				   (((int)(A) & 0x000000ff) << 24))
	
	//将一个无符号长整形数从网络字节顺序转换为主机字节顺序
	int ntohl(int nl)
	{
		return (ENDIANNESS=='l') ? Swap32(nl): nl;	
	}

    void InsertPkt(std::shared_ptr<AVPacket>& pkt)
    {
        std::unique_lock<std::mutex> lk(mutex);
        pktQueue.emplace(pkt);
        sign_.notify_one();
    }
    
    void config_audio_filter()
    {
        AVFilterContext *filt_asrc = NULL, *filt_asink = NULL;
        
        av_get_channel_layout_nb_channels(ctx_->channel_layout);
        ctx_->sample_fmt;
        static const enum AVSampleFormat sample_fmts[] = { AV_SAMPLE_FMT_S16 , AV_SAMPLE_FMT_NONE };
        int sample_rates[] = { 0,-1 };
        int64_t channel_layouts[] = { 0, -1 };
        int channels[] = { 0, -1 };
        AVFilterGraph * graph = avfilter_graph_alloc();
        char asrc_args[256] = { 0 };
        int ret = snprintf(asrc_args, sizeof(asrc_args),
            "sample_rate=%d:sample_fmt=%s:channels=%d:time_base=%d/%d:channel_layout=0x%" PRIx64,
            ctx_->sample_rate, av_get_sample_fmt_name(ctx_->sample_fmt),
            ctx_->channels, 1, ctx_->sample_rate, ctx_->channel_layout);

        ret = avfilter_graph_create_filter(&filt_asrc,
            avfilter_get_by_name("abuffer"), "my_abuffer",
            asrc_args, NULL, graph);

        ret = avfilter_graph_create_filter(&filt_asink,
            avfilter_get_by_name("abuffersink"), "my_abuffersink",
            NULL, NULL, graph);

    }
    void decoderRun()
    {
        if (type_ == MetaType::META_AUDIO)
        {
            config_audio_filter();
        }

        std::cout << "Run...." << std::endl;
        while (true)
        {
            {
                std::unique_lock<std::mutex> lk(mutex);
                sign_.wait(lk, [this] {return pktQueue.size() > 0; });
            }
            while (pktQueue.size() > 0)
            {
                std::shared_ptr<AVPacket> pkt = pktQueue.front();

                //std::cout << "pkt:stream:" << pkt->stream_index << ",pts:" << pkt->pts << ",dts:" << pkt->dts << std::endl;
                pktQueue.pop();
                if (type_ == MetaType::META_VIDEO)
                {
                    
                    static std::once_flag once;
                    std::call_once(once, [&]() {
                        fio.open("avcc.h264", std::ios::binary | std::ios::trunc | std::ios::out);
                        bool isOpen = fio.is_open();
                        int extraSize = ntohl(ctx_->extradata_size);
                        fio.write((char*)&extraSize, sizeof(int));
                        fio.write((char*)ctx_->extradata, ctx_->extradata_size);
                        fio.flush();
                    });
                    int size = ntohl(pkt->size);
                    fio.write((char*)&size, sizeof(int));
                    fio.write((char*)pkt->data, pkt->size);
                    fio.flush();
                }
                auto err = avcodec_send_packet(ctx_.get(), pkt.get());
                if (err != AVERROR(EAGAIN)) {
                    std::shared_ptr<AVFrame> frame(av_frame_alloc(), [](AVFrame* frame) { av_frame_free(&frame); });
                    err = avcodec_receive_frame(ctx_.get(), frame.get());
                    if (type_ == MetaType::META_AUDIO)
                    {
                        std::cout << "AUDIO:";
                    }
                    else
                    {
                        std::cout << "VIDEO:";
                    }
                    std::cout << "err:" << err << ",frame: pts:" << frame->pts << std::endl;
                }
                else
                {
                    std::cout << "err:" << err << std::endl;
                }
            }
        }
    }


    void Exit() {
        thread.join();
    }
private:
    std::shared_ptr<AVFormatContext> ic_;
    std::shared_ptr<AVCodecContext> ctx_;
    int index_;
    std::queue<std::shared_ptr<AVPacket>> pktQueue;
    std::queue< std::shared_ptr<AVFrame>> frameQueue;
    std::mutex mutex;
    std::condition_variable sign_;
    std::thread thread;
    MetaType type_;
};

int main()
{
    std::string input = "./shuimenqiao.mp4";
    avformat_network_init();
    AVFormatContext* temp = avformat_alloc_context();
    std::shared_ptr<AVFormatContext> ic(temp, avformat_free_context);
    if (ic == nullptr)
    {
        std::cout << "avformat_alloc_context failed" << std::endl;
        return -1;
    }
    //可以指定callback
    ic->interrupt_callback.callback = interrupt_callback;
    ic->interrupt_callback.opaque = 0;
    AVDictionary* format_opts = nullptr;
    av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
    char buf[1024] = { 0 };
    
    auto err = avformat_open_input(&temp, input.c_str(), NULL, &format_opts);
    if (0 != err) {
        av_strerror(err, buf, 1024);
        std::cout << "avformat_open_input:" << buf<< std::endl;
        return -1;
    }
    err = avformat_find_stream_info(ic.get(), nullptr);
    if (err < 0)
    {
        std::cout << "avformat_find_stream_info:"<<err << std::endl;
        return -1;
    }
    int videoIndex = -1;
    int autioIndex = -1;
    std::shared_ptr<MetaData> video = nullptr;// std::make_shared<MetaData>(ic, videoIndex);
    std::shared_ptr<MetaData> audio = nullptr;// std::make_shared<MetaData>(ic, autioIndex);
    for (unsigned int i = 0; i < ic->nb_streams; i++)
    {
        AVStream* st = ic->streams[i];
        if (st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoIndex = i;
            AVCodecContext* ctx = avcodec_alloc_context3(nullptr);
            std::shared_ptr<AVCodecContext> avctx(ctx, [](AVCodecContext* ctx) {avcodec_free_context(&ctx); });
            const AVCodec *codec;
            auto ret = avcodec_parameters_to_context(avctx.get(), ic->streams[videoIndex]->codecpar);
            codec = avcodec_find_decoder(avctx->codec_id);
            avcodec_open2(avctx.get(), codec, nullptr);
            video = std::make_shared<MetaData>(ic, avctx, videoIndex, MetaType::META_VIDEO);
        }
        else if (st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            autioIndex = i;
            
            AVCodecContext* ctx = avcodec_alloc_context3(nullptr);
            std::shared_ptr<AVCodecContext> avctx(ctx, [](AVCodecContext* ctx) {avcodec_free_context(&ctx); });
            const AVCodec *codec;
            auto ret = avcodec_parameters_to_context(avctx.get(), ic->streams[autioIndex]->codecpar);
            codec = avcodec_find_decoder(avctx->codec_id);
            avcodec_open2(avctx.get(), codec, nullptr);
            audio = std::make_shared<MetaData>(ic, avctx, autioIndex, MetaType::META_AUDIO);
            
        }
    }

    
    int ret = 0;
    do
    {
        auto pkt = std::shared_ptr<AVPacket>(av_packet_alloc(), av_packet_unref);

        ret = av_read_frame(ic.get(), pkt.get());
        if (ret < 0)
        {
            std::cout << "frame read failed" << std::endl;
            break;
        }

        if (pkt->stream_index == videoIndex)
        {
            video->InsertPkt(pkt);
        }
        else if (pkt->stream_index == autioIndex && audio != nullptr)
        {
            audio->InsertPkt(pkt);
        }

    } while (true);
    
    //MetaData autio = {.ic = ic, .st = ic->streams[autioIndex], .stream_index = autioIndex};
    auto t1 = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto t2 = std::chrono::system_clock::now();
    std::chrono::duration<double> dif = t2 - t1;
    
    auto t = dif.count();
    std::cout << "file:" << __FILE__ <<":"<<__LINE__ << ":" << __DATE__ << ":" << __TIME__ << std::endl;
    video->Exit();
    audio->Exit();
    std::cout << "Hello World!\n";
}
