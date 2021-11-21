#pragma once
#include <SDL.h>
#include "port.h"
#include <SDL_ttf.h>

class Jatekter
{
public:
    Jatekter(SDL_Renderer* renderer);

    void Torol();
    void Keret();
    void Kor(int x, int y, int r, int vastag, TColor szin);
    void Pont(int x, int y, TColor szin);
    uint32_t Szin(int x, int y);
    void Halalfej(int jatekos, bool eng, int x = 0, int y = 0);

    void Megjelenit();
    void TmpMegjelenit();
    void Golyo(int x, int y, uint32_t szin);

    void UjKorSzoveg(bool megjelenit);

    SDL_Rect pozicio; // Pozíció a renderer-en
    SDL_Rect font_pozicio;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* tmp;
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
};
