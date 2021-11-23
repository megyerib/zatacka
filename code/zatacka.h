#pragma once

#include "konstans.h"
#include "eredmenyjelzo.h"
#include "menu.h"
#include "jatekter.h"

class Fegyver
{
public:
    int Szog;
    int Szam; // a tulajdonos száma
    double X;
    double Y;
    Fegyver(int x, int y, int irany, int Tulaj);
};

struct Jatekos {
   int Gomb[gombSzam]; // A játékos 3 gombjának az azonosítója
   int Irany; // A vonal haladási iránya [fok]
   int Kanyar; // Bal: -1, Egyenes: 0, Jobb: 1
   double HelyX;
   double HelyY;
   bool Engedett; // gyk. Enabled
   int Pont; // Pontszám
   Fegyver* fegyver = nullptr; // == kilőtt golyó
   int HalalfejIdozites; // Ennyi idő múlva tűnik el a halálfej
};

class Zatacka
{
public:
    Zatacka(SDL_Renderer* renderer, Menu& menu, Jatekter& jt, Eredmenyjelzo& ej);

    void Timer1Timer();
    void FormKeyDown(SDL_Keycode Key);
    void FormKeyUp(SDL_Keycode Key);
    void FormMouseDown(uint8_t Button, int32_t X, int32_t Y);
    void FormMouseUp(uint8_t Button, int32_t X, int32_t Y);

    bool kilepes = false;

private:
    SDL_Renderer* renderer;

    Eredmenyjelzo& eredmenyjelzo;
    Menu& menu;
    Jatekter& jatekter;

    JatekAllapot jatek_allapot = MENU;
    MenuAllapot menu_allapot = {
        .jatekos_aktiv = {false},
        .jatekmod = STANDARD,
    };

    int timer_tag;
    Jatekos jatekos[Jatekosok];
    int KepSzeles; // Képernyő szélessége - jobb oldali 120 pixeles eredményjelző
    int KepMagas; // Képernyő magassága
    int PanelJatszoEmberek; // Összes résztvevő (ebből számoljuk ki a nyeréshez szükséges pontszámot stb.)
    TJatekMod AktualisMod;

    void NewFegyver(int a);
    bool Utkozes(int x, int y, int jatekos);
    void FormCreate();
    void UresImage(bool Torol, bool VanKeret);
    void UjMenet();
    void PanelLabelHideAll();
    bool JatekosPozicioRendben();
    void PaintBoxRajzol();
    void FegyverTimer(Fegyver** fegyver);
};
