#include "eredmenyjelzo.h"
#include <SDL2_gfxPrimitives.h>
#include <cstdio>
#include "resource.h"

Eredmenyjelzo::Eredmenyjelzo(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer) :
    TwoLayerDrawer(renderer, pos_on_renderer)
{
    if(!TTF_WasInit()) {
        TTF_Init();
    }

    font_rw = SDL_RWFromConstMem(font_bytes, font_bytes_size);
    font = TTF_OpenFontRW(font_rw, 0, 54);
}

Eredmenyjelzo::~Eredmenyjelzo()
{
    TTF_CloseFont(font);
    SDL_FreeRW(font_rw);
}

int Eredmenyjelzo::DrawBase(SDL_Texture* base)
{
    boxColor(renderer, 0, 0, dest_rect.w, dest_rect.h, clBlack);
    
    lineColor(renderer, 0, 0, 0, dest_rect.h - 1, clWhite);
    lineColor(renderer, 0, 0, dest_rect.w - 1, 0, clWhite);

    lineColor(renderer, dest_rect.w - 1, 0, dest_rect.w - 1, dest_rect.h - 1, clLGray);
    lineColor(renderer, 0, dest_rect.h - 1, dest_rect.w - 1, dest_rect.h - 1, clLGray);

    return 0;
}

int Eredmenyjelzo::DrawTemp(SDL_Texture* temp)
{
    for(int i = 0; i < Jatekosok; i++) {
        if(!eng[i]) {
            continue;
        }
        
        SDL_Surface *felirat;
        SDL_Texture *felirat_t;

        uint32_t szin32 = Szinek[i];
        SDL_Color szin = *(SDL_Color*)&szin32;

        char pont_str[4];
        snprintf(pont_str, 4, "%d", pont[i]);
        felirat = TTF_RenderUTF8_Blended(font, pont_str, szin);
        felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);

        SDL_Rect font_poz;

        SDL_QueryTexture(felirat_t, NULL, NULL, &font_poz.w, &font_poz.h);

        font_poz.x = EREDMJ_SZ - 15 - font_poz.w; // Jobbra zárt
        font_poz.y = 66 * i + 13;
       
        SDL_RenderCopy(renderer, felirat_t, NULL, &font_poz);
        SDL_FreeSurface(felirat);
        SDL_DestroyTexture(felirat_t);
    }

    return 0;
}

void Eredmenyjelzo::Beallit(int jatekos, int pont, bool eng)
{
    this->eng[jatekos] = eng;
    this->pont[jatekos] = pont;
}
