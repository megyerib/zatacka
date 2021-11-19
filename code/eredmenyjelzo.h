#include <SDL.h>

class Eredmenyjelzo
{
public:
    Eredmenyjelzo(SDL_Renderer* renderer);
    int Kirajzol();
    int Ujrarajzol();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    SDL_Rect pozicio; // Pozíció a rendereren

    int Keret();
};
