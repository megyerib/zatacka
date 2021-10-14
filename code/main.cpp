#include "port.h"

// type

struct TJatekMod {
    Integer SzaggatasTag;
    Integer StartPont;
    Boolean UjMenetnelTorol;
    Boolean VanKeret;
    Boolean LyukPont;
};

struct THalalfej {
    TPoint Coord_TopLeft;
    Integer Idozites;
};

struct TKettoSzin {
    Byte SajatSzin;
    Byte NemFekete;
};

class TFegyver : public TTriggerable
{
public:
    TTimer Timer;
    Integer Szog;
    Byte Szam; // a játékos száma
    Real X;
    Real Y;
    TFegyver(Integer x, Integer y, Integer irany, Byte Tulaj);
    void OnTimer(TObject Sender) override;
    void Destroy();
};

struct TVektor {
   TPoint Pont;
   Integer Szog;
   Boolean Hasznalt;
};

struct TJatekos {
   Integer Gomb[3 + 1];
   Integer Irany;
   ShortInt Kanyar;
   Real HelyX;
   Real HelyY;
   Boolean Engedett;
   Boolean FegyverAktiv;
   Byte Pont;
   TFegyver* Fegyver;
   THalalfej Halalfej;
   TBitmap* bmpVonal;
   TBitmap* bmpHalalfej;
   TBitmap* PuffBitmap;
};

class TForm1 : TForm
{
public:
    TTimer Timer1;
    TPanel Panel1;
    TImageList ImageList1;
    TPanel Panel2;
    TLabel Label1;
    TLabel Label2;
    TLabel Label3;
    TLabel Label4;
    TLabel Label5;
    TLabel Label6;
    TLabel Label7;
    TLabel Label10;
    TLabel Label11;
    TImage Image2;
    TPaintBox PaintBox2;
    TGroupBox GroupBox2;
    TGroupBox GroupMod;
    TLabel LabelMod1;
    TLabel Label9;
    TLabel LabelMod2;
    TLabel Label13;
    TLabel LabelMod3;
    TLabel Label15;
    TLabel LabelMod4;
    TLabel Label8;
    TLabel Label12;
    TLabel NewRaceLabel;
    void NewFegyver(Byte a);
    static TKettoSzin SzineketSzamol(Word x, Word y, Byte szam);
    void FormCreate(TObject Sender);
    void UresImage(Boolean Torol, Boolean VanKeret);
    void Timer1Timer(TObject Sender);
    void FormKeyDown(TObject Sender, Word Key, TShiftState Shift);
    void FormKeyUp(TObject Sender, Word Key, TShiftState Shift);
    void FormMouseDown(TObject Sender, TMouseButton Button, TShiftState Shift, Integer X, Integer Y);
    void FormMouseUp(TObject Sender, TMouseButton Button, TShiftState Shift, Integer X, Integer Y);
    void FormClose(TObject Sender, TCloseAction Action);
    void FormShow(TObject Sender);
    void UjMenet();
    void PanelLabelHideAll();
    static Boolean JatekosPozicioRendben();
    void JatekosokatLerak();
    void PaintBoxRajzol();
    void PaintBoxOnPaint(TObject Sender);
};

// const

