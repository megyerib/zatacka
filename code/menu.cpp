#include "menu.h"
#include <SDL2_gfxPrimitives.h>

Menu::Menu(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer) :
    TwoLayerDrawer(renderer, pos_on_renderer)
{

}

int Frissit(const MenuAllapot& allapot)
{
    return 0;
}

int Menu::DrawBase(SDL_Texture* base)
{
    // Keret
    boxColor(renderer, 0, 0, dest_rect.w - 1, dest_rect.h - 1, clGray);
    lineColor(renderer, 0, 0, 0, dest_rect.h - 1, clWhite);
    lineColor(renderer, 0, 0, dest_rect.w - 1, 0, clWhite);
    lineColor(renderer, dest_rect.w - 1, 0, dest_rect.w - 1, dest_rect.h - 1, clLGray);
    lineColor(renderer, 0, dest_rect.h - 1, dest_rect.w - 1, dest_rect.h - 1, clLGray);

    return 0;
}

int Menu::DrawTemp(SDL_Texture* temp)
{
    return 0;
}
