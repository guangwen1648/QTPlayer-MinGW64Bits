#ifndef FFMPEGPLAYER_CPP
#define FFMPEGPLAYER_CPP

#include "FFmpegManager/ffmpegmanager.h"

FFmpegManager::FFmpegManager()
{

}

FFmpegManager::~FFmpegManager()
{

}

void FFmpegManager::rePlay()
{
    av_seek_frame(inputctx,videoindex,0,AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);//跳回开头
    numFrame = 0;
}

void FFmpegManager::initFFmPlayer(const char *filepath)
{
    av_register_all();
    avformat_network_init();
    inputctx = avformat_alloc_context();

    if(avformat_open_input(&inputctx,filepath,NULL,NULL)!=0)
    {
        qDebug()<<"打开视频流失败";
        return;
    }

    if(avformat_find_stream_info(inputctx,NULL)<0)
    {
        qDebug()<<"获取文件信息失败";
        return;
    }

    videoindex = -1;
    for(i = 0;i < inputctx->nb_streams;i++)
    {
        if(inputctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoindex = i;
            break;
        }

    }

    if(videoindex == -1)
    {
        qDebug()<<"没有发现视频流";
        return;
    }

    pCodecCtx = inputctx->streams[videoindex]->codec;
    codec  = avcodec_find_decoder(pCodecCtx->codec_id);
    if(NULL == codec)
    {
        qDebug()<<"没有查到解码器";
        return;
    }

    if(avcodec_open2(pCodecCtx,codec,NULL)<0)
    {
        qDebug()<<"打开解码器失败";
        return;
    }

    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();


    outbuffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
    avpicture_fill((AVPicture *)pFrameYUV,outbuffer,AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

    img_convert_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height
                                     ,AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));

    numFrame = 0;
}

AVFrame * FFmpegManager::displayvideo()
{
    if(av_read_frame(inputctx, packet)<0)
    {
        //av_seek_frame(inputctx,videoindex,0,AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);//跳回开头
        return NULL;
    }
    if(packet->stream_index==videoindex)
    {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &(got_picture), packet);

        if(ret < 0)
        {
            qDebug()<<"解码失败";
            return NULL;
        }

        if(got_picture)
        {
            sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
            numFrame++;
        }
    }
    av_free_packet(packet);
    return pFrameYUV;
}

void FFmpegManager::finishFFmpeg()
{
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&inputctx);
}
#endif // FFMPEGPLAYER_CPP