const Integer Jatekosok = 7;
const TColor Szinek[Jatekosok] = {clRed,clYellow,clBlue,clOlive,clLime,clMagenta,clAqua};
const Integer Ut = 3;
const Integer FegyverSebesseg = 1.3 * Ut;
const Integer Fokok = 4;
const Integer Vastagsag = 4; // nem lehet több 10-nél, különben a falnélküli mód hibásan viselkedhet
const Integer VastagsagDelta = 2; // (Vastagsag-1) div 2
const Integer MinimalisJatekos=1;
const String strSzabalyok =
   "  A vezérlés részben látja a vezérlőgombokat. Nyomja meg valamelyik sor első vezérlőgombját (egérnél bal gomb), így a gombhoz tartozó játékost aktiválja, aki részt vesz majd a játékban. "
   "A játékos kikapcsolásához nyomja meg a játékoshoz tartozó második gombot (egérnél jobb gomb).\r\n\r\n"
   "  A játék indítása után az első és második gombobbal irányíthatja a játékos színével megegyező színű vonalat. A cél, hogy ne ütközzön falba vagy egy másik vonalba. Amikor egy vonal falba ütközik, a még játékban lévő játékosok 2-2 pontot kapnak.\r\n"
   "  A vonalakban peridodikusan lyukak vannak (kivéve egyes módokat), amiken át lehet menni.\r\n\r\n"
   "  Játék közben a harmadik vezérlőgomb (egérnél középső gomb) használatával lehet lőni: ez annyit jelent, hogy a játékos vonalából a haladási iránnyal megegyező irányban egy vörös golyó indul el, utat csinálva ezáltal a játékosnak.\r\n"
   "  Egy játékosnak egyszerre egy golyója lehet a képernyőn. Egy golyó kilövése 1 pontba kerül, és legalább ennyi szükséges a kilövéshez.";
const TJatekMod JatekMod[4] = {
    {.SzaggatasTag = 67, .StartPont = 0, .UjMenetnelTorol = True,  .VanKeret = True,  .LyukPont = True},
    {.SzaggatasTag = 70, .StartPont = 3, .UjMenetnelTorol = True,  .VanKeret = True,  .LyukPont = False},
    {.SzaggatasTag = 26, .StartPont = 0, .UjMenetnelTorol = False, .VanKeret = True,  .LyukPont = False},
    {.SzaggatasTag = 66, .StartPont = 0, .UjMenetnelTorol = True,  .VanKeret = False, .LyukPont = True}
};

// var

TForm1 Form1;
TJatekos Jatekos[Jatekosok + 1];
Integer KepSzeles;
Integer KepMagas;
Real Tabla[2 + 1][360];
Byte PanelJatszoEmberek;
TLabel* PontLabel[Jatekosok];
TLabel* PanelLabel[Jatekosok];
TBitmap* BitKep;
TBitmap* BitKep2;
TBitmap* bmpVonalFekete;
TPoint arrSzurkePixelek[];
TVektor arrLyukak[];
Integer Lyukak_SzaggatasFele; // a szaggatás középpontjának a Timer.Tag-je
TJatekMod AktualisMod;
TBitmap* PuffBitmap0;

// implementation

void TForm1::NewFegyver(Byte a)
{
    if (!Jatekos[a].FegyverAktiv && Jatekos[a].Pont >= 1 && Jatekos[a].Engedett) {
        Jatekos[a].Pont--;
        PontLabel[a]->Caption = IntToStr(Jatekos[a].Pont);
        Jatekos[a].Fegyver = new TFegyver(Round(Jatekos[a].HelyX), Round(Jatekos[a].HelyY), Jatekos[a].Irany, a);
    }
}

TFegyver::TFegyver(Integer x, Integer y, Integer Irany, Byte Tulaj) : Timer(Form1)
{
    Szog = Irany;
    X = x + Tabla[2][Szog] * 18;
    Y = y - Tabla[1][Szog] * 18;
    Szam = Tulaj;
    Jatekos[Szam].FegyverAktiv = True;

    Timer.Interval = Form1.Timer1.Interval;
    Timer.OnTimer = TTriggerable::OnTimer;
}

void TFegyver::OnTimer(TObject Sender)
{
    BitKep->Canvas.Draw(Round(X) - 9, Round(Y) - 9, PuffBitmap0);
    if (!Form1.Timer1.Enabled) {
        Timer.Enabled = False;
        return;
    }
    X = X + Tabla[2][Szog] * FegyverSebesseg;
    Y = Y - Tabla[1][Szog] * FegyverSebesseg;
    Timer.Tag++;
    BitKep->Canvas.Draw(Round(X) - 9, Round(Y) - 9, Jatekos[Szam].PuffBitmap);
    if ((X < -10) || (Y < -10) || (X > KepSzeles + 10) || (Y > KepMagas + 10)) {
        Destroy();
    }
}

