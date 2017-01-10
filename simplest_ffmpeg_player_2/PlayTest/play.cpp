#include "stdafx.h"

#include <iostream>
#include "video_decoder.h"
extern "C"
{
#include "sdl/SDL.h"
#include "sdl/SDL_thread.h"

};

int output(const uint8_t* const* data, int w, int h, const int* linesize)
{
#if 1
    static bool run = false;
    static SDL_Window *screen = NULL;
    static SDL_Renderer* sdlRenderer = NULL;
    static SDL_Texture* sdlTexture = NULL;
    static SDL_Rect sdlRect;

    if (!run)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
            printf("Could not initialize SDL - %s\n", SDL_GetError());
            return -1;
        }
        screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            w, h,
            SDL_WINDOW_OPENGL);

        if (!screen) {
            printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
            return -1;
        }
        sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
        //IYUV: Y + U + V  (3 planes)
        //YV12: Y + V + U  (3 planes)
        sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, w, h);
        sdlRect;
        sdlRect.x = 0;
        sdlRect.y = 0;
        sdlRect.w = w;
        sdlRect.h = h;
        run = true;
    }
    //SDL---------------------------
    SDL_UpdateTexture(sdlTexture, &sdlRect, data[0], linesize[0]);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, &sdlRect, &sdlRect);
    SDL_RenderPresent(sdlRenderer);
    //SDL End-----------------------
    //Delay 40ms
    SDL_Delay(40);
#else


#endif

    return 1;
}
void error_out(const std::string& err)
{

}

int _tmain()
{
    video_decoder decoder_;
    decoder_.init("src01_480x272_22.h265", output, error_out);
    decoder_.run();

    return 0;
}