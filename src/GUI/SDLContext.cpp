#include "GUI/SDLContext.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>

SDLContext::SDLContext()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(SDL_GetError());
    }
    if (!TTF_Init())
    {
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }
}

SDLContext::~SDLContext()
{
    TTF_Quit();
    SDL_Quit();
}