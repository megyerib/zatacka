#pragma once
#include <string>
#include <cmath>

using std::string;

// define

#define VK_LEFT 0
#define VK_DOWN 0
#define VK_UP 0
#define VK_DIVIDE 0
#define VK_MULTIPLY 0
#define VK_SUBTRACT 0
#define VK_ESCAPE 0
#define VK_F10 0
#define VK_F1 0
#define VK_F2 0
#define VK_F3 0
#define VK_F4 0
#define VK_SPACE 0

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
    clSilver
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
    struct
    {
        int Width;
        int Height;
    } Screen;

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
    TObject &Parent;
    string Caption;
    TLabel(TForm &form);

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

    void (TForm::*OnMouseDown)(TObject, TMouseButton, TShiftState, int, int);
    void (TForm::*OnMouseUp)(TObject, TMouseButton, TShiftState, int, int);
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

class TShiftState
{
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
    TTimer(TForm1 form) {}
    int Interval;
    void (TTriggerable::*OnTimer)(TObject);
    bool Enabled;
    int Tag; // ???
    void Free();
};

// function

int Length(void *arr);
void SetLength(void *arr, int len);
int Max(int a, int b);
void MessageBox(string szoveg, string cim);
void ProcessMessages();
void Sleep(int ms);
int Random(int to);
void Randomize();
int Ord(char c);
string IntToStr(int i);
int Round(double r);
