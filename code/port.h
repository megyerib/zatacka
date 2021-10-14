#pragma once
#include <cstdint>
#include <string>

typedef int Integer;
typedef bool Boolean;
typedef uint8_t Byte;
typedef int16_t ShortInt;
typedef double Real;
typedef void* TObject;
class TPoint
{
public:
    Integer X;
    Integer Y;
    TPoint();
    TPoint(Integer x, Integer y);
};
class TBitmap;
class TParent{};
enum TBrushStyle {
    bsClear,
    bsSolid
};
class TCanvas
{
public:
    void Draw(Integer x, Integer y, TBitmap* bitmap);
    TColor** Pixels;
    struct {
        TColor Color;
        Integer Width;
    } Pen;
    struct {
        TColor Color;
        TBrushStyle Style;
    } Brush;
    void Rectangle(Integer a, Integer b, Integer c, Integer d);
    void Ellipse(Integer a, Integer b, Integer c, Integer d);
};
enum TPixelFormat {
    pf8bit
};
class TBitmap
{
public:
    TCanvas Canvas;
    Integer Width;
    Integer Height;
    Boolean Transparent;
    TPixelFormat PixelFormat;
};
typedef std::string String;
class TForm
{
public:
    struct {
        Integer Width;
        Integer Height;
    } Screen;

    virtual void FormMouseDown(TObject Sender, TMouseButton Button, TShiftState Shift, Integer X, Integer Y) = 0;
    virtual void FormMouseUp(TObject Sender, TMouseButton Button, TShiftState Shift, Integer X, Integer Y) = 0;
    void Close();
    void ShowCursor(Boolean show);
};
class TPanel : public TParent
{
public:
    Boolean Visible;
    void Show();
    void Hide();
};
class TImageList
{
public:
    void GetBitmap(Integer i, TBitmap* bitmap);
};
enum TAlignment {
    taRightJustify
};
enum TFontStyle {
    fsNone,
    fsBold
};
class TLabel
{
public:
    String Caption;
    TLabel(TForm& form);

    TParent& Parent;
    Integer Left;
    Integer Top;
    struct {
        Integer Size;
        TColor Color;
        TFontStyle Style;
    } Font;
    Boolean AutoSize;
    TAlignment Alignment;
    Boolean Transparent;
    void Hide();
    void Show();

    void (TForm::*OnMouseDown)(TObject, TMouseButton, TShiftState, Integer, Integer);
    void (TForm::*OnMouseUp)(TObject, TMouseButton, TShiftState, Integer, Integer);

    Boolean Visible;
};
class TImage{};
class TPaintBox{
public:
    TCanvas Canvas;
};
class TGroupBox : public TParent
{
public:
    Integer Tag;
};
typedef uint32_t Word;
class TShiftState{};
enum TMouseButton
{
    mbLeft,
    mbRight,
    mbMiddle
};
class TCloseAction{};
enum TColor {
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
String IntToStr(Integer i);
Integer Round(Real r);

#define True true
#define False false

class TTriggerable
{
public:
    virtual void OnTimer(TObject Sender) = 0;
};

class TTimer
{
public:
    TTimer(TForm1 form){}
    Integer Interval;
    void (TTriggerable::*OnTimer)(TObject);
    Boolean Enabled;
    Integer Tag; // ???
    void Free();
};

Real Sin(Real x);
Real Cos(Real x);

#define Pi 3.1415926535

void Randomize();
Integer Ord(char c);

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

Integer Length(void* arr);
void SetLength(void* arr, Integer len);
Integer Max(Integer a, Integer b);
Real Sqr(Real x);
Integer Abs(Integer x);

void MessageBox(String szoveg, String cim);
void ProcessMessages();
void Sleep(Integer ms);

Integer Random(Integer to);
