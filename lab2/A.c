#include "A.h"
#include <math.h>

double A(double x, double y, double z)
{
    double step1 = pow(x, (y + 1)) + exp(y - 1);
    double step2 = 1 + x * fabs(y - tan(z));

    return step1 / step2;
}
