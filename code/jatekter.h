#pragma once
#include <SDL.h>
#include "port.h"
#include <SDL_ttf.h>
#include "two_layer_drawer.h"
#include "konstans.h"

class Jatekter : public TwoLayerDrawer
{
public:
    Jatekter(SDL_Renderer* renderer, SDL_Rect* pos_on_renderer);

    void Torol();
    void Keret();
    void Kor(int x, int y, int r, int vastag, TColor szin);
    void Pont(int x, int y, TColor szin);
    uint32_t Szin(int x, int y);
    void Halalfej(int jatekos, bool eng, int x = 0, int y = 0);

    void TmpMegjelenit();
    void Golyo(int x, int y, uint32_t szin);

    void UjKorSzoveg(bool megjelenit);

    void Szurkit();

    SDL_Rect font_pozicio;

private:
    TTF_Font* font;
    SDL_Texture* halalfej[Jatekosok];

    bool uj_kor_szoveg = true;

    struct {
        int x;
        int y;
        bool eng;
    } halalfej_adat[Jatekosok];

    int halalfej_w;
    int halalfej_h;

    void HalalfejInit();

    virtual int DrawTemp(SDL_Texture* temp) override;
};
