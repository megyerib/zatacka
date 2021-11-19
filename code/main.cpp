#include "port.h"
#include "gomb.h"
#include <vector>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <cstdio>
#include "eredmenyjelzo.h"
#include "jatekter.h"

using namespace std;

// type

// Halálfej adatai (nem a bitmap)
struct THalalfej {
    TPoint Coord_TopLeft;
    int Idozites; // Ennyi idő múlva tűnik el a halálfej
};

struct TKettoSzin {
    int SajatSzin;
    int NemFekete;
};

class TFegyver : public TTriggerable
{
public:
    TTimer Timer;
    int Szog;
    int Szam; // a játékos száma
    double X;
    double Y;
    TFegyver(int x, int y, int irany, int Tulaj);
    void OnTimer(TObject Sender) override;
    void Destroy();
};

// Vektor típus a lyukakon átbújáshoz (nem használt)
/*struct TVektor {
   TPoint Pont;
   int Szog;
   bool Hasznalt;
};*/

struct TJatekos {
   int Gomb[gombSzam]; // A játékos 3 gombjának az azonosítója
   int Irany; // A vonal haladási iránya [fok]
   int Kanyar; // Bal: -1, Egyenes: 0, Jobb: 1
   double HelyX;
   double HelyY;
   bool Engedett; // gyk. Enabled
   bool FegyverAktiv; // akkor aktív, ha éppen van golyó a pályán
   int Pont; // Pontszám
   TFegyver* Fegyver; // == kilőtt golyó
   THalalfej Halalfej; // Halálfej adatai (nem a bitmap)
   TBitmap* bmpVonal; // Vastagsag * Vastagsag méretű négyzet. Ezt pötyögtetjük rá később a BitKep-re, amikor haladunk.
   TBitmap* bmpHalalfej; // Színes halálfej. Ezt a bitmapet rajzoljuk ki, amikor a játékos meghalt.
   TBitmap* PuffBitmap; // erre van kirajzolva a golyó
};

class TForm1 : public TForm
{
    // Ez a kezdeti méret lenne?
    // ClientHeight = 534
    // ClientWidth = 790
    // Úgy látom, a játéktér skálázható (a program indításakor)
    // A menü box a bal felső sarokhoz, a ponttábla a jobb oldalhoz rögzül.
public:
    TPanel Panel1; // Pontszám panel
    TImageList ImageList1; // Halálfej kép
    TPanel Panel2;       // Menü panel
    TLabel Label1;       // '1     Q        2'
    TLabel Label2;       // 'Ctrl   Alt       X'
    TLabel Label3;       // '4     5        R'
    TLabel Label4;       // 'M     ,        K'
    TLabel Label5;       // 'Bal   Le      Fel'
    TLabel Label6;       // '/     *        -'
    TLabel Label7;       // 'Egér'
    TLabel Label10;      // 'Indítás: SPACE'
    TLabel Label11;      // 'Szabályok: F10        Kilépés: Escape'
    TImage Image2;       // Zatacka logo
    TPaintBox PaintBox2; // Erre rajzoljuk a játékteret
    TGroupBox GroupBox2; // ' Vezérlés '
    TGroupBox GroupMod;  // ' Játékmód '
    TLabel LabelMod1;    // 'Standard (F1)'
    TLabel Label9;       // 'a vonalak szaggatottak'
    TLabel LabelMod2;    // 'Folytonos mód (F2)'
    TLabel Label13;      // 'a vonalak nem szaggatottak'
    TLabel LabelMod3;    // 'Öröklődő mód (F3)'
    TLabel Label15;      // 'a vonalak erősen szaggatottak, de új menetnél megmaradnak'
    TLabel LabelMod4;    // 'Falnélküli mód (F4)'
    TLabel Label8;       // 'keret nincs, a pályán ciklikusan át lehet menni (a golyók nem mozognak ciklikusan)'
    TLabel Label12;      // 'Kanyar - Lövés'
    TLabel NewRaceLabel; // 'Új menethez szóköz...'

