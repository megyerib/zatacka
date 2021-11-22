#pragma once
#include <string>
#include <vector>
#include "konstans.h"

using namespace std;

// enum

enum GombID {
    bal = 0,
    jobb,
    loves,

    gombSzam
};

enum TFontStyle
{
    fsNone,
    fsBold
};

// class
class TPoint
{
public:
    int X;
    int Y;
    TPoint(int x, int y);
};

class TPanel
{
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
    bool Transparent;
};

class TGroupBox
{
public:
    int Tag;
};

class TTimer
{
public:
    int Tag; // Ez valami számláló lesz
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
int Round(double r);

double sin_fok(double x);
double cos_fok(double x);

// Négyzet
double sqr(double x);
