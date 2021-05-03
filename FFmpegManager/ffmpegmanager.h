#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

#include <QDebug>
#ifdef __cplusplus
extern "C"
{
#endif
#include "lib/ffmpeg/include/libavformat/avformat.h"//封装格式
#include "lib/ffmpeg/include/libavcodec/avcodec.h"//编码
#include "lib/ffmpeg/include/libavutil/avutil.h"//
#include "lib/ffmpeg/include/libswscale/swscale.h"//像素处理
#ifdef __cplusplus
}
#endif
class FFmpegManager{
public:
    FFmpegManager();
    ~FFmpegManager();
    void initFFmPlayer(const char *filepath);
    AVFrame * displayvideo();
    void finishFFmpeg();
    void rePlay();
public:
    AVFormatContext *inputctx;
    int i,videoindex;
    AVCodecContext *pCodecCtx;
    AVCodec *codec;
    AVFrame *pFrame,*pFrameYUV;
    uint8_t *outbuffer;
    AVPacket *packet;
    int ret,got_picture;
    double numFrame = 0;
    struct SwsContext *img_convert_ctx;

};

#endif // FFMPEGPLAYER_H