    void NewFegyver(int a);
    static TKettoSzin SzineketSzamol(int x, int y, int szam);
    void FormCreate(TObject Sender);
    void UresImage(bool Torol, bool VanKeret);
    void Timer1Timer(TObject Sender);
    void FormKeyDown(TObject Sender, int Key, TShiftState Shift);
    void FormKeyUp(TObject Sender, int Key, TShiftState Shift);
    void FormMouseDown(TObject Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void FormMouseUp(TObject Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void FormClose(TObject Sender, TCloseAction Action);
    void UjMenet();
    void PanelLabelHideAll();
    static bool JatekosPozicioRendben();
    void JatekosokatLerak();
    void PaintBoxRajzol();
    void PaintBoxOnPaint(TObject Sender);
    
    TForm1(SDL_Renderer* renderer);

    Jatekter jatekter;
};

TForm1::TForm1(SDL_Renderer* renderer) : jatekter(renderer)
{

}

// var

TTimer Timer1; // Ez adja a ticket a játékhoz, általában ezt nézegetjük, ha arra vagyunk kíváncsiak, hogy játszunk-e. (Interval: 24 [ms?])
TJatekos Jatekos[Jatekosok];
int KepSzeles; // Képernyő szélessége - jobb oldali 120 pixeles eredményjelző
int KepMagas; // Képernyő magassága
int PanelJatszoEmberek; // Összes résztvevő (ebből számoljuk ki a nyeréshez szükséges pontszámot stb.)
TLabel* PontLabel[Jatekosok]; // Ez mutatja jobb oldalon a pontszámot
TLabel* PanelLabel[Jatekosok]; // Ez mutatja a menüben, hogy aktív-e egy játékos
TBitmap* BitKep; // Alap bitkép
TBitmap* BitKep2; // Ideiglenes bitkép, kb. BitKep + halálfejek, ezt rajzoljuk a képernyőre.
TBitmap* bmpVonalFekete;
vector<TPoint> arrSzurkePixelek; // Falnélküli módban itt tároljuk a bejárt pixeleket. Csak falnélküli módban írunk bele.
//vector<TVektor> arrLyukak;
int Lyukak_SzaggatasFele; // a szaggatás középpontjának a Timer.Tag-je
TJatekMod AktualisMod;
TBitmap* PuffBitmap0; // Fekete kör, amivel ki lehet törölni az előző golyót.

// implementation

// Ha a játékosnak legalább egy pontja van, kilő egy golyót ("fegyvert ad neki")
// Levon tőle egy pontot
void TForm1::NewFegyver(int a)
{
    if (!Jatekos[a].FegyverAktiv && Jatekos[a].Pont >= 1 && Jatekos[a].Engedett) {
        Jatekos[a].Pont--;
        PontLabel[a]->Caption = IntToStr(Jatekos[a].Pont);
        Jatekos[a].Fegyver = new TFegyver(Round(Jatekos[a].HelyX), Round(Jatekos[a].HelyY), Jatekos[a].Irany, a);
    }
}

// Helyzet, irány, tulaj megadása
// + beregisztrálja a timert, amit én valahogy máshogy fogok megoldani.
TFegyver::TFegyver(int x, int y, int Irany, int Tulaj)
{
    Szog = Irany;
    X = x + cos_fok(Szog) * 18;
    Y = y - sin_fok(Szog) * 18;
    Szam = Tulaj;
    Jatekos[Szam].FegyverAktiv = true;

    Timer.Interval = Timer1.Interval;
    Timer.OnTimer = &TTriggerable::OnTimer;
}

// Lépteti a fegyvert/golyót
// A golyó akkor is létezhet, ha a tulajdonosa már meghalt, ezért érdemes külön tárolni.
void TFegyver::OnTimer(TObject Sender)
{
    BitKep->Canvas.Draw(Round(X) - 9, Round(Y) - 9, PuffBitmap0); // kitöröljük a golyót az előző helyről
    if (!Timer1.Enabled) { // Ha nem játszunk, a golyót sem léptetjük tovább
        Timer.Enabled = false;
        return;
    }
    // Kiszámoljuk a golyó új helyét
    X += cos_fok(Szog) * FegyverSebesseg;
    Y -= sin_fok(Szog) * FegyverSebesseg;
    Timer.Tag++;
    // Kirajzoljuk a golyót az új helyén
    BitKep->Canvas.Draw(Round(X) - 9, Round(Y) - 9, Jatekos[Szam].PuffBitmap);
    if ((X < -10) || (Y < -10) || (X > KepSzeles + 10) || (Y > KepMagas + 10)) {
        Destroy();
    }
}

// Letiltja a golyót, törli a timerét
void TFegyver::Destroy()
{
    Timer.Enabled = false;
    Timer.Free();
    Jatekos[Szam].FegyverAktiv = false;
}

// Adott x, y ponttól 'Vastagsag' lépésnyire jobbra és lefele
// hány olyan pixel van, ami nem fekete? (szam == játékos száma)
TKettoSzin TForm1::SzineketSzamol(int x, int y, int szam)
{
    TKettoSzin e;

    TColor c2 = Szinek[szam];
    e.SajatSzin = 0;
    e.NemFekete = 0;

    for (int a = x; a < x + Vastagsag; a++) {
        for (int b = y; b < y + Vastagsag; b++) {
            TColor c = BitKep->Canvas.Pixels[a][b];
            if ((a >= KepSzeles) || (b >= KepMagas)) {
                continue; // Ha a képpont kívül esik a tartományon, akkor nem kell vizsgálni.
            }
            if (c == c2) {
                e.SajatSzin++;
            }
            else if (c != TColor::clBlack) {
                e.NemFekete++;
            }
        }
    }

    return e;
}

// A Form1 tárolja az egész játékot, ezért kb. ez a függvény inicializál mindent.
void TForm1::FormCreate(TObject Sender)
{
    KepSzeles = jatekter.pozicio.w;
    KepMagas = jatekter.pozicio.h;

    PanelJatszoEmberek = 0; // Összes résztvevő

    for (int x = 0; x < Jatekosok; x++) {
        // Pontszám labelek előkészítése
        //PontLabel[x] = new TLabel(Form1);

        PontLabel[x]->Parent = Panel1;
        PontLabel[x]->Caption = '0';
        PontLabel[x]->Left = 75;
        PontLabel[x]->Top = 66 * x - 50;
        PontLabel[x]->Font.Size = 40;
        PontLabel[x]->AutoSize = true;
        PontLabel[x]->Alignment = taRightJustify;
        PontLabel[x]->Font.Color = Szinek[x];
        PontLabel[x]->Transparent = true;
        PontLabel[x]->Hide();
        PontLabel[x]->OnMouseDown = &TForm::FormMouseDown;
        PontLabel[x]->OnMouseUp = &TForm::FormMouseUp;

        // 'Aktív' labelek előkészítése a menüben
        //PanelLabel[x] = new TLabel(Form1);

        PanelLabel[x]->Parent = GroupBox2;
        PanelLabel[x]->Caption = "Aktív";
        PanelLabel[x]->Left = 106;
        PanelLabel[x]->Top = 20 * (x + 1);
        PanelLabel[x]->Font.Style = fsBold;
        PanelLabel[x]->AutoSize = true;
        PanelLabel[x]->Font.Color = Szinek[x];
        PanelLabel[x]->Hide();
        PanelLabel[x]->OnMouseDown = &TForm::FormMouseDown;
        PanelLabel[x]->OnMouseUp = &TForm::FormMouseUp;

        // Csinálunk egy Vastagsag * Vastagsag méretű négyzetet.
        // Ezt pötyögtetjük rá később a BitKep-re, amikor haladunk.
        Jatekos[x].bmpVonal = new TBitmap();

        Jatekos[x].bmpVonal->Width = Vastagsag;
        Jatekos[x].bmpVonal->Height = Vastagsag;
        Jatekos[x].bmpVonal->Canvas.Pen.Color = Szinek[x];
        Jatekos[x].bmpVonal->Canvas.Brush.Color = Szinek[x];
        Jatekos[x].bmpVonal->Canvas.Rectangle(0, 0, Vastagsag, Vastagsag);

        // Megcsináljuk a színes halálfejét minden egyes játékosnak.
        Jatekos[x].bmpHalalfej = new TBitmap();

        Jatekos[x].bmpHalalfej->Width = 50;
        Jatekos[x].bmpHalalfej->Height = 48;
        ImageList1.GetBitmap(0, Jatekos[x].bmpHalalfej);
        Jatekos[x].bmpHalalfej->Transparent = true;

        for (int a = 0; a < Jatekos[x].bmpHalalfej->Width; a++) {
            for (int b = 0; b < Jatekos[x].bmpHalalfej->Height; b++) {
                if (Jatekos[x].bmpHalalfej->Canvas.Pixels[a][b] == clWhite) {
                    Jatekos[x].bmpHalalfej->Canvas.Pixels[a][b] = Szinek[x];
                }
            }
        }

        Jatekos[x].Halalfej.Idozites = 0;
    }

    // Ugyanaz, mint a játékosoknál a bmpVonal, csak ezt akkor rajzoljuk, amikor szaggatás van.
    bmpVonalFekete = new TBitmap();

    bmpVonalFekete->Width = Vastagsag;
    bmpVonalFekete->Height = Vastagsag;
    bmpVonalFekete->Canvas.Pen.Color = clBlack;
    bmpVonalFekete->Canvas.Brush.Color =clBlack;
    bmpVonalFekete->Canvas.Rectangle(0, 0, Vastagsag, Vastagsag);

    //létrehozzuk a fő Bitmap-eket
    //BitKep: Ezen van a játék
    //BitKep2: Ezen vannak a halálfejek (ideiglenes)
    BitKep = new TBitmap();

    BitKep->Width = KepSzeles;
    BitKep->Height = KepMagas;
    BitKep->PixelFormat = pf8bit;

    BitKep2 = new TBitmap();
    BitKep2->Width = KepSzeles;
    BitKep2->Height = KepMagas;

    //létrehozzuk a golyók Bitmap-jét
    //PuffBitmap0: fekete golyó
    PuffBitmap0 = new TBitmap(); // Ez a fekete golyóé
    PuffBitmap0->Width = 19;
    PuffBitmap0->Height = 19;
    PuffBitmap0->Canvas.Brush.Color = clWhite;
    PuffBitmap0->Canvas.Pen.Color = clWhite;
    PuffBitmap0->Canvas.Rectangle(0, 0, 19, 19);
    PuffBitmap0->Canvas.Pen.Color = clBlack;
    PuffBitmap0->Canvas.Brush.Color = clBlack;
    PuffBitmap0->Canvas.Pen.Width = 2;
    PuffBitmap0->Canvas.Ellipse(0,0,19,19);
    PuffBitmap0->Transparent = true; // A sarokban lévő szín a transparent vagy mi?

    for (int x = 0; x < Jatekosok; x++) { // Ez a játékosok színes golyóié, ezeket fogjuk kirajzolni, ha lőnek.
        Jatekos[x].PuffBitmap = new TBitmap();

        Jatekos[x].PuffBitmap->Width = 19;
        Jatekos[x].PuffBitmap->Height = 19;
        Jatekos[x].PuffBitmap->Canvas.Brush.Color = clBlack;
        Jatekos[x].PuffBitmap->Canvas.Rectangle(0, 0, 19, 19);
        Jatekos[x].PuffBitmap->Canvas.Pen.Color = clMaroon;
        Jatekos[x].PuffBitmap->Canvas.Brush.Color = Szinek[x];
        Jatekos[x].PuffBitmap->Canvas.Pen.Width = 2;
        Jatekos[x].PuffBitmap->Canvas.Ellipse(0, 0, 19, 19);
        Jatekos[x].PuffBitmap->Transparent = true;
    }

    Randomize(); // Randomgenerátor init

    // Gombok beállítása
    Jatekos[0].Gomb[bal] = Ord('1');
    Jatekos[0].Gomb[jobb] = Ord('Q');
    Jatekos[0].Gomb[loves] = Ord('2');

    Jatekos[1].Gomb[bal] = 17;
    Jatekos[1].Gomb[jobb] = 18;
    Jatekos[1].Gomb[loves] = Ord('X');

    Jatekos[2].Gomb[bal] = Ord('4');
    Jatekos[2].Gomb[jobb] = Ord('5');
    Jatekos[2].Gomb[loves] = Ord('R');

    Jatekos[3].Gomb[bal] = Ord('M');
    Jatekos[3].Gomb[jobb] = 188;
    Jatekos[3].Gomb[loves] = Ord('K');

    Jatekos[4].Gomb[bal] = VK_LEFT;
    Jatekos[4].Gomb[jobb] = VK_DOWN;
    Jatekos[4].Gomb[loves] = VK_UP;

    Jatekos[5].Gomb[bal] = VK_DIVIDE;
    Jatekos[5].Gomb[jobb] = VK_MULTIPLY;
    Jatekos[5].Gomb[loves] = VK_SUBTRACT;

    // Kép törlése
    UresImage(true, AktualisMod.VanKeret);
}

// Újrarajzoljuk a pályát játék elején
void TForm1::UresImage(bool Torol, bool VanKeret)
{
    if (Torol) { // Ilyenkor keretet sem rajzolunk
        jatekter.Torol();
    } else {
        // az arrSzurkePixelek tömbben tárolt koordinátákat átszínezzük
        int l = Length(arrSzurkePixelek); // (ez a tömb csak falnélküli módban tárol elemeket)

        for (int a = l - 1; a >= 0; a--) {
            TPoint p = arrSzurkePixelek[a];
            for (int x = p.X + Vastagsag - 1; x >= p.X; x--) {
                for (int y = p.Y + Vastagsag - 1; x >= p.Y; y--) {
                    if (BitKep->Canvas.Pixels[x][y] != clBlack) {
                        BitKep->Canvas.Pixels[x][y] = clSilver;
                    }
                }
            }
            SetLength(arrSzurkePixelek, 0);
            //SetLength(arrLyukak, 0);
        }

        // újrarajzoljuk a keretet
        if (VanKeret) {
            jatekter.Keret();
        }
    }

    PaintBoxRajzol(); // Kirajzoltatjuk a bitmapeket
}

// Üt a timer, történnek az időfüggő dolgok
void TForm1::Timer1Timer(TObject Sender)
{
    Timer1.Tag = (Timer1.Tag + 1) % 70;

    for (int a = 0; a < Jatekosok; a++) {
        // nem aktív játékos átlépése
        if (!Jatekos[a].Engedett) {
            continue;
        }

        if (Timer1.Tag >= AktualisMod.SzaggatasTag) {
            int x = Round(Jatekos[a].HelyX);
            int y = Round(Jatekos[a].HelyY);

            if ((x < 10) || (x > KepSzeles - 10) || (y < 10) || (y > KepMagas - 10)) {
                for (int p = -1; p <= 1; p++) {
                    for (int q = -1; q <= 1; q++) {
                        BitKep->Canvas.Draw(x + p * KepSzeles, y + q * KepMagas, bmpVonalFekete);
                    }
                }
            } else {
                BitKep->Canvas.Draw(x, y, bmpVonalFekete);
            }
        }

        // Kanyarodunk
        Jatekos[a].Irany -= Jatekos[a].Kanyar * Fokok;

        if (Jatekos[a].Irany < 0) {
            Jatekos[a].Irany += 360;
        }
        if (Jatekos[a].Irany >= 360) {
            Jatekos[a].Irany -= 360;
        }

        // kiszámítjuk az új koordinátákat
        Jatekos[a].HelyX += Ut * cos_fok(Jatekos[a].Irany);
        Jatekos[a].HelyY -= Ut * sin_fok(Jatekos[a].Irany);

        // Ha nincs keret, átmegy a túloldalra
        if (!AktualisMod.VanKeret) {
            if (Jatekos[a].HelyX >= KepSzeles) {
                Jatekos[a].HelyX -= KepSzeles;
            }
            if (Jatekos[a].HelyX <= 0) {
                Jatekos[a].HelyX += KepSzeles;
            }
            if (Jatekos[a].HelyY >= KepMagas) {
                Jatekos[a].HelyY -= KepMagas;
            }
            if (Jatekos[a].HelyY <= 0) {
                Jatekos[a].HelyY += KepMagas;
            }
        }

        // A kerekített koordinátákból pixeleket számolunk
        int x = Round(Jatekos[a].HelyX);
        int y = Round(Jatekos[a].HelyY);

        // ha a szaggatás felezőpontjánál vagyunk, akkor letároljuk
        /*if (Timer1.Tag == Lyukak_SzaggatasFele) {
            p = Length(arrLyukak);
            SetLength(arrLyukak, p + 1);
            arrLyukak[p].Pont = TPoint(x, y);
            arrLyukak[p].Szog = Jatekos[a].Irany;
            arrLyukak[p].Hasznalt = false;
        }*/

        TKettoSzin KettoSzin = SzineketSzamol(x, y, a);

        // Meghalt a játékos (vonallal, kerettel vagy golyóval ütközött)
        if ((KettoSzin.NemFekete>0) || (KettoSzin.SajatSzin>9) || (AktualisMod.VanKeret && ((x<=0) || (y<=0) || (x>=KepSzeles-Vastagsag) || (y>=KepMagas-Vastagsag)))) {
            Jatekos[a].Engedett = false; // Játékos deaktiválása

            // Halálfej bekapcsolása
            Jatekos[a].Halalfej.Coord_TopLeft = TPoint(x-25,y-24);
            Jatekos[a].Halalfej.Idozites = 50;

            // a még élő játékosok pontokat kapnak
            int p = 0;
            int q = 0;
            bool logi = true; // Még életben van a játékos, nem lőtte le senki

            for (int b = 0; b < Jatekosok; b++) {
                if (Jatekos[b].Engedett) {
                    if (logi) {
                        if (Jatekos[b].FegyverAktiv) {
                            // gyök 138 ~ 11.7473 -> valakinek a golyója közelében halt meg
                            if (sqr(Jatekos[b].Fegyver->X - Jatekos[a].HelyX) + sqr(Jatekos[b].Fegyver->Y - Jatekos[a].HelyY) < 138)
                            {
                                Jatekos[b].Pont += 3; // A lelövő játékos 3 pontot kap
                                logi = false;
                            }
                        }
                    }
                    // ha egy golyó okozta a játékos halálát, akkor a golyó tulajdonosa további pontokat kap
                    // mivel a meghalt játékost már letiltottuk, ezért ha véletlenül a saját golyójától hal meg, nincs pont
                    Jatekos[b].Pont += 2;
                    PontLabel[b]->Caption = IntToStr(Jatekos[b].Pont);
                    p++;
                }
                q = Max(q, Jatekos[b].Pont);
            }

            // senki nincs már életben
            if (p <= 1) {
                Timer1.Enabled = false;
                if (q < (PanelJatszoEmberek - 1) * 25) {
                    NewRaceLabel.Show();
                } else {
                    PanelLabelHideAll();
                }
                PaintBoxRajzol(); // hogy az utolsó halálfej is kirajzolódjon
                return;
            }
        }

        // Ezt a logikát lehet, hogy át lehetne egy kicsit rendezni, hogy pl. egyszerre is meghalhassanak játékosok

        // Lyukért pont jár -> Ebben a verzióban még nincs benne
        /*if (AktualisMod.LyukPont) {
            p = Length(arrLyukak); // ha átmegy egy lyukon, akkor pontot kap
            while (p > 0) {
                p--;
                TVektor vektor = arrLyukak[p];
                if (vektor.Hasznalt) {
                    continue;
                }
                q = abs(vektor.Szog - Jatekos[a].Irany);
                if (q > 180) {
                    q = 360 - q;
                }
                if ((sqrt(x - vektor.Pont.X) + sqrt(y - vektor.Pont.Y)) <= 46) {
                    if (q > 55) {
                        arrLyukak[p].Hasznalt = true;
                        Jatekos[a].Pont++;
                        PontLabel[a]->Caption = IntToStr(Jatekos[a].Pont);
                    }
                }
            }
        }*/

        //kirajzoljuk a vonalat színesen
        // Játéktér szélén vagyunk -> a határra is kerülhet a pixel és "elfeleződhet".
        if ((x < 10) || (x > KepSzeles - 10) || (y < 10) || (y > KepMagas - 10)) { // Itt is lehet egy kicsit finomítani
            for (int p = -1; p <= 1; p++) {
                for (int q = -1; q <= 1; q++) {
                    BitKep->Canvas.Draw(x + p * KepSzeles, y + q * KepMagas, Jatekos[a].bmpVonal);
                }
            }
        } else {
            // Játéktér közepén vagyunk
            BitKep->Canvas.Draw(x, y, Jatekos[a].bmpVonal);
        }

        // Öröklődő mód:
        // letároljuk a pixel koordinátáit egy dinamikus tömbben
        if (!AktualisMod.UjMenetnelTorol)
        {
            int p = Length(arrSzurkePixelek);
            SetLength(arrSzurkePixelek, p + 1);
            arrSzurkePixelek[p] = TPoint(x, y);
        }
    }

    // Mindent kirajzolunk
    PaintBoxRajzol();
}

// Megnyomtunk egy gombot
void TForm1::FormKeyDown(TObject Sender, int Key, TShiftState Shift)
{
    // ESC
    // Játék -> Menü -> Kilépés
    if (Key == VK_ESCAPE) {
        if (Panel2.Visible) {
            Close();
        } else {
            Timer1.Enabled = false;
            PanelLabelHideAll();
        }
    }

    // F10 -> szabályok
    if (Key == VK_F10) {
        bool l = Timer1.Enabled; // letároljuk a Timer állapotát...
        Timer1.Enabled = false;
        MessageBox(strSzabalyok, "Játékszabályok");
        Timer1.Enabled = l; // ...majd visszaállítjuk
    }

    // Ha a menüben vagyunk, lekezeljük a játékmód változását.
    if (Panel2.Visible) {
        int a = 0;

        if (Key == VK_F1) {
            a = 1;
        }
        if (Key == VK_F2) {
            a = 2;
        }
        if (Key == VK_F3) {
            a = 3;
        }
        if (Key == VK_F4) {
            a = 4;
        }

        if (a > 0)
        {
            GroupMod.Tag = a;

            if (a == 1) {
                LabelMod1.Font.Style = fsBold;
            } else {
                LabelMod1.Font.Style = fsNone;
            }
            if (a == 2) {
                LabelMod2.Font.Style = fsBold;
            } else {
                LabelMod2.Font.Style = fsNone;
            }
            if (a == 3) {
                LabelMod3.Font.Style = fsBold;
            } else {
                LabelMod3.Font.Style = fsNone;
            }
            if (a == 4) {
                LabelMod4.Font.Style = fsBold;
            } else {
                LabelMod4.Font.Style = fsNone;
            }
        }
    }

    // Játszunk és a játékosok megnyomták a gombjukat
    if (Timer1.Enabled) {
        for (int a = 0; a < Jatekosok; a++)
        {
            if (Key == Jatekos[a].Gomb[bal]) {
                Jatekos[a].Kanyar = -1;
            }
            if (Key == Jatekos[a].Gomb[jobb]) {
                Jatekos[a].Kanyar = 1;
            }
            if (Key == Jatekos[a].Gomb[loves]) {
                NewFegyver(a);
            }
        }
    }

    // Játszunk és új kör kezdődne (space-re kezdődik)
    if ((Key = VK_SPACE) && NewRaceLabel.Visible) {
        UresImage(AktualisMod.UjMenetnelTorol, AktualisMod.VanKeret);
        for (int a = 0; a < Jatekosok; a++) {
            Jatekos[a].Engedett = PanelLabel[a]->Visible;
        }
        JatekosokatLerak();
        NewRaceLabel.Hide();
        UjMenet();
    }

    // A menüben vagyunk
    if (Panel2.Visible) {
        // A játékosok kiválasztják, hogy szeretnének-e játszani
        for (int a = 0; a < Jatekosok; a++) {
            if (Key=Jatekos[a].Gomb[bal]) {
                PanelLabel[a]->Show();
            }
            if (Key=Jatekos[a].Gomb[jobb]) {
                PanelLabel[a]->Hide();
            }
        }
        // ============ ÚJ MENET INDUL, változók beállítása ============
        if (Key == VK_SPACE)
        {
            PanelJatszoEmberek = 0;
            for (int a = 0; a < Jatekosok; a++) {
                if (PanelLabel[a]->Visible) { // Abból mondjuk meg, hogy valaki akar-e játszani, hogy a labelje látszik-e
                    PanelJatszoEmberek++;
                }
            }

            if (PanelJatszoEmberek >= MinimalisJatekos) {
                Panel2.Hide(); // Elrejtjük a menüt
                AktualisMod = JatekMod[GroupMod.Tag];
                Lyukak_SzaggatasFele = (AktualisMod.SzaggatasTag + 70) / 2;
                UresImage(true, AktualisMod.VanKeret); //új játéknál mindig töröljük a pályát
                for (int a = 0; a < Jatekosok; a++) {
                    Jatekos[a].Engedett = PanelLabel[a]->Visible;
                    PontLabel[a]->Visible = Jatekos[a].Engedett; // Pontszám megjelenítése
                    Jatekos[a].Pont = AktualisMod.StartPont;
                    PontLabel[a]->Caption = IntToStr(AktualisMod.StartPont);
                    Jatekos[a].Halalfej.Idozites = 0;
                }
                JatekosokatLerak();
                UjMenet();
            }
        }
    }
}

// Felengedtük a gombot -> Játék közben visszaállunk egyenesbe
void TForm1::FormKeyUp(TObject Sender, int Key, TShiftState Shift)
{
    if (Timer1.Enabled) {
        for (int a = 0; a < Jatekosok; a++) {
            if ((Key == Jatekos[a].Gomb[bal]) && (Jatekos[a].Kanyar == -1)) {
                Jatekos[a].Kanyar = 0;
            }
            if ((Key == Jatekos[a].Gomb[jobb]) && (Jatekos[a].Kanyar == 1)) {
                Jatekos[a].Kanyar = 0;
            }
        }
    }
}

// Megnyomjuk az egeret -> Játék vezérlése vagy játékos kiválasztása
void TForm1::FormMouseDown(TObject Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Timer1.Enabled) {
        if (Button == mbLeft) {
            Jatekos[6].Kanyar = -1;
        } else if (Button == mbRight) {
            Jatekos[6].Kanyar = 1;
        } else if (Button == mbMiddle) {
            NewFegyver(6);
        }
    }

    if (Panel2.Visible)
    {
        if (Button = mbLeft) {
            PanelLabel[6]->Show();
        }
        if (Button = mbRight) {
            PanelLabel[6]->Hide();
        }
    }
}

// Felengedjük az egeret -> Az egeres játékos abbahagyja a kanyarodást
void TForm1::FormMouseUp(TObject Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Timer1.Enabled)
    {
        if ((Button = mbLeft) && (Jatekos[6].Kanyar = -1)) {
            Jatekos[6].Kanyar = 0;
        }
        if ((Button = mbRight) && (Jatekos[6].Kanyar = 1)) {
            Jatekos[6].Kanyar = 0;
        }
    }
}

