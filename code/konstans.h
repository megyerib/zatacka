#pragma once

#include <string>
#include <SDL.h>

enum TColor
{   // ABGR (little endian)
    clRed     = 0xFF0000FF,
    clYellow  = 0xFF00FFFF,
    clBlue    = 0xFFFF0000,
    clOlive   = 0xFF008080,
    clLime    = 0xFF00FF00,
    clMagenta = 0xFFFF00FF,
    clAqua    = 0xFFFFFF00,
    clBlack   = 0xFF000000,
    clWhite   = 0xFFFFFFFF,
    clMaroon  = 0xFF000080,
    clSilver  = 0xFFC0C0C0,
    clLGray   = 0xFFA0A0A0,
    clGray    = 0xFF202020, // Menü
};

const int EREDMJ_SZ = 120; // Eredményjelző szélessége [px]

const int Jatekosok = 7; // Játékosok max száma
const TColor Szinek[Jatekosok] = {clRed,clYellow,clBlue,clOlive,clLime,clMagenta,clAqua}; // Játékosok színei
const double Ut = 3; // Út/tick (játékos sebessége)
const double FegyverSebesseg = 1.3 * Ut; // Golyó sebessége
const int Fokok = 4; // Ekkora szögben fordul egy játékos
const int Vastagsag = 4; // nem lehet több 10-nél, különben a falnélküli mód hibásan viselkedhet
const int VastagsagDelta = 2; // (Vastagsag-1) div 2
const int MinimalisJatekos = 1;
const int KeretSzeles = 7; // Vastag keret szélessége
const int JATEK_PERIODUS = 24; // [ms]

enum JatekModNev {
    STANDARD = 0,
    FOLYTONOS,
    OROKLODO,
    FALNELKULI,

    JATEKMOD_SZAM
};

struct TJatekMod {
    int SzaggatasTag; // 70 timer tickből mennyi ideig van vonal?
    int StartPont; // Induló pontszám (falnélkülinél kapnak pontot a játékosok)
    bool UjMenetnelTorol;
    bool VanKeret;
    //bool LyukPont;
};

const TJatekMod JatekMod[JATEKMOD_SZAM] = {
    [STANDARD]   = {.SzaggatasTag = 67, .StartPont = 0, .UjMenetnelTorol = true,  .VanKeret = true,  /*.LyukPont = true*/},
    [FOLYTONOS]  = {.SzaggatasTag = 70, .StartPont = 3, .UjMenetnelTorol = true,  .VanKeret = true,  /*.LyukPont = false*/},
    [OROKLODO]   = {.SzaggatasTag = 26, .StartPont = 0, .UjMenetnelTorol = false, .VanKeret = true,  /*.LyukPont = false*/},
    [FALNELKULI] = {.SzaggatasTag = 66, .StartPont = 0, .UjMenetnelTorol = true,  .VanKeret = false, /*.LyukPont = true*/}
};

enum JatekAllapot
{
    MENU,
    JATEK,
    UJ_KOR,
    VEGE,
    SZABALYOK, // Szabályok ablak megjelenítése
};

struct MenuAllapot
{
    bool jatekos_aktiv[Jatekosok];
    JatekModNev jatekmod;
};

static char FONT_UTVONAL[] = "res/font_hun.ttf";
static char HALALFEJ_UTVONAL[] = "res/death.png";
