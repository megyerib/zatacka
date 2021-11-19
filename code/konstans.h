#pragma once

#include <string>
#include <SDL.h>

enum TColor
{
    clRed,
    clYellow,
    clBlue,
    clOlive,
    clLime,
    clMagenta,
    clAqua,
    clBlack,
    clWhite,
    clMaroon,
    clSilver,

    TColor_num
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

// ABGR (little endian)
const uint32_t sdl_szinek[TColor_num] = {
    [clRed]     = 0xFF0000FF,
    [clYellow]  = 0xFF00FFFF,
    [clBlue]    = 0xFFFF0000,
    [clOlive]   = 0xFF008080,
    [clLime]    = 0xFF00FF00,
    [clMagenta] = 0xFFFF00FF,
    [clAqua]    = 0xFFFFFF00,
    [clBlack]   = 0xFF000000,
    [clWhite]   = 0xFFFFFFFF,
    [clMaroon]  = 0xFF000080,
    [clSilver]  = 0xFFA0A0A0,
};

const std::string strSzabalyok =
   "  A vezérlés részben látja a vezérlőgombokat. Nyomja meg valamelyik sor első vezérlőgombját (egérnél bal gomb), így a gombhoz tartozó játékost aktiválja, aki részt vesz majd a játékban. "
   "A játékos kikapcsolásához nyomja meg a játékoshoz tartozó második gombot (egérnél jobb gomb).\r\n\r\n"
   "  A játék indítása után az első és második gombobbal irányíthatja a játékos színével megegyező színű vonalat. A cél, hogy ne ütközzön falba vagy egy másik vonalba. Amikor egy vonal falba ütközik, a még játékban lévő játékosok 2-2 pontot kapnak.\r\n"
   "  A vonalakban peridodikusan lyukak vannak (kivéve egyes módokat), amiken át lehet menni.\r\n\r\n"
   "  Játék közben a harmadik vezérlőgomb (egérnél középső gomb) használatával lehet lőni: ez annyit jelent, hogy a játékos vonalából a haladási iránnyal megegyező irányban egy vörös golyó indul el, utat csinálva ezáltal a játékosnak.\r\n"
   "  Egy játékosnak egyszerre egy golyója lehet a képernyőn. Egy golyó kilövése 1 pontba kerül, és legalább ennyi szükséges a kilövéshez.";