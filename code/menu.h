#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "konstans.h"
#include "two_layer_drawer.h"

using std::string;

class Menu : public TwoLayerDrawer
{
public:
    Menu(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer);
    void Frissit(const MenuAllapot& allapot);

private:
    virtual int DrawBase(SDL_Texture* base) override;
    virtual int DrawTemp(SDL_Texture* temp) override;
    void Text(int x, int y, TTF_Font* font, string text, uint32_t fg, uint32_t bg = 0);

    MenuAllapot allapot = {0};

    TTF_Font* font_13pt;
    TTF_Font* font_13pt_bold;
    TTF_Font* font_16pt_bold;
};
