//#include "stdafx.h"
#include "video_decoder.h"


extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
    //SDL
// #include "sdl/SDL.h"
// #include "sdl/SDL_thread.h"

};

//Output YUV420P data as a file 
#define OUTPUT_YUV420P 0

video_decoder::video_decoder()
    :run_(false)
{
}


video_decoder::~video_decoder()
{
}

bool video_decoder::init(const std::string & name, output outfunc, error func)
{
    name_ = name;
    output_ = outfunc;
    //func(name);
    return true;
}

#pragma warning(disable:4996)
bool video_decoder::run()
{
    run_ = true;
    AVFormatContext	*pFormatCtx;
    int				i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    //char filepath[] = "src01_480x272_22.h265";
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, name_.c_str(), NULL, NULL) != 0){
        printf("Couldn't open input stream.（无法打开输入流）\n");
        return -1;
    }
    if (av_find_stream_info(pFormatCtx) < 0)
    {
        printf("Couldn't find stream information.（无法获取流信息）\n");
        return -1;
    }
    videoindex = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoindex = i;
            break;
        }
    if (videoindex == -1)
    {
        printf("Didn't find a video stream.（没有找到视频流）\n");
        return -1;
    }
    pCodecCtx = pFormatCtx->streams[videoindex]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL)
    {
        printf("Codec not found.（没有找到解码器）\n");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("Could not open codec.（无法打开解码器）\n");
        return -1;
    }
    AVFrame	*pFrame, *pFrameYUV;
    pFrame = avcodec_alloc_frame();
    pFrameYUV = avcodec_alloc_frame();
    uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)

    int ret, got_picture;
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    //printf("File Information（文件信息）---------------------\n");
    av_dump_format(pFormatCtx, 0, name_.c_str(), 0);

#if OUTPUT_YUV420P 
    FILE *fp_yuv = fopen("output.yuv", "wb+");
#endif  

    struct SwsContext *img_convert_ctx;
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        if (packet->stream_index == videoindex)
        {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0)
            {
                printf("Decode Error.（解码错误）\n");
                return -1;
            }
            if (got_picture)
            {
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
                if (output_)
                {
                    output_(pFrameYUV->data, pCodecCtx->width, pCodecCtx->height, pFrameYUV->linesize);
                }
#if OUTPUT_YUV420P
                int y_size = pCodecCtx->width*pCodecCtx->height;
                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv); //Y 
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V
#endif
            }
        }
        av_free_packet(packet);
    }
    sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

    av_free(out_buffer);
    av_free(pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
