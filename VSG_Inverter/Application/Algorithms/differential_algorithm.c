#include "differential_algorithm.h"


void TD_Function(TD *tdtd)
{
	fp32 d, d0, y, a0, a = 0;
	tdtd->x = tdtd->x1 - tdtd->aim;
	d = tdtd->r * tdtd->h;
	d0 = tdtd->h * d;
	y = tdtd->x + tdtd->h * tdtd->x2;
	a0 = sqrt(d * d + 8 * tdtd->r * fabs(y));
	if (fabs(y) > d0)
	{
		a = tdtd->x2 + (a0 - d) * Sgn(y) / 2;
	}
	else
	{
		a = tdtd->x2 + y / tdtd->h;
	}

	if (fabs(a) > d)
		y = -1 * tdtd->r * Sgn(a);
	else
		y = -1 * tdtd->r * a / d;
	tdtd->x1 += 0.001f * tdtd->x2;
	tdtd->x2 += 0.001f * y;
}
