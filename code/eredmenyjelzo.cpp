#include "eredmenyjelzo.h"
#include <SDL2_gfxPrimitives.h>
#include "konstans.h"

Eredmenyjelzo::Eredmenyjelzo(SDL_Renderer* renderer)
{
    this->renderer = renderer;

    int renderer_sz;
    int renderer_m;
    SDL_GetRendererOutputSize(renderer, &renderer_sz, &renderer_m);
    
    pozicio.x = renderer_sz - EREDMJ_SZ;
    pozicio.y = 0;
    pozicio.w = EREDMJ_SZ;
    pozicio.h = renderer_m;

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_TARGET,
        pozicio.w,
        pozicio.h
    );
}

int Eredmenyjelzo::Kirajzol()
{
    return Ujrarajzol(); // Majd később lehet trükközni
}

int Eredmenyjelzo::Ujrarajzol()
{
    SDL_SetRenderTarget(renderer, texture);

    Keret();

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &pozicio);

    return 0;
}

int Eredmenyjelzo::Keret()
{
    lineColor(renderer, 0, 0, 0, pozicio.h - 1, clWhite);
    lineColor(renderer, 0, 0, pozicio.w - 1, 0, clWhite);

    lineColor(renderer, pozicio.w - 1, 0, pozicio.w - 1, pozicio.h - 1, clSilver);
    lineColor(renderer, 0, pozicio.h - 1, pozicio.w - 1, pozicio.h - 1, clSilver);

    return 0;
}