// Bezárjuk a formot, felszabadítunk mindent.
void TForm1::FormClose(TObject Sender, TCloseAction Action)
{
    for (int a = 0; a < Jatekosok; a++) {
        delete PontLabel[a];
        delete PanelLabel[a];
        delete Jatekos[a].bmpHalalfej;
        delete Jatekos[a].PuffBitmap;
    }

    delete BitKep;
    delete BitKep2;
    delete bmpVonalFekete;
    delete PuffBitmap0;
}

// Új menet (nem új játék)
void TForm1::UjMenet()
{
    BitKep->Canvas.Brush.Style = bsClear; // Az üres ellipszishez kikapcsoljuik a kitöltést

    // Visszaállítjuk a játékosok fegyverét (akkor aktív a fegyver, ha épp van golyó a pályán)
    for (int a = 0; a < Jatekosok; a++) {
        if (Jatekos[a].FegyverAktiv) {
            delete Jatekos[a].Fegyver;
        }
    }

    // Szűkülő kört rajzolunk a játékosok köré.
    // Blokkolunk, mert semmi nem történik még.
    for (int a = 80; a >= 2; a--) { // a = sugár
        BitKep->Canvas.Pen.Width = 2;
        for (int b = 0; b < Jatekosok; b++) {
            if (Jatekos[b].Engedett) {
                BitKep->Canvas.Pen.Color = Szinek[b];
                BitKep->Canvas.Ellipse(Round(Jatekos[b].HelyX - a), Round(Jatekos[b].HelyY - a), Round(Jatekos[b].HelyX + a), Round(Jatekos[b].HelyY + a));
            }
        }
        PaintBoxRajzol();
        Sleep(11); // Várunk, amíg az animáció következő lépését kirajzoljuk.
        // Feketével átrajzoljuk az előbb színessel kirajzolt kört, ezzel a vonalakat is letöröljük a játékos körül.
        BitKep->Canvas.Pen.Color = clBlack;
        BitKep->Canvas.Pen.Width = 4;
        for (int b = 0; b < Jatekosok; b++) {
            if (Jatekos[b].Engedett) {
                BitKep->Canvas.Ellipse(Round(Jatekos[b].HelyX - a), Round(Jatekos[b].HelyY - a), Round(Jatekos[b].HelyX + a), Round(Jatekos[b].HelyY + a));
            }
            if (Panel2.Visible) {
                return; //ha ellipszisrajzolás közben nyomtak Escape-t, akkor nem indul a Timer
            }
        }
    }

    // Átállítjuk a pen méretét, érdemes figyelni, hogy hol fog kelleni.
    BitKep->Canvas.Pen.Width = 1;
    BitKep->Canvas.Brush.Style = bsSolid;

    SetLength(arrSzurkePixelek, 0);
    Timer1.Tag = 0;
    Timer1.Enabled = true;
}

