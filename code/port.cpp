#include "port.h"
#include <cmath>
#include "time.h"

int Max(int a, int b)
{
    return (a > b) ? a : b;
}

int Random(int to)
{
    return ((float)rand() / RAND_MAX) * to;
}

void Randomize()
{
    srand(time(NULL));
}

int Round(double r)
{
    return (int)(r + 0.5);
}

double sin_fok(double x)
{
    return sin(x * M_PI / 180);
}

double cos_fok(double x)
{
    return cos(x * M_PI / 180);
}

double sqr(double x)
{
    return x * x;
}
