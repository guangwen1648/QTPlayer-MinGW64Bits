#include "sdlcontroller.h"

#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

SDLController::SDLController()
{

}

void SDLController::run()
{
    while(true)
    {
        if (status == SDLStatus::PLAY)
        {
            SDL_Event event;
            event.type = SFM_REFRESH_EVENT;
            SDL_PushEvent(&event);
            SDL_Delay(10);
        }
        else if (status == SDLStatus::PAUSE)
        {
            continue;
        }
        else if (status == SDLStatus::STOP)
        {
            SDL_Event event;
            event.type = SFM_BREAK_EVENT;
            SDL_PushEvent(&event);
            break;
        }
    }
}
