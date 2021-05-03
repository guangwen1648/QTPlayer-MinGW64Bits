#ifndef SDLCONTROLLER_H
#define SDLCONTROLLER_H

#include <QThread>
#ifdef __cplusplus
extern "C"
{
#endif
#include"lib/SDL2/include/SDL.h"
#ifdef __cplusplus
}
#endif

enum SDLStatus
{
    PLAY=1,
    STOP,
    PAUSE,
};

class SDLController : public QThread
{
    Q_OBJECT
public:
    SDLController();

    void run();

    SDLStatus status;
};

#endif // SDLCONTROLLER_H
