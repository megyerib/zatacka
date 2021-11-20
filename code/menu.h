#pragma once
#include <SDL.h>
#include "konstans.h"

class Menu
{
public:
    Menu(SDL_Renderer* renderer);
    int Frissit(const MenuAllapot& allapot);
    int Ujrarajzol();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    SDL_Rect pozicio; // Pozíció a rendereren
};
