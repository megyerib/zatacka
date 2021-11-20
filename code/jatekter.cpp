#include "jatekter.h"
#include "konstans.h"
#include <SDL2_gfxPrimitives.h>

Jatekter::Jatekter(SDL_Renderer* renderer)
{
    this->renderer = renderer;

    int renderer_sz;
    int renderer_m;
    SDL_GetRendererOutputSize(renderer, &renderer_sz, &renderer_m);
    
    pozicio.x = 0;
    pozicio.y = 0;
    pozicio.w = renderer_sz - EREDMJ_SZ;
    pozicio.h = renderer_m;

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_TARGET,
        pozicio.w,
        pozicio.h
    );
}

int Jatekter::Megjelenit()
{
    return SDL_RenderCopy(renderer, texture, NULL, &pozicio);
}

int Jatekter::Torol()
{
    SDL_SetRenderTarget(renderer, texture);

    boxColor(renderer, 0, 0, pozicio.w - 1, pozicio.h - 1, clBlack);

    SDL_SetRenderTarget(renderer, NULL);

    return 0;
}

int Jatekter::Keret()
{
    int error = SDL_SetRenderTarget(renderer, texture);

    boxColor(renderer, 0, 0, pozicio.w - 1, KeretSzeles - 1, clWhite);
    boxColor(renderer, 0, pozicio.h - KeretSzeles, pozicio.w - 1, pozicio.h - 1, clWhite);
    boxColor(renderer, 0, 0, KeretSzeles - 1, pozicio.h - 1, clWhite);
    boxColor(renderer, pozicio.w - KeretSzeles, 0, pozicio.w - 1, pozicio.h - 1, clWhite);

    error = error || SDL_SetRenderTarget(renderer, NULL);

    return error;
}