// Kirajzolja a menüt resetelt játékosokkal
void TForm1::PanelLabelHideAll()
{
    for (int a = 0; a < Jatekosok; a++) {
        PanelLabel[a]->Hide();
    }
    NewRaceLabel.Hide();
    Panel2.Show();
}

// Akkor van rendben az aktív játékosok helyzete, ha a távolságuk
// egymástól legalább a képernyő szélességének ötöde.
bool TForm1::JatekosPozicioRendben()
{
    bool Result = true;

    for (int a = 0; a < Jatekosok - 1; a++) {
        for (int b = a + 1; b < Jatekosok; b++) {
            if (Jatekos[a].Engedett && Jatekos[b].Engedett) {
                if (sqrt(sqr(Jatekos[a].HelyX - Jatekos[b].HelyX) + sqr(Jatekos[a].HelyY - Jatekos[b].HelyY)) < KepSzeles / 5) {
                    Result = false;
                }
            }
        }
    }

    return Result;
}

// Mindenkit lerakunk, inicializálnuk és ha nincsenek túl közel egymáshoz
// (JatekosPozicioRendben) akkor visszatérünk.
void TForm1::JatekosokatLerak()
{
    do {
        for (int a = 0; a < Jatekosok; a++) {
            Jatekos[a].Irany = Random(360 / Fokok) * Fokok; // 'Fokok' felbontású legyen az irány (minden játékos pontosan ugyanazon a skálán mozogjon)
            Jatekos[a].HelyX = Random(KepSzeles - 240) + 120; // Legalább 12 0pixelre a pálya szélétől
            Jatekos[a].HelyY = Random(KepMagas - 240) + 120;
            Jatekos[a].Kanyar = 0;
            Jatekos[a].FegyverAktiv = false;
            Jatekos[a].Halalfej.Idozites = 0;
        }
    } while (!JatekosPozicioRendben());
}

