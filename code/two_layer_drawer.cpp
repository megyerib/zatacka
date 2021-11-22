#include "two_layer_drawer.h"

TwoLayerDrawer::TwoLayerDrawer(
    SDL_Renderer* renderer,
    const SDL_Rect* pos_on_renderer
) : dest_rect(*pos_on_renderer),
    renderer(renderer)
{
    // Generate and draw base texture
    base_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        dest_rect.w,
        dest_rect.h
    );
}

TwoLayerDrawer::~TwoLayerDrawer()
{
    SDL_DestroyTexture(base_texture);
}

int TwoLayerDrawer::Draw()
{
    if(first_draw) {
        SDL_SetRenderTarget(renderer, base_texture);
        DrawBase(base_texture);
        first_draw = false;
    }
    
    // Copy the base texture onto the renderer
    int ret = SDL_SetRenderTarget(renderer, NULL);
    ret = ret || SDL_RenderCopy(renderer, base_texture, NULL, &dest_rect);
    
    // Create overlay texture and set its blend mode
    SDL_Texture* temp_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        dest_rect.w,
        dest_rect.h
    );
    ret = ret || (temp_texture == NULL);
    ret = ret || SDL_SetTextureBlendMode(temp_texture, SDL_BLENDMODE_BLEND);

    ret = ret || SDL_SetRenderTarget(renderer, temp_texture);
    ret = ret || DrawTemp(temp_texture);
    ret = ret || SDL_SetRenderTarget(renderer, NULL);

    ret = ret || SDL_RenderCopy(renderer, temp_texture, NULL, &dest_rect);

    SDL_DestroyTexture(temp_texture);

    return ret;
}
