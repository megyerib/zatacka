#include "jatekter.h"
#include "konstans.h"
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>

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

    if(!TTF_WasInit()) {
        TTF_Init();
    }

    font = TTF_OpenFont(FONT_UTVONAL, 43);

    HalalfejInit();
}

void Jatekter::Megjelenit()
{
    SDL_RenderCopy(renderer, texture, NULL, &pozicio);
    TmpMegjelenit();
}

void Jatekter::TmpMegjelenit()
{
    SDL_Texture* overlay = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        pozicio.w,
        pozicio.h
    );
    SDL_SetTextureBlendMode(overlay, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, overlay);
    
    for(int i = 0; i < Jatekosok; i++) {
        if(halalfej_adat[i].eng) {
            SDL_Rect rect = {
                .x = halalfej_adat[i].x - halalfej_w / 2,
                .y = halalfej_adat[i].y - halalfej_h / 2,
                .w = halalfej_w,
                .h = halalfej_h,
            };
            SDL_RenderCopy(renderer, halalfej[i], NULL, &rect);
        }
    }
    
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

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, overlay, NULL, &pozicio);
    SDL_DestroyTexture(overlay);
}

void Jatekter::Golyo(int x, int y, uint32_t szin)
{
    SDL_SetRenderTarget(renderer, texture);

    if(szin != clBlack) {
        filledCircleColor(renderer, x, y, 9, clMaroon); // Ha nem fekete a golyó, rajzolunk neki barna szegélyt
        filledCircleColor(renderer, x, y, 7, szin);
    } else {
        filledCircleColor(renderer, x, y, 9, clBlack);
    }

    SDL_SetRenderTarget(renderer, NULL);
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

    SDL_SetTextureBlendMode(kor_textura, SDL_BLENDMODE_BLEND);
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

    return data;
}

void Jatekter::Halalfej(int jatekos, bool eng, int x, int y)
{
    halalfej_adat[jatekos].x = x;
    halalfej_adat[jatekos].y = y;
    halalfej_adat[jatekos].eng = eng;
}

void Jatekter::UjKorSzoveg(bool megjelenit)
{
    uj_kor_szoveg = megjelenit;
}

void Jatekter::HalalfejInit()
{
    SDL_Surface* raw = IMG_Load(HALALFEJ_UTVONAL);
    SDL_Surface* alap = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_RGBA32, 0);

    halalfej_w = alap->w;
    halalfej_h = alap->h;

    uint32_t prev_color = clWhite;
    uint32_t* pixels = (uint32_t*)alap->pixels;

    for(int i = 0; i < Jatekosok; i++) {
        for(int p = 0; p < alap->w * alap->h; p++) {
            if(pixels[p] == prev_color) {
                pixels[p] = Szinek[i];
            }
        }
        halalfej[i] = SDL_CreateTextureFromSurface(renderer, alap);
        prev_color = Szinek[i];
    }

    SDL_FreeSurface(raw);
    SDL_FreeSurface(alap);
}

void Jatekter::Szurkit()
{
    // Mondd ki háromszor egymás után, hogy szürke surface
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, pozicio.w, pozicio.h, 0, SDL_PIXELFORMAT_RGBA32);

    SDL_SetRenderTarget(renderer, texture);

    SDL_RenderReadPixels(renderer, &pozicio, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);

    int pixel_num = surface->w * surface->h;
    uint32_t* pixels = (uint32_t*)surface->pixels;

    for(int p = 0; p < pixel_num; p++) {
        if(pixels[p] != clBlack) {
            pixels[p] = clSilver;
        }
    }

    SDL_Texture* szurke = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, szurke, NULL, &pozicio);

    SDL_SetRenderTarget(renderer, NULL);

    SDL_DestroyTexture(szurke);
    SDL_FreeSurface(surface);
}
