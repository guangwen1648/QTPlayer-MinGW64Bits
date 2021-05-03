#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <QLabel>
#include <QObject>

#ifdef __cplusplus
extern "C"
{
#endif
#include"lib/SDL2/include/SDL.h"
#ifdef __cplusplus
}
#endif

#include "SDLManager/sdlcontroller.h"

class SDLManager : public QObject
{
    Q_OBJECT
public:
    explicit SDLManager();
    void initSDL(int width, int height,QLabel *label);
    void SDLStop();
    void SDLPlay();
    void SDLPause();
    void createTexture(int width, int height);
    void SDLRender(const void *pixels, int pitch);
    void SDLQuit();
    void SDLWaitEvent();
    void setSDLRect(int x, int y, int width, int height);
    SDL_Event getEvent();
    SDL_Rect getRect();

    bool isInitSuccess;

signals:

private:
    SDL_Window *screen;
    SDL_Renderer *render;
    SDL_Texture *texture;
    SDL_Thread *video_thread;
    SDL_Event event;
    SDL_Rect prect;
    SDLController sdlController;
};

#endif // SDLMANAGER_H