void TFegyver::Destroy()
{
    Timer.Enabled = False;
    Timer.Free();
    Jatekos[Szam].FegyverAktiv = False;
}

TKettoSzin TForm1::SzineketSzamol(Word x, Word y, Byte szam)
{
    Word a, b;
    TColor c, c2;
    TKettoSzin e;

    c2 = Szinek[szam];
    e.SajatSzin = 0;
    e.NemFekete = 0;

    for (a = x; a <= x + 3; a++) {
        for (b = y; b <= y + 3; b++) {
            c = BitKep->Canvas.Pixels[a][b];
            if ((a >= KepSzeles) || (b >= KepMagas)) {
                continue; // Ha a képpont kívül esik a tartományon, akkor nem kell vizsgálni.
            }
            if (c == c2) {
                e.SajatSzin++;
            }
            if ((c != TColor::clBlack) && (c != c2)) {
                e.NemFekete++;
            }
        }
    }

    return e;
}

void TForm1::FormCreate(TObject Sender)
{
    Integer x, a, b;

    KepSzeles = Screen.Width - 120;
    KepMagas = Screen.Height;
    //DoubleBuffered = True;

    PanelJatszoEmberek = 0;

    for (x = 0; x < 360; x++) {
        Tabla[1][x] = Sin(x * Pi / 180);
        Tabla[2][x] = Cos(x * Pi / 180);
    }

    for (x = 0; x < Jatekosok; x++) {
        PontLabel[x] = new TLabel(Form1);

        PontLabel[x]->Parent = Panel1;
        PontLabel[x]->Caption = '0';
        PontLabel[x]->Left = 75;
        PontLabel[x]->Top = 66 * x - 50;
        PontLabel[x]->Font.Size = 40;
        PontLabel[x]->AutoSize = True;
        PontLabel[x]->Alignment = taRightJustify;
        PontLabel[x]->Font.Color = Szinek[x];
        PontLabel[x]->Transparent = True;
        PontLabel[x]->Hide();
        PontLabel[x]->OnMouseDown = TForm::FormMouseDown;
        PontLabel[x]->OnMouseUp = TForm::FormMouseUp;

        PanelLabel[x] = new TLabel(Form1);

        PanelLabel[x]->Parent = GroupBox2;
        PanelLabel[x]->Caption = 'Aktív';
        PanelLabel[x]->Left = 106;
        PanelLabel[x]->Top = 20 * (x + 1);
        PanelLabel[x]->Font.Style = fsBold;
        PanelLabel[x]->AutoSize = True;
        PanelLabel[x]->Font.Color = Szinek[x];
        PanelLabel[x]->Hide();
        PanelLabel[x]->OnMouseDown = TForm::FormMouseDown;
        PanelLabel[x]->OnMouseUp = TForm::FormMouseUp;

        Jatekos[x].bmpVonal = new TBitmap();

        Jatekos[x].bmpVonal->Width = Vastagsag;
        Jatekos[x].bmpVonal->Height = Vastagsag;
        Jatekos[x].bmpVonal->Canvas.Pen.Color = Szinek[x];
        Jatekos[x].bmpVonal->Canvas.Brush.Color = Szinek[x];
        Jatekos[x].bmpVonal->Canvas.Rectangle(0, 0, Vastagsag, Vastagsag);
    }

    Jatekos[x].bmpHalalfej = new TBitmap();

    Jatekos[x].bmpHalalfej->Width = 50;
    Jatekos[x].bmpHalalfej->Height = 48;
    ImageList1.GetBitmap(0, Jatekos[x].bmpHalalfej);
    Jatekos[x].bmpHalalfej->Transparent = True;

    for (a = 0; a < Jatekos[x].bmpHalalfej->Width; a++) {
        for (b = 0; b < Jatekos[x].bmpHalalfej->Height; b++) {
            if (Jatekos[x].bmpHalalfej->Canvas.Pixels[a][b] == clWhite) {
                Jatekos[x].bmpHalalfej->Canvas.Pixels[a][b] = Szinek[x];
            }
        }
    }
    Jatekos[x].Halalfej.Idozites = 0;

    bmpVonalFekete = new TBitmap();

    bmpVonalFekete->Width = Vastagsag;
    bmpVonalFekete->Height = Vastagsag;
    bmpVonalFekete->Canvas.Pen.Color = clBlack;
    bmpVonalFekete->Canvas.Brush.Color =clBlack;
    bmpVonalFekete->Canvas.Rectangle(0, 0, Vastagsag, Vastagsag);

    //létrehozzuk a fő Bitmap-eket
    BitKep = new TBitmap();

    BitKep->Width = KepSzeles;
    BitKep->Height = KepMagas;
    BitKep->PixelFormat = pf8bit;

    BitKep2 = new TBitmap();
    BitKep2->Width = KepSzeles;
    BitKep2->Height = KepMagas;

    //létrehozzuk a golyók Bitmap-jét
    PuffBitmap0 = new TBitmap();
    PuffBitmap0->Width = 19;
    PuffBitmap0->Height = 19;
    PuffBitmap0->Canvas.Brush.Color = clWhite;
    PuffBitmap0->Canvas.Pen.Color = clWhite;
    PuffBitmap0->Canvas.Rectangle(0, 0, 19, 19);
    PuffBitmap0->Canvas.Pen.Color = clBlack;
    PuffBitmap0->Canvas.Brush.Color = clBlack;
    PuffBitmap0->Canvas.Pen.Width = 2;
    PuffBitmap0->Canvas.Ellipse(0,0,19,19);
    PuffBitmap0->Transparent = True;

    for (x = 1; x <= Jatekosok; x++) {
        Jatekos[x].PuffBitmap = new TBitmap();

        Jatekos[x].PuffBitmap->Width = 19;
        Jatekos[x].PuffBitmap->Height = 19;
        Jatekos[x].PuffBitmap->Canvas.Brush.Color = clBlack;
        Jatekos[x].PuffBitmap->Canvas.Rectangle(0, 0, 19, 19);
        Jatekos[x].PuffBitmap->Canvas.Pen.Color = clMaroon;
        Jatekos[x].PuffBitmap->Canvas.Brush.Color = Szinek[x];
        Jatekos[x].PuffBitmap->Canvas.Pen.Width = 2;
        Jatekos[x].PuffBitmap->Canvas.Ellipse(0, 0, 19, 19);
        Jatekos[x].PuffBitmap->Transparent = True;
    }

    Randomize();

    Jatekos[1].Gomb[1] = Ord('1');
    Jatekos[1].Gomb[2] = Ord('Q');
    Jatekos[1].Gomb[3] = Ord('2');

    Jatekos[2].Gomb[1] = 17;
    Jatekos[2].Gomb[2] = 18;
    Jatekos[2].Gomb[3] = Ord('X');

    Jatekos[3].Gomb[1] = Ord('4');
    Jatekos[3].Gomb[2] = Ord('5');
    Jatekos[3].Gomb[3] = Ord('R');

    Jatekos[4].Gomb[1] = Ord('M');
    Jatekos[4].Gomb[2] = 188;
    Jatekos[4].Gomb[3] = Ord('K');

    Jatekos[5].Gomb[1] = VK_LEFT;
    Jatekos[5].Gomb[2] = VK_DOWN;
    Jatekos[5].Gomb[3] = VK_UP;

    Jatekos[6].Gomb[1] = VK_DIVIDE;
    Jatekos[6].Gomb[2] = VK_MULTIPLY;
    Jatekos[6].Gomb[3] = VK_SUBTRACT;

    UresImage(True, AktualisMod.VanKeret);
}

