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

    font_pozicio.x = 16;
    font_pozicio.y = pozicio.h - 70;
    font_pozicio.w = 415;
    font_pozicio.h = 52;

    TTF_Init();
    font = TTF_OpenFont(FONT_UTVONAL, 43);
}

void Jatekter::Megjelenit()
{
    SDL_RenderCopy(renderer, texture, NULL, &pozicio);
    TmpMegjelenit();
}

void Jatekter::TmpMegjelenit()
{
    if(uj_kor_szoveg) {
        SDL_Surface *felirat;
        SDL_Texture *felirat_t;

        /* ha sajat kodban hasznalod, csinalj belole fuggvenyt! */
        SDL_Color feher = {0xFF, 0xFF, 0xFF, 0xFF};
        felirat = TTF_RenderUTF8_Blended(font, "Új menethez szóköz...", feher);
        felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
        
        SDL_RenderCopy(renderer, felirat_t, NULL, &font_pozicio);
        SDL_FreeSurface(felirat);
        SDL_DestroyTexture(felirat_t);
    }
}

void Jatekter::Torol()
{
    SDL_SetRenderTarget(renderer, texture);

    boxColor(renderer, 0, 0, pozicio.w - 1, pozicio.h - 1, clBlack);

    SDL_SetRenderTarget(renderer, NULL);
}

void Jatekter::Keret()
{
    SDL_SetRenderTarget(renderer, texture);

    boxColor(renderer, 0, 0, pozicio.w - 1, KeretSzeles - 1, clWhite);
    boxColor(renderer, 0, pozicio.h - KeretSzeles, pozicio.w - 1, pozicio.h - 1, clWhite);
    boxColor(renderer, 0, 0, KeretSzeles - 1, pozicio.h - 1, clWhite);
    boxColor(renderer, pozicio.w - KeretSzeles, 0, pozicio.w - 1, pozicio.h - 1, clWhite);

    SDL_SetRenderTarget(renderer, NULL);
}

void Jatekter::UjKorSzoveg(bool megjelenit)
{
    uj_kor_szoveg = megjelenit;
}
