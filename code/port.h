#pragma once
#include <string>
#include <vector>
#include "konstans.h"

using namespace std;

// enum

enum TBrushStyle
{
    bsClear,
    bsSolid
};

enum TPixelFormat
{
    pf8bit
};

enum TAlignment
{
    taRightJustify
};
enum TFontStyle
{
    fsNone,
    fsBold
};

enum TMouseButton
{
    mbLeft,
    mbRight,
    mbMiddle
};

enum TShiftState
{
    state
};

// class
class TObject
{
public:
    bool Visible;
    void Show();
    void Hide();
};
class TPoint
{
public:
    int X;
    int Y;
    TPoint();
    TPoint(int x, int y);
};

class TBitmap;

class TCanvas
{
public:
    void Draw(int x, int y, TBitmap *bitmap);
    TColor **Pixels;
    struct
    {
        TColor Color;
        int Width;
    } Pen;
    struct
    {
        TColor Color;
        TBrushStyle Style;
    } Brush;
    void Rectangle(int a, int b, int c, int d);
    void Ellipse(int a, int b, int c, int d);
};

class TBitmap
{
public:
    TCanvas Canvas;
    int Width;
    int Height;
    bool Transparent;
    TPixelFormat PixelFormat;
};

class TForm : public TObject
{
public:
    virtual void FormMouseDown(TObject Sender, TMouseButton Button, TShiftState Shift, int X, int Y) = 0;
    virtual void FormMouseUp(TObject Sender, TMouseButton Button, TShiftState Shift, int X, int Y) = 0;
    void Close();
    void ShowCursor(bool show);
};
class TPanel : public TObject
{
};
class TImageList
{
public:
    void GetBitmap(int i, TBitmap *bitmap);
};

class TLabel : public TObject
{
public:
    string Caption;
    TLabel(){}

    int Left;
    int Top;
    struct
    {
        int Size;
        TColor Color;
        TFontStyle Style;
    } Font;
    bool AutoSize;
    TAlignment Alignment;
    bool Transparent;
};
class TImage
{
};
class TPaintBox
{
public:
    TCanvas Canvas;
};
class TGroupBox : public TObject
{
public:
    int Tag;
};

class TCloseAction
{
};

class TTriggerable
{
public:
    virtual void OnTimer(TObject Sender) = 0;
};

class TTimer
{
public:
    TTimer() {}
    int Interval;
    void (TTriggerable::*OnTimer)(TObject);
    bool Enabled;
    int Tag; // Ez valami számláló lesz
    void Free();
};

// function

template<typename T>
int Length(vector<T>& vec)
{
    return vec.size();
}

template<typename T>
void SetLength(vector<T>& vec, int len)
{
    vec.reserve(len);
}

int Max(int a, int b);
void MessageBox(string szoveg, string cim);
void Sleep(int ms);
int Random(int to);
void Randomize();
string IntToStr(int i);
int Round(double r);

double sin_fok(double x);
double cos_fok(double x);

// Négyzet
double sqr(double x);
