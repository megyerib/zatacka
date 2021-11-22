#pragma once
#include <SDL.h>

// Use case:
//
// There is something you need to draw. It has two major parts. One keeps its
// state between redraws, the other not. This base class has two virtual
// functions which you can override. DrawBase() draws the stationery texture
// once, and DrawTemp() draws the temporary texture every time you draw the full
// image.
// 
// The DrawBase() function generates the base texture only at the first draw,
// then it won't be modified again. Of course, child classes can draw on the
// base texture too, its purpose is only to keep its state between drawings.
// 
// When you call the Draw() function it copies the base texture onto the main
// renderer, then creates a brand new temporary texture then calls the
// DrawTemp() function to create the overlay layer from scratch, then copies it
// onto the main renderer as well. Then the temporary layer will be destroyed,
// you can start over the redraw in the next turn.

class TwoLayerDrawer
{
public:
    TwoLayerDrawer(SDL_Renderer* renderer, const SDL_Rect* pos_on_renderer);
    ~TwoLayerDrawer();

    const SDL_Rect dest_rect;

    int Draw();

protected:
    SDL_Renderer* renderer;
    SDL_Texture* base_texture;

    // Draw the base layer which keeps its state between redraws. You don't need
    // this function if your base texture is not stationary.
    virtual int DrawBase(SDL_Texture* base) {return 0;};

    // Draw everything temporary, which you can re-generate in every iteration.
    // If you don't override this function, you probably don't need this class
    // either.
    virtual int DrawTemp(SDL_Texture* temp) = 0;

private:
    bool first_draw = true;
};
