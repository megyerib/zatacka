#include "zatacka.h"
#include "port.h"
#include "szabalyok.h"

Zatacka::Zatacka(
    SDL_Renderer* renderer, Menu& menu, Jatekter& jt, Eredmenyjelzo& ej
) : renderer(renderer), jatekter(jt), eredmenyjelzo(ej), menu(menu)
{
    FormCreate();
    PaintBoxRajzol();
}

// Ha a játékosnak legalább egy pontja van, kilő egy golyót ("fegyvert ad neki")
// Levon tőle egy pontot
void Zatacka::NewFegyver(int a)
{
    if (!jatekos[a].fegyver && jatekos[a].Pont >= 1 && jatekos[a].Engedett) {
        jatekos[a].Pont--;
        eredmenyjelzo.Beallit(a, jatekos[a].Pont, true);
        eredmenyjelzo.Draw();

        jatekos[a].fegyver = new Fegyver(Round(jatekos[a].HelyX), Round(jatekos[a].HelyY), jatekos[a].Irany, a);
    }
}

// Helyzet, irány, tulaj megadása
// + beregisztrálja a timert, amit én valahogy máshogy fogok megoldani.
Fegyver::Fegyver(int x, int y, int Irany, int Tulaj)
{
    Szog = Irany;
    double offset = (Vastagsag / 2.0) - 0.5; // A pixelpaca közepéről induljon a golyó
    X = x + offset + cos_fok(Szog) * 18;
    Y = y + offset - sin_fok(Szog) * 18;
    Szam = Tulaj;
}

// Lépteti a fegyvert/golyót
void Zatacka::FegyverTimer(Fegyver** fegyver_ptr)
{
    if(fegyver_ptr == nullptr || *fegyver_ptr == nullptr) {
        return;
    }

    Fegyver* fegyver = *fegyver_ptr;

    // Kitöröljük a golyót az előző helyről
    jatekter.Golyo(fegyver->X, fegyver->Y, clBlack);

    // Kiszámoljuk a golyó új helyét
    fegyver->X += cos_fok(fegyver->Szog) * FegyverSebesseg;
    fegyver->Y -= sin_fok(fegyver->Szog) * FegyverSebesseg;

    // Kirajzoljuk a golyót az új helyén
    jatekter.Golyo(fegyver->X, fegyver->Y, Szinek[fegyver->Szam]);

    // Töröljük a golyót, ha kiért a pályáról
    if ((fegyver->X < -10) || (fegyver->Y < -10) || (fegyver->X > KepSzeles + 10) || (fegyver->Y > KepMagas + 10)) {
        delete fegyver;
        *fegyver_ptr = NULL;
    }
}

// Adott x, y ponttól 'Vastagsag' lépésnyire jobbra és lefele
// hány olyan pixel van, ami nem fekete? (szam == játékos száma)
bool Zatacka::Utkozes(int x, int y, int jatekos)
{
    int SajatSzin = 0;
    int NemFekete = 0;

    for (int a = x; a < x + Vastagsag; a++) {
        for (int b = y; b < y + Vastagsag; b++) {
            uint32_t c = jatekter.Szin(a, b);
            if ((a >= KepSzeles) || (b >= KepMagas)) {
                continue; // Ha a képpont kívül esik a tartományon, akkor nem kell vizsgálni.
            }
            if (c == Szinek[jatekos]) {
                SajatSzin++;
            }
            else if (c != clBlack) {
                NemFekete++;
            }
        }
    }

    return (NemFekete > 0) || (SajatSzin > 9) || (AktualisMod.VanKeret && ((x<=0) || (y<=0) || (x>=KepSzeles-Vastagsag) || (y>=KepMagas-Vastagsag)));
}