void TForm1::UresImage(Boolean Torol, Boolean VanKeret) {
    Integer x;
    Integer y;
    Integer a;
    Integer l;
    TPoint p;

    if (Torol) {
        BitKep->Canvas.Brush.Color = clBlack;
        BitKep->Canvas.Rectangle(-1, -1, KepSzeles + 1, KepMagas + 1);
    } else { // az arrSzurkePixelek tömbben tárolt koordinátákat átszínezzük
        l = Length(arrSzurkePixelek);
        for (a = l - 1; a >= 0; a--) {
            p = arrSzurkePixelek[a];
            for (x = p.X + Vastagsag - 1; x >= p.X; x--) {
                for (y = p.Y + Vastagsag - 1; x >= p.Y; y--) {
                    if (BitKep->Canvas.Pixels[x][y] != clBlack) {
                        BitKep->Canvas.Pixels[x][y] = clSilver;
                    }
                }
            }
            SetLength(arrSzurkePixelek, 0);
            SetLength(arrLyukak, 0);
        }
        if (VanKeret) { // újrarajzoljuk a keretet
            BitKep->Canvas.Brush.Color = clWhite;
            BitKep->Canvas.Pen.Color = clWhite;
            BitKep->Canvas.Rectangle(0, 0, KepSzeles, 7);
            BitKep->Canvas.Rectangle(0, 0, 7, KepMagas);
            BitKep->Canvas.Rectangle(KepSzeles - 7, 0, KepSzeles, KepMagas);
            BitKep->Canvas.Rectangle(0, KepMagas - 7, KepSzeles, KepMagas);
        }
    }

    PaintBoxRajzol;
}