// Az állandó bitképet rárajzoljuk az ideiglenesre, mellé a halálfejeket, majd az egészet a képernyőre.
void TForm1::PaintBoxRajzol()
{
    BitKep2->Canvas.Draw(0, 0, BitKep);
    for (int x = 0; x < Jatekosok; x++) {
        if (Jatekos[x].Halalfej.Idozites > 0) {
            BitKep2->Canvas.Draw(Jatekos[x].Halalfej.Coord_TopLeft.X, Jatekos[x].Halalfej.Coord_TopLeft.Y, Jatekos[x].bmpHalalfej);
            Jatekos[x].Halalfej.Idozites--;
        }
    }
    PaintBox2.Canvas.Draw(0, 0, BitKep2);
}

// Paint event handler
void TForm1::PaintBoxOnPaint(TObject Sender)
{
    PaintBoxRajzol();
}

// Ablakméret teszteléshez
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

int main()
{
    /* SDL inicializálása és ablak megnyitása */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Zatacka", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);
 
    /* csinaljunk valamit */
    Eredmenyjelzo eredmenyjelzo(renderer);

    TForm1 main_form(renderer);

    main_form.UresImage(false, true);
    main_form.jatekter.Megjelenit();

    /* az elvegzett rajzolasok a kepernyore */
    SDL_RenderPresent(renderer);

    /* varunk a kilepesre */
    SDL_Event ev;
    bool quit = false;
    while (!quit && SDL_WaitEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            quit = true;
            break;
        
        default:
            break;
        }
    }
 
    /* ablak bezarasa */
    SDL_Quit();
 
    return 0;
}