// A Form1 tárolja az egész játékot, ezért kb. ez a függvény inicializál mindent.
void Zatacka::FormCreate()
{
    KepSzeles = jatekter.dest_rect.w;
    KepMagas = jatekter.dest_rect.h;

    PanelJatszoEmberek = 0; // Összes résztvevő

    for (int x = 0; x < Jatekosok; x++) {
        menu_allapot.jatekos_aktiv[x] = false;
        jatekos[x].HalalfejIdozites = 0;
        jatekter.Halalfej(x, false);
    }

    Randomize(); // Randomgenerátor init

    // Gombok beállítása
    jatekos[0].Gomb[bal]   = SDLK_1;
    jatekos[0].Gomb[jobb]  = SDLK_q;
    jatekos[0].Gomb[loves] = SDLK_2;

    jatekos[1].Gomb[bal]   = SDLK_LCTRL;
    jatekos[1].Gomb[jobb]  = SDLK_LALT;
    jatekos[1].Gomb[loves] = SDLK_x;

    jatekos[2].Gomb[bal]   = SDLK_4;
    jatekos[2].Gomb[jobb]  = SDLK_5;
    jatekos[2].Gomb[loves] = SDLK_r;

    jatekos[3].Gomb[bal]   = SDLK_m;
    jatekos[3].Gomb[jobb]  = SDLK_COMMA;
    jatekos[3].Gomb[loves] = SDLK_k;

    jatekos[4].Gomb[bal]   = SDLK_LEFT;
    jatekos[4].Gomb[jobb]  = SDLK_DOWN;
    jatekos[4].Gomb[loves] = SDLK_UP;

    jatekos[5].Gomb[bal]   = SDLK_KP_DIVIDE;
    jatekos[5].Gomb[jobb]  = SDLK_KP_MULTIPLY;
    jatekos[5].Gomb[loves] = SDLK_KP_MINUS;

    // Kép törlése
    UresImage(true, false);
}

// Újrarajzoljuk a pályát játék elején
void Zatacka::UresImage(bool Torol, bool VanKeret)
{
    if (Torol) {
        jatekter.Torol();
    } else {
        jatekter.Szurkit();
    }

    if (VanKeret) {
        jatekter.Keret();
    }

    PaintBoxRajzol();
}

// Üt a timer, történnek az időfüggő dolgok
void Zatacka::Timer1Timer()
{
    if(jatek_allapot != JATEK) {
        return;
    }
    
    timer_tag = (timer_tag + 1) % 70;

    for (int a = 0; a < Jatekosok; a++) {
        if(jatekos[a].fegyver) { // Akkor is repül tovább a golyó, ha a gazdája már meghalt
            FegyverTimer(&jatekos[a].fegyver);
        }

        if(jatekos[a].HalalfejIdozites > 0) {
            jatekos[a].HalalfejIdozites--;
        } else {
            jatekter.Halalfej(a, false);
        }
    }

    for (int a = 0; a < Jatekosok; a++) {
        // nem aktív játékos átlépése
        if (!jatekos[a].Engedett) {
            continue;
        }

        if (timer_tag >= AktualisMod.SzaggatasTag) {
            int x = Round(jatekos[a].HelyX);
            int y = Round(jatekos[a].HelyY);

            if ((x < 10) || (x > KepSzeles - 10) || (y < 10) || (y > KepMagas - 10)) {
                for (int p = -1; p <= 1; p++) {
                    for (int q = -1; q <= 1; q++) {
                        jatekter.Pont(x + p * KepSzeles, y + q * KepMagas, clBlack);
                    }
                }
            } else {
                jatekter.Pont(x, y, clBlack);
            }
        }

        // Kanyarodunk
        jatekos[a].Irany -= jatekos[a].Kanyar * Fokok;

        if (jatekos[a].Irany < 0) {
            jatekos[a].Irany += 360;
        }
        if (jatekos[a].Irany >= 360) {
            jatekos[a].Irany -= 360;
        }

        // kiszámítjuk az új koordinátákat
        jatekos[a].HelyX += Ut * cos_fok(jatekos[a].Irany);
        jatekos[a].HelyY -= Ut * sin_fok(jatekos[a].Irany);

        // Ha nincs keret, átmegy a túloldalra
        if (!AktualisMod.VanKeret) {
            if (jatekos[a].HelyX >= KepSzeles) {
                jatekos[a].HelyX -= KepSzeles;
            }
            if (jatekos[a].HelyX <= 0) {
                jatekos[a].HelyX += KepSzeles;
            }
            if (jatekos[a].HelyY >= KepMagas) {
                jatekos[a].HelyY -= KepMagas;
            }
            if (jatekos[a].HelyY <= 0) {
                jatekos[a].HelyY += KepMagas;
            }
        }

        // A kerekített koordinátákból pixeleket számolunk
        int x = Round(jatekos[a].HelyX);
        int y = Round(jatekos[a].HelyY);

        // Meghalt a játékos (vonallal, kerettel vagy golyóval ütközött)
        if (Utkozes(x, y, a)) {
            jatekos[a].Engedett = false; // Játékos deaktiválása

            // Halálfej bekapcsolása
            jatekos[a].HalalfejIdozites = 50;
            jatekter.Halalfej(a, true, x, y);

            // a még élő játékosok pontokat kapnak
            int p = 0;
            int q = 0;
            bool logi = true; // Még életben van a játékos, nem lőtte le senki

            for (int b = 0; b < Jatekosok; b++) {
                if (jatekos[b].Engedett) {
                    if (logi) {
                        if (jatekos[b].fegyver) {
                            // gyök 138 ~ 11.7473 -> valakinek a golyója közelében halt meg
                            if (sqr(jatekos[b].fegyver->X - jatekos[a].HelyX) + sqr(jatekos[b].fegyver->Y - jatekos[a].HelyY) < 138)
                            {
                                jatekos[b].Pont += 3; // A lelövő játékos 3 pontot kap
                                logi = false;
                            }
                        }
                    }
                    // ha egy golyó okozta a játékos halálát, akkor a golyó tulajdonosa további pontokat kap
                    // mivel a meghalt játékost már letiltottuk, ezért ha véletlenül a saját golyójától hal meg, nincs pont
                    jatekos[b].Pont += 2;
                    eredmenyjelzo.Beallit(b, jatekos[b].Pont, true);
                    p++;
                }
                q = Max(q, jatekos[b].Pont);
            }

            // senki nincs már életben
            if (p <= 1) {
                if (q < (PanelJatszoEmberek - 1) * 25) {
                    jatekter.UjKorSzoveg(true);
                    jatek_allapot = UJ_KOR;
                } else {
                    PanelLabelHideAll();
                    jatek_allapot = MENU;
                }
                PaintBoxRajzol(); // hogy az utolsó halálfej is kirajzolódjon
                return;
            }
        }

        // Ezt a logikát lehet, hogy át lehetne egy kicsit rendezni, hogy pl. egyszerre is meghalhassanak játékosok

        //kirajzoljuk a vonalat színesen
        // Játéktér szélén vagyunk -> a határra is kerülhet a pixel és "elfeleződhet".
        if ((x < 10) || (x > KepSzeles - 10) || (y < 10) || (y > KepMagas - 10)) { // Itt is lehet egy kicsit finomítani
            for (int p = -1; p <= 1; p++) {
                for (int q = -1; q <= 1; q++) {
                    jatekter.Pont(x + p * KepSzeles, y + q * KepMagas, Szinek[a]);
                }
            }
        } else {
            // Játéktér közepén vagyunk
            jatekter.Pont(x, y, Szinek[a]);
        }
    }

    // Mindent kirajzolunk
    PaintBoxRajzol();
}

