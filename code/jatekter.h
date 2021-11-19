#pragma once
#include <SDL.h>
#include "port.h"

class Jatekter
{
public:
    Jatekter(SDL_Renderer* renderer);

    int Torol();
    int Keret();

    int Megjelenit();

    SDL_Rect pozicio; // Pozíció a renderer-en

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
