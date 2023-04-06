#include "B.h"
#include <math.h>

double B(double x, double y, double z)
{
    double B = 1 + fabs(y + x) + (pow(fabs(y - x), 2)) / 2 - (pow(fabs(y - x), 3)) / 3;

    return B;
}
