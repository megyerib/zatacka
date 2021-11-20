#pragma once
#include <SDL.h>

class Eredmenyjelzo
{
public:
    Eredmenyjelzo(SDL_Renderer* renderer);
    int Kirajzol();
    int Ujrarajzol();
    int Frissit(int jatekos, int pont);

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    SDL_Rect pozicio; // Pozíció a rendereren

    int Keret();
};
