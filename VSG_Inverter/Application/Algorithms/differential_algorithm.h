#ifndef __DIFFERENTIAL_ALGORITHM_H__
#define __DIFFERENTIAL_ALGORITHM_H__

#include "basic_type.h"
#include "math_algorithm.h"

typedef struct
{
	fp32 x1;
	fp32 x2;
	fp32 x;
	fp32 r;
	fp32 h;
	fp32 aim;
}TD;

extern void TD_Function(TD *tdtd);

#endif // __DIFFERENTIAL_ALGORITHM_H__
