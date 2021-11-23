#pragma once
#include <SDL.h>
#include "port.h"
#include <SDL_ttf.h>
#include "two_layer_drawer.h"
#include "konstans.h"

struct Halalfej
{
    SDL_Rect poz;
    SDL_Texture* texture;
    bool eng;
};

class Jatekter : public TwoLayerDrawer
{
public:
    Jatekter(SDL_Renderer* renderer, SDL_Rect* pos_on_renderer);
    ~Jatekter();

    // Rajzol
    void Torol();
    void Keret();
    void Kor(int x, int y, int r, int vastag, TColor szin);
    void Pont(int x, int y, TColor szin);
    void Golyo(int x, int y, uint32_t szin);
    void Szurkit();

    // Beállít
    void Halalfej(int jatekos, bool eng, int x = 0, int y = 0);
    void UjKorSzoveg(bool megjelenit);
    void VerziotMutat(bool mutat);

    // Lekérdez
    uint32_t Szin(int x, int y);

private:
    struct Halalfej halalfej[Jatekosok];

    void HalalfejInit();

    bool uj_kor_szoveg = true;
    SDL_Texture* felirat;
    SDL_Rect felirat_poz;

    void FeliratInit();

    virtual int DrawTemp(SDL_Texture* temp) override;

    bool verziot_mutat = true;
};
