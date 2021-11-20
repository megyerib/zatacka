#pragma once
#include <SDL.h>
#include "port.h"
#include <SDL_ttf.h>

class Jatekter
{
public:
    Jatekter(SDL_Renderer* renderer);

    void Torol();
    void Keret();
    void Kor(int x, int y, int r, int vastag, TColor szin);

    void Megjelenit();
    void TmpMegjelenit();

    void UjKorSzoveg(bool megjelenit);

    SDL_Rect pozicio; // Pozíció a renderer-en
    SDL_Rect font_pozicio;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* tmp;
    TTF_Font* font;

    bool uj_kor_szoveg = true;
};
