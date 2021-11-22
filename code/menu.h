#pragma once
#include <SDL.h>
#include "konstans.h"
#include "two_layer_drawer.h"

class Menu : public TwoLayerDrawer
{
public:
    Menu(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer);
    int Frissit(const MenuAllapot& allapot);

private:
    virtual int DrawBase(SDL_Texture* base) override;
    virtual int DrawTemp(SDL_Texture* temp) override;
};
