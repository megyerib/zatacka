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

enum TShiftState
{
    state
};

// class
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

class TForm
{
public:
    void Close();
    void ShowCursor(bool show);
};
class TPanel
{
};
class TImageList
{
public:
    void GetBitmap(int i, TBitmap *bitmap);
};

class TLabel
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
class TGroupBox
{
public:
    int Tag;
};

class TCloseAction
{
};

class TTimer
{
public:
    TTimer() {}
    int Interval;
    //void (TTriggerable::*OnTimer)(TObject);
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
int Random(int to);
void Randomize();
string IntToStr(int i);
int Round(double r);

double sin_fok(double x);
double cos_fok(double x);

// Négyzet
double sqr(double x);
