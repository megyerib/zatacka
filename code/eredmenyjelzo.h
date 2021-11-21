#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "konstans.h"

class Eredmenyjelzo
{
public:
    Eredmenyjelzo(SDL_Renderer* renderer);
    void Kirajzol();
    void Beallit(int jatekos, int pont, bool eng);

private:
    SDL_Renderer* renderer;
    SDL_Texture* alap;
    TTF_Font* font;

    SDL_Rect pozicio; // Pozíció a rendereren

    bool eng[Jatekosok] = {false};
    int pont[Jatekosok] = {0};

    void InitAlap();
    void FeliratokatRajzol();
};
