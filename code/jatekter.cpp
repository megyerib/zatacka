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
        SDL_PIXELFORMAT_RGBA32,
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

void Jatekter::Kor(int x, int y, int r, int vastag, TColor szin)
{
    int r_k = r + (vastag / 2);
    int r_b = r_k - vastag;
    r_b = (r_b) > 0 ? r_b : 0;

    SDL_Rect dst = {
        .x = x - r_k,
        .y = y - r_k,
        .w = 2 * r_k + 1,
        .h = 2 * r_k + 1,
    };
    
    SDL_Texture* kor_textura = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        dst.w,
        dst.h
    );

    SDL_SetRenderTarget(renderer, kor_textura);

    filledCircleColor(renderer, r_k, r_k, r_k, szin);
    filledCircleColor(renderer, r_k, r_k, r_b, clBlack); // TODO: legyen átlátszó

    SDL_SetRenderTarget(renderer, texture);

    SDL_RenderCopy(renderer, kor_textura, NULL, &dst);

    SDL_SetRenderTarget(renderer, NULL);

    SDL_DestroyTexture(kor_textura);
}

void Jatekter::Pont(int x, int y, TColor szin)
{
    SDL_SetRenderTarget(renderer, texture);

    boxColor(renderer, x, y, x + Vastagsag - 1, y + Vastagsag - 1, szin);

    SDL_SetRenderTarget(renderer, NULL);
}

// TODO: Ezt SDL-ben nem igazán így kell csinálni
uint32_t Jatekter::Szin(int x, int y)
{
    SDL_Rect px = {
        .x = x,
        .y = y,
        .w = 1,
        .h = 1
    };

    uint32_t data;
    int pitch = pozicio.w * 4;
    
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderReadPixels(renderer, &px, SDL_PIXELFORMAT_RGBA32, &data, pitch);
    SDL_SetRenderTarget(renderer, NULL);

    return data | 0xFF000000; // Csalunk egy kicsit az alfával
}

void Jatekter::UjKorSzoveg(bool megjelenit)
{
    uj_kor_szoveg = megjelenit;
}
