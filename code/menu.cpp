#include "menu.h"
#include <SDL2_gfxPrimitives.h>

Menu::Menu(SDL_Renderer* renderer)
{
    this->renderer = renderer;

    int renderer_sz;
    int renderer_m;
    SDL_GetRendererOutputSize(renderer, &renderer_sz, &renderer_m);
    
    pozicio.x = 16;
    pozicio.y = 16;
    pozicio.w = 457;
    pozicio.h = 321;

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        pozicio.w,
        pozicio.h
    );
}

int Frissit(const MenuAllapot& allapot)
{
    return 0;
}

int Menu::Ujrarajzol()
{
    SDL_SetRenderTarget(renderer, texture);

    // Keret
    boxColor(renderer, 0, 0, pozicio.w - 1, pozicio.h - 1, clGray);
    lineColor(renderer, 0, 0, 0, pozicio.h - 1, clWhite);
    lineColor(renderer, 0, 0, pozicio.w - 1, 0, clWhite);
    lineColor(renderer, pozicio.w - 1, 0, pozicio.w - 1, pozicio.h - 1, clSilver);
    lineColor(renderer, 0, pozicio.h - 1, pozicio.w - 1, pozicio.h - 1, clSilver);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &pozicio);

    return 0;
}
