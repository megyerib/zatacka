#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "konstans.h"
#include "two_layer_drawer.h"

class Eredmenyjelzo : public TwoLayerDrawer
{
public:
    Eredmenyjelzo(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer);
    ~Eredmenyjelzo();
    void Beallit(int jatekos, int pont, bool eng);

private:
    SDL_RWops* font_rw;
    TTF_Font* font;

    bool eng[Jatekosok] = {false};
    int pont[Jatekosok] = {0};

    virtual int DrawBase(SDL_Texture* base) override;
    virtual int DrawTemp(SDL_Texture* temp) override;
};
