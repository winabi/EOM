#include "A.h"
#include "B.h"

#include <stdio.h>
#include <math.h>

int main()
{

	double x = 2.44;
	double y = 0.86;
	double z = -0.16;

	printf("Result of func A: ");
	printf("%lf\n", A(x, y, z));

	printf("Result of func B: ");
	printf("%lf\n", B(x, y, z));

	return 0;
}