void TForm1::Timer1Timer(TObject Sender) {
    Byte a, b;
    Integer p, q, x, y;
    TKettoSzin KettoSzin;
    Boolean logi;

    Timer1.Tag = (Timer1.Tag + 1) % 70;

    for (a = 1; a <= Jatekosok; a++) {
        if (!Jatekos[a].Engedett) {
            continue; // nem aktív játékos átlépése
        }
        if (Timer1.Tag >= AktualisMod.SzaggatasTag) {
            x = Round(Jatekos[a].HelyX);
            y = Round(Jatekos[a].HelyY);

            if ((x < 10) || (x > KepSzeles - 10) || (y < 10) || (y > KepMagas - 10)) {
                for (p = -1; p <= 1; p++) {
                    for (q = -1; q <= 1; q++) {
                        BitKep->Canvas.Draw(x + p * KepSzeles, y + q * KepMagas, bmpVonalFekete);
                    }
                }
            } else {
                BitKep->Canvas.Draw(x, y, bmpVonalFekete);
            }

            Jatekos[a].Irany -= Jatekos[a].Kanyar * Fokok;

            if (Jatekos[a].Irany < 0) {
                Jatekos[a].Irany += 360;
            }
            if (Jatekos[a].Irany >= 360) {
                Jatekos[a].Irany -= 360;
            }

            // kiszámítjuk az új koordinátákat
            Jatekos[a].HelyX += Ut * Tabla[2][Jatekos[a].Irany];
            Jatekos[a].HelyY -= Ut * Tabla[1][Jatekos[a].Irany];

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

            x = Round(Jatekos[a].HelyX);
            y = Round(Jatekos[a].HelyY);

            // ha a szaggatás felezőpontjánál vagyunk, akkor letároljuk
            if (Timer1.Tag == Lyukak_SzaggatasFele) {
                p = Length(arrLyukak);
                SetLength(arrLyukak, p + 1);
                arrLyukak[p].Pont = TPoint(x, y);
                arrLyukak[p].Szog = Jatekos[a].Irany;
                arrLyukak[p].Hasznalt = False;
            }

            KettoSzin = SzineketSzamol(x, y, a);

            if ((KettoSzin.NemFekete>0) || (KettoSzin.SajatSzin>9) || (AktualisMod.VanKeret && ((x<=0) || (y<=0) || (x>=KepSzeles-Vastagsag) || (y>=KepMagas-Vastagsag)))) {
                Jatekos[a].Engedett = False;

                Jatekos[a].Halalfej.Coord_TopLeft = TPoint(x-25,y-24);
                Jatekos[a].Halalfej.Idozites = 50;

                // a még élő játékosok pontokat kapnak
                p = 0;
                q = 0;
                logi = True;

                for (b = 1; b <= Jatekosok; b++) {
                    if (Jatekos[b].Engedett) {
                        if (logi) {
                            if (Jatekos[b].FegyverAktiv) {
                                if (Sqr(Jatekos[b].Fegyver->X - Jatekos[a].HelyX) + Sqr(Jatekos[b].Fegyver->Y - Jatekos[a].HelyY) < 138)
                                {
                                    Jatekos[b].Pont += 3;
                                    logi = False;
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

                if (p <= 1) {
                    // senki nincs már életben
                    Timer1.Enabled = False;
                    if (q < (PanelJatszoEmberek - 1) * 25) {
                        NewRaceLabel.Show();
                    } else {
                        PanelLabelHideAll();
                    }
                    PaintBoxRajzol(); // hogy az utolsó halálfej is kirajzolódjon
                    return;
                }
            }

            if (AktualisMod.LyukPont) {
                p = Length(arrLyukak); // ha átmegy egy lyukon, akkor pontot kap
                while (p > 0) {
                    p--;
                    TVektor vektor = arrLyukak[p];
                    if (vektor.Hasznalt) {
                        continue;
                    }
                    q = Abs(vektor.Szog - Jatekos[a].Irany);
                    if (q > 180) {
                        q = 360 - q;
                    }
                    if ((Sqr(x - vektor.Pont.X) + Sqr(y - vektor.Pont.Y)) <= 46) {
                        if (q > 55) {
                            arrLyukak[p].Hasznalt = True;
                            Jatekos[a].Pont++;
                            PontLabel[a]->Caption = IntToStr(Jatekos[a].Pont);
                        }
                    }
                }
            }

            //kirajzoljuk a vonalat színesen
            if ((x < 10) || (x > KepSzeles - 10) || (y < 10) || (y > KepMagas - 10)) {
                for (p = -1; p <= 1; p++) {
                    for (q = -1; q <= 1; q++) {
                        BitKep->Canvas.Draw(x + p * KepSzeles, y + q * KepMagas, Jatekos[a].bmpVonal);
                    }
                }
            } else {
                BitKep->Canvas.Draw(x, y, Jatekos[a].bmpVonal);
            }

            if (!AktualisMod.UjMenetnelTorol) // letároljuk a pixel koordinátáit egy dinamikus tömbben
            {
                p = Length(arrSzurkePixelek);
                SetLength(arrSzurkePixelek, p + 1);
                arrSzurkePixelek[p] = TPoint(x, y);
            }
        }

        PaintBoxRajzol();
    }
}

void TForm1::FormKeyDown(TObject Sender, Word Key, TShiftState Shift)
{
    Byte a;
    Boolean l;

    if (Key == VK_ESCAPE) {
        if (Panel2.Visible) {
            Close();
        } else {
            Timer1.Enabled = False;
            PanelLabelHideAll();
        }
    }

    if (Key == VK_F10) {
        l = Timer1.Enabled; // letároljuk a Timer állapotát...
        Timer1.Enabled = False;
        MessageBox(strSzabalyok, "Játékszabályok");
        Timer1.Enabled = l; // ...majd visszaállítjuk
    }

    //lekezeljük az esetleges mód-módosítást
    if (Panel2.Visible) {
        a = 0;

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

    if (Timer1.Enabled) {
        for (a = 1; a <= Jatekosok; a++)
        {
            if (Key == Jatekos[a].Gomb[1]) {
                Jatekos[a].Kanyar = -1;
            }
            if (Key == Jatekos[a].Gomb[2]) {
                Jatekos[a].Kanyar = 1;
            }
            if (Key == Jatekos[a].Gomb[3]) {
                NewFegyver(a);
            }
        }
    }

    if ((Key = VK_SPACE) && NewRaceLabel.Visible) {
        UresImage(AktualisMod.UjMenetnelTorol, AktualisMod.VanKeret);
        for (a = 1; a <= Jatekosok; a++) {
            Jatekos[a].Engedett = PanelLabel[a]->Visible;
        }
        JatekosokatLerak();
        NewRaceLabel.Hide();
        UjMenet();
    }
    
    if (Panel2.Visible) {
        for (a = 1 ; a <= Jatekosok; a++) {
            if (Key=Jatekos[a].Gomb[1]) {
                PanelLabel[a]->Show();
            }
            if (Key=Jatekos[a].Gomb[2]) {
                PanelLabel[a]->Hide();
            }
        }
        // ============ ÚJ MENET INDUL, változók beállítása ============
        if (Key == VK_SPACE)
        {
            PanelJatszoEmberek = 0;
            for (a = 1; a <= Jatekosok; a++) {
                if (PanelLabel[a]->Visible) {
                    PanelJatszoEmberek++;
                }
            }

            if (PanelJatszoEmberek >= MinimalisJatekos)
            {
                Panel2.Hide();
                AktualisMod = JatekMod[GroupMod.Tag];
                Lyukak_SzaggatasFele = (AktualisMod.SzaggatasTag + 70) / 2;
                UresImage(True, AktualisMod.VanKeret); //új játéknál mindig töröljük a pályát
                for (a = 1; a <= Jatekosok; a++)
                {
                    Jatekos[a].Engedett = PanelLabel[a]->Visible;
                    PontLabel[a]->Visible = Jatekos[a].Engedett;
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

void TForm1::FormKeyUp(TObject Sender, Word Key, TShiftState Shift)
{
    Byte a;

    if (Timer1.Enabled) {
        for (a = 1; a <= Jatekosok; a++) {
            if ((Key == Jatekos[a].Gomb[1]) && (Jatekos[a].Kanyar == -1)) {
                Jatekos[a].Kanyar = 0;
            }
            if ((Key == Jatekos[a].Gomb[2]) && (Jatekos[a].Kanyar == 1)) {
                Jatekos[a].Kanyar = 0;
            }
        }
    }
}

void TForm1::FormMouseDown(TObject Sender, TMouseButton Button, TShiftState Shift, Integer X, Integer Y)
{
    if (Timer1.Enabled) {
        if (Button == mbLeft) {
            Jatekos[7].Kanyar = -1;
        } else if (Button == mbRight) {
            Jatekos[7].Kanyar = 1;
        } else if (Button == mbMiddle) {
            NewFegyver(7);
        }
    }
    
    if (Panel2.Visible)
    {
        if (Button = mbLeft) {
            PanelLabel[7]->Show();
        }
        if (Button = mbRight) {
            PanelLabel[7]->Hide();
        }
    }
}

void TForm1::FormMouseUp(TObject Sender, TMouseButton Button, TShiftState Shift, Integer X, Integer Y)
{
    if (Timer1.Enabled)
    {
        if ((Button = mbLeft) && (Jatekos[7].Kanyar = -1)) {
            Jatekos[7].Kanyar = 0;
        }
        if ((Button = mbRight) && (Jatekos[7].Kanyar = 1)) {
            Jatekos[7].Kanyar = 0;
        }
    }
}

void TForm1::FormClose(TObject Sender, TCloseAction Action)
{
    Byte a;

    for (a = 1; a <= Jatekosok; a++) {
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

void TForm1::FormShow(TObject Sender)
{
    ShowCursor(False);
}

void TForm1::UjMenet()
{
    Byte a, b;
    BitKep->Canvas.Brush.Style = bsClear;
    for (a = 1; a <= Jatekosok; a++) {
        if (Jatekos[a].FegyverAktiv) {
            delete Jatekos[a].Fegyver;
        }
    }

    for (a = 80; a >= 2; a--) {
        BitKep->Canvas.Pen.Width = 2;
        for (b = 1; b <= Jatekosok; b++) {
            if (Jatekos[b].Engedett) {
                BitKep->Canvas.Pen.Color = Szinek[b];
                BitKep->Canvas.Ellipse(Round(Jatekos[b].HelyX - a), Round(Jatekos[b].HelyY - a), Round(Jatekos[b].HelyX + a), Round(Jatekos[b].HelyY + a));
            }
        }
        PaintBoxRajzol();
        ProcessMessages();
        Sleep(11);
        BitKep->Canvas.Pen.Color = clBlack;
        BitKep->Canvas.Pen.Width = 4;
        for (b = 1; b <= Jatekosok; b++) {
            if (Jatekos[b].Engedett) {
                BitKep->Canvas.Ellipse(Round(Jatekos[b].HelyX - a), Round(Jatekos[b].HelyY - a), Round(Jatekos[b].HelyX + a), Round(Jatekos[b].HelyY + a));
            }
            if (Panel2.Visible) {
                return; //ha ellipszisrajzolás közben nyomtak Escape-t, akkor nem indul a Timer
            }
        }
    }
    BitKep->Canvas.Pen.Width = 1;
    BitKep->Canvas.Brush.Style = bsSolid;

    SetLength(arrSzurkePixelek, 0);
    Timer1.Tag = 0;
    Timer1.Enabled = True;
}

void TForm1::PanelLabelHideAll()
{
    for (Byte a = 1; a <= Jatekosok; a++) {
        PanelLabel[a]->Hide();
    }
    NewRaceLabel.Hide();
    Panel2.Show();
}

Boolean TForm1::JatekosPozicioRendben()
{
    Boolean Result = True;

    for (Byte a = 1; a <= Jatekosok - 1; a++) {
        for (Byte b = a + 1; b <= Jatekosok; b++) {
            if (Jatekos[a].Engedett && Jatekos[b].Engedett) {
                if (Sqr(Sqr(Jatekos[a].HelyX - Jatekos[b].HelyX) + Sqr(Jatekos[a].HelyY - Jatekos[b].HelyY)) < KepSzeles / 5) {
                    Result = False;
                }
            }
        }
    }

    return Result;
}

void TForm1::JatekosokatLerak()
{
    do {
        for (Byte a = 1; a <= Jatekosok; a++) {
            Jatekos[a].Irany = Random(360 / Fokok) * Fokok;
            Jatekos[a].HelyX = Random(KepSzeles - 240) + 120;
            Jatekos[a].HelyY = Random(KepMagas - 240) + 120;
            Jatekos[a].Kanyar = 0;
            Jatekos[a].FegyverAktiv = False;
            Jatekos[a].Halalfej.Idozites = 0;
        }
    } while (!JatekosPozicioRendben());
}

void TForm1::PaintBoxRajzol()
{
    BitKep2->Canvas.Draw(0, 0, BitKep);
    for (Integer x = 1; x <= Jatekosok; x++) {
        if (Jatekos[x].Halalfej.Idozites > 0) {
            BitKep2->Canvas.Draw(Jatekos[x].Halalfej.Coord_TopLeft.X, Jatekos[x].Halalfej.Coord_TopLeft.Y, Jatekos[x].bmpHalalfej);
            Jatekos[x].Halalfej.Idozites--;
        }
    }
    PaintBox2.Canvas.Draw(0, 0, BitKep2);
}

void TForm1::PaintBoxOnPaint(TObject Sender)
{
    PaintBoxRajzol();
}
