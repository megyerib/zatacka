#include "szabalyok.h"
#include <SDL.h>

const char strSzabalyok[] =
   "A vezérlés részben látja a vezérlőgombokat. Nyomja meg\n"
   "valamelyik sor első vezérlőgombját (egérnél bal gomb), így a\n"
   "gombhoz tartozó játékost aktiválja, aki részt vesz majd a\n"
   "játékban. A játékos kikapcsolásához nyomja meg a játékoshoz\n"
   "tartozó második gombot (egérnél jobb gomb).\n"
   "\n"
   "A játék indítása után az első és második gombokkal\n"
   "irányíthatja a játékos színével megegyező színű vonalat. A cél,\n"
   "hogy ne ütközzön falba vagy egy másik vonalba. Amikor egy\n"
   "vonal falba ütközik, a még játékban lévő játékosok 2-2 pontot\n"
   "kapnak.\n"
   "A vonalakban peridodikusan lyukak vannak (kivéve egyes\n"
   "módokat), amiken át lehet menni.\n"
   "\n"
   "Játék közben a harmadik vezérlőgomb (egérnél középső\n"
   "gomb) használatával lehet lőni: ez annyit jelent, hogy a\n"
   "játékos vonalából a haladási iránnyal megegyező irányban egy\n"
   "golyó indul el, utat csinálva ezáltal a játékosnak.\n"
   "Egy játékosnak egyszerre egy golyója lehet a képernyőn. Egy\n"
   "golyó kilövése 1 pontba kerül, és legalább ennyi szükséges a\n"
   "kilövéshez.";

Szabalyok::Szabalyok()
{
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_INFORMATION,
        "Játékszabályok",
        strSzabalyok,
        NULL
    );
}
