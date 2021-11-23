#include "menu.h"
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include "resource.h"

Menu::Menu(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer) :
    TwoLayerDrawer(renderer, pos_on_renderer)
{
    if(!TTF_WasInit()) {
        TTF_Init();
    }

    font_13_rw = SDL_RWFromConstMem(font_bytes, font_bytes_size);
    font_13 = TTF_OpenFontRW(font_13_rw, 0, 13);

    font_13b_rw = SDL_RWFromConstMem(font_bold_bytes, font_bold_bytes_size);
    font_13b = TTF_OpenFontRW(font_13b_rw, 0, 13);

    font_16b_rw = SDL_RWFromConstMem(font_bold_bytes, font_bold_bytes_size);
    font_16b = TTF_OpenFontRW(font_16b_rw, 0, 16);
}

Menu::~Menu()
{
    TTF_CloseFont(font_13);
    TTF_CloseFont(font_13b);
    TTF_CloseFont(font_16b);
    
    SDL_FreeRW(font_13_rw);
    SDL_FreeRW(font_13b_rw);
    SDL_FreeRW(font_16b_rw);
}

void Menu::Frissit(const MenuAllapot& allapot)
{
    this->allapot = allapot;
}

int Menu::DrawBase(SDL_Texture* base)
{
    // Keret
    boxColor(renderer, 0, 0, dest_rect.w - 1, dest_rect.h - 1, clGray);
    lineColor(renderer, 0, 0, 0, dest_rect.h - 1, clWhite);
    lineColor(renderer, 0, 0, dest_rect.w - 1, 0, clWhite);
    lineColor(renderer, dest_rect.w - 1, 0, dest_rect.w - 1, dest_rect.h - 1, clLGray);
    lineColor(renderer, 0, dest_rect.h - 1, dest_rect.w - 1, dest_rect.h - 1, clLGray);

    // Logo
    SDL_RWops* logo_rwops = SDL_RWFromConstMem(logo_bytes, logo_bytes_size);
    SDL_Texture* logo = IMG_LoadTexture_RW(renderer, logo_rwops, 0);
    SDL_RWclose(logo_rwops);

    SDL_Rect logo_poz = {
        .x = 6,
        .y = 6,
    };
    SDL_QueryTexture(logo, NULL, NULL, &logo_poz.w, &logo_poz.h);
    SDL_RenderCopy(renderer, logo, NULL, &logo_poz);
    pixelColor(renderer, 232, 66, clGray); // Volt egy pötty a renderelt képen
    SDL_DestroyTexture(logo);

    // 'Vezérlés' box (8, 68)
    rectangleColor(renderer, 9, 76, 156, 248, clWhite);
    rectangleColor(renderer, 8, 75, 155, 247, clLGray);
    Text( 20,  68, font_13, "Vezérlés", clWhite, clGray);
    Text( 18,  88, font_13b, "Kanyar - Lövés", clWhite);
    Text( 24, 108, font_13, "1     Q        2", clWhite);
    Text( 16, 128, font_13, "Ctrl   Alt       X", clWhite);
    Text( 24, 148, font_13, "4     5        R", clWhite);
    Text( 24, 168, font_13, "M     ,        K", clWhite);
    Text( 20, 188, font_13, "Bal   Le      Fel", clWhite);
    Text( 28, 208, font_13, "/     *        -", clWhite);
    Text( 48, 228, font_13, "Egér", clWhite);

    // 'Játékmód' box (160, 68)
    rectangleColor(renderer, 161, 76, 448, 288, clWhite);
    rectangleColor(renderer, 160, 75, 447, 287, clLGray);
    Text(167,  73, font_13, "Játékmód", clWhite, clGray);
    Text(180, 106, font_13, "a vonalak szaggatottak", clSilver);
    Text(180, 148, font_13, "a vonalak nem szaggatottak", clSilver);
    Text(180, 192, font_13, "a vonalak erősen szaggatottak, de új", clSilver);
    Text(180, 208, font_13, "menetnél megmaradnak", clSilver);
    Text(180, 252, font_13, "keret nincs, a pályán ciklikusan át lehet menni", clSilver);
    Text(180, 268, font_13, "(a golyók nem mozognak ciklikusan)", clSilver);

    // Alsó feliratok
    Text(  7, 299, font_13, "Szabályok: F10        Kilépés: Escape", clWhite);
    Text(328, 296, font_16b, "Indítás: SPACE", clWhite);

    return 0;
}

int Menu::DrawTemp(SDL_Texture* temp)
{
    for(int i = 0; i < Jatekosok; i++) {
        if(allapot.jatekos_aktiv[i]) {
            Text(114, 108 + 20 * i, font_13b, "Aktív", Szinek[i]);
        }
    }

    TTF_Font* mod_fontok[] = {font_13, font_13, font_13, font_13};
    mod_fontok[allapot.jatekmod] = font_13b;

    Text(168,  88, mod_fontok[STANDARD],   "Standard (F1)",       clWhite);
    Text(168, 132, mod_fontok[FOLYTONOS],  "Folytonos mód (F2)",  clWhite);
    Text(168, 176, mod_fontok[OROKLODO],   "Öröklődő mód (F3)",   clWhite);
    Text(168, 236, mod_fontok[FALNELKULI], "Falnélküli mód (F4)", clWhite);
    
    return 0;
}

void Menu::Text(int x, int y, TTF_Font* font, string text, uint32_t fg, uint32_t bg)
{
    SDL_Color color = *(SDL_Color*)&fg;

    SDL_Surface *surface;
    SDL_Texture *texture;
    
    surface = TTF_RenderUTF8_Blended(font, text.data(), color);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect position = {
        .x = x,
        .y = y,
    };
    SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
    
    if(bg != 0) {
        boxColor( // +3px padding
            renderer,
            position.x - 3,
            position.y,
            position.x + position.w + 5,
            position.y + position.h - 1,
            bg
        );
    }

    SDL_RenderCopy(renderer, texture, NULL, &position);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}