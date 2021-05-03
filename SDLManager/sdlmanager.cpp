#include "SDLManager/sdlmanager.h"
#include <QDebug>

#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)


SDLManager::SDLManager()
{
//    initSDL(width,height);
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        qDebug()<<"init sdl failed!";
        isInitSuccess = false;
        return;
    }
}

void SDLManager::initSDL(int width, int height,QLabel *label)
{
    isInitSuccess = false;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        qDebug()<<"init sdl failed!";
        isInitSuccess = false;
        return;
    }

    screen = SDL_CreateWindowFrom((void *)label->winId());
    if(screen == NULL)
    {
        qDebug()<<"set window fail!";
        isInitSuccess = false;
        return;
    }
    render = SDL_CreateRenderer(screen,-1,0);
    texture = SDL_CreateTexture(render,SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,width,height);
    sdlController.status = SDLStatus::PLAY;
    sdlController.start();
//    video_thread = SDL_CreateThread(wait_thread,NULL,NULL);
    isInitSuccess = true;
}

void SDLManager::SDLRender(const void *pixels, int pitch)
{
    SDL_UpdateTexture(texture, NULL, pixels, pitch);
    SDL_RenderClear(render);

    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);//显示该帧YUV数据
}

void SDLManager::SDLQuit()
{
    if (screen !=nullptr && isInitSuccess)
    {
        SDL_DestroyWindow(screen);
        SDL_Quit();
    }
}

void SDLManager::SDLPlay()
{
    sdlController.status = SDLStatus::PLAY;
}

void SDLManager::SDLStop()
{
    sdlController.status = SDLStatus::STOP;
}

void SDLManager::SDLPause()
{
    sdlController.status = SDLStatus::PAUSE;
}

void SDLManager::SDLWaitEvent()
{
    SDL_WaitEvent(&event);
}

void SDLManager::setSDLRect(int x, int y, int width, int height)
{
    prect.x = x;
    prect.y = y;
    prect.w = width;
    prect.h = height;
}

SDL_Event SDLManager::getEvent()
{
    return event;
}

SDL_Rect SDLManager::getRect()
{
    return prect;
}

//int SDLManager::wait_thread(void *)
//{
//    thread_exit=0;
//    thread_pause=0;

//    while (thread_exit==0)
//    {
//        if(!thread_pause)
//        {
//            SDL_Event event;
//            event.type = SFM_REFRESH_EVENT;
//            SDL_PushEvent(&event);
//        }

//        SDL_Delay(10);

//    }

//    SDL_Event event;
//    event.type = SFM_BREAK_EVENT;
//    SDL_PushEvent(&event);
//    thread_exit=0;
//    thread_pause=0;
//    return 0;
//}

