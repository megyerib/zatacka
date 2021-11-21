#include "eredmenyjelzo.h"
#include <SDL2_gfxPrimitives.h>
#include "konstans.h"
#include <cstdio>

const int JOBB_MARGO = 18;

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

    alap = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        pozicio.w,
        pozicio.h
    );

    InitAlap();

    if(!TTF_WasInit()) {
        TTF_Init();
    }

    font = TTF_OpenFont(FONT_UTVONAL, 40); // TODO: A valóságban miért nem ekkora?
}

void Eredmenyjelzo::Kirajzol()
{
    SDL_RenderCopy(renderer, alap, NULL, &pozicio);
    FeliratokatRajzol();
}

void Eredmenyjelzo::InitAlap()
{
    SDL_SetRenderTarget(renderer, alap);

    boxColor(renderer, 0, 0, pozicio.w, pozicio.h, clBlack);
    
    lineColor(renderer, 0, 0, 0, pozicio.h - 1, clWhite);
    lineColor(renderer, 0, 0, pozicio.w - 1, 0, clWhite);

    lineColor(renderer, pozicio.w - 1, 0, pozicio.w - 1, pozicio.h - 1, clSilver);
    lineColor(renderer, 0, pozicio.h - 1, pozicio.w - 1, pozicio.h - 1, clSilver);

    SDL_SetRenderTarget(renderer, NULL);
}

/*
PontLabel[x].Caption = '0';
PontLabel[x].Left = 75;
PontLabel[x].Top = 66 * x - 50;
PontLabel[x].Font.Size = 40;
PontLabel[x].AutoSize = true;
PontLabel[x].Alignment = taRightJustify;
PontLabel[x].Font.Color = Szinek[x];
PontLabel[x].Transparent = true;
*/

// TODO: Úgy rajzolódjon ki, mint az eredeti
void Eredmenyjelzo::FeliratokatRajzol()
{
    SDL_Texture* overlay = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        pozicio.w,
        pozicio.h
    );

    SDL_SetRenderTarget(renderer, overlay);
    
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

        int text_w;
        int text_h;

        SDL_QueryTexture(felirat_t, NULL, NULL, &text_w, &text_h);

        SDL_Rect font_poz;
        font_poz.w = text_w * 60 / text_h;
        font_poz.h = 60; // Valamiért így lesz 40 pixel magas.
        font_poz.x = EREDMJ_SZ - JOBB_MARGO - font_poz.w; // Jobbra zárt
        font_poz.y = 66 * i + 50;
       
        SDL_RenderCopy(renderer, felirat_t, NULL, &font_poz);
        SDL_FreeSurface(felirat);
        SDL_DestroyTexture(felirat_t);
    }

    SDL_SetTextureBlendMode(overlay, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, overlay, NULL, &pozicio);
    SDL_DestroyTexture(overlay);
}

void Eredmenyjelzo::Beallit(int jatekos, int pont, bool eng)
{
    this->eng[jatekos] = eng;
    this->pont[jatekos] = pont;
}
