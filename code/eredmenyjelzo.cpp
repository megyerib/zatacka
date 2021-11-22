#include "eredmenyjelzo.h"
#include <SDL2_gfxPrimitives.h>
#include "konstans.h"
#include <cstdio>

const int JOBB_MARGO = 18;

Eredmenyjelzo::Eredmenyjelzo(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer) :
    TwoLayerDrawer(renderer, pos_on_renderer)
{
    if(!TTF_WasInit()) {
        TTF_Init();
    }

    font = TTF_OpenFont(FONT_UTVONAL, 40); // TODO: A valóságban miért nem ekkora?
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

    return 0;
}

void Eredmenyjelzo::Beallit(int jatekos, int pont, bool eng)
{
    this->eng[jatekos] = eng;
    this->pont[jatekos] = pont;
}