// Megnyomtunk egy gombot
void Zatacka::FormKeyDown(SDL_Keycode Key)
{
    // ESC
    // Játék -> Menü -> Kilépés
    if (Key == SDLK_ESCAPE) {
        if (jatek_allapot == MENU) {
            kilepes = true;
        } else {
            jatek_allapot = MENU;
            PanelLabelHideAll();
            jatekter.UjKorSzoveg(true);
            PaintBoxRajzol();
        }
    }

    // F10 -> szabályok
    if (Key == SDLK_F10) {
        JatekAllapot elozo = jatek_allapot;
        jatek_allapot = SZABALYOK;
        Szabalyok szabalyok_ablak;
        jatek_allapot = elozo;
    }

    // Ha a menüben vagyunk, lekezeljük a játékmód változását.
    if (jatek_allapot == MENU) {
        if (Key == SDLK_F1) {
            menu_allapot.jatekmod = STANDARD;
        }
        if (Key == SDLK_F2) {
            menu_allapot.jatekmod = FOLYTONOS;
        }
        if (Key == SDLK_F3) {
            menu_allapot.jatekmod = OROKLODO;
        }
        if (Key == SDLK_F4) {
            menu_allapot.jatekmod = FALNELKULI;
        }

        menu.Frissit(menu_allapot);
        PaintBoxRajzol();
    }

    // Játszunk és a játékosok megnyomták a gombjukat
    if (jatek_allapot == JATEK) {
        for (int a = 0; a < Jatekosok; a++)
        {
            if (Key == jatekos[a].Gomb[bal]) {
                jatekos[a].Kanyar = -1;
            }
            if (Key == jatekos[a].Gomb[jobb]) {
                jatekos[a].Kanyar = 1;
            }
            if (Key == jatekos[a].Gomb[loves]) {
                NewFegyver(a);
            }
        }
    }

    // Játszunk és új kör kezdődne (space-re kezdődik)
    if ((Key == SDLK_SPACE) && (jatek_allapot == UJ_KOR)) {
        for (int a = 0; a < Jatekosok; a++) {
            jatekos[a].Engedett = menu_allapot.jatekos_aktiv[a];
        }
        jatekter.UjKorSzoveg(false);
        jatek_allapot = JATEK;
        UjMenet();
    }

    // A menüben vagyunk
    if (jatek_allapot == MENU) {
        // A játékosok kiválasztják, hogy szeretnének-e játszani
        for (int a = 0; a < Jatekosok; a++) {
            if (Key == jatekos[a].Gomb[bal]) {
                menu_allapot.jatekos_aktiv[a] = true;
            }
            if (Key == jatekos[a].Gomb[jobb]) {
                menu_allapot.jatekos_aktiv[a] = false;
            }
        }
        menu.Frissit(menu_allapot);
        PaintBoxRajzol();
        // ============ ÚJ MENET INDUL, változók beállítása ============
        if (Key == SDLK_SPACE)
        {
            PanelJatszoEmberek = 0;
            for (int a = 0; a < Jatekosok; a++) {
                if (menu_allapot.jatekos_aktiv[a]) {
                    PanelJatszoEmberek++;
                }
            }

            if (PanelJatszoEmberek >= MinimalisJatekos) {
                AktualisMod = JatekMod[menu_allapot.jatekmod];
                UresImage(true, AktualisMod.VanKeret); //új játéknál mindig töröljük a pályát
                for (int a = 0; a < Jatekosok; a++) {
                    jatekos[a].Engedett = menu_allapot.jatekos_aktiv[a];
                    jatekos[a].Pont = AktualisMod.StartPont;
                    eredmenyjelzo.Beallit(a, jatekos[a].Pont, jatekos[a].Engedett);
                    jatekos[a].HalalfejIdozites = 0;
                    jatekter.Halalfej(a, false);
                }
                eredmenyjelzo.Draw();
                UjMenet();
            }
        }
    }
}

