#include "eredmenyjelzo.h"
#include <SDL2_gfxPrimitives.h>
#include "log.h"
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

    Ujrarajzol();
}

int Eredmenyjelzo::Kirajzol()
{
    return Ujrarajzol(); // Majd később lehet trükközni
}

int Eredmenyjelzo::Ujrarajzol()
{
    int error = 0;
    
    error = error || SDL_SetRenderTarget(renderer, texture) SDL_LOG_ERROR;

    error = error || Keret();

    error = error || SDL_SetRenderTarget(renderer, NULL) SDL_LOG_ERROR;
    error = error || SDL_RenderCopy(renderer, texture, NULL, &pozicio) SDL_LOG_ERROR;

    return error;
}

int Eredmenyjelzo::Keret()
{
    int error = 0;

    error = error || lineColor(renderer, 0, 0, 0, pozicio.h - 1, clWhite) SDL_LOG_ERROR;
    error = error || lineColor(renderer, 0, 0, pozicio.w - 1, 0, clWhite) SDL_LOG_ERROR;

    error = error || lineColor(renderer, pozicio.w - 1, 0, pozicio.w - 1, pozicio.h - 1, clSilver) SDL_LOG_ERROR;
    error = error || lineColor(renderer, 0, pozicio.h - 1, pozicio.w - 1, pozicio.h - 1, clSilver) SDL_LOG_ERROR;

    return error;
}