// Felengedtük a gombot -> Játék közben visszaállunk egyenesbe
void Zatacka::FormKeyUp(SDL_Keycode Key)
{
    if (jatek_allapot == JATEK) {
        for (int a = 0; a < Jatekosok; a++) {
            if ((Key == jatekos[a].Gomb[bal]) && (jatekos[a].Kanyar == -1)) {
                jatekos[a].Kanyar = 0;
            }
            if ((Key == jatekos[a].Gomb[jobb]) && (jatekos[a].Kanyar == 1)) {
                jatekos[a].Kanyar = 0;
            }
        }
    }
}

// Megnyomjuk az egeret -> Játék vezérlése vagy játékos kiválasztása
void Zatacka::FormMouseDown(uint8_t Button, int32_t X, int32_t Y)
{
    if (jatek_allapot == JATEK) {
        if (Button == SDL_BUTTON_LEFT) {
            jatekos[6].Kanyar = -1;
        } else if (Button == SDL_BUTTON_RIGHT) {
            jatekos[6].Kanyar = 1;
        } else if (Button == SDL_BUTTON_MIDDLE) {
            NewFegyver(6);
        }
    }

    if (jatek_allapot == MENU)
    {
        if (Button == SDL_BUTTON_LEFT) {
            menu_allapot.jatekos_aktiv[6] = true;
        }
        if (Button == SDL_BUTTON_RIGHT) {
            menu_allapot.jatekos_aktiv[6] = false;
        }
        menu.Frissit(menu_allapot);
        PaintBoxRajzol();
    }
}

// Felengedjük az egeret -> Az egeres játékos abbahagyja a kanyarodást
void Zatacka::FormMouseUp(uint8_t Button, int32_t X, int32_t Y)
{
    if (jatek_allapot == JATEK)
    {
        if ((Button == SDL_BUTTON_LEFT) && (jatekos[6].Kanyar == -1)) {
            jatekos[6].Kanyar = 0;
        }
        if ((Button == SDL_BUTTON_RIGHT) && (jatekos[6].Kanyar == 1)) {
            jatekos[6].Kanyar = 0;
        }
    }
}

// Új menet (nem új játék)
void Zatacka::UjMenet()
{
    jatek_allapot = JATEK;
    jatekter.UjKorSzoveg(false);
    jatekter.VerziotMutat(false); // Az első játéknál letiltjuk és többet nem jelenítjük meg.
    
    // Visszaállítjuk a játékosok fegyverét (akkor aktív a fegyver, ha épp van golyó a pályán)
    for (int a = 0; a < Jatekosok; a++) {
        if (jatekos[a].fegyver) {
            jatekter.Golyo(jatekos[a].fegyver->X, jatekos[a].fegyver->Y, clBlack); // Töröljük a golyót az öröklődő mód miatt
            delete jatekos[a].fegyver;
            jatekos[a].fegyver = NULL;
        }

        // Éppen szaggatunk -> töröljük az előző pozíciónkat. (öröklődő mód)
        if (jatekos[a].Engedett && timer_tag >= AktualisMod.SzaggatasTag) {
            int x = Round(jatekos[a].HelyX);
            int y = Round(jatekos[a].HelyY);

            if ((x < 10) || (x > KepSzeles - 10) || (y < 10) || (y > KepMagas - 10)) {
                for (int p = -1; p <= 1; p++) {
                    for (int q = -1; q <= 1; q++) {
                        jatekter.Pont(x + p * KepSzeles, y + q * KepMagas, clBlack);
                    }
                }
            } else {
                jatekter.Pont(x, y, clBlack);
            }
        }
    }

    UresImage(AktualisMod.UjMenetnelTorol, AktualisMod.VanKeret);

    // Lerakjuk a játékosokat
    do {
        for (int a = 0; a < Jatekosok; a++) {
            jatekos[a].Irany = Random(360 / Fokok) * Fokok; // 'Fokok' felbontású legyen az irány (minden játékos pontosan ugyanazon a skálán mozogjon)
            jatekos[a].HelyX = Random(KepSzeles - 240) + 120; // Legalább 12 0pixelre a pálya szélétől
            jatekos[a].HelyY = Random(KepMagas - 240) + 120;
            jatekos[a].Kanyar = 0;
            jatekos[a].HalalfejIdozites = 0;
            jatekter.Halalfej(a, false);
        }
    } while (!JatekosPozicioRendben());

    // Szűkülő kört rajzolunk a játékosok köré.
    // Blokkolunk, mert semmi nem történik még.
    for (int a = 80; a >= 2; a--) { // a = sugár
        for (int b = 0; b < Jatekosok; b++) {
            if (jatekos[b].Engedett) {
                jatekter.Kor(Round(jatekos[b].HelyX), Round(jatekos[b].HelyY), a, 2, Szinek[b]);
            }
        }
        PaintBoxRajzol();
        SDL_Delay(11); // Várunk, amíg az animáció következő lépését kirajzoljuk.
        // Feketével átrajzoljuk az előbb színessel kirajzolt kört, ezzel a vonalakat is letöröljük a játékos körül.
        for (int b = 0; b < Jatekosok; b++) {
            if (jatekos[b].Engedett) {
                jatekter.Kor(Round(jatekos[b].HelyX), Round(jatekos[b].HelyY), a, 4, clBlack);
            }
            if (jatek_allapot == MENU) { // Visszaléptünk a menübe
                // TODO: Valahogyan itt lekezelni a kilépést
                return; //ha ellipszisrajzolás közben nyomtak Escape-t, akkor nem indul a Timer
            }
        }
    }

    timer_tag = 0;

    SDL_FlushEvent(SDL_USEREVENT); // Az eddigi timer leütéseket töröljük
}

// Reseteli a menüt (nem rajzolja ki)
void Zatacka::PanelLabelHideAll()
{
    for (int a = 0; a < Jatekosok; a++) {
        menu_allapot.jatekos_aktiv[a] = false;
    }
    menu_allapot.jatekmod = STANDARD;
    menu.Frissit(menu_allapot);
}

// Akkor van rendben az aktív játékosok helyzete, ha a távolságuk
// egymástól legalább a képernyő szélességének ötöde.
bool Zatacka::JatekosPozicioRendben()
{
    bool Result = true;

    for (int a = 0; a < Jatekosok - 1; a++) {
        for (int b = a + 1; b < Jatekosok; b++) {
            if (jatekos[a].Engedett && jatekos[b].Engedett) {
                if (sqrt(sqr(jatekos[a].HelyX - jatekos[b].HelyX) + sqr(jatekos[a].HelyY - jatekos[b].HelyY)) < KepSzeles / 5) {
                    Result = false;
                }
            }
        }
    }

    return Result;
}

// Az állandó bitképet rárajzoljuk az ideiglenesre, mellé a halálfejeket, majd az egészet a képernyőre.
void Zatacka::PaintBoxRajzol()
{
    switch (jatek_allapot)
    {
    case JATEK:
    case UJ_KOR:
        jatekter.Draw();
        eredmenyjelzo.Draw();
        break;
    
    case MENU:
        jatekter.Draw();
        eredmenyjelzo.Draw();
        menu.Draw();
        break;
    }

    SDL_RenderPresent(renderer);
}
